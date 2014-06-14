//
//  MOAIHockeyAppIOS.h
//  libmoai
//
//  Created by Sylvain Rebaud on 3/28/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#ifndef	MOAIHOCKEYAPPIOS_H
#define	MOAIHOCKEYAPPIOS_H

#import <moai-core/headers.h>

@class MOAIHockeyAppIOSDelegate;

//================================================================//
// MOAIHockeyAppIOS
//================================================================//
/**	@name	MOAIHockeyAppIOS
	@text	Wrapper for HockeyApp integration on iOS devices.
	HockeyApp provides real-time, actionable crash reports
	for mobile apps. Exposed to lua via MOAIHockeyApp on
	all mobile platforms.
 */
class MOAIHockeyAppIOS :
	public MOAIGlobalClass < MOAIHockeyAppIOS, MOAILuaObject > {
private:
		
	MOAIHockeyAppIOSDelegate*		mHockeyAppDelegate;
		
	//----------------------------------------------------------------//
	static int	_init					( lua_State* L );
	static int	_crash					( lua_State* L );
	static int	_disableCrashReporting 	( lua_State* L );
	static int	_sendFeedback			( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIHockeyAppIOS );
	
			MOAIHockeyAppIOS		();
			~MOAIHockeyAppIOS		();
	void	RegisterLuaClass		( MOAILuaState& state );
};


#endif  //MOAIHOCKEYAPPIOS_H
