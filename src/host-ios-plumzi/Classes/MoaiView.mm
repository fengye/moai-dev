//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>

//extern "C" {
//	#include <lua.h>
//	#include <lauxlib.h>
//	#include <lualib.h>
//}

#import <host-modules/aku_modules_ios.h>
#import <moai-audiosampler/AKU-audiosampler.h>

#import "LocationObserver.h"
#import "MoaiView.h"

namespace MoaiInputDeviceID {
	enum {
		DEVICE,
		TOTAL,
	};
}

namespace MoaiInputDeviceSensorID {
	enum {
		COMPASS,
		LEVEL,
		LOCATION,
		TOUCH,
		TOTAL,
	};
}

//================================================================//
// MoaiView ()
//================================================================//
@interface MoaiView ()

	//----------------------------------------------------------------//
	-( void )	drawView;
	-( void )	handleTouches		:( NSSet* )touches :( BOOL )down;
	-( void )	onUpdateAnim;
	-( void )	onUpdateHeading		:( LocationObserver* )observer;
	-( void )	onUpdateLocation	:( LocationObserver* )observer;
	-( void )	startAnimation;
	-( void )	stopAnimation;
    -( void )   dummyFunc;

@end

//================================================================//
// MoaiView
//================================================================//
@implementation MoaiView
    SYNTHESIZE	( GLint, width, Width );
    SYNTHESIZE	( GLint, height, Height );

static bool gOpaque = YES;

// <-Plumzi Addition
-(void) handleAcceleration:(CMAcceleration)acceleration
{
	float x = acceleration.x;
	float y = acceleration.y;
	float z = acceleration.z;
	
	// rotate the acceleration vector depending on the screen orientation
	switch ([UIApplication sharedApplication].statusBarOrientation) {
		case UIInterfaceOrientationLandscapeLeft:
			x = acceleration.y;
			y = acceleration.x;
			break;
			
		case UIInterfaceOrientationLandscapeRight:
			// rotated 180° from landscape left
			x = -acceleration.y;
			y = -acceleration.x;
			break;
			
		case UIInterfaceOrientationPortrait:
			// we want y to be inverted
			x = acceleration.x;
			y = -acceleration.y;
			break;
			
		case UIInterfaceOrientationPortraitUpsideDown:
			// rotated 180° from portrait
			x = -acceleration.x;
			y = acceleration.y;
			break;
	}
	
	AKUEnqueueLevelEvent (
						  MoaiInputDeviceID::DEVICE,
						  MoaiInputDeviceSensorID::LEVEL,
						  x,
						  y,
						  z
						  );
	
}
	//----------------------------------------------------------------//
	-( void ) accelerometer:( UIAccelerometer* )acel didAccelerate:( UIAcceleration* )acceleration {
		( void )acel;
		
		AKUEnqueueLevelEvent (
			MoaiInputDeviceID::DEVICE,
			MoaiInputDeviceSensorID::LEVEL,
			( float )acceleration.x,
			( float )acceleration.y,
			( float )acceleration.z
		);
	}

// Plumzi Addition ->
    //----------------------------------------------------------------//
    -( AKUContextID ) akuInitialized {

        return mAku;
    }

	//----------------------------------------------------------------//
	-( void ) dealloc {
	
		AKUDeleteContext ( mContext );
		
		// <-Plumzi Addition
		[ mMotionManager release ];
		mMotionManager = nil;
		// Plumzi Addition ->
		[ super dealloc ];
	}

	//----------------------------------------------------------------//
	-( void ) drawView {
		
		// <-Plumzi Addition
        if (AKUHasOpaqueBackground() != gOpaque) {
            gOpaque = AKUHasOpaqueBackground();
            self.opaque = gOpaque;
        }
		// ->
		[ self beginDrawing ];
		
		AKUSetContext ( mAku );
		AKURender ();

		[ self endDrawing ];
	}
	
    //----------------------------------------------------------------//
    -( void ) dummyFunc {
        //dummy to fix weird input bug
    }

	//----------------------------------------------------------------//
	-( void ) handleTouches :( NSSet* )touches :( BOOL )down {
	
		for ( UITouch* touch in touches ) {
			
			CGPoint p = [ touch locationInView:self ];
			
			AKUEnqueueTouchEvent (
				MoaiInputDeviceID::DEVICE,
				MoaiInputDeviceSensorID::TOUCH,
				( int ) (long) touch, // use the address of the touch as a unique id
				down,
				p.x * [[ UIScreen mainScreen ] scale ],
				p.y * [[ UIScreen mainScreen ] scale ]
			);
		}
	}
	
	//----------------------------------------------------------------//
	-( id )init {
		
        mAku = 0;
		self = [ super init ];
		if ( self ) {
		}
		return self;
	}

	//----------------------------------------------------------------//
	-( id ) initWithCoder:( NSCoder* )encoder {

        mAku = 0;
		self = [ super initWithCoder:encoder ];
		if ( self ) {
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( id ) initWithFrame :( CGRect )frame {

        mAku = 0;
		self = [ super initWithFrame:frame ];
		if ( self ) {
		}
		return self;
	}
	
	//----------------------------------------------------------------//
	-( void ) moaiInit :( UIApplication* )application {
	
		mAku = AKUCreateContext ();
		AKUSetUserdata ( self );
		
		AKUModulesContextInitialize ();
		AKUModulesIosContextInitialize ();
        AKUModulesRunLuaAPIWrapper ();
		
		AKUSetInputConfigurationName ( "iPhone" );

		AKUReserveInputDevices			( MoaiInputDeviceID::TOTAL );
		AKUSetInputDevice				( MoaiInputDeviceID::DEVICE, "device" );
		
		AKUReserveInputDeviceSensors	( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOTAL );
		AKUSetInputDeviceCompass		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::COMPASS,		"compass" );
		AKUSetInputDeviceLevel			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LEVEL,		"level" );
		AKUSetInputDeviceLocation		( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::LOCATION,		"location" );
		AKUSetInputDeviceTouch			( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOUCH,		"touch" );
		
		CGRect screenRect = [[ UIScreen mainScreen ] bounds ];
		// Plumzi Addition ->
        // Apps always start in portrait mode so force landscape startup
		screenRect.size.width = [ UIScreen mainScreen ].bounds.size.height;
        screenRect.size.height = [ UIScreen mainScreen ].bounds.size.width;
		// Plumzi Addition ->
		CGFloat scale = [[ UIScreen mainScreen ] scale ];
		CGFloat screenWidth = screenRect.size.width * scale;
		CGFloat screenHeight = screenRect.size.height * scale;
		
		AKUSetScreenSize ( screenWidth, screenHeight );
		AKUSetScreenDpi([ self guessScreenDpi ]);
		AKUSetViewSize ( mWidth, mHeight );
		
        AKUIosSetFrameBuffer ( mFramebuffer );
		AKUDetectGfxContext ();
		
		mAnimInterval = 1; // 1 for 60fps, 2 for 30fps
		
		// disable completely location if not used to avoid prompting user on first launch
#ifndef DISABLE_LOCATION
		mLocationObserver = [[[ LocationObserver alloc ] init ] autorelease ];
		
		[ mLocationObserver setHeadingDelegate:self :@selector ( onUpdateHeading: )];
		[ mLocationObserver setLocationDelegate:self :@selector ( onUpdateLocation: )];
		
#endif
		
		// disable motion
#ifndef DISABLE_MOTION
		// <-Plumzi Addition
		mMotionManager = [[ CMMotionManager alloc] init ];
		if ( !mMotionManager.accelerometerAvailable ) {
			[ mMotionManager release ];
			mMotionManager = nil;
		}
		// Plumzi Addition ->
//		UIAccelerometer* accel = [ UIAccelerometer sharedAccelerometer ];
//		accel.delegate = self;
//		accel.updateInterval = mAnimInterval / 60;
#endif
		// init aku
		AKUIphoneInit ( application );
        AKUModulesRunLuaAPIWrapper ();
		
		// add in the particle presets
		ParticlePresets ();
	}
	
	//----------------------------------------------------------------//
	-( int ) guessScreenDpi {
		float dpi;
		float scale = 1;
		if ([[ UIScreen mainScreen ] respondsToSelector:@selector(scale) ]) {
			scale = [[ UIScreen mainScreen ] scale];
		}
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {
			//Not working for iPad Mini, but appropriate solution doesn't exist yet
			dpi = 132 * scale;
		}else{
			dpi = 163 * scale;
		}
		return dpi;
	}

    //----------------------------------------------------------------//
	-( void ) onUpdateAnim {
		
		// <-Plumzi Addition
		// handle acceleration on each display update.
		if ( mMotionManager && mMotionManager.accelerometerAvailable ) {
			[self handleAcceleration:mMotionManager.accelerometerData.acceleration];
		}
		
		static double lastStep;
		double step = AKUGetSimStep();
		if (lastStep != step) {
			lastStep = step;
			// this will give the smallest anim interval needed for the desired step
			// 60 hz step => 1 interval
			// 30 hz step => 2 interval
			// 40 hz step => 1 interval and MOAISim will take care of "adapting" ie let 2 ticks then 1 tick then 2 ticks etc
			// obviously staying on 60 or 30 or 20 is way better than being in between
			mAnimInterval = (int)(60 * step);
			[ mDisplayLink setFrameInterval:mAnimInterval ];
		}
		// Plumzi Addition ->
		[ self openContext ];
		AKUSetContext ( mAku );
		AKUModulesUpdate ();
		AKUModulesIosUpdate ();
		
		[ self drawView ];
        
		// <-Plumzi Addition
		// Disable auto lock which puts the app in background and aborts
		// all network communications.
#ifdef TARGET_OS_IPHONE
		[UIApplication sharedApplication].idleTimerDisabled = AKUGetNetworkActivity();
#endif
		// Plumzi Addition ->
        //sometimes the input handler will get 'locked out' by the render, this will allow it to run
        [ self performSelector: @selector(dummyFunc) withObject:self afterDelay: 0 ];
	}
	
	//----------------------------------------------------------------//
	-( void ) onUpdateHeading :( LocationObserver* )observer {
	
		AKUEnqueueCompassEvent (
			MoaiInputDeviceID::DEVICE,
			MoaiInputDeviceSensorID::COMPASS,
			( float )[ observer heading ]
		);
	}
	
	//----------------------------------------------------------------//
	-( void ) onUpdateLocation :( LocationObserver* )observer {
	
		AKUEnqueueLocationEvent (
			MoaiInputDeviceID::DEVICE,
			MoaiInputDeviceSensorID::LOCATION,
			[ observer longitude ],
			[ observer latitude ],
			[ observer altitude ],
			( float )[ observer hAccuracy ],
			( float )[ observer vAccuracy ],
			( float )[ observer speed ]
		);
	}
	
	//----------------------------------------------------------------//
	-( void ) pause :( BOOL )paused {
	
		if ( paused ) {
			AKUPause ( YES );
			[ self stopAnimation ];
		}
		else {
			[ self startAnimation ];
			AKUPause ( NO );
		}
	}
	
	//----------------------------------------------------------------//
	-( void ) run :( NSString* )filename {
	
		AKUSetContext ( mAku );
		AKURunScript ([ filename UTF8String ]);
	}
	
	//----------------------------------------------------------------//
	-( void ) startAnimation {
		
		if ( !mDisplayLink ) {
			CADisplayLink* aDisplayLink = [[ UIScreen mainScreen ] displayLinkWithTarget:self selector:@selector( onUpdateAnim )];
			[ aDisplayLink setFrameInterval:mAnimInterval ];
			[ aDisplayLink addToRunLoop:[ NSRunLoop currentRunLoop ] forMode:NSDefaultRunLoopMode ];
			mDisplayLink = aDisplayLink;
			// <-Plumzi Addition
			if ( mMotionManager && mMotionManager.accelerometerAvailable ) {
				[ mMotionManager startAccelerometerUpdates ];
			}
			
			// When the view is not opaque, when the app is coming back from inactive state, the framerate can drop to 40 fps
			// and stay there. A work around is to quickly switch back the view to opaque and back to non opaque.
			if (!gOpaque && mAnimInterval == 1) {
				dispatch_after(dispatch_time(DISPATCH_TIME_NOW, .1 * NSEC_PER_SEC), dispatch_get_main_queue(), ^{
					self.opaque = NO;
				});
			}
			// Plumzi Addition ->
			[self onUpdateAnim];
		}
	}

	//----------------------------------------------------------------//
	-( void ) stopAnimation {
		
		// <-Plumzi Addition
		if (!gOpaque && mAnimInterval == 1) {
			self.opaque = YES;
			// try to force the runloop to run for a moment for the opacity to be set on the view
			[[NSRunLoop currentRunLoop] runMode:NSDefaultRunLoopMode beforeDate:[NSDate dateWithTimeIntervalSinceNow:0.03]];
		}
		// Plumzi Addition ->
        [ mDisplayLink invalidate ];
		// <-Plumzi Addition
		if ( mMotionManager && mMotionManager.accelerometerAvailable ) {
			[ mMotionManager stopAccelerometerUpdates ];
		}
		// Plumzi Addition ->
        mDisplayLink = nil;
	}
	
	//----------------------------------------------------------------//
	-( void )touchesBegan:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :YES ];
	}
	
	//----------------------------------------------------------------//
	-( void )touchesCancelled:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )touches;
		( void )event;
		
		AKUEnqueueTouchEventCancel ( MoaiInputDeviceID::DEVICE, MoaiInputDeviceSensorID::TOUCH );
	}
	
	//----------------------------------------------------------------//
	-( void )touchesEnded:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :NO ];
	}

	//----------------------------------------------------------------//
	-( void )touchesMoved:( NSSet* )touches withEvent:( UIEvent* )event {
		( void )event;
		
		[ self handleTouches :touches :YES ];
	}
	
@end
