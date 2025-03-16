#include <wx/wx.h>
class MainFrame : public wxFrame {
    public:
        MainFrame(const wxString& title);
    private:
        void OnChoice(wxCommandEvent& event);
        void OnExit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
};

class App : public wxApp {
    public:
        bool OnInit();
};