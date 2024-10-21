#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <tuple>
#include <algorithm>
#include <memory>

#include "graphedge.h"
#include "graphnode.h"
#include "chatbot.h"
#include "chatlogic.h"

using namespace std;

ChatLogic::ChatLogic()
{
    //// STUDENT CODE
    ////

    // create instance of chatbot
    //_chatBot = new ChatBot("../images/chatbot.png");      // do NOT need Another instance of "_chatBot"

    // add pointer to chatlogic so that chatbot answers can be passed on to the GUI
    //_chatBot->SetChatLogicHandle(this);       // "_chatBot" is NOT a pointer, it's an instance

    ////
    //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic()
{
    //// STUDENT CODE       Task 3: Change "_nodes" into a smart pointer
    ////

    // delete chatbot instance
    //delete _chatBot;    // deletes ChatBot, once it moves away from graph node OR graph node is destroyed       1st dellocation       do NOT want, need previous instances

    // delete all nodes
    /*for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        delete *it;  // frees up memory  ---> do NOT want; need the pointer
    }*/

    // delete all edges
    /*for (auto it = std::begin(_edges); it != std::end(_edges); ++it)
    {
        delete *it;  // frees up memory  ---> do NOT want; need the pointer
    }*/

    ////
    //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element)
{
    // find all occurences for current node
    auto token = tokens.begin();
    while (true)
    {
        token = std::find_if(token, tokens.end(), [&tokenID](const std::pair<std::string, std::string> &pair) { return pair.first == tokenID;; });
        if (token != tokens.end())
        {
            element.AddToken(token->second); // add new keyword to edge
            token++;                         // increment iterator to next element
        }
        else
        {
            break; // quit infinite while-loop
        }
    }
}

void ChatLogic::LoadAnswerGraphFromFile(std::string filename)     // main function that loads text file & converts it into graph structure
{
    // load file with answer graph elements
    std::ifstream file(filename);

    // check for file availability and process it line by line
    if (file)
    {
        // loop over all lines in the file
        std::string lineStr;
        while (getline(file, lineStr))
        {
            // extract all tokens from current line
            tokenlist tokens;
            while (lineStr.size() > 0)
            {
                // extract next token
                int posTokenFront = lineStr.find("<");
                int posTokenBack = lineStr.find(">");
                if (posTokenFront < 0 || posTokenBack < 0)
                    break; // quit loop if no complete token has been found
                std::string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - 1);

                // extract token type and info
                int posTokenInfo = tokenStr.find(":");
                if (posTokenInfo != std::string::npos)
                {
                    std::string tokenType = tokenStr.substr(0, posTokenInfo);
                    std::string tokenInfo = tokenStr.substr(posTokenInfo + 1, tokenStr.size() - 1);

                    // add token to vector
                    tokens.push_back(std::make_pair(tokenType, tokenInfo));
                }

                // remove token from current line
                lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
            }

            // process tokens for current line
            auto type = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "TYPE"; });
            if (type != tokens.end())
            {
                // check for id
                auto idToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "ID"; });
                if (idToken != tokens.end())
                {
                    // extract id from token
                    int id = std::stoi(idToken->second);

                    // node-based processing
                    if (type->second == "NODE")
                    {
                        //// STUDENT CODE       Task 3: Change "_nodes" into a smart pointer
                        ////

                        // check if node with this ID exists already
                        auto newNode = std::find_if(_nodes.begin(), _nodes.end(), [&id](const unique_ptr<GraphNode> &node) { return node->GetID() == id; });    // "const unique_ptr<GraphNode> &node" - Represents constant reference to 'unique_ptr' that manages 'GraphNode' object          "unique_ptr<GraphNode> node" - Declares smart pointer (specifically unique pointer)

                        // create new element if ID does not yet exist
                        if (newNode == _nodes.end())
                        {
                            _nodes.emplace_back(unique_ptr<GraphNode> (new GraphNode(id)));
                            newNode = _nodes.end() - 1; // get iterator to last element

                            // add all answers to current node
                            AddAllTokensToElement("ANSWER", tokens, **newNode);
                        }

                        ////
                        //// EOF STUDENT CODE
                    }

                    // edge-based processing
                    if (type->second == "EDGE")
                    {
                        //// STUDENT CODE       Task 3: Change "_nodes" into a smart pointer
                        ////

                        // find tokens for incoming (parent) and outgoing (child) node
                        auto parentToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "PARENT"; });
                        auto childToken = std::find_if(tokens.begin(), tokens.end(), [](const std::pair<std::string, std::string> &pair) { return pair.first == "CHILD"; });

                        if (parentToken != tokens.end() && childToken != tokens.end())
                        {
                            // get iterator on incoming and outgoing node via ID search
                            auto parentNode = std::find_if(_nodes.begin(), _nodes.end(), [&parentToken](const unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(parentToken->second); });    // "const unique_ptr<GraphNode> &node" - Represents constant reference to 'unique_ptr' that manages 'GraphNode' object          "unique_ptr<GraphNode> node" - Declares smart pointer (specifically unique pointer)
                            auto childNode = std::find_if(_nodes.begin(), _nodes.end(), [&childToken](const unique_ptr<GraphNode> &node) { return node->GetID() == std::stoi(childToken->second); });    // "const unique_ptr<GraphNode> &node" - Represents constant reference to 'unique_ptr' that manages 'GraphNode' object          "unique_ptr<GraphNode> node" - Declares smart pointer (specifically unique pointer)

                            // create new edge
                            unique_ptr<GraphEdge> edge = unique_ptr<GraphEdge> (new GraphEdge(id));    // changed '*edge' into UNIQUE pointer
                            edge->SetChildNode((*childNode).get());    // returns raw pointer
                            edge->SetParentNode((*parentNode).get());    // returns raw pointer
                            _edges.push_back(edge.get());    // returns raw pointer

                            // find all keywords for current node
                            AddAllTokensToElement("KEYWORD", tokens, *edge);

                            // store reference in child node and parent node
                            (*childNode)->AddEdgeToParentNode(edge.get());    // returns raw pointer
                            (*parentNode)->AddEdgeToChildNode(move(edge));    // moves "parentNode" so "childNode" can take place
                        }

                        ////
                        //// EOF STUDENT CODE
                    }
                }
                else
                {
                    std::cout << "Error: ID missing. Line is ignored!" << std::endl;
                }
            }
        } // eof loop over all lines in the file

        file.close();

    } // eof check for file availability
    else
    {
        std::cout << "File could not be opened!" << std::endl;
        return;
    }

    //// STUDENT CODE       Task 3: Change "_nodes" into a smart pointer
    ////

    // identify root node
    GraphNode *rootNode = nullptr;
    for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it)
    {
        // search for nodes which have no incoming edges
        if ((*it)->GetNumberOfParents() == 0)
        {

            if (rootNode == nullptr)
            {
                rootNode = (*it).get(); // assign current node to root       converts "unique_ptr<ChatLogic>" to "ChatLogic*"
            }
            else
            {
                std::cout << "ERROR : Multiple root nodes detected" << std::endl;
            }
        }
    }

    // add chatbot to graph root node
    _chatBot->SetRootNode(rootNode);
    rootNode->MoveChatbotHere(move(_chatBot));
    
    ////
    //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog *panelDialog)
{
    _panelDialog = panelDialog;
}

void ChatLogic::SetChatbotHandle(ChatBot *chatbot)      // links CURRENT ChatBot instance with ChatLogic manager
{
    _chatBot = chatbot;
}

void ChatLogic::SendMessageToChatbot(std::string message)     // sends message to ChatBot
{
    _chatBot->ReceiveMessageFromUser(message);
}

void ChatLogic::SendMessageToUser(std::string message)    // sends message to user from ChatBot
{
    _panelDialog->PrintChatbotResponse(message);
}

wxBitmap *ChatLogic::GetImageFromChatbot()    // retrieves handle to image that is managed by ChatBot, so it can be displayed in graphical user interface
{
    return _chatBot->GetImageHandle();
}
