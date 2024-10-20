#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;   // ChatBot needs once it reaches dead end
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);     // helps compute similarity between user query & available keywords in an edge          Levenshtein distance - measure that looks at similarity between 2 strings

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    ~ChatBot();     // 1 : destructor

    //// STUDENT CODE       Task 2: Make code to compile Rule of Five
    ////

    ChatBot(const ChatBot &source);     // 2 : copy constructor
    ChatBot &operator=(const ChatBot &source);      // 3 : copy assignment operator
    ChatBot(ChatBot &&source);      // 4 : move constructor
    ChatBot &operator=(ChatBot &&source);   // 5 : move assignment operator

    ////
    //// EOF STUDENT CODE

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    ChatLogic* GetChatLogicHandle() { return _chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
