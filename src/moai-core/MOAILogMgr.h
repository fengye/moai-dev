// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAILOGMGR_H
#define	MOAILOGMGR_H

#include <moai-core/MOAIGlobals.h>
#include <moai-core/MOAILua.h>

#define MOAI_LUA_SETUP(type,str)										\
	MOAILuaState state ( L );											\
	type* self = MOAILogMgr::Get ().LuaSetup < type >( state, str );	\
	if ( !self ) return 0;

//================================================================//
// MOAILogMessage
//================================================================//
class MOAILogMessage {
	private:
	
	friend class MOAILogMgr;
	
	u32				mLevel;
	STLString		mFormatString;
};

//================================================================//
// MOAILogMgr
//================================================================//
/**	@name	MOAILogMgr
	@text	Singleton for managing debug log messages and log level.

	@const LOG_NONE
	@const LOG_ERROR
	@const LOG_WARNING
	@const LOG_STATUS
*/
class MOAILogMgr :
	public MOAIGlobalClass < MOAILogMgr, MOAILuaObject > {
private:

	typedef STLMap < u32, MOAILogMessage >::iterator MessageMapIt;
	STLMap < u32, MOAILogMessage > mMessageMap;

	u32			mLevel;
	FILE*		mFile;
	bool		mOwnsFileHandle;
	bool		mTypeCheckLuaParams;

	//----------------------------------------------------------------//
	static int		_closeFile					( lua_State* L );
	static int		_isDebugBuild				( lua_State* L );
	static int		_log						( lua_State* L );
	static int		_openFile					( lua_State* L );
	static int		_registerLogMessage			( lua_State* L );
	static int		_setLogLevel				( lua_State* L );
	static int		_setTypeCheckLuaParams		( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAILogMgr )
	
	enum {
		LOG_NONE,
		LOG_ERROR,
		LOG_WARNING,
		LOG_STATUS,
		LOG_DEBUG
	};
	
	GET ( FILE*, File, mFile )
	
	//----------------------------------------------------------------//
	void			CloseFile				();
	void			Log						( u32 level, cc8* tag, cc8* message, ... );
	void			Log						( lua_State *L, u32 level, cc8* tag,u32 messageID, ... );
	void			LogV					( u32 level, cc8* tag, cc8* message, va_list args );
	void			LogV					( lua_State *L, u32 level, cc8* tag,u32 messageID, va_list args );
					MOAILogMgr				();
					~MOAILogMgr				();
	void			OpenFile				( cc8* filename );
	void			RegisterLogMessage		( u32 messageID, u32 level, cc8* formatString );
	void			RegisterLuaClass		( MOAILuaState& state );
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	TYPE* LuaSetup ( MOAILuaState& state, cc8* typeStr ) {
	
		if ( this->mTypeCheckLuaParams ) {
			if ( !state.CheckParams ( 1, typeStr, true )) return 0;
		}
		TYPE* self = state.GetLuaObject < TYPE >( 1, true );
		return self;
	}
};

//================================================================//
// helpers
//================================================================//

extern void MOAILog		( u32 level, cc8* tag, cc8* message, ... );
extern void MOAILog		( lua_State *L, u32 level, cc8* tag, u32 messageID, ... );

#endif
