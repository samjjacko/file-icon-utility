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
// placeholder code to check that objc/c++ interoperability works
// int main(int argc, char* argv[]) {
//     char* file_path = argv[1];
//     char* img_url = argv[2];
//     int option = atoi(argv[3]);
//     OSIconInterface* intrfce = new OSIconInterface; 
//     switch(option) {
//         case 1:
//             if(intrfce->setIconWithLocalImg(file_path, img_url)) {
//                 printf("Icon set successfully!");
//             } else {
//                 printf("Icon not set successfully!");
//             }
//             return 0;
//         case 2:
//             if(intrfce->setIconWithURL(file_path, img_url)) {
//                 printf("Icon set successfully!");
//             } else {
//                 printf("Icon not set successfully!");
//             }
//             return 0;
//         default:
//             printf("Invalid option! Supply either 1 or 2 as the third argument.\n");
//             break;
//     }
// }

// wxWidgets entry point - this is the GUI thread
wxIMPLEMENT_APP(App);
// main function 
bool App::OnInit() {
    MainFrame *mainFrame = new MainFrame("C++ GUI");
    // "client size" refers to the inside of the window, not including the border
    mainFrame->Show(true);
    return true;

}