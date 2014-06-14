// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <zl-util/ZLDeviceTime.h>
#include <zl-util/ZLLog.h>

#ifdef ANDROID
	#include <android/log.h>
#endif

//================================================================//
// CurrentTime
//================================================================//
struct CurrentTime {

	int		h;
	int		m;
	int		s;
	int		ms;

	//----------------------------------------------------------------//
	void Update () {
		
		static double startTime = ZLDeviceTime::GetTimeInSeconds ();
		double now = ZLDeviceTime::GetTimeInSeconds ();
		
		double elapsed = now - startTime;
		
		this->h = ( int )elapsed / 3600;
	    elapsed -= this->h * 3600;
	    this->m = ( int )elapsed / 60;
	    elapsed -= this->m * 60;
	    this->s = ( int )elapsed;
	    elapsed -= this->s;
		this->ms = elapsed * 1000;
	}
};



//================================================================//
// ZLLog
//================================================================//

void*			ZLLog::CONSOLE				= 0;
ZLLog::LogFunc	ZLLog::sLogFunc				= 0;
void*			ZLLog::sLogFuncUserdata		= 0;

//----------------------------------------------------------------//
void ZLLog::LogF ( void* file, u32 level, cc8* tag, cc8* format, ... ) {

	va_list args;
	va_start ( args, format );	
	
	ZLLog::LogV ( file, level, tag, format, args );
	
	va_end ( args );
}

//----------------------------------------------------------------//
void ZLLog::LogV ( void* file, u32 level, cc8* tag, cc8* format, va_list args ) {
	
	tag = tag ? tag : "";
	
	static cc8* levels = "NEWID";
	static cc8* logFormat = "%.2d:%.2d:%.2d.%.3d %c/[%s] %s";

	CurrentTime t;
	t.Update ();

	// TODO: this is unsafe
	char str [ 1024 ];
	vsnprintf ( str, 1024, format, args );
	if ( level > 4 ) {
		level = 4;
	}

	// TODO: all this mess should move up the channel to MOAILogMgr
	// ZLLog should just make sure we can print to a file or console
	// actually, this should just be a way to grab any output to a stream and re-route it via a callback
	// it's supposed to be low level, yo
	if ( sLogFunc ) {
	
		sLogFunc ( file, level, tag, format, args, sLogFuncUserdata );
	}
	else {
	
		if ( file && ( file != zl_stdout )) {
			fprintf (( ZLFILE* )file, logFormat, t.h, t.m, t.s, t.ms, levels [ level ], tag, str );
		}
		else {
			// TODO: this should really be hooked by the host
			// should only need to fall back on this if MOAILogMgr isn't used
			// in either case this should get set up via a callback at the host level
			#ifdef ANDROID
				__android_log_vprint ( ANDROID_LOG_INFO, "MoaiLog", logFormat, t.h, t.m, t.s, t.ms, _levels[level], tag, str );
			#else
				printf ( logFormat, t.h, t.m, t.s, t.ms, levels [ level] , tag, str );
			#endif
		}
	}
}

//----------------------------------------------------------------//
void ZLLog::SetLogFunc	( LogFunc logFunc, void* userdata ) {

	sLogFunc = logFunc;
	sLogFuncUserdata = userdata;
}
