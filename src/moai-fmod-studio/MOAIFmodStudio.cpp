// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
//#include <fmod.h>

#ifdef MOAI_OS_NACL
#include <fmodnacl.h>
#include "moai_nacl.h"
#include "ppapi/c/ppb.h"
#include "ppapi/cpp/instance.h"
#endif

#ifdef DEBUG
#define CHECKSTATUS(status) \
	while (0) { \
		if (status != noErr) { \
			fprintf(stderr, "audioio status: %d (%s: %d)\n", (int)status,  __FILE__, __LINE__); \
		} \
	}
#else
#define CHECKSTATUS(status) UNUSED(status)
#endif

FMOD::Studio::System* MOAIFmodStudio::mSoundSys = NULL;

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getMemoryStats
	@text	Get memory usage.

	@opt	boolean blocking - Default value is 'false.'
	@out	number currentAlloc
	@out	number maxAlloc
*/
int	MOAIFmodStudio::_getMemoryStats( lua_State* L ) {
	
	MOAIScopedLuaState state ( L );
	
	bool blocking = state.GetValue < bool >( 1, false );
	
	int currentAlloc;
	int maxAlloc;
	
	FMOD_Memory_GetStats ( &currentAlloc, &maxAlloc, blocking );
	
	lua_pushnumber ( state, currentAlloc );
	lua_pushnumber ( state, maxAlloc );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	init
	@text	Initializes the sound system.

	@out	nil
*/
int MOAIFmodStudio::_init ( lua_State* L ) {

	MOAIScopedLuaState state ( L );
	
#ifdef MOAI_OS_NACL
	printf ( "Cannot initialize fmod on background thread\n" );
	return -1;
#endif
	
	u32 sampleRate = state.GetValue ( 1, DEFAULT_SAMPLE_RATE );
	u32 options = state.GetValue ( 2, DEFAULT_OPTIONS );
	
	MOAIFmodStudio::Get ().OpenSoundSystem ( sampleRate, options );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	mute
	@text	Mute all sounds.
 
	@out	nil
 */
int MOAIFmodStudio::_mute ( lua_State* L ) {
	
	MOAIScopedLuaState state ( L );
	
	bool mute = state.GetValue < bool >( 1, false );
	MOAIFmodStudio::Get().MuteChannels ( mute );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Set the system level volume.
 
	@opt	number volume		Valid Range: 0 >= x <= 1.0 (Default value is 1.0)
	@out	nil
 */
int MOAIFmodStudio::_setVolume ( lua_State* L ) {
	
	MOAIScopedLuaState state ( L );
	
	float volume = ( float )state.GetValue ( 1, 1.0 );
	MOAIFmodStudio::Get().SetVolume ( volume );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getVolume
	@text	Return the system's current volume
 
	@out	number volume
 */
int MOAIFmodStudio::_getVolume ( lua_State* L ) {
	
	MOAIScopedLuaState state ( L );
	
	float volume = MOAIFmodStudio::Get().GetVolume ();
	lua_pushnumber ( L, volume );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setOptions
	@text	Sets the system's current options
 
	@int	number options
 */
int MOAIFmodStudio::_setOptions ( lua_State* L ) {
	
	MOAIScopedLuaState state ( L );
	
	u32 options = state.GetValue ( 1, DEFAULT_OPTIONS );
	MOAIFmodStudio::Get().SetOptions ( options );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getOptions
	@text	Return the system's current options
 
	@out	number options
	*/
int MOAIFmodStudio::_getOptions ( lua_State* L ) {
	
	u32 options = MOAIFmodStudio::Get().GetOptions ();
	lua_pushnumber ( L, options );
	
	return 1;
}

//================================================================//
// MOAIFmodStudio
//================================================================//

//----------------------------------------------------------------//
MOAIFmodStudio::MOAIFmodStudio () :
	mOptions ( -1 ),
	mMainChannelGroup ( 0 ),
	mIsActive( false ),
	mInterruptedOnPlayback ( false ) {
		
	RTTI_SINGLE ( MOAIGlobalEventSource )
}

//----------------------------------------------------------------//
MOAIFmodStudio::~MOAIFmodStudio () {

	this->CloseSoundSystem ();
}

//----------------------------------------------------------------//
void MOAIFmodStudio::MuteChannels ( bool mute ) {
	
	if ( !this->mMainChannelGroup ) return;
	
	this->mMainChannelGroup->setMute ( mute );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::SetVolume ( float volume ) {
	
	if ( !this->mMainChannelGroup ) return;
	
	this->mMainChannelGroup->setVolume ( volume );
}

//----------------------------------------------------------------//
float MOAIFmodStudio::GetVolume () {
	
	if ( !this->mMainChannelGroup ) return -1;
	
	float volume = -1;
	this->mMainChannelGroup->getVolume ( &volume );
	return volume;
}

#ifdef __cplusplus
extern "C" {
#endif
	
	F_DECLSPEC F_DLLEXPORT FMOD_CODEC_DESCRIPTION * F_API FMODGetBltCodecDescription();
	
#ifdef __cplusplus
}
#endif

#ifdef MOAI_OS_IPHONE

//----------------------------------------------------------------//
void MOAIFmodStudio::audioPropertyListener ( void *inClientData,
											 AudioSessionPropertyID inID,
											 UInt32 inDataSize,
											 const void *inData ) {
	
	u32 options = MOAIFmodStudio::Get ().GetOptions();
	
	// This is triggered by plugging a headset into a iPod touch
	if ( inID == kAudioSessionProperty_AudioInputAvailable ) {
		
		// Original options required recording but had no input then so
		// reinitialize session with original options
		if ( options & MOAIFmodStudio::RECORDABLE ) {
			
			MOAIFmodStudio::Get ().SetOptions ( options, true );
		}
	} else if ( inID == kAudioSessionProperty_AudioRouteChange ) {
		
		CFDictionaryRef routeChangeDictionary = (CFDictionaryRef)inData;
        CFNumberRef routeChangeReasonRef =
			(CFNumberRef)CFDictionaryGetValue( routeChangeDictionary,
											   CFSTR ( kAudioSession_AudioRouteChangeKey_Reason ));
		
        SInt32 routeChangeReason;
        CFNumberGetValue(routeChangeReasonRef, kCFNumberSInt32Type, &routeChangeReason);
		fprintf(stderr, "AudioRouteChange reason:%d\n", routeChangeReason);
		
		if ( routeChangeReason == kAudioSessionRouteChangeReason_OldDeviceUnavailable ) {
			
			fprintf(stderr, "Old device unavailable...\n");
		} else if ( routeChangeReason == kAudioSessionRouteChangeReason_CategoryChange ) {
			
			// when a video player starts, the category doesn't necessarily change
			// but we reactivate the session to force restart any active audio recorder
			// which are monitoring audio session changes.
			bool wasActive = MOAIFmodStudio::Get ().IsActive ();
			MOAIFmodStudio::Get ().SetActive ( false );
			
			UInt32 category;
			UInt32 ui32PropertySize = sizeof(category);
			OSStatus status = AudioSessionGetProperty(kAudioSessionProperty_AudioCategory,
													  &ui32PropertySize, &category);
			CHECKSTATUS(status);
			if ( status == 0 && category != MOAIFmodStudio::Get ().GetAudioSessionCategory ( options )) {
				
				fprintf(stderr, "category changed to: %d\n", category);
				MOAIFmodStudio::Get ().SetOptions ( options, true );
			}
			
			MOAIFmodStudio::Get ().SetActive(wasActive);
		}
	}
}

//----------------------------------------------------------------//
void MOAIFmodStudio::audioSessionInterruptionListener ( void *inClientData, UInt32 inInterruptionState ) {
	
	if ( inInterruptionState == kAudioSessionBeginInterruption && MOAIFmodStudio::Get ().IsActive ()) {
		
		fprintf(stderr, "Interruption Started, pausing...\n");
		MOAIFmodStudio::Get ().mInterruptedOnPlayback = MOAIFmodStudio::Get ().IsActive ();
		MOAIFmodStudio::Get ().SetActive ( false );
	} else if ( inInterruptionState == kAudioSessionEndInterruption && MOAIFmodStudio::Get ().mInterruptedOnPlayback ) {
		
		UInt32 interruptionType;
		UInt32 ui32PropertySize = sizeof(interruptionType);
		OSStatus status = AudioSessionGetProperty ( kAudioSessionProperty_InterruptionType,
												    &ui32PropertySize,
												    &interruptionType );
		
		if ( interruptionType == kAudioSessionInterruptionType_ShouldResume ) {
			
			fprintf(stderr, "Interruption Ended, resuming...\n");
			
			// reapply audio session category after interruption
			MOAIFmodStudio::Get ().SetupAudioSessionCategory ( MOAIFmodStudio::Get ().GetOptions());
			MOAIFmodStudio::Get ().mInterruptedOnPlayback = false;
			MOAIFmodStudio::Get ().SetActive ( true );
		}
	}
}
#endif

//----------------------------------------------------------------//
void MOAIFmodStudio::OpenSoundSystem ( float sample_rate, u32 options ) {

	FMOD_RESULT result;
	
	this->CloseSoundSystem ();

	FMOD::Debug_SetLevel( FMOD_DEBUG_LEVEL_ALL );
	result = FMOD::Studio::System::create( &this->mSoundSys );
	if ( result != FMOD_OK ) return;
	
	FMOD::System* system;
	result = this->mSoundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
	
	FMOD_SPEAKERMODE mode;
	int samplerate, numrawspeakers;
	result = system->getSoftwareFormat ( &samplerate, &mode, &numrawspeakers );
	if ( result != FMOD_OK ) return;

	result = system->setSoftwareFormat ( sample_rate, FMOD_SPEAKERMODE_STEREO, numrawspeakers );
	if ( result != FMOD_OK ) return;
	
#ifdef MOAI_OS_NACL

	FMOD_NACL_EXTRADRIVERDATA extraDriverData;
	memset ( &extraDriverData, 0, sizeof(FMOD_NACL_EXTRADRIVERDATA) );
	extraDriverData.instance = g_instance->pp_instance ();

	this->mSoundSys->setDSPBufferSize( 1024, 4 );
	result = this->mSoundSys.initialize ( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, &extraDriverData );
#else
	
	result = this->mSoundSys->initialize ( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0 );
#endif
	if ( result != FMOD_OK ) return;
	
	result = system->getSoftwareFormat ( &samplerate, &mode, &numrawspeakers );
	
    unsigned int mycodec;
    result = system->registerCodec ( FMODGetBltCodecDescription(), &mycodec, 1000 );
	if ( result != FMOD_OK ) return;
	
	result = system->getMasterChannelGroup ( &this->mMainChannelGroup );
	if ( result != FMOD_OK ) return;
	
#ifdef MOAI_OS_IPHONE
    // Set up the audio session interruption callback.
    OSStatus status = AudioSessionInitialize ( NULL, NULL, audioSessionInterruptionListener, NULL );
    CHECKSTATUS(status);
	
    // Listen for audio input changes
    status = AudioSessionAddPropertyListener ( kAudioSessionProperty_AudioInputAvailable,
											   audioPropertyListener,
											   NULL );
    CHECKSTATUS ( status );
	
    // Listen for route changes
    status = AudioSessionAddPropertyListener ( kAudioSessionProperty_AudioRouteChange,
											   audioPropertyListener,
											   NULL );
    CHECKSTATUS ( status );
#endif
	
	this->SetOptions ( options );
	this->Resume ();
}

//----------------------------------------------------------------//
void MOAIFmodStudio::CloseSoundSystem () {
	
	if ( !this->mSoundSys || !this->mSoundSys->isValid()) return;
	
	if ( this->mMainChannelGroup ) {
		
		int numChannels;
		this->mMainChannelGroup->getNumChannels ( &numChannels );
		for ( int i=0; i<numChannels; i++ ) {
			FMOD::Channel* channel;
			this->mMainChannelGroup->getChannel ( i, &channel );
			channel->stop ();
		}
		
		this->mMainChannelGroup->release ();
		this->mMainChannelGroup = NULL;
	}
	
	this->mSoundSys->release ();
	this->mSoundSys = NULL;
}

//----------------------------------------------------------------//
void MOAIFmodStudio::SetOptions ( u32 newOptions, bool force ) {
	
	if ( newOptions != mOptions || force ) {
		
		mOptions = newOptions;
		bool wasActive = mIsActive;
		this->SetActive ( false );
		
		this->SetupAudioSessionCategory ( mOptions );
		
		// reactivate session if previously active
		if ( wasActive ) {
			this->SetActive ( true );
		}
	}
}

//----------------------------------------------------------------//
void MOAIFmodStudio::SetActive ( bool active ) {
	
	if ( active && mInterruptedOnPlayback ) {
		fprintf ( stderr, "was interrupted, waiting for interruption handler\n" );
		return;
	}
	
#ifndef MACOSX
	OSStatus result = AudioSessionSetActive ( active );
	CHECKSTATUS ( result );
#endif
	
	if ( active && !mIsActive ) {
		
		this->NotifyAudioSessionStateChange ( AUDIOSESSION_DID_RESUME );
	} else if ( !active && mIsActive ) {
		
		this->NotifyAudioSessionStateChange ( AUDIOSESSION_DID_PAUSE );
	}
		
	mIsActive = active;
}

//----------------------------------------------------------------//
u32 MOAIFmodStudio::GetAudioSessionCategory ( u32 options ) {
	
#ifndef MACOSX
	// Check for audio input
	UInt32 inputAvailable;
	UInt32 ui32PropertySize = sizeof ( inputAvailable );
	OSStatus status = AudioSessionGetProperty ( kAudioSessionProperty_AudioInputAvailable,
											    &ui32PropertySize,
											    &inputAvailable );
    CHECKSTATUS(status);
	
	// Set up the audio session category
	UInt32 category = kAudioSessionCategory_SoloAmbientSound;
	if ( options & MOAIFmodStudio::RECORDABLE && inputAvailable ) {
		
		category = kAudioSessionCategory_PlayAndRecord;
	} else if ( options & MOAIFmodStudio::MIX_WITH_OTHERS ) {
		
		category = kAudioSessionCategory_AmbientSound;
	}
	return category;
#else
	return 0;
#endif
}

//----------------------------------------------------------------//
void MOAIFmodStudio::SetupAudioSessionCategory ( u32 options ) {
	
#ifndef MACOSX
	// Set up the audio session category
	UInt32 category = GetAudioSessionCategory ( options );
    OSStatus status = AudioSessionSetProperty ( kAudioSessionProperty_AudioCategory,
											    sizeof ( category ),
											    &category );
    CHECKSTATUS ( status );
	
	// Set audio session property "allow mixing" to true so audio can be recorded while it is playing
	UInt32 allowMixing = options & MOAIFmodStudio::MIX_WITH_OTHERS ? 1 : 0;
	status = AudioSessionSetProperty ( kAudioSessionProperty_OverrideCategoryMixWithOthers,
									   sizeof ( allowMixing ),
									   &allowMixing );
	CHECKSTATUS ( status );
	
	// Set up the session mode
	UInt32 mode = kAudioSessionMode_Default;
	status = AudioSessionSetProperty ( kAudioSessionProperty_Mode,
									   sizeof ( mode ),
									   &mode );
	CHECKSTATUS ( status );
	
#if !TARGET_IPHONE_SIMULATOR
	// Set up to allow bluetooth inputs
	UInt32 allowBluetoothInput = 1;
	status = AudioSessionSetProperty ( kAudioSessionProperty_OverrideCategoryEnableBluetoothInput,
									   sizeof ( allowBluetoothInput ),
									   &allowBluetoothInput );
	CHECKSTATUS ( status );
	
	// Make the default sound route for the session be to use the speaker
	// instead of receiver when kAudioSessionCategory_PlayAndRecord category is used
	// This persists through interruptions and route changes such as when
	// plugging/unplugging headsets.
	UInt32 doChangeDefaultRoute = 1;
	status = AudioSessionSetProperty ( kAudioSessionProperty_OverrideCategoryDefaultToSpeaker,
									   sizeof ( doChangeDefaultRoute ),
									   &doChangeDefaultRoute );
	CHECKSTATUS ( status );
#endif
#endif
}

//----------------------------------------------------------------//
void MOAIFmodStudio::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAILuaObject::RegisterLuaClass( state );

	state.SetField ( -1, "AUDIOSESSION_DID_PAUSE",	( u32 )AUDIOSESSION_DID_PAUSE );
	state.SetField ( -1, "AUDIOSESSION_DID_RESUME", ( u32 )AUDIOSESSION_DID_RESUME );
	
	state.SetField ( -1, "RECORDABLE",      ( u32 )MOAIFmodStudio::RECORDABLE ); // bitwise
	state.SetField ( -1, "MIX_WITH_OTHERS", ( u32 )MOAIFmodStudio::MIX_WITH_OTHERS ); // bitwise
	
	luaL_Reg regTable [] = {
		{ "getMemoryStats",		_getMemoryStats },
		{ "init",				_init },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIFmodStudio > },
		{ "mute",				_mute },
		{ "setVolume",			_setVolume },
		{ "getVolume",			_getVolume },
		{ "setOptions",			_setOptions },
		{ "getOptions",			_getOptions },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::NotifyAudioSessionStateChange ( int event ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( event, state )) {
		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Suspend () {
	
	if ( !this->mSoundSys || !this->mSoundSys->isValid()) return;
	
	FMOD::System* system;
	FMOD_RESULT result = this->mSoundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
	
	system->mixerSuspend ();
	
	this->SetActive ( false );
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Resume () {
	
	this->SetActive ( true );
	
	if ( !this->mSoundSys || !this->mSoundSys->isValid()) return;
	
	FMOD::System* system;
	FMOD_RESULT result = this->mSoundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
	
	system->mixerResume ();
}

//----------------------------------------------------------------//
void MOAIFmodStudio::Update () {
	
	if ( !this->mSoundSys || !this->mSoundSys->isValid()) return;
	
	this->mSoundSys->update ();
}

