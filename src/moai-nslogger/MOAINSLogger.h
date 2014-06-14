#ifndef	MOAINSLOGGER_H
#define	MOAINSLOGGER_H

#if __APPLE__

#include <moai-core/headers.h>
#include <moai-sim/headers.h>

#include <Foundation/Foundation.h>
#import "DDNSLoggerLogger.h"

//================================================================//
// MOAINSLogger
//================================================================//
/**	@name	MOAINSLogger
	@text	NSLogger singleton wrapper
*/
class MOAINSLogger :
	public MOAIGlobalClass< MOAINSLogger, MOAILuaObject > {

private:
		
	//----------------------------------------------------------------//
	static int		_setRemoteHostName		( lua_State* L );
	static int		_setRemoteHostAddress	( lua_State* L );

	DDNSLoggerLogger	*mDNSLogger;
		
public:
		
	DECL_LUA_SINGLETON ( MOAINSLogger )

					MOAINSLogger		();
					~MOAINSLogger	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
};

#endif

#endif
