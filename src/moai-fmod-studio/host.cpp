// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/host.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioBank.h>
#include <moai-fmod-studio/MOAIFmodStudioEvent.h>
#include <moai-fmod-studio/MOAIFmodSound.h>
#include <moai-fmod-studio/MOAIFmodSoundQueue.h>

#ifdef MOAI_OS_IPHONE
	#include <fmod_ios.h>
#endif

#include <fmod.h>

//================================================================//
// AKU-fmod-studio
//================================================================//

//----------------------------------------------------------------//
void AKUFmodStudioAppFinalize () {
	
//	MOAIFmodStudio::Get ().CloseSoundSystem ();
}

//----------------------------------------------------------------//
void AKUFmodStudioAppInitialize () {
	
}

//----------------------------------------------------------------//
void AKUFmodStudioContextInitialize () {
	
	REGISTER_LUA_CLASS ( MOAIFmodStudio )
	MOAIFmodStudio::Affirm ();
	
	REGISTER_LUA_CLASS ( MOAIFmodSound )
	REGISTER_LUA_CLASS ( MOAIFmodSoundQueue )
	REGISTER_LUA_CLASS ( MOAIFmodStudioEvent )
	REGISTER_LUA_CLASS ( MOAIFmodStudioBank )

	#ifdef MOAI_OS_NACL
		MOAIFmodStudio::Get ().OpenSoundSystem ();
	#endif
}

////----------------------------------------------------------------//
//void AKUFmodMuteSystem ( bool mute ) {
//	
//	MOAIFmodStudio::Get().MuteChannels ( mute );
//}

//----------------------------------------------------------------//
void AKUFmodStudioPause() {
	
	MOAIFmodStudio::Get ().Suspend ();
}

//----------------------------------------------------------------//
void AKUFmodStudioResume() {
	
	MOAIFmodStudio::Get ().Resume ();
}

//----------------------------------------------------------------//
void AKUFmodStudioUpdate () {

	MOAIFmodStudio::Get ().Update ();
}
