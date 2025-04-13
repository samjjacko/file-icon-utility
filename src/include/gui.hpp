#include <wx/wx.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/notebook.h>
#include <wx/clrpicker.h>
class wxImagePanel : public wxPanel
{
    int w, h;
    
public:
    wxBitmap resized; 
    wxImage image; // stores the active image in the ImagePanel
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);
    wxImage* getImage();
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);
    void updateImage(wxString file, wxBitmapType format);
    void updateImage();
    unsigned char* GetRGBA();
    // some useful events
    /*
     void mouseMoved(wxMouseEvent& event);
     void mouseDown(wxMouseEvent& event);
     void mouseWheelMoved(wxMouseEvent& event);
     void mouseReleased(wxMouseEvent& event);
     void rightClick(wxMouseEvent& event);
     void mouseLeftWindow(wxMouseEvent& event);
     void keyPressed(wxKeyEvent& event);
     void keyReleased(wxKeyEvent& event);
     */
    
    DECLARE_EVENT_TABLE()
};

class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString& title);
    private:
        wxNotebook* notebook;
        wxChoice* file_path_picker;
        wxImagePanel* img_panel;

        wxFrame* img_frame;
        wxString icon_path;
        wxSlider *tint_slider;
        wxSlider *red_slider;
        wxSlider *grn_slider;
        wxSlider *blu_slider;
        wxImage *tinted_img;
        wxColourPickerCtrl* colour_picker;
        // event bindings
        void OnTintSlider(wxCommandEvent& e);
        void ConstructImgModifiers(wxScrolled<wxPanel>* img_modifiers);
        void OnChoice(wxCommandEvent& event);
        void GetImagePath(wxCommandEvent& event);
        void SetIcon(wxCommandEvent& e);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void SetSliders(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
        void TintSetImg(wxSlider* rgb[3]);
        void updateTintOnAllChannels();
        wxArrayString file_paths;
};

class App : public wxApp {
    public:
        bool OnInit();
};