// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIO_H
#define	MOAIFMODSTUDIO_H

#include <moai-sim/headers.h>
#include <fmod_studio.hpp>

#ifdef MOAI_OS_IPHONE
#import <AudioToolbox/AudioServices.h>
#import <AudioToolbox/AudioToolbox.h>
#endif

struct FMOD_STUDIO_SYSTEM;
struct FMOD_CHANNELGROUP;

//================================================================//
// MOAIFmodStudio
//================================================================//
/**	@name	MOAIFmodStudio
	@text	FMOD singleton.
*/
class MOAIFmodStudio :
	public MOAIGlobalClass < MOAIFmodStudio, MOAIGlobalEventSource > {
private:
		
	typedef enum {
		RECORDABLE = 0x100,
		MIX_WITH_OTHERS = 0x200,
	} SystemOption;  // bitwise options
		
	static const u32 DEFAULT_SAMPLE_RATE = 44100;
	static const u32 DEFAULT_OPTIONS	 = 0;
	
	static FMOD::Studio::System* mSoundSys;
		
	FMOD::ChannelGroup*   mMainChannelGroup;
	u32					  mOptions;
	bool				  mIsActive;
	bool				  mInterruptedOnPlayback;

	//----------------------------------------------------------------//
	static int	_getMemoryStats		( lua_State* L );
	static int	_init				( lua_State* L );
	static int	_getOptions			( lua_State* L );
	static int	_setOptions			( lua_State* L );
	static int  _mute				( lua_State* L );
	static int	_setVolume			( lua_State* L );
	static int	_getVolume			( lua_State* L );

#ifdef MOAI_OS_IPHONE
	//----------------------------------------------------------------//
	static void audioPropertyListener ( void *inClientData,
									    AudioSessionPropertyID inID,
									    UInt32 inDataSize,
									    const void *inData );
	static void audioSessionInterruptionListener ( void *inClientData,
												  UInt32 inInterruptionState );
#endif
		
public:
	friend class MOAIFmodSound;
		
	DECL_LUA_SINGLETON ( MOAIFmodStudio )

	enum {
		AUDIOSESSION_DID_PAUSE,
		AUDIOSESSION_DID_RESUME
	};

	GET ( FMOD::Studio::System*, SoundSys, mSoundSys );
	GET ( FMOD::ChannelGroup*, MainChannelGroup, mMainChannelGroup );

	//----------------------------------------------------------------//
					MOAIFmodStudio		();
					~MOAIFmodStudio		();
	void			MuteChannels		( bool mute );
	void			SetVolume			( float volume );
	float			GetVolume			( );
	bool			IsActive			() { return mIsActive; }
	void			SetActive			(bool active);
	void			SetOptions			(u32 options, bool force = false);
	u32				GetOptions			() { return mOptions; }
	u32				GetAudioSessionCategory(u32 options);
	void			SetupAudioSessionCategory(u32 options);
	void			OpenSoundSystem		( float sample_rate, u32 options );
	void			CloseSoundSystem	();
	void			RegisterLuaClass	( MOAILuaState& state );
	void			RegisterLuaFuncs	( MOAILuaState& state );
	void			Update				();
	void			Resume				();
	void			Suspend				();
	STLString		ToString			();
		
	void			NotifyAudioSessionStateChange ( int event );
};

#endif