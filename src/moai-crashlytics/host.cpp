// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-crashlytics/host.h>

#ifdef MOAI_OS_IPHONE
	#include <moai-iphone/MOAICrashlyticsIOS.h>
#endif

//================================================================//
// aku-crashlytics
//================================================================//

//----------------------------------------------------------------//
void AKUCrashlyticsAppFinalize () {
}

//----------------------------------------------------------------//
void AKUCrashlyticsAppInitialize () {
}

//----------------------------------------------------------------//
void AKUCrashlyticsContextInitialize () {
	
#ifdef MOAI_OS_IPHONE
	REGISTER_LUA_CLASS ( MOAICrashlyticsIOS )
#endif
}