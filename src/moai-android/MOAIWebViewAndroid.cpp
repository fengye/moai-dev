#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIWebViewAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name   canGoBack
    @text   Checks if the UIWebView can go back.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_canGoBack ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    lua_pushboolean ( state, false );

    return 1;
}

//----------------------------------------------------------------//
/** @name   canGoForward
    @text   Checks if the UIWebView can go forward.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_canGoForward ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    lua_pushboolean ( state, false );

    return 1;
}

//----------------------------------------------------------------//
/** @name   clickBack
    @text   Sends an event to the UIWebView that the user has clicked
            'back'.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_clickBack ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   clickForward
    @text   Sends an event to the UIWebView that the user has clicked
            'forward'.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_clickForward ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

   // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   clickRefresh
    @text   Sends an event to the UIWebView that the user has clicked
            'refresh'.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_clickRefresh ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   clickStop
    @text   Sends an event to the UIWebView that the user has clicked
            'stop'.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_clickStop ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   closeWebView
    @text   Closes the current UIWebView.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_closeWebView ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   getAllowsInlineMediaPlayback
    @text   Returns whether or not the UIWebView allows inline media
            playback.

    @in     MOAIWebViewAndroid self
    @out    bool allowsInlineMediaPlayback
*/
int MOAIWebViewAndroid::_getAllowsInlineMediaPlayback ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   getCurrentRequest
    @text   Returns the currently active URL.

    @in     MOAIWebViewAndroid self
    @out    string currentURL
*/
int MOAIWebViewAndroid::_getCurrentRequest ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented


    return 0;
}

//----------------------------------------------------------------//
/** @name   getMediaPlaybackRequiresAction
    @text   Returns whether or not the UIWebView requires user input
            to start media playback or if it start automatically.

    @in     MOAIWebViewAndroid self
    @out    bool mediaPlaybackRequiresUserAction
*/
int MOAIWebViewAndroid::_getMediaPlaybackRequiresAction ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented


    return 0;
}

//----------------------------------------------------------------//
/** @name   getScalesPageToFit
    @text   Returns whether or not the UIWebView automatically scales
            pages to fit the view.

    @in     MOAIWebViewAndroid self
    @out    bool scalesPageToFit
*/
int MOAIWebViewAndroid::_getScalesPageToFit ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   hasToolBar
    @text   Sets whether or not the UIWebView has a tool bar. Default is true.

    @in     MOAIWebViewAndroid self
    @in     bool hasToolBar
    @out    nil
*/
int MOAIWebViewAndroid::_hasToolBar ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   hideWebView
    @text   Sets the UIWebView to hidden.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_hideWebView ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   initWebView
    @text   Kills current UIWebView (if one exists) and creates a
            new one with desired dimensions.

    @in     MOAIWebViewAndroid self
    @out    bool scalesPageToFit
*/
int MOAIWebViewAndroid::_initWebView ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    int left = lua_tointeger ( state, 2 );
    int top = lua_tointeger ( state, 3 );
    int width = lua_tointeger ( state, 4 );
    int height = lua_tointeger ( state, 5 );
    bool hidden = lua_toboolean ( state, 6 );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   isHidden
    @text   Returns whether or not the UIWebView is hidden.

    @in     MOAIWebViewAndroid self
    @out    bool isHidden
*/
int MOAIWebViewAndroid::_isHidden ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   isLoading
    @text   Returns whether or not the UIWebView is loading a page.

    @in     MOAIWebViewAndroid self
    @out    bool isLoading
*/
int MOAIWebViewAndroid::_isLoading ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented


    return 0;
}

//----------------------------------------------------------------//
// TODO
int MOAIWebViewAndroid::_loadData ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    return 0;
}

//----------------------------------------------------------------//
/** @name   loadHTML
    @text   Sets the UIWebView main page content and base URL.

    @in     MOAIWebViewAndroid self
    @in     string string
    @in     string baseURL
    @out    nil
*/
int MOAIWebViewAndroid::_loadHTML ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );
    JNI_GET_ENV(jvm, env);

    cc8* st = lua_tostring ( state, 2 );
    cc8* url = lua_tostring ( state, 3 );

    if (st) {
        jstring data = env->NewStringUTF(st);

        jclass moai = findClassOrDie(env, JAVA_CLASS);
        jmethodID method = findJavaMethod(env, moai, "openWebView", "(Ljava/lang/String;)V", true);
        env->CallStaticVoidMethod(moai, method, data);
    }


    // TODO : not implemented


    return 0;
}

//----------------------------------------------------------------//
/** @name   loadRequest
    @text   Sets the UIWebView main page content and base URL.

    @in     MOAIWebViewAndroid self
    @in     string request
    @out    nil
*/
int MOAIWebViewAndroid::_loadRequest ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    cc8* urlStr = lua_tostring ( state, 2 );
    // TODO : not implemented


    return 0;
}

//----------------------------------------------------------------//
/** @name   openUrlExternally
    @text   Opens the URL in Safari.

    @in     string url
    @out    boolean true if successful
*/
int MOAIWebViewAndroid::_openUrlExternally ( lua_State* L ) {

    MOAILuaState state ( L );

    cc8* url = state.GetValue < cc8* >( 1, "" );

    // TODO : not implemented


    return 0;
}

//----------------------------------------------------------------//
/** @name   runJavaScript
    @text   Runs the specified JavaScript stringin the UIWebView.

    @in     MOAIWebViewAndroid self
    @in     string script
    @out    string result   Result of the JavaScript or nil if script failed
*/
int MOAIWebViewAndroid::_runJavaScript ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

   // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   setAllowsInlineMediaPlayback
    @text   Sets the value of the allowsInlineMediaPlayback on the
            UIWebView.

    @in     MOAIWebViewAndroid self
    @in     bool allowsInlineMediaPlayback
    @out    nil
*/
int MOAIWebViewAndroid::_setAllowsInlineMediaPlayback ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   setMediaPlaybackRequiresAction
    @text   Sets the value of the mediaPlaybackRequiresUserAction on the
            UIWebView.

    @in     MOAIWebViewAndroid self
    @in     bool mediaPlaybackRequiresAction
    @out    nil
*/
int MOAIWebViewAndroid::_setMediaPlaybackRequiresAction ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   setScalesPageToFit
    @text   Sets the value of the scalesPageToFit on the UIWebView.

    @in     MOAIWebViewAndroid self
    @in     bool scalesPageToFit
    @out    nil
*/
int MOAIWebViewAndroid::_setScalesPageToFit ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented

    return 0;
}

//----------------------------------------------------------------//
/** @name   show
    @text   Shows the UIWebView.

    @in     MOAIWebViewAndroid self
    @out    nil
*/
int MOAIWebViewAndroid::_show ( lua_State* L ) {

    MOAI_LUA_SETUP ( MOAIWebViewAndroid, "U" );

    // TODO : not implemented


    return 0;
}

//================================================================//
// MOAIWebViewAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIWebViewAndroid::MOAIWebViewAndroid ()  {

    RTTI_SINGLE ( MOAIInstanceEventSource )

    // TODO : not implemented

}

//----------------------------------------------------------------//
MOAIWebViewAndroid::~MOAIWebViewAndroid () {

    // TODO : not implemented

}

//----------------------------------------------------------------//
void MOAIWebViewAndroid::RegisterLuaClass ( MOAILuaState& state ) {


    luaL_Reg regTable [] = {
        { "openUrlExternally",  _openUrlExternally },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIWebViewAndroid::RegisterLuaFuncs ( MOAILuaState& state ) {

    MOAIInstanceEventSource::RegisterLuaFuncs ( state );

    luaL_Reg regTable [] = {
        { "canGoBack",                      _canGoBack },
        { "canGoForward",                   _canGoForward },
        { "clickBack",                      _clickBack },
        { "clickForward",                   _clickForward },
        { "clickRefresh",                   _clickRefresh },
        { "clickStop",                      _clickStop },
        { "closeWebView",                   _closeWebView },
        { "getAllowsInlineMediaPlayback",   _getAllowsInlineMediaPlayback },
        { "getCurrentRequest",              _getCurrentRequest },
        { "getMediaPlaybackRequiresAction", _getMediaPlaybackRequiresAction },
        { "getScalesPageToFit",             _getScalesPageToFit },
        { "hasToolBar",                     _hasToolBar },
        { "hideWebView",                    _hideWebView },
        { "initWebView",                    _initWebView },
        { "isHidden",                       _isHidden },
        { "isLoading",                      _isLoading },
        { "loadData",                       _loadData },
        { "loadHTML",                       _loadHTML },
        { "loadRequest",                    _loadRequest },
        { "runJavaScript",                  _runJavaScript },
        { "setAllowsInlineMediaPLayback",   _setAllowsInlineMediaPlayback },
        { "setMediaPlaybackRequiresAction", _setMediaPlaybackRequiresAction },
        { "setScalesPageToFit",             _setScalesPageToFit },
        { "show",                           _show },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}

