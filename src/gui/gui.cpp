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

void MainFrame::ConstructImgModifiers(wxScrolled<wxPanel>* img_modifiers) {
    // ok, what do I want to be able to control about this image?
    // add a fancy tint box
    // add three other bars for RGB 
    // space them out and add labels 29 31 32
    img_modifiers->SetBackgroundColour(wxColor(36, 37, 38));
    auto slider_sizer = new wxFlexGridSizer(2, wxSize(5, 15));
    tint_slider = new wxSlider(img_modifiers,  wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "tint amount");
    red_slider = new wxSlider(img_modifiers,  wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "R");
    grn_slider = new wxSlider(img_modifiers,  wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "G");
    blu_slider = new wxSlider(img_modifiers,  wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "B");

    auto tint_txt = new wxStaticText(img_modifiers, wxID_ANY, "Tint amount");
    auto red_txt = new wxStaticText(img_modifiers, wxID_ANY, "Red");
    auto grn_txt = new wxStaticText(img_modifiers, wxID_ANY, "Green");
    auto blu_txt = new wxStaticText(img_modifiers, wxID_ANY, "Blue");

    // these should have our own enum IDS so we can refer to them without checking later
    slider_sizer->Add(tint_txt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(tint_slider, 2, wxALIGN_RIGHT | wxALL, 15);

    slider_sizer->Add(red_txt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(red_slider, 0, wxALIGN_CENTER | wxALL, 15);

    slider_sizer->Add(grn_txt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(grn_slider, 0, wxALIGN_CENTER | wxALL, 15);

    slider_sizer->Add(blu_txt, 0, wxALIGN_CENTER | wxALL, 15);
    slider_sizer->Add(blu_slider, 0, wxALIGN_CENTER | wxALL, 15);

    img_modifiers->SetSizer(slider_sizer);
    tint_slider->Bind(wxEVT_SLIDER, &MainFrame::OnTintSlider, this);
    red_slider->Bind(wxEVT_SLIDER, &MainFrame::OnTintSlider, this);
    grn_slider->Bind(wxEVT_SLIDER, &MainFrame::OnTintSlider, this);
    blu_slider->Bind(wxEVT_SLIDER, &MainFrame::OnTintSlider, this);
}

void MainFrame::OnTintSlider(wxCommandEvent& e) {
    // wxImage* img = img_panel->getImage(); 
    // no, need to fetch image from path. Should maybe store images in memory to make it faster

    if(strcmp(icon_path, "\0") == 0) { // image hasn't been selected
        return;
    }
    auto ev_obj = e.GetEventObject();
    wxSlider* rgb[3] = {NULL, NULL, NULL};
    if (ev_obj == red_slider) {
        rgb[0] = red_slider;
    } else if (ev_obj == grn_slider) {
        rgb[1] = grn_slider;
    } else if (ev_obj == grn_slider) {
        rgb[2] = blu_slider;
    }

    TintSetImg(rgb);
}

void MainFrame::TintSetImg(wxSlider* rgb[3]) {
    wxImage selected_img(icon_path, wxBITMAP_TYPE_PNG);
    // Get image dimensions 
    int width = selected_img.GetWidth();
    int height = selected_img.GetHeight();
    if (!selected_img.HasAlpha()) {
        selected_img.InitAlpha();
    }
    // an array of integers representing colour values for each pixel
    // e.g. R G B R G B R G B etc
    unsigned char* icon_path_rgb = selected_img.GetData();
    // for some reason this is processed seperately to the alpha channel
    unsigned char* icon_path_alpha = selected_img.GetAlpha();
    // existing tinted image loaded into the img_panel
    unsigned char* active_img_data = img_panel->image.GetData();
    // to avoid double free-ing 
    unsigned char* datacpy = new unsigned char[width * height *3];
    unsigned char* alphacpy = new unsigned char[width * height * 3];
    memcpy(datacpy, active_img_data, width * height * 3);
    memcpy(alphacpy, icon_path_alpha, width * height);
    // Apply the tint
    for(int i = 0; i < 3; i++) {
        if(rgb[i] == NULL) {
            continue;
        } else {
            int tint_amt = rgb[i]->GetValue();
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) { // iterate through columns
                    int index = (y * width + x) * 3; // because R G B 
                    // transparent pixels are skipped 
                    if(icon_path_alpha[y * width + x] == 0) continue; 
                    // don't bother adding stuff it's already above RGB threshold
                    if (tint_amt > 0 && datacpy[index + i] == 255) continue;
                    if (tint_amt < 0 && datacpy[index + i] == 0) continue;
        
                    datacpy[index + i] = std::max(
                        std::min(255, icon_path_rgb[index + i] + tint_amt), 0);
                }
            }
        }
    }
    img_panel->image.SetData(datacpy);
    img_panel->image.SetAlpha(alphacpy); // probably slow
    img_panel->updateImage();
    int* x = NULL;
}

// void tintloop(wxSlider* rgb[3], unsigned char* data, unsigned char* alpha, int height, int width) {
//     for(int i = 0; i < 3; i++) {
//         if(rgb[i] == NULL) {
//             continue;
//         } else {
//             int tint_amt = rgb[i]->GetValue();
//             for (int y = 0; y < height; ++y) {
//                 for (int x = 0; x < width; ++x) { // iterate through columns
//                     int index = (y * width + x) * 3; // because R G B 
//                     // transparent pixels are skipped 
//                     if(alpha[y * width + x] == 0) continue; 
//                     // don't bother adding stuff it's already above RGB threshold
//                     if (tint_amt > 0 && data[index + i] == 255) continue;
//                     if (tint_amt < 0 && data[index + i] == 0) continue;
        
//                     data[index + i] = std::max(
//                         std::min(255, data[index + i] + tint_amt), 0); // possibly inefficient...
//                 }
//             }
//         }
//     }
// }

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    icon_path = "\0";
    
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
    wxButton* generateTint = new wxButton(modifyPanel, wxID_ANY, "Set colour as tint", wxDefaultPosition, wxSize(150, 40));

    colour_picker = new wxColourPickerCtrl(modifyPanel, wxID_ANY, *wxWHITE, wxDefaultPosition, wxSize(100, 20));
    
    // Add controls to the top sizer with proper spacing
    topControlsSizer->Add(generateTint, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    topControlsSizer->Add(colour_picker, 0, wxALIGN_CENTER_VERTICAL | wxLEFT);
    
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
    colour_picker->Bind(wxEVT_COMMAND_COLOURPICKER_CHANGED, &MainFrame::SetSliders, this);
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

// segfaults after tint has been applied, probably because of bad memory management
void MainFrame::GetImagePath(wxCommandEvent& e) {
    wxFileDialog openFileDialog(this, _("Open image file"), 
    wxEmptyString, wxEmptyString, "image files (*.png)|*.png", 
    wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() != wxID_CANCEL) {
        img_panel->updateImage(openFileDialog.GetPath(),wxBITMAP_TYPE_PNG);
        icon_path = openFileDialog.GetPath(); 
    }
}
// Need to get this to behave properly
// set the image using the wxImage stored in img_panel, 
// not from the path. 
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

void MainFrame::SetSliders(wxCommandEvent& e) {
    wxColour col = colour_picker->GetColour();
    // set the value of each slider according to the colour values of the colour we're picking
    red_slider->SetValue(col.Red());
    grn_slider->SetValue(col.Green());
    blu_slider->SetValue(col.Blue());
    wxSlider* temp[3] = {red_slider, grn_slider, blu_slider};
    TintSetImg(temp);
}

