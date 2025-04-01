#include <wx/wx.h>
#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/notebook.h>

class wxImagePanel : public wxPanel
{
    wxImage image;
    wxBitmap resized; 
    int w, h;
    
public:
    wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format);
    
    void paintEvent(wxPaintEvent & evt);
    void paintNow();
    void OnSize(wxSizeEvent& event);
    void render(wxDC& dc);
    void updateImage(wxString file, wxBitmapType format);
    
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
        // event bindings
        void OnChoice(wxCommandEvent& event);
        void GetImagePath(wxCommandEvent& event);
        void SetIcon(wxCommandEvent& e);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
        wxArrayString file_paths;
};

class App : public wxApp {
    public:
        bool OnInit();
};