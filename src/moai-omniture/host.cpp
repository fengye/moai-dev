// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-omniture/host.h>

#ifdef MOAI_OS_IPHONE
	#include <moai-iphone/MOAIOmnitureIOS.h>
#endif

//================================================================//
// aku-omniture
//================================================================//

//----------------------------------------------------------------//
void AKUOmnitureAppFinalize () {
}

//----------------------------------------------------------------//
void AKUOmnitureAppInitialize () {
}

//----------------------------------------------------------------//
void AKUOmnitureContextInitialize () {

	REGISTER_LUA_CLASS ( MOAIOmnitureIOS )
}
