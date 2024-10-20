#ifndef GRAPHNODE_H_
#define GRAPHNODE_H_

#include <vector>
#include <string>
#include <memory>
#include "chatbot.h"

using namespace std;

// forward declarations
class GraphEdge;

class GraphNode
{
private:
    //// STUDENT CODE
    ////

    // data handles (owned)
    //std::vector<GraphEdge *> _childEdges;  // edges to subsequent nodes         responsible for allocating & de-allocating memory
    std::vector<unique_ptr<GraphEdge>> _childEdges;  // edges to subsequent nodes         responsible for allocating & de-allocating memory         changed "_childEdges" into UNIQUE pointer since function using pointer takes Unique pointer

    // data handles (not owned)
    std::vector<GraphEdge *> _parentEdges; // edges to preceding nodes          NOT responsible for allocating & de-allocating memory
    ChatBot _chatBot;     // declares instance, not POD (Plain Old Data) type

    ////
    //// EOF STUDENT CODE

    // proprietary members
    int _id;    // loaded from 'answergraph.txt' file
    std::vector<std::string> _answers;    // loaded from 'answergraph.txt' file 

public:
    // constructor / destructor
    GraphNode(int id);
    ~GraphNode();

    // getter / setter
    int GetID() { return _id; }
    int GetNumberOfChildEdges() { return _childEdges.size(); }
    GraphEdge *GetChildEdgeAtIndex(int index);
    std::vector<std::string> GetAnswers() { return _answers; }
    int GetNumberOfParents() { return _parentEdges.size(); }

    // proprietary functions
    void AddToken(std::string token); // add answers to list
    void AddEdgeToParentNode(GraphEdge *edge);
    void AddEdgeToChildNode(unique_ptr<GraphEdge> edge);

    //// STUDENT CODE
    ////

    void MoveChatbotHere(ChatBot chatbot);     // pass handle to ChatBot & move it here         chatbot (or "_chatBot") is an INSTANCE

    ////
    //// EOF STUDENT CODE

    void MoveChatbotToNewNode(GraphNode *newNode);
};

#endif /* GRAPHNODE_H_ */