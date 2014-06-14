// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-sim/headers.h>
#include <moai-billing/AKU-billing.h>

#if defined(MOAI_OS_IPHONE) || defined(MOAI_OS_OSX)
#include <moai-billing/MOAIBilling.h>
#elif defined(MOAI_OS_ANDROID)
#include <moai-billing/MOAIBillingAndroid.h>
#endif

//================================================================//
// aku-billing
//================================================================//

//----------------------------------------------------------------//
void AKUBillingAppFinalize () {
}

//----------------------------------------------------------------//
void AKUBillingAppInitialize () {
}

//----------------------------------------------------------------//
void AKUBillingContextInitialize () {
	
#if defined(MOAI_OS_IPHONE) || defined(MOAI_OS_OSX)
    REGISTER_LUA_CLASS ( MOAIBilling )
	MOAIBilling::Affirm();
#elif defined(MOAI_OS_ANDROID)
    REGISTER_LUA_CLASS ( MOAIBillingAndroid )
#endif
}

