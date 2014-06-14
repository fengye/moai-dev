// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <string.h>
#include <host-modules/aku_modules.h>
#include <host-modules/aku_modules_custom.h>

//================================================================//
// implementation
//================================================================//

//----------------------------------------------------------------//
void AKUModulesAppFinalize () {
	
	#if AKU_WITH_AUDIOSAMPLER
		AKUAudioSamplerAppFinalize ();
	#endif
	
	#if AKU_WITH_BILLING
		AKUBillingAppFinalize ();
	#endif

	#if AKU_WITH_BOX2D
		AKUBox2DAppFinalize ();
	#endif

	#if AKU_WITH_CHIPMUNK
		AKUChipmunkAppFinalize ();
	#endif

	#if AKU_WITH_COCOALUMBERJACK
		AKUCocoaLumberjackAppFinalize ();
	#endif
	
	#if AKU_WITH_CRASHLYTICS
		AKUCrashlyticsAppFinalize ();
	#endif

	#if AKU_WITH_CRYPTO
		AKUCryptoAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExAppFinalize ();
	#endif

	#if AKU_WITH_FMOD_STUDIO
		AKUFmodStudioAppFinalize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessAppFinalize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientAppFinalize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerAppFinalize ();
	#endif

	#if AKU_WITH_HOCKEYAPP
		AKUHockeyAppAppFinalize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtAppFinalize ();
	#endif

	#if AKU_WITH_NSLOGGER
		AKUNSLoggerAppFinalize ();
	#endif
		
	#if AKU_WITH_MIXPANEL
		AKUMixpanelAppFinalize ();
	#endif
		
	#if AKU_WITH_OMNITURE
		AKUOmnitureAppFinalize ();
	#endif
		
	#if AKU_WITH_PLUMZI
		AKUPlumziAppFinalize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimAppFinalize ();
	#endif

	#if AKU_WITH_TEST
		AKUTestAppFinalize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzAppFinalize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppFinalize ();
	#endif
	#if AKU_WITH_PLUGINS
		AKUPluginsAppFinalize ();
	#endif
	
	AKUModulesCustomAppFinalize ();
}

//----------------------------------------------------------------//
void AKUModulesAppInitialize () {

	#if AKU_WITH_AUDIOSAMPLER
		AKUAudioSamplerAppInitialize ();
	#endif
			
	#if AKU_WITH_BILLING
		AKUBillingAppInitialize ();
	#endif

	#if AKU_WITH_BOX2D
		AKUBox2DAppInitialize ();
	#endif

	#if AKU_WITH_CHIPMUNK
		AKUChipmunkAppInitialize ();
	#endif

	#if AKU_WITH_COCOALUMBERJACK
		AKUCocoaLumberjackAppInitialize ();
	#endif

	#if AKU_WITH_CRASHLYTICS
		AKUCrashlyticsAppInitialize ();
	#endif

	#if AKU_WITH_CRYPTO
		AKUCryptoAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExAppInitialize ();
	#endif

	#if AKU_WITH_FMOD_STUDIO
		AKUFmodStudioAppInitialize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessAppInitialize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientAppInitialize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerAppInitialize ();
	#endif

	#if AKU_WITH_HOCKEYAPP
		AKUHockeyAppAppInitialize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtAppInitialize ();
	#endif

	#if AKU_WITH_MIXPANEL
		AKUMixpanelAppInitialize ();
	#endif
	
	#if AKU_WITH_NSLOGGER
		AKUNSLoggerAppInitialize ();
	#endif
	
	#if AKU_WITH_OMNITURE
		AKUOmnitureAppInitialize ();
	#endif
		
	#if AKU_WITH_PLUMZI
		AKUPlumziAppInitialize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimAppInitialize ();
	#endif

	#if AKU_WITH_TEST
		AKUTestAppInitialize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzAppInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilAppInitialize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsAppInitialize ();
	#endif
	
	AKUModulesCustomAppInitialize ();
}

//----------------------------------------------------------------//
void AKUModulesContextInitialize () {

	#if AKU_WITH_AUDIOSAMPLER
		AKUAudioSamplerContextInitialize ();
	#endif
	
	#if AKU_WITH_BILLING
		AKUBillingContextInitialize ();
	#endif

	#if AKU_WITH_BOX2D
		AKUBox2DContextInitialize ();
	#endif

	#if AKU_WITH_CHIPMUNK
		AKUChipmunkContextInitialize ();
	#endif

	#if AKU_WITH_COCOALUMBERJACK
		AKUCocoaLumberjackContextInitialize ();
	#endif

	#if AKU_WITH_CRASHLYTICS
		AKUCrashlyticsContextInitialize ();
	#endif

	#if AKU_WITH_CRYPTO
		AKUCryptoContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_EX
		AKUFmodExContextInitialize ();
	#endif

	#if AKU_WITH_FMOD_STUDIO
		AKUFmodStudioContextInitialize ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessContextInitialize ();
	#endif

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientContextInitialize ();
	#endif

	#if AKU_WITH_HTTP_SERVER
		AKUHttpServerContextInitialize ();
	#endif

	#if AKU_WITH_HOCKEYAPP
		AKUHockeyAppContextInitialize ();
	#endif

	#if AKU_WITH_LUAEXT
		AKULuaExtContextInitialize ();
	#endif

	#if AKU_WITH_MIXPANEL
		AKUMixpanelContextInitialize ();
	#endif
	
	#if AKU_WITH_NSLOGGER
		AKUNSLoggerContextInitialize ();
	#endif
		
	#if AKU_WITH_OMNITURE
		AKUOmnitureContextInitialize ();
	#endif
		
	#if AKU_WITH_PLUMZI
		AKUPlumziContextInitialize ();
	#endif

	#if AKU_WITH_SIM
		AKUSimContextInitialize ();
	#endif

	#if AKU_WITH_TEST
		AKUTestContextInitialize ();
	#endif

	#if AKU_WITH_UNTZ
		AKUUntzContextInitialize ();
	#endif

	#if AKU_WITH_UTIL
		AKUUtilContextInitialize ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsContextInitialize ();
	#endif
	
	AKUModulesCustomContextInitialize ();
}

//----------------------------------------------------------------//
void AKUModulesPause ( bool pause ) {
	
	#if AKU_WITH_SIM
		AKUPause ( pause );
	#endif
	
	#if AKU_WITH_UNTZ
		AKUUntzPause ( pause );
	
	#endif
}

//----------------------------------------------------------------//
void AKUModulesUpdate () {

	#if AKU_WITH_HTTP_CLIENT
		AKUHttpClientUpdate ();
	#endif

	#if AKU_WITH_FMOD_DESIGNER
		AKUFmodDesignerUpdate ();
	#endif
	
	#if AKU_WITH_FMOD_EX
		AKUFmodExUpdate ();
	#endif
	
	#if AKU_WITH_FMOD_STUDIO
		AKUFmodStudioUpdate ();
	#endif

	#if AKU_WITH_HARNESS
		AKUHarnessUpdate ()
	#endif

	#if AKU_WITH_SIM
		AKUUpdate ();
	#endif

	#if AKU_WITH_PLUGINS
		AKUPluginsUpdate ();
	#endif
		
	AKUModulesCustomUpdate ();
}
