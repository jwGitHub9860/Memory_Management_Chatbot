#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <vector>
#include <string>
#include "chatgui.h"

using namespace std;

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic
{
private:
    //// STUDENT CODE       Task 3: Change "_nodes" into a smart pointer
    ////

    // data handles (owned)
    //std::vector<GraphNode *> _nodes;    // pointers to graph nodes
    vector<unique_ptr<GraphNode *>> _nodes;    // UNIQUE pointers to graph nodes
    std::vector<GraphEdge *> _edges;    // pointers to graph edges

    ////
    //// EOF STUDENT CODE

    // data handles (not owned)
    GraphNode *_currentNode;
    ChatBot *_chatBot;
    ChatBotPanelDialog *_panelDialog;

    // proprietary type definitions
    typedef std::vector<std::pair<std::string, std::string>> tokenlist;

    // proprietary functions
    template <typename T>
    void AddAllTokensToElement(std::string tokenID, tokenlist &tokens, T &element);

public:
    // constructor / destructor
    ChatLogic();
    ~ChatLogic();

    // getter / setter
    void SetPanelDialogHandle(ChatBotPanelDialog *panelDialog);
    void SetChatbotHandle(ChatBot *chatbot);    // links CURRENT ChatBot instance with ChatLogic manager

    // proprietary functions
    void LoadAnswerGraphFromFile(std::string filename);     // main function that loads text file & converts it into graph structure
    void SendMessageToChatbot(std::string message);     // sends message to ChatBot
    void SendMessageToUser(std::string message);    // sends message to user from ChatBot
    wxBitmap *GetImageFromChatbot();    // retrieves handle to image that is managed by ChatBot, so it can be displayed in graphical user interface
};

#endif /* CHATLOGIC_H_ */