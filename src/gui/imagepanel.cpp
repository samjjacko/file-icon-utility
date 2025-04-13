#include "../include/gui.hpp"
BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
// some useful events
/*
 EVT_MOTION(wxImagePanel::mouseMoved)
 EVT_LEFT_DOWN(wxImagePanel::mouseDown)
 EVT_LEFT_UP(wxImagePanel::mouseReleased)
 EVT_RIGHT_DOWN(wxImagePanel::rightClick)
 EVT_LEAVE_WINDOW(wxImagePanel::mouseLeftWindow)
 EVT_KEY_DOWN(wxImagePanel::keyPressed)
 EVT_KEY_UP(wxImagePanel::keyReleased)
 EVT_MOUSEWHEEL(wxImagePanel::mouseWheelMoved)
 */

// catch paint events
EVT_PAINT(wxImagePanel::paintEvent)
//Size event
EVT_SIZE(wxImagePanel::OnSize)
END_EVENT_TABLE()


// some useful events
/*
 void wxImagePanel::mouseMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseDown(wxMouseEvent& event) {}
 void wxImagePanel::mouseWheelMoved(wxMouseEvent& event) {}
 void wxImagePanel::mouseReleased(wxMouseEvent& event) {}
 void wxImagePanel::rightClick(wxMouseEvent& event) {}
 void wxImagePanel::mouseLeftWindow(wxMouseEvent& event) {}
 void wxImagePanel::keyPressed(wxKeyEvent& event) {}
 void wxImagePanel::keyReleased(wxKeyEvent& event) {}
 */

wxImagePanel::wxImagePanel(wxFrame* parent, wxString file, wxBitmapType format) :
wxPanel(parent)
{
    // load the file... ideally add a check to see if loading was successful
    image.LoadFile(file, format);
    w = -1;
    h = -1;
}

/*
 * Called by the system of by wxWidgets when the panel needs
 * to be redrawn. You can also trigger this call by
 * calling Refresh()/Update().
 */

void wxImagePanel::paintEvent(wxPaintEvent & evt)
{
    // depending on your system you may need to look at double-buffered dcs
    wxPaintDC dc(this);
    render(dc);
}

/*
 * Alternatively, you can use a clientDC to paint on the panel
 * at any time. Using this generally does not free you from
 * catching paint events, since it is possible that e.g. the window
 * manager throws away your drawing when the window comes to the
 * background, and expects you will redraw it when the window comes
 * back (by sending a paint event).
 */
void wxImagePanel::paintNow()
{
    // depending on your system you may need to look at double-buffered dcs
    wxClientDC dc(this);
    render(dc);
}

/*
 * Here we do the actual rendering. I put it in a separate
 * method so that it can work no matter what type of DC
 * (e.g. wxPaintDC or wxClientDC) is used.
 */
void wxImagePanel::render(wxDC&  dc)
{
    int panelSize = std::min(GetSize().GetWidth(), GetSize().GetHeight());

    if (panelSize != w || panelSize != h) {
        resized = wxBitmap(image.Scale(panelSize, panelSize, wxIMAGE_QUALITY_HIGH));
        w = panelSize;
        h = panelSize;
    }

    if (resized.IsOk()) {
        dc.DrawBitmap(resized, 0, 0, true);
    }
}

/*
 * Here we call refresh to tell the panel to draw itself again.
 * So when the user resizes the image panel the image should be resized too.
 */
void wxImagePanel::OnSize(wxSizeEvent& event){
    Refresh();
    //skip the event.
    event.Skip();
}

// image size doesn't seem to be determined here...
void wxImagePanel::updateImage(wxString file, wxBitmapType format) {
    std::cout << file << std::endl;
    if (!image.LoadFile(file, format)) {
        wxLogError("Failed to load image: %s", file);
        return;
    } // loads the image just fine, but doesn't update the display
    if (!image.HasAlpha()) {
        image.InitAlpha(); // Init alpha channel if curr // don't do this every time!
    }
    // w = image.GetWidth();
    // h = image.GetHeight();
    int tempw, temph;
    GetClientSize(&tempw, &temph);
    w = tempw; h = temph; // for future rescaling events
    resized = wxBitmap(image.Scale(w, h, wxIMAGE_QUALITY_HIGH));
    Refresh();
    Update();
} // can i overload this method

void wxImagePanel::updateImage() {
    /*
        Updates the image after new data is written 
        to the image panel
    */
    int tempw, temph;
    GetClientSize(&tempw, &temph);
    w = tempw; h = temph; // for future rescaling events
    resized = wxBitmap(image.Scale(w, h, wxIMAGE_QUALITY_HIGH));
    Refresh();
    Update();
}