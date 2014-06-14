// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-nslogger/host.h>

#include <moai-nslogger/MOAINSLogger.h>

//================================================================//
// aku-NSLogger
//================================================================//

//----------------------------------------------------------------//
void AKUNSLoggerAppFinalize () {
}

//----------------------------------------------------------------//
void AKUNSLoggerAppInitialize () {
}

//----------------------------------------------------------------//
void AKUNSLoggerContextInitialize () {

	REGISTER_LUA_CLASS ( MOAINSLogger )
	MOAINSLogger::Affirm ();
}
