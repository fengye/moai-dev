// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSOUND_H
#define	MOAIFMODSOUND_H

#include <moai-sim/headers.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>

//================================================================//
// MOAIFmodSound
//================================================================//
/**	@name	MOAIFmodSound
	@text	Fmod sound object.
 
	@attr	ATTR_VOLUME
*/
class MOAIFmodSound :
	public virtual MOAINode {
private:
		
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;
	STLString mFileName;
		
	float	mVolume;
	bool	mPaused;
	bool	mLooping;
	
	//----------------------------------------------------------------//
	static int	_getLength			( lua_State* L );
	static int	_getPosition		( lua_State* L );
	static int	_getVolume			( lua_State* L );
	static int	_isLooping			( lua_State* L );
	static int	_isPaused			( lua_State* L );
	static int	_isPlaying			( lua_State* L );
	static int	_load				( lua_State* L );
	static int	_moveVolume			( lua_State* L );
	static int	_pause				( lua_State* L );
	static int	_play				( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int  _setLooping			( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int	_stop				( lua_State* L );
	static int	_release			( lua_State* L );

public:
		
	friend class MOAIFmodSoundQueue;

	DECL_LUA_FACTORY ( MOAIFmodSound )
	DECL_ATTR_HELPER ( MOAIFmodSound )
	
	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	bool		ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
				MOAIFmodSound			();
				~MOAIFmodSound			();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
		
	void		Load					( MOAIDataBuffer& data, bool streaming );
	void		Load					( cc8* filename, bool streaming, bool async );
	void		Play					( int loopCount );
	void		Stop					();
	void		SetLooping				( bool looping );
	void		SetPaused				( bool paused );
	void		SetVolume				( float volume );
	float		GetVolume				();
	void		Release					();
	STLString	ToString				();
};

#endif
