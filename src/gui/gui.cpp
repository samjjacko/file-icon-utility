#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <cstdio>
#include "../include/gui.hpp"
#define WIDTH 800
#define HEIGHT 400
// main method implememtation and entry point are in ../executable/main.cpp 

// should maybe put these event handlers in a separate file
void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}
 
void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Yooooo",
                 "About my infinite swag", wxOK | wxICON_INFORMATION);
}

// MainFrame constructor - does it need to inherit anything from existing wxFrame?
// arg one of wxFrame is the parent, arg two is the frame id, wxID_ANY just grabs a random unused identifier
enum {
    ID_SELECT = 1,
    ID_ALL = 2
};

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    this->SetClientSize(WIDTH, HEIGHT); 
    // best practice is to add a wxPanel first https://docs.wxwidgets.org/3.2/classwx_panel.html
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxColourPickerCtrl *colourPicker = new wxColourPickerCtrl(this, wxID_ANY, *wxWHITE, wxPoint(150, 50), wxSize(100, 35));
    wxArrayString choices;
    choices.Add("Select individual"); choices.Add("Select all");
    img_path_picker = new wxChoice(this, wxID_ANY, wxPoint(0, HEIGHT - 21), wxSize(100, 21), choices);
    
    wxInitAllImageHandlers();
    // img_frame = new wxFrame(this, wxID_ANY, "Image", wxPoint(50, 50), wxSize(400, 400));
    img_panel = new wxImagePanel(this, wxT("/Users/samjackson/Desktop/dev/ricing_tool_project/samples/image.png"), wxBITMAP_TYPE_PNG);
    // wxBoxSizer* img_sizer = new wxBoxSizer(wxHORIZONTAL);
    // img_sizer->Add(img_panel, 1, wxSHAPED);
    // img_frame->SetSizer(img_sizer);
    // img_frame->Show(true);
    // Initialise sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    // alignment flags must be in the opposite axis to the wxBoxSizer orientation
    // use wxSHAPED for the image panel
    sizer->Add(img_path_picker, 0, wxALIGN_BOTTOM); 
    sizer->Add(colourPicker, 1);
    sizer->Add(img_panel, 1, wxSHAPED);
    // SetSizerAndFit(sizer);
    SetSizer(sizer);
    // Bindings
    img_path_picker->Bind(wxEVT_CHOICE, &MainFrame::OnChoice, this);  
    // OnResize is called whenever the window is resized
    // Bind(wxEVT_SIZE, &MainFrame::OnResize, this); 
}

void MainFrame::OnChoice(wxCommandEvent& e) {
    wxFileDialog openFileDialog(this, _("Open txt file"), wxEmptyString, wxEmptyString, "txt files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() != wxID_CANCEL) { // skips if user cancels
        // could also load selected file chosen by user with wxWidgets input streams
        // but idk why you wouldn't just use standard input streams...?
        FILE* fp = fopen(openFileDialog.GetPath(), "r");
        char buff[1024];
        
        while(fgets(buff, 1024, fp) != NULL) {
            printf("Contents of buff: %s \n", buff);
        }
        fclose(fp);
    } 
    return; 
}
void MainFrame::OnResize(wxSizeEvent& e) {
    wxSize clientSize = GetClientSize();
    wxSize choiceSize(clientSize.GetWidth() * 3 / 14,21);
    //don't need to worry about recalculating the position of the choice 
    // because it's always at the bottom left
    img_path_picker->SetSize(choiceSize);
    img_path_picker->SetPosition(wxPoint(0, clientSize.GetHeight() - choiceSize.GetHeight()));
    e.Skip();
}

// watching this https://www.youtube.com/watch?v=b5RtMxMZikg
// reading this https://wiki.wxwidgets.org/An_image_panel