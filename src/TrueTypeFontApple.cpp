#include "Platform.hpp"
#if defined(ROOLE_IS_MAC)

#include "TrueTypeFont.hpp"
#include "Log.hpp"

#include "IO.hpp"
#include "Text.hpp"

#import <CoreText/CoreText.h>
#import <Foundation/Foundation.h>

#include <map>
using namespace std;

const unsigned char* Roole::ttf_data_by_name(const string& font_name, unsigned font_flags)
{
    // TODO: Make this cache thread-safe.
    static map<pair<string, unsigned>, const unsigned char*> ttf_file_cache;
    
    auto& ttf_ptr = ttf_file_cache[make_pair(font_name, font_flags)];
    if (ttf_ptr) return ttf_ptr;

    log("Trying to find a font named '%s', flags=%x", font_name.c_str(), font_flags);

    unsigned symbolic_traits = 0;
    if (font_flags & Roole::FF_BOLD)   symbolic_traits |= kCTFontBoldTrait;
    if (font_flags & Roole::FF_ITALIC) symbolic_traits |= kCTFontItalicTrait;

    NSDictionary *attributes = @{
        ((__bridge id) kCTFontNameAttribute): [NSString stringWithUTF8String:font_name.c_str()],
        ((__bridge id) kCTFontTraitsAttribute): @{
            ((__bridge id) kCTFontSymbolicTrait): @(symbolic_traits)
        }
    };
    CTFontDescriptorRef descriptor =
        CTFontDescriptorCreateWithAttributes((__bridge CFDictionaryRef) attributes);

    if (descriptor) {
        CTFontRef font = CTFontCreateWithFontDescriptorAndOptions(descriptor, 20, nullptr, 0);
        if (font) {
            NSURL *url = CFBridgingRelease(CTFontCopyAttribute(font, kCTFontURLAttribute));
            if (url && url.fileSystemRepresentation) {
                log("Loading file '%s'", url.fileSystemRepresentation);
                ttf_ptr = ttf_data_from_file(url.fileSystemRepresentation);
            }
            CFRelease(font);
        }
        CFRelease(descriptor);
    }
    
    return ttf_ptr;
}

const unsigned char* Roole::ttf_fallback_data()
{
    // Prefer Arial Unicode MS as a fallback because it covers a lot of Unicode.
    static const unsigned char* arial_unicode = ttf_data_by_name("Arial Unicode MS", 0);
    if (arial_unicode) return arial_unicode;
    
    return ttf_data_from_file("/Library/Fonts/Arial.ttf");
}

string Roole::default_font_name()
{
    return "Arial";
}

#endif
