//
//  MOAICrashlyticsIOS.h
//  libmoai
//
//  Created by Sylvain Rebaud on 3/28/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#ifndef	MOAICRASHLYTICSIOS_H
#define	MOAICRASHLYTICSIOS_H

#import <moai-core/headers.h>

//================================================================//
// MOAICrashlyticsIOS
//================================================================//
/**	@name	MOAICrashlyticsIOS
	@text	Wrapper for Crashlytics integration on iOS devices.
	Crashlytics provides real-time, actionable crash reports
	for mobile apps. Exposed to lua via MOAICrashlytics on
	all mobile platforms.
 */
class MOAICrashlyticsIOS :
	public MOAIGlobalClass < MOAICrashlyticsIOS, MOAILuaObject > {
private:
		
	//----------------------------------------------------------------//
	static int	_init 				( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAICrashlyticsIOS );
	
			MOAICrashlyticsIOS		();
			~MOAICrashlyticsIOS		();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //MOAICRASHLYTICSIOS_H
