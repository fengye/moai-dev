#ifndef DISABLE_PLAYHAVEN

#import <moai-sim/headers.h>
#import <moai-iphone/MOAIPlayhavenIOS.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Playhaven.
	
	@in		string	token			Available in Playhaven dashboard settings.
	@in 	string	secret			Available in Playhaven dashboard settings.
	@out 	nil
*/
int MOAIPlayhavenIOS::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* token = lua_tostring ( state, 1 );
	cc8* secret = lua_tostring ( state, 2 );
	
	[ MOAIPlayhavenIOS::Get ().mToken release ];
	MOAIPlayhavenIOS::Get ().mToken = [[ NSString alloc ] initWithUTF8String:token ];
	[ MOAIPlayhavenIOS::Get ().mSecret release ];
	MOAIPlayhavenIOS::Get ().mSecret = [[ NSString alloc ] initWithUTF8String:secret ];
	
	PHPublisherOpenRequest *request = [PHPublisherOpenRequest requestForApp:MOAIPlayhavenIOS::Get().mToken secret:MOAIPlayhavenIOS::Get().mSecret];
	[request send];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	resume
	 @text	Send a PHPublisherOpenRequest
	 
	 @in	nil
	 @out	boolean success flag
	 */
int MOAIPlayhavenIOS::_resume ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	/* Check class has been properly initialized */
	if ( !MOAIPlayhavenIOS::Get ().mToken || !MOAIPlayhavenIOS::Get ().mSecret) {
		
		lua_pushboolean ( state, false );
		return 1;
	}
    
    PHPublisherOpenRequest * request = [ PHPublisherOpenRequest requestForApp:MOAIPlayhavenIOS::Get ().mToken
																	   secret:MOAIPlayhavenIOS::Get ().mSecret ];
	request.delegate = MOAIPlayhavenIOS::Get ().mPHListener;
	[ request send ];
	
	lua_pushboolean ( state, true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setOptOutStatus
	@text	Sets the OptOut status flag.
 
	@in		boolean		optOut flag
	@out	nil
 */
int MOAIPlayhavenIOS::_setOptOutStatus ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	bool status = state.GetValue < bool >( 1, false );
	
	[ PHAPIRequest setOptOutStatus:status ];
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getOptOutStatus
	@text	Gets the OptOut status flag.
 
	@in		nil
	@out	boolean optOut flag
 */
int MOAIPlayhavenIOS::_getOptOutStatus ( lua_State *L ) {
	
	MOAILuaState state ( L );
	
	lua_pushboolean ( state, [ PHAPIRequest optOutStatus ] );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getContent
	@text	Shows Playhaven content.
 
	@in		string	placement		Placement position.
	@opt	bool	preload			Preload the request. Default is false.
	@opt	bool	overlay			Determines whether the overlay should be displayed immediately. Default is true.
	@opt	bool	animated		Determines whether the overlay should be animated. Default is true.
	@opt	bool	MOAIColor		Overlay background color. Default is white, 50% transparent.
	@out	boolean success flag
*/
int MOAIPlayhavenIOS::_getContent(lua_State* L) {
	
	MOAILuaState state ( L );
	
	cc8* placement = lua_tostring(state, 1);
	
	/* check params */
	if ( placement[0] == '\0' || !MOAIPlayhavenIOS::Get ().mToken || !MOAIPlayhavenIOS::Get ().mSecret) {
		
		lua_pushboolean ( state, false );
		return 1;
	}
	
	PHPublisherContentRequest *request = [PHPublisherContentRequest requestForApp:MOAIPlayhavenIOS::Get().mToken
																		   secret:MOAIPlayhavenIOS::Get().mSecret
																		placement:[NSString stringWithUTF8String:placement]
																		 delegate:MOAIPlayhavenIOS::Get().mPHListener];

	bool preload = state.GetValue < bool >( 2, false );
	bool showOverlayImmediately = state.GetValue < bool >( 3, request.showsOverlayImmediately );
	bool animated = state.GetValue < bool >( 4, request.animated );
	
	[ request setShowsOverlayImmediately: showOverlayImmediately ];
	[ request setAnimated:animated ];
	
	MOAIColor* color = state.GetLuaObject < MOAIColor >( 5, true );
	if ( color ) {
		
		ZLColorVec _color = color->GetColorTrait();
		[ request setOverlayWindowBackgroundColor:[UIColor colorWithRed:_color.mR
																  green:_color.mG
																   blue:_color.mB
																  alpha:_color.mA ]];
	}
	
	if ( preload ) {
		
		[ request preload ];
	} else {
		
		[ request send ];
	}
	
	lua_pushboolean ( state, true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	reportIAPSolution
	@text	Reports IAP resolution status.

	@in		number	status	Status code.
	@out 	nil
*/
int MOAIPlayhavenIOS::_reportIAPSolution(lua_State* L) {
	
	MOAILuaState state ( L );
	
	int status = lua_tointeger(state, 1);
	
	if (MOAIPlayhavenIOS::Get().mPHPurchase) {
		PHPurchaseResolutionType resolution = PHPurchaseResolutionBuy;
		if (status > 0) {
			resolution = PHPurchaseResolutionCancel;
		} else if (status < 0) {
			resolution = PHPurchaseResolutionError;
		}
		[MOAIPlayhavenIOS::Get().mPHPurchase reportResolution:resolution];
		
		[MOAIPlayhavenIOS::Get().mPHPurchase release];
		MOAIPlayhavenIOS::Get().mPHPurchase = nil;
	}
	
	return 0;
}

//================================================================//
// MOAIPlayhavenIOS
//================================================================//

//----------------------------------------------------------------//
MOAIPlayhavenIOS::MOAIPlayhavenIOS () :
	mToken ( 0 ),
	mSecret ( 0 ),
	mPHPurchase ( 0 ) {
		
	RTTI_SINGLE ( MOAILuaObject )
	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	mPHListener = [[MOAIPlayhavenListener alloc] init];
}

//----------------------------------------------------------------//
MOAIPlayhavenIOS::~MOAIPlayhavenIOS () {
	
	[ PHAPIRequest cancelAllRequestsWithDelegate:mPHListener ];
	[ PHAPIRequest cancelAllRequestsWithDelegate:mPHListener ];
	
	[ mToken release ];
	[ mSecret release ];
	[ mPHListener release ];
}

//----------------------------------------------------------------//
void MOAIPlayhavenIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "WILL_GET_CONTENT",		( u32 )WILL_GET_CONTENT );
	state.SetField ( -1, "DID_GET_CONTENT", 		( u32 )DID_GET_CONTENT );
	state.SetField ( -1, "WILL_DISPLAY_CONTENT", 	( u32 )WILL_DISPLAY_CONTENT );
	state.SetField ( -1, "DID_DISPLAY_CONTENT", 	( u32 )DID_DISPLAY_CONTENT );
	state.SetField ( -1, "DID_DISMISS_CONTENT", 	( u32 )DID_DISMISS_CONTENT );
	state.SetField ( -1, "DID_FAIL_CONTENT",		( u32 )DID_FAIL_CONTENT );
	state.SetField ( -1, "UNLOCKED_REWARD",			( u32 )UNLOCKED_REWARD );
	state.SetField ( -1, "SHOULD_MAKE_IAP",			( u32 )SHOULD_MAKE_IAP );
	
	luaL_Reg regTable [] = {
		{ "init",				_init},
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIPlayhavenIOS > },
		{ "getContent",			_getContent},
		{ "reportIAPSolution",	_reportIAPSolution},
		{ "resume",				_resume },
		{ "setOptOutStatus",	_setOptOutStatus },
		{ "getOptOutStatus",	_getOptOutStatus },
		{NULL, NULL}
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
UIImage* MOAIPlayhavenIOS::GetImageForCloseButtonState ( UIControlState state ) {
	
	return nil;
}

//----------------------------------------------------------------//
void MOAIPlayhavenIOS::NotifyPublisherContentEventOccurred ( int event, cc8* placement, cc8* extra, int* qty ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( event, state )) {
		
		int args = 0;
		if ( placement ) {
		
			++args;
			state.Push ( placement );
		}
		
		if ( extra ) {
			
			++args;
			state.Push ( extra );
		}
		
		if ( qty ) {
			
			++args;
			state.Push ( *qty );
		}
		state.DebugCall ( args, 0 );
	}
}


@implementation MOAIPlayhavenListener

	//================================================================//
	#pragma mark -
	#pragma mark Protocol PHPublisherContentRequestDelegate
	//================================================================//

	-( void ) requestWillGetContent:( PHPublisherContentRequest * )request {
		
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().WILL_GET_CONTENT, [ request.placement UTF8String ]);
	}

	-( void ) requestDidGetContent:( PHPublisherContentRequest * )request {
		
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().DID_GET_CONTENT, [ request.placement UTF8String ] );
	}

	-( void ) request:( PHPublisherContentRequest * )request contentWillDisplay:( PHContent * )content {
		
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().WILL_DISPLAY_CONTENT, [ request.placement UTF8String ]);
	}

	-( void ) request:( PHPublisherContentRequest * )request contentDidDisplay:( PHContent * )content {
		
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().DID_DISPLAY_CONTENT, [ request.placement UTF8String ] );
	}

	-( void ) request:( PHPublisherContentRequest * )request contentDidDismissWithType:( PHPublisherContentDismissType * )type {
		
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().DID_DISMISS_CONTENT, [ request.placement UTF8String ], [ type UTF8String ]);
	}

	-( void ) request:( PHPublisherContentRequest * )request didFailWithError:( NSError * )error {
		
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().DID_FAIL_CONTENT, [ request.placement UTF8String ]);
	}

	-( void ) request:( PHPublisherContentRequest * )request unlockedReward:( PHReward * )reward {
		
		int quantity = reward.quantity;
		MOAIPlayhavenIOS::Get ().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().UNLOCKED_REWARD, nil, [ reward.name UTF8String ], &quantity );
	}

	-( void ) request:( PHPublisherContentRequest * )request makePurchase:( PHPurchase * )purchase {
		
		MOAIPlayhavenIOS::Get().mPHPurchase = purchase;
		MOAIPlayhavenIOS::Get().NotifyPublisherContentEventOccurred ( MOAIPlayhavenIOS::Get ().SHOULD_MAKE_IAP, nil, [ purchase.productIdentifier UTF8String ] );
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol PHContentViewDelegate
	//================================================================//

	- ( UIImage * ) contentView:( PHContentView * )contentView imageForCloseButtonState:( UIControlState ) state {
		
		return MOAIPlayhavenIOS::Get ().GetImageForCloseButtonState ( state );
	}

@end

#endif