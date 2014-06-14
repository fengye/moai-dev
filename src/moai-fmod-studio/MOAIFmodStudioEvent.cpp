// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioEvent.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Lookup the specified event name.

	@in		MOAIFmodStudioBank	self
	@in		string name			The name of the event.
	@out	nil
 */
int MOAIFmodStudioEvent::_init ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "US" )

	cc8* name = state.GetValue < cc8* >( 2, "" );
	bool result = self->Init( name );

	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getVolume
	@text	Returns the current volume of the event.

	@in		MOAIFmodStudioEvent self
	@out	float Volume - the volume currently set in this event.
 */
int MOAIFmodStudioEvent::_getVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )

	lua_pushnumber ( state, self->GetVolume () );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isValid
	@text	Return the validity status of the event.
 
	@in		MOAIFmodStudioEvent self
	@out	boolean vald
 */
int MOAIFmodStudioEvent::_isValid ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )
	
	lua_pushboolean ( state, self->mEventInstance && self->mEventInstance->isValid() );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPaused
	@text	Return the pause status of the event.

	@in		MOAIFmodStudioEvent self
	@out	boolean paused
 */
int MOAIFmodStudioEvent::_isPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )

	lua_pushboolean ( state, self->mPaused );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
	@text	Returns true if event is playing.

	@in		MOAIFmodStudioEvent self
	@out	boolean.
 */
int MOAIFmodStudioEvent::_isPlaying ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )

	bool isPlaying = false;

	if ( !self->mPaused && self->mEventInstance->isValid()) {

		FMOD_STUDIO_PLAYBACK_STATE state;
		FMOD_RESULT result = self->mEventInstance->getPlaybackState( &state );
		if ( result != FMOD_OK )
			return 0;

		isPlaying = state != FMOD_STUDIO_PLAYBACK_IDLE &&
					state != FMOD_STUDIO_PLAYBACK_STOPPED;
	}

	lua_pushboolean ( state, isPlaying );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	moveVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodStudioEvent		self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
 */
int MOAIFmodStudioEvent::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "UNN" )

	float delta		= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );

	if ( length > 0.0f ) {

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );

		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );

		action->SetLink ( 0, self, MOAIFmodStudioEventAttr::Pack ( ATTR_VOLUME ), delta, mode );

		action->SetSpan ( length );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}

	self->SetVolume( self->GetVolume() + delta );
	self->ScheduleUpdate();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.

	@in		MOAIFmodStudioEvent self
	@in		number target			The target volume.
	@in		number length			Length of animation in seconds.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
 MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.

	@out	MOAIEaseDriver easeDriver
 */
int MOAIFmodStudioEvent::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "UNN" )

	float target	= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );

	if ( length > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );

		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );

		action->SetLink ( 0, self, MOAIFmodStudioEventAttr::Pack ( ATTR_VOLUME ), target - self->mVolume, mode );

		action->SetSpan ( length );
		action->Start ();
		action->PushLuaUserdata ( state );

		return 1;
	}

	self->SetVolume( target );
	self->ScheduleUpdate();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	start
	@text	Starts the event.

	@in		MOAIFmodStudioEvent self
	@out	nil
*/
int MOAIFmodStudioEvent::_start ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )

	self->Start ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops playing the sound on this event.

	@in		MOAIFmodStudioEvent self
	@in		boolean immediate	Whether this event should immediately stop or fade out.
	@out	nil
*/
int MOAIFmodStudioEvent::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )

	bool immediate = state.GetValue < bool >( 2, true );
	self->Stop ( immediate );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Immediately sets the volume of this event.

	@in		MOAIFmodStudioEvent self
	@in		number volume		The volume of this event.
	@out	nil
 */
int MOAIFmodStudioEvent::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "UN" )

	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPaused
	@text	Sets whether this event is paused and hence does not play any sounds.

	@in		MOAIFmodStudioEvent		self
	@in		boolean paused			Whether this event is paused.
	@out	nil
 */
int MOAIFmodStudioEvent::_setPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "UB" )

	bool paused = state.GetValue < bool >( 2, false );
	self->SetPaused ( paused );

	return 0;
}
//----------------------------------------------------------------//
/**	@name	setParameterValue
	@text	Sets value associated with a event parameter.

	@in		MOAIFmodStudioEvent		self
	@in		string name				The name of the parameter.
	@in		float value				Parameter value.
	@out	nil
 */
int MOAIFmodStudioEvent::_setParameterValue  ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "USN" )

	cc8* name	= state.GetValue < cc8* >( 2, "" );
	float value	= state.GetValue < float >( 3, 1.0f );

	bool result = self->SetParameterValue ( name, value );
	lua_pushboolean ( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getParameterValue
	@text	Gets value associated to a event parameter.

	@in		MOAIFmodStudioEvent		self
	@in		string name				The name of the parameter.
	@out	float value				Parameter value
 */
int MOAIFmodStudioEvent::_getParameterValue	( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "US" )

	cc8* name = state.GetValue < cc8* >( 2, "" );
	float value;
	bool result = self->GetParameterValue ( name, &value );

	if ( result ) {
		lua_pushnumber ( state, value );
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases the event data from memory.
 
	@in		MOAIFmodStudioEvent self
	@out	nil
 */
int MOAIFmodStudioEvent::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioEvent, "U" )
	
	self->Release ();
	
	return 0;
}

//================================================================//
// MOAIFmodStudioEvent
//================================================================//

//----------------------------------------------------------------//
bool MOAIFmodStudioEvent::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAIFmodStudioEventAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );

		if ( attrID == ATTR_VOLUME ) {
			this->mVolume = attrOp.Apply ( this->mVolume, op, MOAIAttrOp::ATTR_READ_WRITE );
			this->SetVolume ( this->mVolume );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
float MOAIFmodStudioEvent::GetVolume () {
	return this->mVolume;
}

//----------------------------------------------------------------//
MOAIFmodStudioEvent::MOAIFmodStudioEvent () :
	mEventInstance ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false ) {

	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodStudioEvent::~MOAIFmodStudioEvent () {

	this->Release();
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::Release () {

	if ( this->mEventInstance && this->mEventInstance->isValid() ) {
		FMOD::Studio::EventDescription *description = NULL;
		this->mEventInstance->getDescription(&description);
		
		FMOD_STUDIO_LOADING_STATE state;
		this->mEventInstance->getLoadingState(&state);
		if (state == FMOD_STUDIO_LOADING_STATE_LOADING || state == FMOD_STUDIO_LOADING_STATE_LOADED) {
			
			this->mEventInstance->stop ( FMOD_STUDIO_STOP_IMMEDIATE );
			this->mEventInstance->release ();
		}
		
		// Unload sample data as soon as the last event instance has been reset
		int count;
		if (description && description->getInstanceCount(&count) == FMOD_OK && count == 0) {
			description->unloadSampleData();
		}
		
		this->mEventInstance = NULL;
	}
}

//----------------------------------------------------------------//
FMOD_RESULT MOAIFmodStudioEvent::Callback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, void *parameters) {
	FMOD_RESULT result;

	if ( type == FMOD_STUDIO_EVENT_CALLBACK_STOPPED) {
//		FMOD_STUDIO_EVENT_INSTANCE_HANDLE* handle = (FMOD_STUDIO_EVENT_INSTANCE_HANDLE*)parameters;
//		FMOD_Studio_EventInstance_Stop( (FMOD_STUDIO_EVENTINSTANCE*)handle, FMOD_STUDIO_STOP_IMMEDIATE );

	}

	return FMOD_OK;
}

//----------------------------------------------------------------//
bool MOAIFmodStudioEvent::Init ( cc8* name ) {

	if ( this->mEventInstance->isValid () ) return false;

	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid() ) return false;

	FMOD_RESULT result;
	
    FMOD::Studio::ID eventID = {0};
	result = soundSys->lookupID( name, &eventID );
	if ( result != FMOD_OK ) return false;

	FMOD::Studio::EventDescription* eventDescription = NULL;
	result = soundSys->getEvent( &eventID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription );
	if ( result != FMOD_OK ) return false;

	result = eventDescription->createInstance( &this->mEventInstance );
	if ( result != FMOD_OK ) return false;

//	result = this->mEventInstance->setCallback( Callback );
//	if ( result != FMOD_OK ) return false;

	return true;
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::Start ( ) {

	this->Stop ();
	
	if ( !this->mEventInstance->isValid () ) return;

	this->mEventInstance->start ( );
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::SetPaused ( bool paused ) {

	this->mPaused = paused;
	if ( !this->mEventInstance->isValid () ) return;

	this->mEventInstance->setPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::SetVolume ( float volume ) {

	this->mVolume = volume;
	if ( !this->mEventInstance->isValid () ) return;

	this->mEventInstance->setVolume ( this->mVolume );
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::Stop ( bool immediate ) {

	if ( !this->mEventInstance->isValid() ) return;

	this->mEventInstance->stop ( immediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT );
}


//----------------------------------------------------------------//
bool MOAIFmodStudioEvent::SetParameterValue ( cc8* name, float value ) {

	if ( !this->mEventInstance->isValid () ) {
		return false;
	}

    FMOD::Studio::ParameterInstance* parameter;
	FMOD_RESULT result = this->mEventInstance->getParameter ( name, &parameter );
	if ( result != FMOD_OK ) return false;

	result = parameter->setValue ( value );
	if ( result != FMOD_OK ) return false;

	return true;
}

//----------------------------------------------------------------//
bool MOAIFmodStudioEvent::GetParameterValue ( cc8* name, float* value ) {

	if ( !this->mEventInstance->isValid () ) {
		return false;
	}

    FMOD::Studio::ParameterInstance* parameter;
	FMOD_RESULT result = this->mEventInstance->getParameter ( name, &parameter );
	if ( result != FMOD_OK ) return false;

	result = parameter->getValue ( value );
	if ( result != FMOD_OK ) return false;

	return true;
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::RegisterLuaClass ( MOAILuaState& state ) {

	MOAINode::RegisterLuaClass ( state );

	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodStudioEventAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIFmodStudioEvent::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAINode::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "release",			_release },
		{ "isValid",			_isValid },
		{ "getVolume",			_getVolume },
		{ "isPaused",			_isPaused },
		{ "isPlaying",			_isPlaying },
		{ "moveVolume",			_moveVolume },
		{ "start",				_start },
		{ "seekVolume",			_seekVolume },
		{ "setPaused",			_setPaused },
		{ "setVolume",			_setVolume },
		{ "stop",				_stop },
		{ "setParameterValue",	_setParameterValue },
		{ "getParameterValue",	_getParameterValue },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

