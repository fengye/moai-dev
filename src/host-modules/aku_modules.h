// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	AKU_MODULES_H
#define	AKU_MODULES_H

#include <host-modules/aku_modules_config.h>
#include <host-modules/aku_modules_util.h>

#if AKU_WITH_AUDIOSAMPLER
	#include <moai-audiosampler/AKU-audiosampler.h>
#endif

#if AKU_WITH_BILLING
	#include <moai-billing/AKU-billing.h>
#endif

#if AKU_WITH_BOX2D
	#include <moai-box2d/host.h>
#endif

#if AKU_WITH_CHIPMUNK
	#include <moai-chipmunk/host.h>
#endif

#if AKU_WITH_COCOALUMBERJACK
	#include <moai-cocoalumberjack/host.h>
#endif

#if AKU_WITH_CRASHLYTICS
	#include <moai-crashlytics/host.h>
#endif

#if AKU_WITH_CRYPTO
	#include <moai-crypto/host.h>
#endif

#if AKU_WITH_FMOD_DESIGNER
	#include <moai-fmod-designer/host.h>
#endif

#if AKU_WITH_FMOD_EX
	#include <moai-fmod-ex/host.h>
#endif

#if AKU_WITH_FMOD_STUDIO
	#include <moai-fmod-studio/host.h>
#endif

#if AKU_WITH_HARNESS
	#include <moai-harness/host.h>
#endif

#if AKU_WITH_HTTP_CLIENT
	#include <moai-http-client/host.h>
#endif

#if AKU_WITH_HTTP_SERVER
	#include <moai-http-server/host.h>
#endif

#if AKU_WITH_HOCKEYAPP
	#include <moai-hockeyapp/host.h>
#endif

#if AKU_WITH_LUAEXT
	#include <moai-luaext/host.h>
#endif

#if AKU_WITH_OMNITURE
	#include <moai-omniture/host.h>
#endif

#if AKU_WITH_MIXPANEL
	#include <moai-mixpanel/host.h>
#endif

#if AKU_WITH_NSLOGGER
	#include <moai-nslogger/host.h>
#endif

#if AKU_WITH_PLUGINS
	#include <moai-plugins/host.h>
#endif

#if AKU_WITH_SIM
	#include <moai-sim/host.h>
#endif

#if AKU_WITH_TEST
	#include <moai-test/host.h>
#endif

#if AKU_WITH_UNTZ
	#include <moai-untz/host.h>
#endif

#if AKU_WITH_UTIL
	#include <moai-util/host.h>
#endif

#if AKU_WITH_PLUGINS
	#include <moai-plugins/host.h>
#endif

//----------------------------------------------------------------//
void		AKUModulesAppFinalize					();
void		AKUModulesAppInitialize					();
void		AKUModulesContextInitialize				();
void		AKUModulesParseArgs						( int argc, char** argv );
void		AKUModulesPause							( bool pause );
void		AKUModulesRunLuaAPIWrapper				();
void		AKUModulesUpdate						();

void		AKUModulesPause							();
void		AKUModulesResume						();
#endif
