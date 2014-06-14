// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-mixpanel/host.h>

#ifdef MOAI_OS_IPHONE
	#include <moai-iphone/MOAIMixpanelIOS.h>
#endif

//================================================================//
// aku-mixpanel
//================================================================//

//----------------------------------------------------------------//
void AKUMixpanelAppFinalize () {
}

//----------------------------------------------------------------//
void AKUMixpanelAppInitialize () {
}

//----------------------------------------------------------------//
void AKUMixpanelContextInitialize () {
	
	REGISTER_LUA_CLASS ( MOAIMixpanelIOS )
}
