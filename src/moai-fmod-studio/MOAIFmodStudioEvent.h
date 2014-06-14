// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIOEVENT_H
#define	MOAIFMODSTUDIOEVENT_H

#include <moai-sim/headers.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <fmod_studio.hpp>

//================================================================//
// MOAIFmodStudioEvent
//================================================================//
/**	@name	MOAIFmodStudioEvent
	@text	Fmod Studio EventInstance object.
*/
class MOAIFmodStudioEvent :
	public virtual MOAINode {
private:
		
	FMOD::Studio::EventInstance* mEventInstance;
		
	float	mVolume;
	bool	mPaused;
		
	static FMOD_RESULT Callback( FMOD_STUDIO_EVENT_CALLBACK_TYPE type, void *parameters );
		
	//----------------------------------------------------------------//'
	static int	_init				( lua_State* L );
	static int	_release			( lua_State* L );
	static int	_isValid			( lua_State* L );
	static int	_getVolume			( lua_State* L );
	static int	_isPaused			( lua_State* L );
	static int	_setPaused			( lua_State* L );
	static int  _isPlaying			( lua_State* L );
	static int	_moveVolume			( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int	_start				( lua_State* L );
	static int	_stop				( lua_State* L );
	static int	_setParameterValue  ( lua_State* L );
	static int	_getParameterValue	( lua_State* L );

public:

	friend class MOAIFmodStudio;

	DECL_LUA_FACTORY ( MOAIFmodStudioEvent )
	DECL_ATTR_HELPER ( MOAIFmodStudioEvent )

	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};

	//----------------------------------------------------------------//
	bool		ApplyAttrOp			( u32 attrID, MOAIAttrOp& attrOp, u32 op );
				MOAIFmodStudioEvent	();
				~MOAIFmodStudioEvent();
	bool		Init				( cc8* name );
	void		Release				();
	void		Start				();
	void		RegisterLuaClass	( MOAILuaState& state );
	void		RegisterLuaFuncs	( MOAILuaState& state );
	float		GetVolume			();
	void		SetVolume			( float volume );
	void		SetPaused			( bool paused );
	void		Stop				( bool immediate = true );
	bool		GetParameterValue	( cc8* name, float* value );
	bool		SetParameterValue	( cc8* name, float value );
	STLString	ToString			();
};

#endif
