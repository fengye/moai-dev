//
//  MOAIHockeyAppIOS.mm
//  libmoai
//
//  Created by Sylvain Rebaud on 3/28/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#ifndef DISABLE_HOCKEYAPP

#include "pch.h"

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <MobileCoreServices/MobileCoreServices.h>

#import <moai-iphone/MOAIHockeyAppIOS.h>
#import <moai-cocoalumberjack/MOAICocoaLumberjack.h>
#import "AFNetworking.h"

#include <sys/types.h>
#include <sys/sysctl.h>

#import <HockeySDK/HockeySDK.h>

//================================================================//
// MOAIHockeyAppIOSDelegate
//================================================================//
@interface MOAIHockeyAppIOSDelegate : NSObject < BITHockeyManagerDelegate > {
@private
}
@end

//================================================================//
// MOAIHockeyAppIOSDelegate
//================================================================//
@implementation MOAIHockeyAppIOSDelegate

//================================================================//
#pragma mark -
#pragma mark Protocol BITUpdateManagerDelegate
//================================================================//

- (BOOL)updateManagerShouldSendUsageData:(BITUpdateManager *)updateManager {
#ifdef CONFIGURATION_Distribution
	return NO;
#else
	return YES;
#endif
}

//================================================================//
#pragma mark -
#pragma mark Protocol BITCrashManagerDelegate
//================================================================//
- (NSString *)applicationLogForCrashManager:(BITCrashManager *)crashManager {
	const char *log = MOAICocoaLumberjack::Get().GetCrashLog();
	return log?[NSString stringWithUTF8String:log]:nil;
}
@end

#define HOCKEYAPP_LOG_HEADER \
@"Package: %@\n\
Version: %@\n\
Date: %@\n\
Model: %@\n\
OS: %@\n\
\n"

static NSString *hockeyAppId = nil;

static void _ErrorTraceback ( const char* message, struct lua_State* L, int level ) {
	// use the system hardware model for hardware reporting instead of the "translated" model name
	size_t size = 100;
	char *hw_machine = (char *)malloc(size);
	int name[] = {CTL_HW, HW_MACHINE};
	sysctl(name, 2, hw_machine, &size, NULL, 0);
	NSString *hardwareModel = [NSString stringWithUTF8String:hw_machine];

	NSString *hockeyStackTrace = [NSString stringWithFormat:HOCKEYAPP_LOG_HEADER,
					 [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleIdentifier"],
					 [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleVersion"],
					 [NSDate date],
					 hardwareModel,
					 [UIDevice currentDevice].systemVersion];
	hockeyStackTrace = [hockeyStackTrace stringByAppendingFormat:@"LUA error: %@\n", [NSString stringWithUTF8String:message]];
	
	// parse stack trace to log
    MOAIScopedLuaState state ( L );
//    state.PrintStackTrace ( ZLLog::CONSOLE, level );
	
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
			hockeyStackTrace = [hockeyStackTrace stringByAppendingFormat:@"	at %@ (%@)\n", func, where];
		}
	};
	
	// Log stack trace
	MOAIPrint( 1, "traceback", [hockeyStackTrace UTF8String] );
	[DDLog flushLog];
	
	// Only upload in non debug build
	if ([BITHockeyManager sharedHockeyManager].crashManager.crashManagerStatus != BITCrashManagerStatusDisabled) {
		NSURL *url = [NSURL URLWithString:[NSString stringWithFormat:@"https://rink.hockeyapp.net/api/2/apps/%@/crashes/upload", hockeyAppId]];
		AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
		[manager setResponseSerializer:[AFXMLParserResponseSerializer new]];
		[manager POST:url.absoluteString
			parameters:nil
			constructingBodyWithBlock:^(id formData) {
				 [formData appendPartWithFileData:[NSData dataWithBytes:(void*)[hockeyStackTrace UTF8String] length:hockeyStackTrace.length]
											 name:@"log"
										 fileName:@"crash.log"
										 mimeType:@"application/octet-stream"];
				 
				NSString *log = [NSString stringWithUTF8String:MOAICocoaLumberjack::Get().GetCrashLog()];
				 [formData appendPartWithFileData:[NSData dataWithBytes:(void*)[log UTF8String] length:log.length]
											 name:@"description"
										 fileName:@"description.txt"
										 mimeType:@"application/octet-stream"];
			 } success:^(AFHTTPRequestOperation *operation, id responseObject) {
				 if (operation.response.statusCode == 200 || operation.response.statusCode == 201) {
					 MOAIPrint(3, "traceback", "Successfully uploaded crash to HockeyApp.");
				 } else {
					 MOAIPrint(1, "traceback", "HTTP error uploading crash to HockeyAp: %d", operation.response.statusCode);
				 }
				 exit(1);
			 } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
				 if (error) MOAIPrint(1, "traceback", "Error uploading crash to HockeyAp: %s", [[error description] UTF8String]);
				 
				 exit(1);
			 }];
	} else {
		exit(1);
	}
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize HockeyApp.
 
	@in		string betaID			Available in HockeyApp dashboard settings.
	@in		string liveID			Available in HockeyApp dashboard settings.
	@out	nil
 */
int MOAIHockeyAppIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* appBetaId	= state.GetValue < cc8* >( 1, "" );
	cc8* appLiveId	= state.GetValue < cc8* >( 2, "" );
	
	NSString* betaID = [[[ NSString alloc ] initWithUTF8String:appBetaId ] autorelease];
	NSString* liveID = [[[ NSString alloc ] initWithUTF8String:appLiveId ] autorelease];
	
	[ [BITHockeyManager sharedHockeyManager] configureWithBetaIdentifier:betaID
														  liveIdentifier:liveID
																delegate:(MOAIHockeyAppIOSDelegate*)MOAIHockeyAppIOS::Get ().mHockeyAppDelegate ];
	
	[BITHockeyManager sharedHockeyManager].crashManager.delegate = (MOAIHockeyAppIOSDelegate*)MOAIHockeyAppIOS::Get ().mHockeyAppDelegate;
	[BITHockeyManager sharedHockeyManager].updateManager.delegate = (MOAIHockeyAppIOSDelegate*)MOAIHockeyAppIOS::Get ().mHockeyAppDelegate;
	[BITHockeyManager sharedHockeyManager].crashManager.crashManagerStatus = BITCrashManagerStatusAutoSend;
	
//	if ( state.CheckParams ( 4, "B" )) {
//		
//		bool disabled  = state.GetValue < bool >( 4, false );
//		[BITHockeyManager sharedHockeyManager].crashManager.crashManagerStatus = disabled?BITCrashManagerStatusDisabled:BITCrashManagerStatusAutoSend;
//	}
	
#ifdef CONFIGURATION_Distribution
	hockeyAppId = liveID;
#else
	hockeyAppId = betaID;
	[[BITHockeyManager sharedHockeyManager].authenticator authenticateInstallation];
#endif

	if (hockeyAppId) {
		
		AKUSetFunc_ErrorTraceback ( _ErrorTraceback );
	}
	
	// Test SDK is properly installed, does nothing when ran in AppStore
	[[BITHockeyManager sharedHockeyManager] testIdentifier];

	// Start
	[[BITHockeyManager sharedHockeyManager] startManager ];

	return 0;
}

//----------------------------------------------------------------//
/**	@name	_disableCrashReporting
	@text	Enable/Disable lua crash reporting
 
	@in		bool   disabled		Disable crash reporting. Default to false.
	@out	nil
 */
int MOAIHockeyAppIOS::_disableCrashReporting ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	bool disabled  = state.GetValue < bool >( 1, false );
	[BITHockeyManager sharedHockeyManager].crashManager.crashManagerStatus = disabled?BITCrashManagerStatusDisabled:BITCrashManagerStatusAutoSend;
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	crash
	@text	Simulate a crash. Only works when app is not running in AppStore
 
	@out	nil
 */
int MOAIHockeyAppIOS::_crash ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	[[BITHockeyManager sharedHockeyManager].crashManager generateTestCrash];
	
	return 0;
}


//----------------------------------------------------------------//
/**	@name	sendFeedback
	@text	Open feedback dialog.

	@out	nil
 */
int MOAIHockeyAppIOS::_sendFeedback ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if (hockeyAppId) {
		
		[BITHockeyManager sharedHockeyManager].feedbackManager.requireUserName = BITFeedbackUserDataElementOptional;
		[BITHockeyManager sharedHockeyManager].feedbackManager.requireUserEmail = BITFeedbackUserDataElementOptional;
	}
	
	BITFeedbackComposeViewController *feedbackCompose = [[BITHockeyManager sharedHockeyManager].feedbackManager feedbackComposeViewController];
	[feedbackCompose prepareWithItems:
		@[[NSString stringWithUTF8String:MOAICocoaLumberjack::Get().GetCrashLog()]]];

	UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:feedbackCompose];
	navController.modalPresentationStyle = UIModalPresentationFormSheet;
	
	UIViewController* rootViewController = [[UIApplication sharedApplication].keyWindow rootViewController];
	[rootViewController presentViewController:navController animated:YES completion:nil];
	
	return 0;
}

//================================================================//
// MOAIHockeyAppIOS
//================================================================//

//----------------------------------------------------------------//
MOAIHockeyAppIOS::MOAIHockeyAppIOS () {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	mHockeyAppDelegate = [ MOAIHockeyAppIOSDelegate alloc ];
}

//----------------------------------------------------------------//
MOAIHockeyAppIOS::~MOAIHockeyAppIOS () {
	
	hockeyAppId = nil;
}

//----------------------------------------------------------------//
void MOAIHockeyAppIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable[] = {
		{ "init",					_init },
		{ "crash",					_crash },
		{ "disableCrashReporting",	_disableCrashReporting },
		{ "sendFeedback",			_sendFeedback },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
