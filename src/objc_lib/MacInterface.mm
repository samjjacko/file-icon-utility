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
@end