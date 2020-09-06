#include "Platform.hpp"
#if defined(ROOLE_IS_MAC)

#import "Utility.hpp"
#import <Foundation/Foundation.h>
using namespace std;

string Roole::language()
{
  @autoreleasepool {
    NSString* language = [NSLocale preferredLanguages][0];
    return language.UTF8String ?: "en";
  }
}

#endif
