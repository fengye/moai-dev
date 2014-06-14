#ifndef MOAIHOCKEYAPPANDROID_H_
# define MOAIHOCKEYAPPANDROID_H_

#include <moaicore/moaicore.h>


#define HOCKEYAPP_CLASS "com/plumzi/hockeyapp/HockeyAppWrapper"

//================================================================//
// MOAIHockeyAppAndroid
//================================================================//
/** @name   MOAIHockeyAppAndroid
    @text   Wrapper for HockeyApp integration on Android devices.
    HockeyApp provides real-time, actionable crash reports
    for mobile apps. Exposed to lua via MOAIHockeyApp on
    all mobile platforms.
 */
class MOAIHockeyAppAndroid :
    public MOAIGlobalClass < MOAIHockeyAppAndroid, MOAILuaObject > {
private:

    jclass mHockeyApp;
    jmethodID mInitHockeyApp, mCrash, mSendFeedback, mOnLuaCrash;

    //----------------------------------------------------------------//
    static int  _init               ( lua_State* L );
    static int  _crash              ( lua_State* L );
    static int  _sendFeedback       ( lua_State* L );

public:
    void onLuaCrash(const char *message, const char *trace);

    DECL_LUA_SINGLETON ( MOAIHockeyAppAndroid );

            MOAIHockeyAppAndroid        ();
            ~MOAIHockeyAppAndroid       ();
    void    RegisterLuaClass        ( MOAILuaState& state );
};


#endif // MOAIHOCKEYAPPANDROID_H_
