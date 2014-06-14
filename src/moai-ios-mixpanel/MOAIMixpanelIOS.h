//
//  MOAIMixpanelIOS.h
//  libmoai
//
//  Created by Sylvain Rebaud on 7/5/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#ifndef	MOAIMIXPANEL_H
#define	MOAIMIXPANEL_H

#import <moai-core/headers.h>

//================================================================//
// MOAIMixpanelIOS
//================================================================//
/**	@name	MOAIMixpanelIOS
	@text	Wrapper for Mixpanel integration on iOS devices.
 */
class MOAIMixpanelIOS :
	public MOAIGlobalClass < MOAIMixpanelIOS, MOAILuaObject > {
private:
	
	//----------------------------------------------------------------//
	static int	_init						( lua_State* L );
	static int	_trackEvent					( lua_State* L );
	static int	_registerSuperProperties	( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIMixpanelIOS );
		
			 MOAIMixpanelIOS		();
			 ~MOAIMixpanelIOS		();
	void	 RegisterLuaClass		( MOAILuaState& state );
};
		
#endif  //MOAIMIXPANEL_H