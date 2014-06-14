// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodSoundQueue.h>

#ifdef MOAI_OS_IPHONE
	#include <fmod_ios.h>
#endif

#ifdef MOAI_OS_NACL
#include <NaClFileSystem.h>
#include <moai_nacl.h>
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getVolume
	@text	Returns the current volume of the channel.
 
	@in		MOAIFmodSoundQueue self
	@out	float Volume - the volume currently set in this channel.
 */
int MOAIFmodSoundQueue::_getVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "U" )
	
	lua_pushnumber ( state, self->GetVolume() );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPaused
	@text	Return the pause status of the sound.
 
	@in		MOAIFmodSoundQueue self
	@out	boolean paused
 */
int MOAIFmodSoundQueue::_isPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "U" )
	
	lua_pushboolean ( state, self->mPaused );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
	@text	Returns true if channel is playing.
	
	@in		MOAIFmodSoundQueue self
	@out	boolean.
 */
int MOAIFmodSoundQueue::_isPlaying ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "U" )
	
	bool isPlaying = false;
	
	if ( !self->mPaused && self->mChannel ) {
		
		FMOD_RESULT result = self->mChannel->isPlaying ( &isPlaying );
		if ( result != FMOD_OK ) return 0;
	}
	
	lua_pushboolean ( state, isPlaying );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	moveVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.
 
	@in		MOAIFmodSoundQueue self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
 */
int MOAIFmodSoundQueue::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "UNN" )
	
	float delta		= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	
	if ( length > 0.0f ) {
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );
		
		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
		
		action->SetLink ( 0, self, MOAIFmodSoundQueueAttr::Pack ( ATTR_VOLUME ), delta, mode );
		
		action->SetSpan ( length );
		action->Start ();
		action->PushLuaUserdata ( state );
		
		return 1;
	}
	
	self->SetVolume ( self->GetVolume() + delta );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	addSound
	@text	enqueue a MOAIUntzSound into the queue
 
	@in		MOAIUntzSoundQueue self
	@in		MOAIUntzSound the sound to queue up
	@out	nil
 */
int MOAIFmodSoundQueue::_addSound ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "UU" )
	
	MOAIFmodSound* sound = state.GetLuaObject < MOAIFmodSound >( 2, false );
	if ( sound ) {
		// better retain the sound until we're done with it
		sound->Retain();
		self->mQueue.push_back( sound);
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	pause
	@text	Pause the channel.
 
	@in		MOAIFmodSoundQueue self
	@out	nil
 */
int MOAIFmodSoundQueue::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "U" )
	
//	self->SetPaused ( true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Plays the specified sound queue.
 
	@in		MOAIFmodSoundQueue		self
	@out	nil
 */
int MOAIFmodSoundQueue::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "U" )
	
	self->Play ( );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.
 
	@in		MOAIFmodSoundQueue self
	@in		number target			The target volume.
	@in		number length			Length of animation in seconds.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
	MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
 
	@out	MOAIEaseDriver easeDriver
 */
int MOAIFmodSoundQueue::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "UNN" )
	
	float target	= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	
	if ( length > 0.0f ) {
		
		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );
		
		action->SetLink ( 0, self, MOAIFmodSoundQueueAttr::Pack ( ATTR_VOLUME ), target - self->mVolume, mode );
		
		action->SetSpan ( length );
		action->Start ();
		action->PushLuaUserdata ( state );
		
		return 1;
	}
	
	self->SetVolume ( target );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Immediately sets the volume of this sound.
 
	@in		MOAIFmodSoundQueue	self
	@in		number volume	The volume of this sound.
	@out	nil
 */
int MOAIFmodSoundQueue::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "UN" )
	
	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops playing the sound on this channel.
 
	@in		MOAIFmodSoundQueue self
	@out	nil
 */
int MOAIFmodSoundQueue::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSoundQueue, "U" )
	
	self->Stop ();
	
	return 0;
}

//================================================================//
// MOAIFmodSoundQueue
//================================================================//
//----------------------------------------------------------------//

bool MOAIFmodSoundQueue::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	
	if ( MOAIFmodSoundQueueAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );
		
		if ( attrID == ATTR_VOLUME ) {
			
			this->SetVolume ( attrOp.Apply ( this->mVolume, op, MOAIAttrOp::ATTR_READ_WRITE ));
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
MOAIFmodSoundQueue::MOAIFmodSoundQueue () :
	mChannel ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false )  {

	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodSoundQueue::~MOAIFmodSoundQueue () {

//	this->Release ();
}

//----------------------------------------------------------------//
void MOAIFmodSoundQueue::Play () {
	
	this->Stop ();
	
	if ( this->mQueue.empty() ) return;
	
	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid()) return;
	
	FMOD::System* system;
	FMOD_RESULT result;
	result = soundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
		
	FMOD::Channel* channel = NULL;
	
	FMOD::Sound* sound = 0;
	FMOD_CREATESOUNDEXINFO info;
	memset ( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ));
	
	info.cbsize = sizeof ( FMOD_CREATESOUNDEXINFO );
    info.defaultfrequency = 44100;
    info.numsubsounds = this->mQueue.size();
    info.numchannels = 2;
    info.format = FMOD_SOUND_FORMAT_PCM16;
	result = system->createSound ( 0, FMOD_LOOP_OFF | FMOD_OPENUSER, &info, &sound );
	
	int i = 0;
	for (std::vector< MOAIFmodSound* >::const_iterator itr = this->mQueue.begin(); itr != this->mQueue.end(); ++itr) {
		
		result = sound->setSubSound( i++, ((MOAIFmodSound *)*(itr))->mSound );
	}
//	int soundlist[2] = { 0, 1 };
//	sound->setSubSoundSentence(soundlist, 2);
	this->mSound = sound;
	
	//printf ( "PLAY SOUND %s, @ %f\n", sound->GetFileName (), USDeviceTime::GetTimeInSeconds () );
	result = system->playSound ( this->mSound, 0, true, &channel );
	if ( result != FMOD_OK ) {
		
		printf (" FMOD ERROR: Sound did not play\n" );
		return;
	}
	
	this->mChannel = channel;
	this->SetVolume ( this->mVolume );
	this->SetPaused( false );
}

//----------------------------------------------------------------//
void MOAIFmodSoundQueue::SetPaused ( bool paused ) {
	
	this->mPaused = paused;
	
	if ( !this->mChannel ) return;
	
	this->mChannel->setPaused ( this->mPaused );
}


//----------------------------------------------------------------//
void MOAIFmodSoundQueue::SetVolume ( float volume ) {
	
	this->mVolume = volume;
	
	if ( !this->mChannel ) return;
	
	this->mChannel->setVolume ( this->mVolume );
}

//----------------------------------------------------------------//
float MOAIFmodSoundQueue::GetVolume () {
	
	return this->mVolume;
}

//----------------------------------------------------------------//
void MOAIFmodSoundQueue::Stop () {
	
	if ( !this->mChannel ) return;
	this->mChannel->stop ();
	
	// once a channel is stopped, it is not valid anymore
	this->mChannel = NULL;
}

//----------------------------------------------------------------//
void MOAIFmodSoundQueue::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodSoundQueueAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIFmodSoundQueue::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getVolume",			_getVolume },
		{ "moveVolume",			_moveVolume },
		{ "seekVolume",			_seekVolume },
		{ "setVolume",			_setVolume },
		{ "addSound",			_addSound },
		{ "isPaused",			_isPaused },
		{ "isPlaying",			_isPlaying },
		{ "pause",				_pause },
		{ "play",				_play },
		{ "stop",				_stop },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
