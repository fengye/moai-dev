#include <moai-nslogger/MOAINSLogger.h>
#include "DDLog.h"

//----------------------------------------------------------------//
int MOAINSLogger::_setRemoteHostName(lua_State* L)
{
	MOAILuaState state ( L );
	
	cc8* name = state.GetValue < cc8* >( 1, "" );
	[[DDNSLoggerLogger sharedInstance] setupWithBonjourServiceName:[NSString stringWithUTF8String:name]];
	return 0;
}

//----------------------------------------------------------------//
int MOAINSLogger::_setRemoteHostAddress(lua_State* L)
{
	return 0;
}

//----------------------------------------------------------------//
MOAINSLogger::MOAINSLogger ()
{
    RTTI_SINGLE ( MOAILuaObject )
	
	// Network logger
	[DDLog addLogger:[DDNSLoggerLogger sharedInstance]];
}

//----------------------------------------------------------------//
MOAINSLogger::~MOAINSLogger ()
{
}

//----------------------------------------------------------------//
void MOAINSLogger::RegisterLuaClass ( MOAILuaState& state )
{
	luaL_Reg regTable[] = {
		{ "setRemoteHostName", _setRemoteHostName },
		{ "setRemoteHostAddress", _setRemoteHostAddress },
		{ NULL, NULL }
	};
	
	luaL_register(state, 0, regTable);
}

//----------------------------------------------------------------//
void MOAINSLogger::RegisterLuaFuncs ( MOAILuaState& state )
{
	UNUSED(state);
}

