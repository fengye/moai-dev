//
//  MOAIMixpanelIOS.m
//  libmoai
//
//  Created by Sylvain Rebaud on 3/28/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//
#ifndef DISABLE_MIXPANEL

#include "pch.h"

#import <UIKit/UIKit.h>

#import "Mixpanel.h"
#import "moai-iphone/MOAIMixpanelIOS.h"
#import <moai-iphone/NSString+MOAILib.h>
#import <moai-iphone/NSDictionary+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Mixpanel.
 
	@in		string	token		Mixpanel token.
	@out 	nil
 */
int MOAIMixpanelIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	cc8* token_test = state.GetValue < cc8* >( 1, NULL );
	cc8* token_live = state.GetValue < cc8* >( 2, NULL );
	
	if (!token_test || !token_live) {
			
		lua_pushboolean ( state, false );
		return 1;
	}
	
#ifndef CONFIGURATION_Distribution
	[Mixpanel sharedInstanceWithToken:[ NSString stringWithUTF8String:token_test ]];
#else
	[Mixpanel sharedInstanceWithToken:[ NSString stringWithUTF8String:token_live ]];
#endif
	
	Mixpanel *mixpanel = [Mixpanel sharedInstance];
	NSString *distinctId = mixpanel.distinctId;
	if (distinctId) {
		[mixpanel identify:mixpanel.distinctId];
	} else {
		MOAIPrint( MOAILogMgr::LOG_ERROR, (char*)"Mixpanel distinctId is nil. Is it iOS5?" );
	}
	
	mixpanel.showNetworkActivityIndicator = NO;

	// Mixpanel 2.2.0 specifics
	//	mixpanel.checkForSurveysOnActive = YES;
	//	mixpanel.showSurveyOnActive = YES;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	trackEvent
	@text	Track Event.
 
	@in		string	event		Mixpanel event name.
	@in		table	data		Mixpanel event properties.
	@out 	nil
 */
int MOAIMixpanelIOS::_trackEvent ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	NSString* event = [[ NSString alloc ] initWithLua: state stackIndex: 1 ];
	if ( event == NULL ) {
	
		lua_pushboolean ( state, false );
		return 1;
	}

	NSMutableDictionary *properties = [ NSMutableDictionary dictionary ];
	if ( state.IsType ( 2, LUA_TTABLE )) {
		
		[ properties initWithLua:state stackIndex:2 ];
	}
	
	Mixpanel *mixpanel = [Mixpanel sharedInstance];
	[mixpanel track:event properties:properties];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	registerSuperProperties
	@text	Attach super properties to subsequent events
 
	@in		table	data		properties.
	@in		boolean once		if true, does not override previous values.
	@out 	nil
 */
int MOAIMixpanelIOS::_registerSuperProperties ( lua_State* L ) {
	
	MOAILuaState state ( L );
    
	if ( !state.IsType ( 1, LUA_TTABLE )) {
		lua_pushboolean ( state, false );
		return 1;
	}
	bool once = state.GetValue < bool >( 2, false );
	
	NSMutableDictionary *properties = nil;
	properties = [[ NSMutableDictionary alloc ] init ];
	[ properties initWithLua:state stackIndex:1 ];
	
	Mixpanel *mixpanel = [Mixpanel sharedInstance];
	if ( once) {
		
		[ mixpanel registerSuperPropertiesOnce:properties ];
	} else {
		
		[mixpanel registerSuperProperties:properties];
	}
	
	return 0;
}

//----------------------------------------------------------------//
MOAIMixpanelIOS::MOAIMixpanelIOS () {

}

//----------------------------------------------------------------//
MOAIMixpanelIOS::~MOAIMixpanelIOS () {

}

//----------------------------------------------------------------//
void MOAIMixpanelIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "init",					 _init },
		{ "trackEvent",				 _trackEvent },
		{ "registerSuperProperties", _registerSuperProperties },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif  //DISABLE_MIXPANEL