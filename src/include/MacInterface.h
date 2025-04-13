@import Cocoa;
@interface  MacInterface : NSWorkspace
    - (id)init;
    - (NSString*)convertCharArray: (char*)cString; // helper method to convert a char array into an NSString
    // updates a single file icon using an image file supplied by a path
    - (bool)setFileIconWithLocalImg: (char*)target_path image_path:(char*)img_path; 
    - (bool)setFileIconWithUrl: (char*)target_path image_url:(char*)img_url;
    - (const char*)fetchDesktopImgURL;
    - (bool)setFileIconWithBitmap:(char*)target_path
        bitmapData:(unsigned char*)bitmap_data 
        height:(int)height 
        width:(int)width;
    // do i store an image as a char array??? need to watch more of that handy image
    // processing in C youtube video
    // - (type)getFileIcon: (char*)target_path; 
@end