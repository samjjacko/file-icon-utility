#include <Cocoa/Cocoa.h> 
#include <stdio.h>
#import "../include/MacInterface.h"
#import <Foundation/Foundation.h>

@implementation MacInterface
    - (id)init { // dodgy constructor
        return self;
    }
    - (NSString*)convertCharArray: (char*)cString {
        return [[NSString alloc] initWithString:[NSString stringWithCString:cString encoding:NSASCIIStringEncoding]];
    }
    - (bool)setFileIconWithLocalImg: (char*)target_path image_path:(char*)img_path {
        NSString* NStarget_path = [self convertCharArray:target_path];
        NSString* NSimage_path = [self convertCharArray:img_path];
        // What does this return if contents of file at image_path is empty
        NSImage* iconImage = [[NSImage alloc] initWithContentsOfFile:NSimage_path];
        // NSImage* iconImage = [[NSImage alloc] initWithData:[NSData dataWithContentsOfFile:NSimage_path options:NSDataReadingMappedIfSafe error:(NSError * _Nullable * _Nullable)]];

        // returns false if image setting does not succeed, true otherwise
        return [[NSWorkspace sharedWorkspace]
                        setIcon: iconImage
                        forFile: NStarget_path
                        options: 0]; 
        // try and handle errors for this; check if the supplied paths actually exist..?
        // construct an enum for the different error types so that the relative error message
        // can be printed out??
        // or maybe return the error message as a char * and return NULL if succeeded...?
    }
    - (bool)setFileIconWithUrl: (char*)target_path image_url:(char*)img_url {
        NSString* NStarget_path = [self convertCharArray:target_path];

        NSURL* NSimage_url = [NSURL URLWithString:[self convertCharArray:img_url] 
                                encodingInvalidCharacters:true];

        if (NSimage_url == nil) return false; // could not encode the characters properly - dodgy link

        NSImage* iconImage = [[NSImage alloc] initWithContentsOfURL:NSimage_url];
        // returns false if image setting does not succeed, true otherwise
        return [[NSWorkspace sharedWorkspace]
                        setIcon: iconImage
                        forFile: NStarget_path
                        options: 0]; 
    }
    - (const char*)fetchDesktopImgURL { // must be called from the main thread...
        NSURL* desktopImgURL = [self desktopImageURLForScreen:[NSScreen mainScreen]];
        NSString* urlstring = (NSString *) desktopImgURL.filePathURL;
        return [urlstring cStringUsingEncoding:NSUTF8StringEncoding];
        // return [NSString string];
    }
- (bool)setFileIconWithBitmap:(char*)target_path 
                   bitmapData:(unsigned char*)bitmap_data 
                       height:(int)height 
                        width:(int)width {
    // Convert the target path to NSString
    NSString* NStarget_path = [self convertCharArray:target_path];
    
    if (!bitmap_data || width <= 0 || height <= 0) {
        NSLog(@"Invalid bitmap data or dimensions");
        return false;
    }
    
    NSLog(@"Creating icon with dimensions: %dx%d", width, height);
    
    // Create a CGDataProvider from the raw data
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bitmap_data, width * height * 4, NULL);
    
    // Create a CGColorSpace for RGBA
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    
    // Create a CGImage from the data
    CGImageRef cgImage = CGImageCreate(width,
                                      height,
                                      8,  // bits per component
                                      32, // bits per pixel
                                      width * 4, // bytes per row
                                      colorSpace,
                                      kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big,
                                      provider,
                                      NULL,
                                      NO,
                                      kCGRenderingIntentDefault);
    
    if (!cgImage) {
        NSLog(@"Failed to create CGImage");
        CGColorSpaceRelease(colorSpace);
        CGDataProviderRelease(provider);
        return false;
    }
    
    // Create NSImage from CGImage
    NSImage* iconImage = [[NSImage alloc] initWithCGImage:cgImage size:NSMakeSize(width, height)];
    
    if (!iconImage) {
        NSLog(@"Failed to create NSImage");
        CGImageRelease(cgImage);
        CGColorSpaceRelease(colorSpace);
        CGDataProviderRelease(provider);
        return false;
    }
    
    // Scale to standard icon size
    [iconImage setSize:NSMakeSize(1024, 1024)];
    
    // Set the icon for the file
    bool success = [[NSWorkspace sharedWorkspace]
        setIcon:iconImage
        forFile:NStarget_path
        options:0];
    
    NSLog(@"Icon set %@", success ? @"successfully" : @"failed");
    
    // Clean up
    CGImageRelease(cgImage);
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(provider);
    [iconImage release];
    
    return success;
}
@end