//
//  MOAIOmnitureIOS.h
//  libmoai
//
//  Created by Sylvain Rebaud on 7/5/13.
//  Copyright (c) 2012 Plumzi. All rights reserved.
//

#ifndef	MOAIOMNITURE_H
#define	MOAIOMNITURE_H

#import <moai-core/headers.h>

//================================================================//
// MOAIOmnitureIOS
//================================================================//
/**	@name	MOAIOmnitureIOS
	@text	Wrapper for Omniture integration on iOS devices.
 */
class MOAIOmnitureIOS :
	public MOAIGlobalClass < MOAIOmnitureIOS, MOAILuaObject > {
private:
	
	//----------------------------------------------------------------//
	static int	_init				( lua_State* L );
	static int	_trackCustomEvents	( lua_State* L );
	
public:
	
	DECL_LUA_SINGLETON ( MOAIOmnitureIOS );
		
			 MOAIOmnitureIOS		();
			 ~MOAIOmnitureIOS		();
	void	 RegisterLuaClass		( MOAILuaState& state );
};
		
#endif  //MOAIOMNITURE_H