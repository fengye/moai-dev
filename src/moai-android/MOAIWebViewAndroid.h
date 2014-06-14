// Made by plumzi

#ifndef MOAIWEBVIEWANDROID_H
#define MOAIWEBVIEWANDROID_H

#include <moaicore/moaicore.h>
#include <jni.h>

#include <moaiext-android/moaiext-jni.h>

#define JAVA_CLASS "com/ziplinegames/moai/Moai"

class MOAIWebViewAndroid :
    public MOAIInstanceEventSource {
private:



    //----------------------------------------------------------------//
    static int  _canGoBack                      ( lua_State* L );
    static int  _canGoForward                   ( lua_State* L );
    static int  _clickBack                      ( lua_State* L );
    static int  _clickForward                   ( lua_State* L );
    static int  _clickRefresh                   ( lua_State* L );
    static int  _clickStop                      ( lua_State* L );
    static int  _closeWebView                   ( lua_State* L );
    static int  _getAllowsInlineMediaPlayback   ( lua_State* L );
    static int  _getCurrentRequest              ( lua_State* L );
    static int  _getMediaPlaybackRequiresAction ( lua_State* L );
    static int  _getScalesPageToFit             ( lua_State* L );
    static int  _hasToolBar                     ( lua_State* L );
    static int  _hideWebView                    ( lua_State* L );
    static int  _isHidden                       ( lua_State* L );
    static int  _initWebView                    ( lua_State* L );
    static int  _isLoading                      ( lua_State* L );
    static int  _loadData                       ( lua_State* L );
    static int  _loadHTML                       ( lua_State* L );
    static int  _loadRequest                    ( lua_State* L );
    static int  _openUrlExternally              ( lua_State* L );
    static int  _runJavaScript                  ( lua_State* L );
    static int  _setAllowsInlineMediaPlayback   ( lua_State* L );
    static int  _setMediaPlaybackRequiresAction ( lua_State* L );
    static int  _setScalesPageToFit             ( lua_State* L );
    static int  _show                           ( lua_State* L );



public:

    DECL_LUA_FACTORY ( MOAIWebViewAndroid )



                MOAIWebViewAndroid                          ();
                ~MOAIWebViewAndroid                         ();
    void        RegisterLuaClass                        ( MOAILuaState& state );
    void        RegisterLuaFuncs                        ( MOAILuaState& state );

};

#endif // MOAIWEBVIEWANDROID_H