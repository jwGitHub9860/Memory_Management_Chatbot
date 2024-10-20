#include <iostream>
#include <random>
#include <algorithm>
#include <ctime>

#include "chatlogic.h"
#include "graphnode.h"
#include "graphedge.h"
#include "chatbot.h"

using namespace std;

// constructor WITHOUT memory allocation
ChatBot::ChatBot()
{
    // invalidate data handles
    _image = nullptr;
    _chatLogic = nullptr;
    _rootNode = nullptr;
}

// constructor WITH memory allocation
ChatBot::ChatBot(std::string filename)      // constructor
{
    std::cout << "ChatBot Constructor" << std::endl;
    
    // invalidate data handles
    _chatLogic = nullptr;
    _rootNode = nullptr;

    // load image into heap memory
    _image = new wxBitmap(filename, wxBITMAP_TYPE_PNG);
}

ChatBot::~ChatBot()     // 1 : destructor
{
    std::cout << "ChatBot Destructor" << std::endl;

    // deallocate heap memory
    if(_image != nullptr) // Attention: wxWidgets used NULL and not nullptr
    {
        delete _image;  // crash HERE (Task 0)
        _image = nullptr;
    }
}

//// STUDENT CODE       Task 2: Make code to compile Rule of Five
////
ChatBot::ChatBot(const ChatBot &source)     // 2 : copy constructor
{
    cout << "ChatBot Copy Constructor" << endl;

    _chatLogic = source._chatLogic;   // creates copy of "_chatLogic" from source
    _rootNode = source._rootNode;     // creates copy of "_rootNode" from source

    _image = new wxBitmap();     // dynamically allocates its own Heap memory       new ---> allocates memory on heap 
    
    _chatLogic->SetChatbotHandle(this);     // confirms ChatLogic knows which ChatBot instance is currently active & should be interacted with: between Copy & Original
    
    *_image = *source._image;           // copies content of source to newly allocated heap memory (Deep Copy)
}
ChatBot &ChatBot::operator=(const ChatBot &source)      // 3 : copy assignment operator
{
    cout << "ChatBot Copy Assignment Operator" << endl;
    if (this == &source)    // protects against self assignment
    {
        return *this;
    }
    delete[] _image; // deletes "_image" since code looped before & need to deallocate data

    _chatLogic = source._chatLogic;   // creates copy of "chatLogic" from source
    _rootNode = source._rootNode;     // creates copy of "rootNode" from source
    _image = new wxBitmap();          // dynamically allocates its own Heap memory       new ---> allocates memory on heap 

    _chatLogic->SetChatbotHandle(this);     // confirms ChatLogic knows which ChatBot instance is currently active & should be interacted with: between Copy & Original
    
    *_image = *source._image;           // copies content of source to newly allocated heap memory (Deep Copy)
    
    return *this;
}
ChatBot::ChatBot(ChatBot &&source)      // 4 : move constructor
{
    cout << "ChatBot Move Constructor" << endl;

    _chatLogic = source._chatLogic;     // copies "_chatLogic" from source
    _rootNode = source._rootNode;       // copies "_rootNode" from source
    _image = source._image;             // copies "_image" from source

    _chatLogic->SetChatbotHandle(this);     // confirms ChatLogic knows which ChatBot instance is currently active & should be interacted with: after ChatBot instance is moved & ChatLogic location is updated

    source._chatLogic = nullptr;    // prevents "_chatLogic" from being used again
    source._rootNode = nullptr;     // prevents "_rootNode" from being used again
    source._image = nullptr;        // prevents "_image" from being used again
}
ChatBot &ChatBot::operator=(ChatBot &&source)   // 5 : move assignment operator
{
    cout << "ChatBot Move Assignment Operator" << endl;
    if (this == &source)    // proctects against self assignment
    {
        return *this;
    }
    delete[] _image;     // deletes "_image" since code looped before & needs to be deallocated

    _chatLogic = source._chatLogic;   // copies "_chatLogic" from source
    _rootNode = source._rootNode;   // copies "_rootNode" from source
    _image = source._image;         // copies "_image" from source

    _chatLogic->SetChatbotHandle(this);     // confirms ChatLogic knows which ChatBot instance is currently active & should be interacted with: after ChatBot instance is moved & ChatLogic location is updated

    source._chatLogic = nullptr;     // prevents "_chatLogic" from being used again
    source._rootNode = nullptr;     // prevents "_rootNode" from being used again
    source._image = nullptr;        // prevents "_image" from being used again

    return *this;
}
////
//// EOF STUDENT CODE

void ChatBot::ReceiveMessageFromUser(std::string message)
{
    // loop over all edges and keywords and compute Levenshtein distance to query
    typedef std::pair<GraphEdge *, int> EdgeDist;
    std::vector<EdgeDist> levDists; // format is <ptr,levDist>

    for (size_t i = 0; i < _currentNode->GetNumberOfChildEdges(); ++i)
    {
        GraphEdge *edge = _currentNode->GetChildEdgeAtIndex(i);
        for (auto keyword : edge->GetKeywords())
        {
            EdgeDist ed{edge, ComputeLevenshteinDistance(keyword, message)};
            levDists.push_back(ed);
        }
    }

    // select best fitting edge to proceed along
    GraphNode *newNode;
    if (levDists.size() > 0)
    {
        // sort in ascending order of Levenshtein distance (best fit is at the top)
        std::sort(levDists.begin(), levDists.end(), [](const EdgeDist &a, const EdgeDist &b) { return a.second < b.second; });
        newNode = levDists.at(0).first->GetChildNode(); // after sorting the best edge is at first position
    }
    else
    {
        // go back to root node
        newNode = _rootNode;
    }

    // tell current node to move chatbot to new node
    _currentNode->MoveChatbotToNewNode(newNode);
}

void ChatBot::SetCurrentNode(GraphNode *node)
{
    // update pointer to current node
    _currentNode = node;

    // select a random node answer (if several answers should exist)
    std::vector<std::string> answers = _currentNode->GetAnswers();
    std::mt19937 generator(int(std::time(0)));
    std::uniform_int_distribution<int> dis(0, answers.size() - 1);
    std::string answer = answers.at(dis(generator));

    // send selected node answer to user
    _chatLogic->SendMessageToUser(answer);
}

int ChatBot::ComputeLevenshteinDistance(std::string s1, std::string s2)
{
    // convert both strings to upper-case before comparing
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);

    // compute Levenshtein distance measure between both strings
    const size_t m(s1.size());
    const size_t n(s2.size());

    if (m == 0)
        return n;
    if (n == 0)
        return m;

    size_t *costs = new size_t[n + 1];

    for (size_t k = 0; k <= n; k++)
        costs[k] = k;

    size_t i = 0;
    for (std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i)
    {
        costs[0] = i + 1;
        size_t corner = i;

        size_t j = 0;
        for (std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j)
        {
            size_t upper = costs[j + 1];
            if (*it1 == *it2)
            {
                costs[j + 1] = corner;
            }
            else
            {
                size_t t(upper < corner ? upper : corner);
                costs[j + 1] = (costs[j] < t ? costs[j] : t) + 1;
            }

            corner = upper;
        }
    }

    int result = costs[n];
    delete[] costs;

    return result;
}