#include <wx/wx.h>
#include <wx/clrpicker.h>
#include <wx/filedlg.h>
#include <wx/wfstream.h>
#include <wx/gbsizer.h>
#include <cstdio>
#include "../include/gui.hpp"
#include "../include/objcWrapper.hpp"
#define WIDTH 800
#define HEIGHT 400
// main method implememtation and entry point are in ../executable/main.cpp 

// should maybe put these event handlers in a separate file0
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

void ConstructImgModifiers(wxScrolled<wxPanel>* img_modifiers) {
    // ok, what do I want to be able to control about this image?
    // add a fancy tint box
    // add three other bars for RGB 
    // space them out and add labels 29 31 32
    img_modifiers->SetBackgroundColour(wxColor(36, 37, 38));
    auto slider_sizer = new wxBoxSizer(wxVERTICAL);
    wxSlider *tint_amt = new wxSlider(img_modifiers,  wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "tint amount");
    wxSlider *red_amt = new wxSlider(img_modifiers,  wxID_ANY, 0, -255, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "R");
    wxSlider *grn_amt = new wxSlider(img_modifiers,  wxID_ANY, 0, -255, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "G");
    wxSlider *blu_amt = new wxSlider(img_modifiers,  wxID_ANY, 0, -255, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "B");
    
    slider_sizer->Add(tint_amt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(red_amt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(grn_amt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(blu_amt, 0, wxALIGN_CENTER | wxALL, 15);
    img_modifiers->SetSizer(slider_sizer);
}

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    int ARGB[] = {0, 0, 0, 0}; // 
    
    // Set a consistent window size
    this->SetClientSize(WIDTH, HEIGHT); 
   
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    //left half of the main layout: the notebook
    notebook = new wxNotebook(this, wxID_ANY, wxPoint(0,0), wxSize(WIDTH / 2, HEIGHT));

    // page one of the notebook
    wxPanel* modifyPanel = new wxPanel(notebook, wxID_ANY); // stores sliders associated with modifying image
    modifyPanel->SetBackgroundColour(wxColour(29, 31, 32, 255));
    
    // Create a vertical sizer for the top controls
    wxBoxSizer* topControlsSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // page one child widgets with consistent sizing
    wxButton* generateTint = new wxButton(modifyPanel, wxID_ANY, "Generate Tint", wxDefaultPosition, wxSize(150, 40));
    wxColourPickerCtrl *colourPicker = new wxColourPickerCtrl(modifyPanel, wxID_ANY, *wxWHITE, wxDefaultPosition, wxSize(100, 20));
    
    // Add controls to the top sizer with proper spacing
    topControlsSizer->Add(generateTint, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    topControlsSizer->Add(colourPicker, 0, wxALIGN_CENTER_VERTICAL);
    
    // Create a horizontal sizer for the bottom controls
    wxBoxSizer* bottomControlsSizer = new wxBoxSizer(wxHORIZONTAL);
    
    wxArrayString choices;
    choices.Add("Select files"); choices.Add("Select directories");
    file_path_picker = new wxChoice(modifyPanel, wxID_ANY, wxDefaultPosition, wxSize(120, 30), choices);
    wxButton* get_img = new wxButton(modifyPanel, wxID_ANY, "Get image", wxDefaultPosition, wxSize(100, 30));
    wxButton* set_icon = new wxButton(modifyPanel, wxID_ANY, "Set icon!", wxDefaultPosition, wxSize(100, 30));
    
    // Add controls to the bottom sizer with proper spacing
    bottomControlsSizer->Add(get_img, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    bottomControlsSizer->Add(set_icon, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    bottomControlsSizer->Add(file_path_picker, 0, wxALIGN_CENTER_VERTICAL);
    
    // Create the image modifiers panel
    wxScrolled<wxPanel>* img_modifiers = new wxScrolled<wxPanel>(modifyPanel, wxID_ANY);
    img_modifiers->SetScrollRate(0, FromDIP(10));
    ConstructImgModifiers(img_modifiers);

    const auto margin = FromDIP(10);
    
    // sizing of modify page with a vertical box sizer for better alignment
    wxBoxSizer* notebookSizer = new wxBoxSizer(wxVERTICAL);
    
    // Add all components with proper spacing
    notebookSizer->Add(topControlsSizer, 0, wxEXPAND | wxALL, margin);
    notebookSizer->Add(img_modifiers, 1, wxEXPAND | wxALL, margin);
    notebookSizer->Add(bottomControlsSizer, 0, wxEXPAND | wxALL, margin);
    
    modifyPanel->SetSizerAndFit(notebookSizer);

    notebook->InsertPage(0, modifyPanel, "Modify");
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 5);
    wxInitAllImageHandlers();
    
    // image setup with proper sizing
    img_panel = new wxImagePanel(this, wxT("/Users/samjackson/Desktop/dev/ricing_tool_project/samples/image.png"), wxBITMAP_TYPE_PNG);
    
    // Set a reasonable minimum size for the image panel
    modifyPanel->SetMinSize(wxSize(WIDTH/2, HEIGHT/2));
    img_panel->SetMinSize(wxSize(WIDTH/2, HEIGHT/2));
    mainSizer->Add(img_panel, 1, wxEXPAND | wxALL, 5);
    
    this->SetSizerAndFit(mainSizer);
    // Bindings
    file_path_picker->Bind(wxEVT_CHOICE, &MainFrame::OnChoice, this);  

    get_img->Bind(wxEVT_BUTTON, &MainFrame::GetImagePath, this);
    set_icon->Bind(wxEVT_BUTTON, &MainFrame::SetIcon, this);
    // OnResize is called whenever the window is resized
    // Bind(wxEVT_SIZE, &MainFrame::OnResize, this); 
    SetMinSize(wxSize(WIDTH/2, HEIGHT/2));
    SetMaxSize(wxSize(WIDTH, HEIGHT));
}



void MainFrame::OnChoice(wxCommandEvent& e) { 
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

    int squareSize = std::min(clientSize.GetWidth() / 2, clientSize.GetHeight());
    img_panel->SetSize(wxSize(squareSize, squareSize));

    wxSize choiceSize(clientSize.GetWidth() * 3 / 14,21);
    //don't need to worry about recalculating the position of the choice 
    // because it's always at the bottom left
    file_path_picker->SetSize(choiceSize);
    file_path_picker->SetPosition(wxPoint(0, clientSize.GetHeight() - choiceSize.GetHeight()));
    e.Skip(); // Allow the event to propagate
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