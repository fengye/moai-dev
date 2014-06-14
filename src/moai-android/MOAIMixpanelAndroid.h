

#ifndef MOAIMIXPANELANDROID_H
#define MOAIMIXPANELANDROID_H

#ifndef DISABLE_MIXPANEL

#import <moaicore/moaicore.h>
#include <jni.h>
#include <moaiext-android/moaiext-jni.h>

#define JAVA_CLASS "com/plumzi/mixpanel/MixPanelWrapper"
#define JSON_CLASS "org/json/JSONObject"
#define DOUBLE_CLASS "java/lang/Double"
#define BOOLEAN_CLASS "java/lang/Boolean"

//================================================================//
// MOAIMixpanelAndroid
//================================================================//
/** @name   MOAIMixpanelAndroid
    @text   Wrapper for Mixpanel integration on Android devices.
 */
class MOAIMixpanelAndroid :
    public MOAIGlobalClass < MOAIMixpanelAndroid, MOAILuaObject > {
private:
    jclass  mClass,
            mJSONClass,
            mDoubleClass,
            mBooleanClass;
    jmethodID   mInit,
                mTrackEvent,
                mRegisterSuperProperties,
                mJSONObjectCtor,
                mJSONPut,
                mDoubleCtor,
                mBooleanCtor;

    //----------------------------------------------------------------//
    static int  _init                       ( lua_State* L );
    static int  _trackEvent                 ( lua_State* L );
    static int  _registerSuperProperties    ( lua_State* L );

    jobject tableToJSONObject(MOAILuaState &state, int stackLocation);
    jobject parseValue(MOAILuaState &state, int idx);

public:

    DECL_LUA_SINGLETON ( MOAIMixpanelAndroid );

             MOAIMixpanelAndroid        ();
             ~MOAIMixpanelAndroid       ();
    void     RegisterLuaClass       ( MOAILuaState& state );
};

#endif  // DISABLE_MIXPANEL

#endif  // MOAIMIXPANELANDROID_H