// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSOUNDQUEUE_H
#define	MOAIFMODSOUNDQUEUE_H

#include <moai-sim/headers.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodSound.h>

//================================================================//
// MOAIFmodSound
//================================================================//
/**	@name	MOAIFmodSoundQueue
	@text	Fmod sound queue abstract object.
 
	@attr	ATTR_VOLUME
*/
class MOAIFmodSoundQueue :
	public virtual MOAINode {
private:

	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;
	std::vector< MOAIFmodSound* > mQueue;
		
	float	mVolume;
	bool	mPaused;
	
	//----------------------------------------------------------------//
	static int	_getVolume			( lua_State* L );
	static int	_moveVolume			( lua_State* L );
	static int	_seekVolume			( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int  _addSound			( lua_State* L );
	static int	_isPaused			( lua_State* L );
	static int	_isPlaying			( lua_State* L );
	static int	_pause				( lua_State* L );
	static int	_play				( lua_State* L );
	static int	_stop				( lua_State* L );
	
public:

	DECL_LUA_FACTORY ( MOAIFmodSoundQueue )
	DECL_ATTR_HELPER ( MOAIFmodSoundQueue )
	
	enum {
		ATTR_VOLUME,
		TOTAL_ATTR,
	};
	
	//----------------------------------------------------------------//
	bool		ApplyAttrOp				( u32 attrID, MOAIAttrOp& attrOp, u32 op );
				MOAIFmodSoundQueue		();
				~MOAIFmodSoundQueue		();
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
		
	void		Play					();
	void		Stop					();
	void		SetPaused				( bool paused );
	void		SetVolume				( float volume );
	float		GetVolume				();
};

#endif
