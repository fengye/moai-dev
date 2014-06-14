#ifndef	MOAIPLAYHAVENIOS_H
#define	MOAIPLAYHAVENIOS_H

#ifndef DISABLE_PLAYHAVEN

#include <moai-core/headers.h>
#import "PlayHavenSDK.h"

@class MOAIPlayhavenListener;

//================================================================//
// MOAIPlayhavenIOS
//================================================================//
class MOAIPlayhavenIOS :
	public MOAIGlobalClass < MOAIPlayhavenIOS, MOAILuaObject >,
	public MOAIGlobalEventSource {
private:
		static int _init( lua_State* L );
		static int _getContent( lua_State* L );
		static int _reportIAPSolution(lua_State* L);
		
		static int _resume( lua_State* L );
		static int _setOptOutStatus( lua_State* L );
		static int _getOptOutStatus( lua_State* L );
		static int _requestPlacement( lua_State* L );
		static int _createPlacement( lua_State* L );
		static int _cancelPlacement( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIPlayhavenIOS );
	
	enum {
		WILL_GET_CONTENT,
		DID_GET_CONTENT,
		WILL_DISPLAY_CONTENT,
		DID_DISPLAY_CONTENT,
		DID_DISMISS_CONTENT,
		DID_FAIL_CONTENT,
		UNLOCKED_REWARD,
		SHOULD_MAKE_IAP
	};
	
	
	NSString*				mToken;
	NSString*				mSecret;
	MOAIPlayhavenListener*	mPHListener;
	PHPurchase*				mPHPurchase;
		
	MOAIPlayhavenIOS();
	~MOAIPlayhavenIOS();
	void RegisterLuaClass( MOAILuaState& state );
	
	void NotifyPublisherContentEventOccurred ( int event, cc8* placement, cc8* extra = NULL,  int* qty = NULL );
	UIImage* GetImageForCloseButtonState ( UIControlState state );
};


@interface MOAIPlayhavenListener : NSObject<PHPublisherContentRequestDelegate, PHContentViewDelegate> {
}
@end

#endif  //DISABLE_PLAYHAVEN

#endif  //MOAIPLAYHAVENIOS_H