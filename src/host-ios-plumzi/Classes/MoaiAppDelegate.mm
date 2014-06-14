//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <host-modules/aku_modules_ios.h>

#import "MoaiAppDelegate.h"
#import "LocationObserver.h"
#import "MoaiVC.h"
#import "MoaiView.h"

// this is the tag value set on the holder view which contains both the video
// and the openGL view. It's also the view that gets rotated by the root view controller
#define TAG_ROOT_HOLDER_VIEW 1
//================================================================//
// AppDelegate
//================================================================//
@implementation MoaiAppDelegate

	@synthesize window = mWindow;
	@synthesize rootViewController = mMoaiVC;

	//----------------------------------------------------------------//
	-( void ) dealloc {

		[ mMoaiVC release ];
		[ mMoaiView release ];
		[ mWindow release ];
		[ super dealloc ];
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol UIApplicationDelegate
	//================================================================//	

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didFailToRegisterForRemoteNotificationsWithError:( NSError* )error {
	
		AKUNotifyRemoteNotificationRegistrationComplete ( nil, error );
	}

	//----------------------------------------------------------------//
	-( BOOL ) application:( UIApplication* )application didFinishLaunchingWithOptions:( NSDictionary* )launchOptions {
		
		[ application setStatusBarHidden:true ];
		
		AKUAppInitialize ();
        // landscape startup
        CGRect landscapeBounds;
        landscapeBounds.origin.x = [ UIScreen mainScreen ].bounds.origin.x;
        landscapeBounds.origin.y = [ UIScreen mainScreen ].bounds.origin.y;
        landscapeBounds.size.width = [ UIScreen mainScreen ].bounds.size.height;
        landscapeBounds.size.height = [ UIScreen mainScreen ].bounds.size.width;
        
        // plumzi video change: instead of just having the openglview as the view
        // of the root view controller, we use a "holder" view in which we put the opengl
        // and the video.
        // if the user rotates the device, this holder view will get rotated with an animation
        // by UIKit, and both the openGL & the video will get rotated as they are children
        // of the holder
        mMoaiView = [[ MoaiView alloc ] initWithFrame:landscapeBounds ];
		[ mMoaiView setUserInteractionEnabled:YES ];
		[ mMoaiView setMultipleTouchEnabled:YES ];
		[ mMoaiView setOpaque:YES ];
		[ mMoaiView setAlpha:1.0f ];

        UIView *holderView = [[[UIView alloc] initWithFrame:landscapeBounds] autorelease];
        holderView.tag = TAG_ROOT_HOLDER_VIEW;
        [holderView addSubview:mMoaiView];
		mMoaiVC = [[ MoaiVC alloc ]	init ];
		[ mMoaiVC setView:holderView ];
		
		mWindow = [[ UIWindow alloc ] initWithFrame:[ UIScreen mainScreen ].bounds ];
		[ mWindow setUserInteractionEnabled:YES ];
		[ mWindow setMultipleTouchEnabled:YES ];
		[ mWindow setOpaque:YES ];
		[ mWindow setAlpha:1.0f ];
		[ mWindow addSubview:holderView ];
		[ mWindow setRootViewController:mMoaiVC ];
		[ mWindow makeKeyAndVisible ];
        
		[ mMoaiView moaiInit:application ];
		
		// select product folder
		NSString* luaFolder = [[[ NSBundle mainBundle ] resourcePath ] stringByAppendingString:@"/lua" ];
		AKUSetWorkingDirectory ([ luaFolder UTF8String ]);
		
		// run scripts
		[ mMoaiView run:@"main.lua" ];
		
        // start to run the moai thread immediately so it renders the view before returning from here
        // to get a chance to display a splash screen for example while the rest loads
        [ mMoaiView pause:false ];
		
        // check to see if the app was lanuched from a remote notification
        NSDictionary* pushBundle = [ launchOptions objectForKey:UIApplicationLaunchOptionsRemoteNotificationKey ];
        if ( pushBundle != NULL ) {
            AKUNotifyRemoteNotificationReceived ( pushBundle );
        }
		
        AKUAppDidStartSession ( false );
		// return
		return true;
	}

	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didReceiveRemoteNotification:( NSDictionary* )pushBundle {
		
		AKUNotifyRemoteNotificationReceived ( pushBundle );
	}
	
	//----------------------------------------------------------------//
	-( void ) application:( UIApplication* )application didRegisterForRemoteNotificationsWithDeviceToken:( NSData* )deviceToken {
	
		AKUNotifyRemoteNotificationRegistrationComplete ( deviceToken, nil );
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationDidBecomeActive:( UIApplication* )application {
	
        // update connectivity
        AKUGetIphoneNetworkReachability ();
		// restart moai view
		AKUIosDidBecomeActive ();
		[ mMoaiView pause:NO ];
        // update for next time applicationDidBecomeActive
        mResumed = YES;
	}

	//----------------------------------------------------------------//
	-( void ) applicationDidEnterBackground:( UIApplication* )application {
	
        AKUAppWillEndSession ();
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillEnterForeground:( UIApplication* )application {
	
        // update connectivity
        AKUGetIphoneNetworkReachability();
        
        AKUAppDidStartSession ( mResumed );
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillResignActive:( UIApplication* )application {
	
		// pause moai view
		[ mMoaiView pause:YES ];
	}
	
	//----------------------------------------------------------------//
	-( void ) applicationWillTerminate :( UIApplication* )application {
        
		AKUAppWillEndSession ();
		AKUAppFinalize ();
	}

	//----------------------------------------------------------------//
	#if __IPHONE_OS_VERSION_MIN_REQUIRED > __IPHONE_4_1
		
		//----------------------------------------------------------------//
		// For iOS 4.2+ support
		-( BOOL )application:( UIApplication* )application openURL:( NSURL* )url sourceApplication:( NSString* )sourceApplication annotation:( id )annotation {

			AKUAppOpenFromURL ( url );
			return YES;
		}
	
	#else

		//----------------------------------------------------------------//
		-( BOOL )application :( UIApplication* )application handleOpenURL :( NSURL* )url {

			AKUAppOpenFromURL(url);
			return YES;
		}

	#endif

@end
