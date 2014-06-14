//
//  MOAICrashlyticsIOS.mm
//  libmoai
//
//  Created by Sylvain Rebaud on 3/28/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#ifndef DISABLE_CRASHLYTICS

#include "pch.h"

#import <UIKIT/UIKit.h>
#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <MobileCoreServices/MobileCoreServices.h>

#import <moai-iphone/MOAICrashlyticsIOS.h>
#import <moai-plumzi/PZLogger.h>
#import <Crashlytics/Crashlytics.h>

#define CRASH_LOG_HEADER \
@"Package: %@\n\
Version: %@\n\
Date: %@\n\
Model: %@\n\
OS: %@\n\
\n"

//================================================================//
// lua
//================================================================//

static void _ErrorTraceback ( const char* message, struct lua_State* L, int level ) {
	
    MOAIScopedLuaState state ( L );
	
	NSString *crash = [NSString stringWithFormat:CRASH_LOG_HEADER,
								  [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"],
								  [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"],
								  [NSDate date],
								  [UIDevice currentDevice].model,
								  [UIDevice currentDevice].systemVersion];
	crash = [crash stringByAppendingFormat:@"LUA error: %@\n", [NSString stringWithUTF8String:message]];
	
//	// print stack trace to log
//    USLog::Print ( "%s\n", message );
//    state.PrintStackTrace ( USLog::CONSOLE, level );
	
	// parse stack trace and format HockeyApp compatible data
	NSString *stackTrace = [NSString stringWithUTF8String:state.GetStackTrace ( level ).c_str()];
	NSArray *lines = [stackTrace componentsSeparatedByCharactersInSet: [NSCharacterSet newlineCharacterSet]];
	for (NSString *line in lines) {
		NSError *error = NULL;
		NSRegularExpression *regex = [NSRegularExpression
									  regularExpressionWithPattern:@"\\s*(?:([^:]*?):(\\d*)|(.*?)) in (.*)"
									  options:NSRegularExpressionCaseInsensitive
									  error:&error];
		NSTextCheckingResult *match = [regex firstMatchInString:line options:0 range:NSMakeRange(0, [line length])];
		if (match) {
			NSString *func = nil;
			NSString *where = nil;
			if ([match rangeAtIndex:4].location != NSNotFound) {
				// we have a function name
				func = [line substringWithRange:[match rangeAtIndex:4]];
				func = [func stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"<>'"]];
				
				if ([match rangeAtIndex:1].location != NSNotFound) {
					// lua path
					where = [NSString stringWithFormat:@"%@:%@",
							 [line substringWithRange:[match rangeAtIndex:1]],
							 [line substringWithRange:[match rangeAtIndex:2]]];
				} else if ([match rangeAtIndex:3].location != NSNotFound) {
					// C path / anything else
					where = [line substringWithRange:[match rangeAtIndex:3]];
				} else {
					assert(@"unknown path format?? (should not be possible)");
				}
			} else {
				assert(@"couldn't parse the function name (should not be possible)");
			}
			crash = [crash stringByAppendingFormat:@"	at %@ (%@)\n", func, where];
		}
	};
	
	// Log stack trace
	NSLog(@"Crash: %@", crash);
	MOAIPrint("traceback", [crash UTF8String]);
//	PZLogFlush();
	
#ifndef DEBUG
	// crash
	[[ Crashlytics sharedInstance ] crash];
#endif
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Crashlytics.
 
	@in		string betaID			Available in HockeyApp dashboard settings.
	@in		string liveID			Available in HockeyApp dashboard settings.
	@out	nil
 */
int MOAICrashlyticsIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* _apiKey = lua_tostring ( state, 1 );
	NSString* apiKey = [[ NSString alloc ] initWithUTF8String:_apiKey ];

	if (apiKey) {
		[ Crashlytics startWithAPIKey:apiKey ];
		AKUSetFunc_ErrorTraceback ( _ErrorTraceback );
	}
	
	[apiKey release];

	return 0;
}

//================================================================//
// MOAICrashlyticsIOS
//================================================================//

//----------------------------------------------------------------//
MOAICrashlyticsIOS::MOAICrashlyticsIOS () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAICrashlyticsIOS::~MOAICrashlyticsIOS () {

}

//----------------------------------------------------------------//
void MOAICrashlyticsIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable[] = {
		{ "init",	_init },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
