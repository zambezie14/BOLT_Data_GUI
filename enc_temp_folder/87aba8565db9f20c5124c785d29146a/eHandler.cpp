/* Event Handler of Desktop Application */

/* Include */
#include "eHandler.h"

/* Event IDs */
enum{
    ID_Hello = 1,
    ID_View_SystemAnalysis = 2,
    ID_View_SystemOverview = 3,
    ID_Button_SA = 4,
    ID_Button_SO = 5
};

/* Event Table */
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(ID_View_SystemAnalysis, MyFrame::OnViewSA)
EVT_MENU(ID_View_SystemOverview, MyFrame::OnViewSO)
EVT_BUTTON(ID_Button_SA, MyFrame::OnViewSA)
EVT_BUTTON(ID_Button_SO, MyFrame::OnViewSO)
wxEND_EVENT_TABLE()





/* System Analysis Panel Class */
class SystemAnalysisPanel : public wxPanel
{
public:
    /* Default Constructor */
    SystemAnalysisPanel(wxWindow* parent);

private:
    /* Variables for System Analysis Panel */

};

/* Default Constructor */
SystemAnalysisPanel::SystemAnalysisPanel(wxWindow * parent): wxPanel(parent, wxID_ANY)
{
    SetBackgroundColour(wxColor(*wxRED));

    /* Create Com Port to Read UART Data */
    HANDLE hComm;
    //TCHAR* pcCommPort = TEXT("COM7");
    //const char* device = "COM7";
    
    LPCWSTR a = L"COM7";

    hComm = CreateFile(a, GENERIC_READ, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);

    /* Check if Port is Open */
    if (hComm == INVALID_HANDLE_VALUE)
        wxLogMessage("Error in opening serial port");
    else
        wxLogMessage("opening serial port successful"); 



    DCB dcbSerialParams = { 0 }; // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    GetCommState(hComm, &dcbSerialParams);

    dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None

    SetCommState(hComm, &dcbSerialParams);




    SetCommMask(hComm, EV_RXCHAR);

    DWORD dwEventMask;
    WaitCommEvent(hComm, &dwEventMask, NULL);

    char TempChar; //Temporary character used for reading
    char SerialBuffer[256];//Buffer for storing Rxed Data
    DWORD NoBytesRead;
    int i = 0;

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadTotalTimeoutConstant = 100;

    /* Continuous Execution B/C UART is ALWAYS Transmitting */
    do
    {
        ReadFile(hComm,           //Handle of the Serial port
            &TempChar,       //Temporary character
            sizeof(TempChar),//Size of TempChar
            &NoBytesRead,    //Number of bytes read
            NULL);

        SerialBuffer[i] = TempChar;// Store Tempchar into buffer
        i++;
    }
    while (NoBytesRead > 0);
    //while (TempChar != '7');
    wxLogMessage(SerialBuffer);


    CloseHandle(hComm);//Closing the Serial Port


    //wxPanel* p_Panel = new wxPanel(parent, wxID_ANY);
    //wxBoxSizer* p_Box = new wxBoxSizer(wxVERTICAL);
    //TopPanel* p_Tpanel = new TopPanel(p_Panel);
    //BottomPanel* p_Bpanel = new BottomPanel(p_Panel);

    //p_Box->Add(p_Tpanel, 1, wxEXPAND | wxALL, 5);
    //p_Box->Add(p_Bpanel, 1, wxEXPAND | wxALL, 5);

    //p_Panel->SetSizer(p_Box);

    //this->Centre();
}








/* Data Structure for Initial Frame */
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxSize minSize(500, 500);
    SetMinSize(minSize);
 
    
    /* NEW MENU */
    wxMenuBar* menuBar = new wxMenuBar;

    /* NEW MENU ITEM - FILE */
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    menuBar->Append(menuFile, "&File");

    /* NEW MENU ITEM - ABOUT */
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    menuBar->Append(menuHelp, "&Help");

    /* NEW MENU ITEM - VIEW */
    wxMenu* menuView = new wxMenu;
    menuView->Append(ID_View_SystemAnalysis, "&System Analysis", "View System Analysis");
    menuView->Append(ID_View_SystemOverview, "&System Overview", "View System Overview");
    menuBar->Append(menuView, "&View");

    SetMenuBar(menuBar);

    /* Status Bar at Bottom of Page */
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    /* Create System Analysis Panel*/
    p_SystemAnalysisPanel = new SystemAnalysisPanel(this);
    p_SystemAnalysisPanel->Show(true);


}





/* EVENT HANDLER - ABOUT */
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

/* EVENT HANDLER - EXIT */
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

/* EVENT HANDLER - HELLO */
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

/* EVENT HANDLER - SYSTEM ANALYSIS */
void MyFrame::OnViewSA(wxCommandEvent& event)
{
    wxLogMessage("Change to System Analysis");
}

/* EVENT HANDLER - SYSTEM OVERVIEW */
void MyFrame::OnViewSO(wxCommandEvent& event)
{
    wxLogMessage("Default Gone");
}