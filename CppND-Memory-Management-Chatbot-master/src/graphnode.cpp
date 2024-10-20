#include <memory>

#include "graphedge.h"
#include "graphnode.h"

using namespace std;

GraphNode::GraphNode(int id)    // constructor
{
    _id = id;   // assigns ID
}

GraphNode::~GraphNode()     // destructor
{
    //// STUDENT CODE
    ////

    //delete _chatBot;    // deletes ChatBot, once it moves away from graph node OR graph node is destroyed     solves Task 0 (Warm-up task) by deleting the 2nd dellocation

    ////
    //// EOF STUDENT CODE
}

void GraphNode::AddToken(std::string token)
{
    _answers.push_back(token);
}

void GraphNode::AddEdgeToParentNode(GraphEdge *edge)
{
    _parentEdges.push_back(edge);
}

void GraphNode::AddEdgeToChildNode(unique_ptr<GraphEdge> edge)  // changed "edge" to UNIQUE pointer to match 'new edge' pointer change
{
    _childEdges.push_back(move(edge));  // changed "edge" to "move(edge)" since function will take "move(edge)" from '*parentNode'
}

//// STUDENT CODE
////
void GraphNode::MoveChatbotHere(ChatBot *chatbot)
{
    _chatBot = chatbot;
    //_chatBot->SetCurrentNode(this);       // do NOT need since "_chatBot" is no longer a pointer
}

void GraphNode::MoveChatbotToNewNode(GraphNode *newNode)
{
    newNode->MoveChatbotHere(_chatBot);
    //_chatBot = nullptr; // invalidate pointer at source   do NOT need since "_chatBot" is no longer a pointer
}
////
//// EOF STUDENT CODE

GraphEdge *GraphNode::GetChildEdgeAtIndex(int index)
{
    //// STUDENT CODE
    ////

    return _childEdges[index];

    ////
    //// EOF STUDENT CODE
}