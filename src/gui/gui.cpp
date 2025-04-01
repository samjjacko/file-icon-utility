#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <cstdio>
#include "../include/gui.hpp"
#include "../include/objcWrapper.hpp"
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
// I want to use wxNotebook on the left, and a wxPanel on the right to contain the image
// Then add a status bar displaying active icon path on the right
// Add some method to save the settings made - save file as .json?

// move modifyPanel construction to its own method 
// make sure everything's lined up properly 
// get the window to maintain a constant ratio so half the window is the image and the other half is the scrolling section 
MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    this->SetClientSize(WIDTH, HEIGHT); 
   
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    //left half of the main layout: the notebook
    notebook = new wxNotebook(this, wxID_ANY, wxPoint(0,0), wxSize(WIDTH / 2, HEIGHT));

    // page one of the notebook
    wxPanel* modifyPanel = new wxPanel(notebook, wxID_ANY); // stores sliders associated with modifying image

    // page one child widgets
    wxButton* generateTint = new wxButton(modifyPanel, wxID_ANY, "Generate Tint Settings", wxPoint(150, 0), wxSize(150, 50));
    wxColourPickerCtrl *colourPicker = new wxColourPickerCtrl(modifyPanel, wxID_ANY, *wxWHITE, wxPoint(150, 50), wxSize(100, 35));
    wxArrayString choices;
    choices.Add("Select individual"); choices.Add("Select all");
    file_path_picker = new wxChoice(modifyPanel, wxID_ANY, wxPoint(0, HEIGHT - 21), wxSize(100, 21), choices);
    wxButton* get_img = new wxButton(modifyPanel, wxID_ANY, "Get image");
    wxButton* set_icon = new wxButton(modifyPanel, wxID_ANY, "Set icon!");

    // sizing of modify page
    wxBoxSizer* notebookSizer = new wxBoxSizer(wxHORIZONTAL);
    // alignment flags must be in the opposite axis to the wxBoxSizer orientation
    // use wxSHAPED for the image panel
    notebookSizer->Add(generateTint, 0, wxALIGN_TOP | wxALL, 5);
    notebookSizer->Add(file_path_picker, 0, wxALIGN_BOTTOM | wxALL, 5); 
    notebookSizer->Add(get_img, 0, wxALIGN_BOTTOM | wxALL, 5);
    notebookSizer->Add(set_icon, 0, wxALIGN_BOTTOM | wxALL, 5);
    notebookSizer->Add(colourPicker, 0,wxALIGN_TOP | wxALL, 5);
    modifyPanel->SetSizer(notebookSizer);

    notebook->InsertPage(0, modifyPanel, "Modify");
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
    wxInitAllImageHandlers();
    
    // image setup
    img_panel = new wxImagePanel(this, wxT("/Users/samjackson/Desktop/dev/ricing_tool_project/samples/image.png"), wxBITMAP_TYPE_PNG);
    
    mainSizer->Add(img_panel, 1, wxEXPAND |wxALL, 5);
    this->SetSizerAndFit(mainSizer);
    // Bindings
    file_path_picker->Bind(wxEVT_CHOICE, &MainFrame::OnChoice, this);  

    get_img->Bind(wxEVT_BUTTON, &MainFrame::GetImagePath, this);
    set_icon->Bind(wxEVT_BUTTON, &MainFrame::SetIcon, this);
    // OnResize is called whenever the window is resized
    // Bind(wxEVT_SIZE, &MainFrame::OnResize, this); 
    SetSize(1000, 500);
}

void MainFrame::OnChoice(wxCommandEvent& e) { // should write paths to file_paths instance variable
    if(file_path_picker->GetCurrentSelection() == 0) { // select multiple files
        wxFileDialog openFileDialog(this, _("Files whose icons will be replaced"), 
    wxEmptyString, wxEmptyString, "any files (*.*)|*", 
    wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
        if(openFileDialog.ShowModal() != wxID_CANCEL) {
            openFileDialog.GetPaths(file_paths); // definitely looks sus....
        }
    } else {
        wxDirDialog openDirDialog(this, "select directories", wxEmptyString, wxDD_DIR_MUST_EXIST | wxDD_MULTIPLE);
        if(openDirDialog.ShowModal() != wxID_CANCEL) {
            openDirDialog.GetPaths(file_paths); // definitely looks sus....
        }
    }
}
void MainFrame::OnResize(wxSizeEvent& e) {
    wxSize clientSize = GetClientSize();
    wxSize choiceSize(clientSize.GetWidth() * 3 / 14,21);
    //don't need to worry about recalculating the position of the choice 
    // because it's always at the bottom left
    file_path_picker->SetSize(choiceSize);
    file_path_picker->SetPosition(wxPoint(0, clientSize.GetHeight() - choiceSize.GetHeight()));
    e.Skip();
}

void MainFrame::GetImagePath(wxCommandEvent& e) {
    wxFileDialog openFileDialog(this, _("Open image file"), 
    wxEmptyString, wxEmptyString, "image files (*.png)|*.png", 
    wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() != wxID_CANCEL) {
        img_panel->updateImage(openFileDialog.GetPath(),wxBITMAP_TYPE_PNG);
        icon_path = openFileDialog.GetPath(); 
    }
}

void MainFrame::SetIcon(wxCommandEvent& e) {
    // not getting here
    // std::cout << "can we get much higher" << std::endl;
    // printf("fuck me");
    OSIconInterface* intrfce = new OSIconInterface; // should probably make this an instance variable
    for(int i = 0; i < file_paths.GetCount(); i++) {
        if(intrfce->setIconWithLocalImg((char*)(const char*)file_paths[i], (char*)(const char*)icon_path.mb_str())) {
            printf("Icon set successfully!");
        } else {
            printf("Icon not set successfully!");
            return;
        }
    }
}
// watching this https://www.youtube.com/watch?v=b5RtMxMZikg
// reading this https://wiki.wxwidgets.org/An_image_panel