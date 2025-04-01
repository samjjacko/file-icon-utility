# include "../include/MacInterface.h"
# include "../include/objcWrapper.hpp"
#include <stdio.h>
// this feels really stupid.... whatever make it work then refactor when you're smarter
struct OSIconInterface::Impl
{
    MacInterface* intrfce;
};

OSIconInterface::OSIconInterface()
{
    impl = new Impl(); // need to initialize the Impl struct's pointer
    MacInterface* macCommands = [[MacInterface alloc] init]; 
    impl->intrfce = macCommands; 
}

OSIconInterface::~OSIconInterface()
{
    [impl->intrfce release];
    delete impl;
}
// note that objc might have a weird bool that isn't c++ std
// doubt it though
bool OSIconInterface::setIconWithLocalImg(char *target_path, char* img_path)
{
    return [impl->intrfce setFileIconWithLocalImg:target_path image_path:img_path];
}

int OSIconInterface::setIconWithURL(char* target_path, char* img_url)
{
    return [impl->intrfce setFileIconWithUrl:target_path image_url:img_url];
}
const char* OSIconInterface::fetchDtopImgURL() 
{
    return [impl->intrfce fetchDesktopImgURL];
}