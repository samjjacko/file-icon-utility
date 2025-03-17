#include <wx/wx.h>
#include <wx/wx.h>
#include <wx/sizer.h>

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
        wxChoice* img_path_picker;
        wxImagePanel* img_panel;
        wxFrame* img_frame;
        void OnChoice(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnResize(wxSizeEvent& event);
};

class App : public wxApp {
    public:
        bool OnInit();
};