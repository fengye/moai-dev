// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-cocoaLumberjack/host.h>

#include <moai-cocoalumberjack/MOAICocoaLumberjack.h>

//================================================================//
// aku-CocoaLumberjack
//================================================================//

//----------------------------------------------------------------//
void AKUCocoaLumberjackAppFinalize () {
}

//----------------------------------------------------------------//
void AKUCocoaLumberjackAppInitialize () {
}

//----------------------------------------------------------------//
void AKUCocoaLumberjackContextInitialize () {

	REGISTER_LUA_CLASS ( MOAICocoaLumberjack )
	MOAICocoaLumberjack::Affirm ();
}
