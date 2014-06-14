// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-hockeyapp/host.h>

#ifdef MOAI_OS_IPHONE
	#include <moai-iphone/MOAIHockeyAppIOS.h>
#endif

//================================================================//
// aku-hockeyapp
//================================================================//

//----------------------------------------------------------------//
void AKUHockeyAppAppFinalize () {
}

//----------------------------------------------------------------//
void AKUHockeyAppAppInitialize () {
}

//----------------------------------------------------------------//
void AKUHockeyAppContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIHockeyAppIOS )
}
