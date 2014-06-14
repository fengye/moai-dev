#include <jni.h>
#include <moaiext-android/MOAIHockeyAppAndroid.h>
#include <moaiext-android/MOAIWebViewAndroid.h>
#include <moaiext-android/moaiext-jni.h>
#import "PZLogger.h"

extern JavaVM* jvm;


static void _ErrorTraceback ( const char *message, struct lua_State* L, int level ) {
    MOAILuaState state ( L );
    MOAIHockeyAppAndroid *self = &MOAIHockeyAppAndroid::Get();
    JNI_GET_ENV(jvm, env);
    state.PrintStackTrace ( USLog::CONSOLE, level );

    STLString trace = state.GetStackTrace ( level );
    self->onLuaCrash(message, trace.str());
}

void MOAIHockeyAppAndroid::onLuaCrash(const char *message, const char *trace) {
    JNI_GET_ENV(jvm, env);
    JNI_GET_JSTRING(trace, jtrace);
    JNI_GET_JSTRING(message, jmessage);
    env->CallStaticVoidMethod(mHockeyApp, mOnLuaCrash, jmessage, jtrace);
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/** @name   init
    @text   Initialize HockeyApp.

    @in     string betaID           Available in HockeyApp dashboard settings.
    @in     string liveID           Available in HockeyApp dashboard settings.
    @in     bool   disabled         Disable crash reporting. Default to false.
    @out    nil
 */
int MOAIHockeyAppAndroid::_init ( lua_State* L ) {
    MOAILuaState state ( L );
    MOAIHockeyAppAndroid *self = &Get();
    JNI_GET_ENV(jvm, env);

    cc8* appBetaId = lua_tostring ( state, 1 );
    cc8* appLiveId = lua_tostring ( state, 2 );
    bool disabled  = state.GetValue < bool >( 3, false );
    bool distribution = false;
    bool adHoc = false;

#ifdef CONFIGURATION_Distribution
    distribution = true;
#endif

#ifdef CONFIGURATION_AdHoc
    adHoc = true;
#endif

    AKUSetFunc_ErrorTraceback ( _ErrorTraceback );

    JNI_GET_JSTRING(appLiveId, jappLiveId);
    JNI_GET_JSTRING(appBetaId, jappBetaId);
    env->CallStaticVoidMethod(self->mHockeyApp, self->mInitHockeyApp, jappLiveId, jappBetaId, !disabled, distribution, adHoc);


    return 0;
}

//----------------------------------------------------------------//
/** @name   crash
    @text   Simulate a crash. Only works when app is not running in AppStore

    @out    nil
 */
int MOAIHockeyAppAndroid::_crash ( lua_State* L ) {
    MOAILuaState state ( L );
    MOAIHockeyAppAndroid *self = &Get();
    JNI_GET_ENV(jvm, env);

    env->CallStaticVoidMethod(self->mHockeyApp, self->mCrash);

    return 0;
}


//----------------------------------------------------------------//
/** @name   sendFeedback
    @text   Open feedback dialog.

    @out    nil
 */
int MOAIHockeyAppAndroid::_sendFeedback ( lua_State* L ) {
    MOAILuaState state ( L );
    MOAIHockeyAppAndroid *self = &Get();
    JNI_GET_ENV(jvm, env);

    env->CallStaticVoidMethod(self->mHockeyApp, self->mSendFeedback);

    return 0;
}




//================================================================//
// MOAIHockeyAppAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIHockeyAppAndroid::MOAIHockeyAppAndroid () {

    RTTI_SINGLE ( MOAILuaObject )

    JNI_GET_ENV(jvm, env);

    mHockeyApp = findClassOrDie(env, HOCKEYAPP_CLASS);
    mHockeyApp = (jclass)env->NewGlobalRef(mHockeyApp);

    mInitHockeyApp = findJavaMethod(env, mHockeyApp, "initHockeyApp", "(Ljava/lang/String;Ljava/lang/String;ZZZ)V", true);
    mCrash = findJavaMethod(env, mHockeyApp, "crash", "()V", true);
    mSendFeedback = findJavaMethod(env, mHockeyApp, "sendFeedback", "()V", true);
    mOnLuaCrash = findJavaMethod(env, mHockeyApp, "onLuaCrash", "(Ljava/lang/String;Ljava/lang/String;)V", true);
}

//----------------------------------------------------------------//
MOAIHockeyAppAndroid::~MOAIHockeyAppAndroid () {

}

//----------------------------------------------------------------//
void MOAIHockeyAppAndroid::RegisterLuaClass ( MOAILuaState& state ) {


    luaL_Reg regTable[] = {
        { "init",               _init },
        { "crash",              _crash },
        { "sendFeedback",       _sendFeedback },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}


extern "C" {

    // Curently this returns an empty string because PZ doesn't cache the log for android
    jstring Java_com_ziplinegames_moai_Moai_AKUPZGetCrashLog() {
        JNI_GET_ENV(jvm, env);
        const char * log = PZGetCrashLog();
        return env->NewStringUTF(log);
    }
}



