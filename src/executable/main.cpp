// #ifdef __APPLE__
// #   include "MacInterface.h"
// #   define Intrfce MacInterface
// # else
// #   include "WindowsInterface.h"
// #   define Intrfce WindowsInterface
// #endif
#include "../include/objcWrapper.hpp"
#include "../include/gui.hpp"
#include <stdio.h>
#include <stdlib.h>

// wxWidgets entry point - this is the GUI thread
wxIMPLEMENT_APP(App);
// main function 
bool App::OnInit() {
    MainFrame *mainFrame = new MainFrame("C++ GUI");
    // "client size" refers to the inside of the window, not including the border
    mainFrame->Show(true);
    return true;

}