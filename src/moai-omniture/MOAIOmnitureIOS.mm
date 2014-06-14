//
//  MOAIOmnitureIOS.m
//  libmoai
//
//  Created by Sylvain Rebaud on 3/28/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#include "pch.h"

//#import <UIKit/UIKit.h>

#import <Foundation/Foundation.h>
#import "moai-iphone/MOAIOmnitureIOS.h"
#import <moai-iphone/NSString+MOAILib.h>
#import <moai-iphone/NSDictionary+MOAILib.h>

#import "ADMS_Measurement.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Omniture.
 
	@in		string	rsid		Suite Name.
	@in		string	ts			Tracking Server.
	@out 	nil
 */
int MOAIOmnitureIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	cc8* rsid = state.GetValue < cc8* >( 1, "" );
	cc8* ts = state.GetValue < cc8* >( 2, "" );
	
	
	ADMS_Measurement *measurement = [ADMS_Measurement sharedInstance];
	
	// extra configs
	measurement.ssl = NO;
	measurement.debugLogging = YES;
	measurement.offlineTrackingEnabled = YES;
	
	[ measurement configureMeasurementWithReportSuiteIDs:[ NSString stringWithUTF8String:rsid ]
										  trackingServer:[ NSString stringWithUTF8String:ts ]];
	[ measurement setAutoTrackingOptions:ADMS_AutoTrackOptionsLifecycle ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	trackCustomEvents
	@text	Track Custom Events.
 
	@in		string	events		Omniture events variable.
	@in		table	data		Omniture context data variables.
	@out 	nil
 */
int MOAIOmnitureIOS::_trackCustomEvents ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	NSString* events = [[[ NSString alloc ] initWithLua: state stackIndex: 1 ] autorelease ];
	if ( events == NULL ) return 0;
	
	NSMutableDictionary *contextData = [ NSMutableDictionary dictionary ];
	if ( state.IsType ( 2, LUA_TTABLE )) {
		
		[ contextData initWithLua:state stackIndex:2 ];
	}
	
    ADMS_Measurement *measurement = [ADMS_Measurement sharedInstance];
	[measurement trackEvents:events
			 withContextData:contextData];
	
	return 0;
}

//----------------------------------------------------------------//
MOAIOmnitureIOS::MOAIOmnitureIOS () {

}

//----------------------------------------------------------------//
MOAIOmnitureIOS::~MOAIOmnitureIOS () {

}

//----------------------------------------------------------------//
void MOAIOmnitureIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "trackCustomEvents",	_trackCustomEvents },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}
