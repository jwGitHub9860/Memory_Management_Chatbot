#ifndef CHATGUI_H_
#define CHATGUI_H_

#include <wx/wx.h>
#include <memory>

using namespace std;

class ChatLogic; // forward declaration

// middle part of the window containing the dialog between user and chatbot
class ChatBotPanelDialog : public wxScrolledWindow      // holds elements needed for displaying dialogue between user & chatbot
{
private:
    // control elements
    wxBoxSizer *_dialogSizer;
    wxBitmap _image;

    //// STUDENT CODE       Task 1: Change "_chatLogic" into a smart pointer
    ////

    ChatLogic *_chatLogic;  // pointer to chat logic itself

    unique_ptr<ChatLogic> uniquePtr(*_chatLogic);     // construct unique pointer
    //shared_ptr<ChatLogic> sharedPtr;       // Shared pointer
    //ChatLogic *rawPtr;  // Raw pointer

    ////
    //// EOF STUDENT CODE

public:
    // constructor / destructor         for drawing background image of San Francisco Golden Gate Bridge
    ChatBotPanelDialog(wxWindow *parent, wxWindowID id);    // have window functions
    ~ChatBotPanelDialog();

    // getter / setter                  for drawing background image of San Francisco Golden Gate Bridge
    ChatLogic *GetChatLogicHandle() { return _chatLogic; }

    // events                           for drawing background image of San Francisco Golden Gate Bridge
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    // proprietary functions
    void AddDialogItem(wxString text, bool isFromUser = true);      // possibility to add new dialogue item & print chatbot response to Next code line
    void PrintChatbotResponse(std::string response);

    DECLARE_EVENT_TABLE()
};

// dialog item shown in ChatBotPanelDialog
class ChatBotPanelDialogItem : public wxPanel   // item which could either be from user or chatbot
{
private:
    // control elements
    wxStaticBitmap *_chatBotImg;    // has image in case chatbot is answering image rode along by chatbot depending on position in current graph node network
    wxStaticText *_chatBotTxt;

public:
    // constructor / destructor
    ChatBotPanelDialogItem(wxPanel *parent, wxString text, bool isFromUser);
};

// frame containing all control elements
class ChatBotFrame : public wxFrame
{
private:
    // control elements
    ChatBotPanelDialog *_panelDialog;
    wxTextCtrl *_userTextCtrl;

    // events
    void OnEnter(wxCommandEvent &WXUNUSED(event));

public:
    // constructor / desctructor
    ChatBotFrame(const wxString &title);
};

// control panel for background image display
class ChatBotFrameImagePanel : public wxPanel
{
    // control elements
    wxBitmap _image;

public:
    // constructor / desctructor
    ChatBotFrameImagePanel(wxFrame *parent);

    // events
    void paintEvent(wxPaintEvent &evt);
    void paintNow();
    void render(wxDC &dc);

    DECLARE_EVENT_TABLE()
};

// wxWidgets app that hides main()
class ChatBotApp : public wxApp
{
public:
    // events
    virtual bool OnInit();
};

#endif /* CHATGUI_H_ */
