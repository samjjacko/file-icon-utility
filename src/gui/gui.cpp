#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <cstdio>
#include "../include/gui.hpp"

// main method implememtation and entry point are in ../executable/main.cpp 
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
    // best practice is to add a wxPanel first https://docs.wxwidgets.org/3.2/classwx_panel.html
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxColourPickerCtrl *colourPicker = new wxColourPickerCtrl(panel, wxID_ANY, *wxWHITE, wxPoint(150, 50), wxSize(100, 35));
    wxArrayString choices;
    choices.Add("Select individual"); choices.Add("Select all");

    wxChoice* img_path_picker = new wxChoice(panel, wxID_ANY, wxPoint(150, 375), wxSize(100, -1), choices);

    img_path_picker->Bind(wxEVT_CHOICE, &MainFrame::OnChoice, this);  
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
