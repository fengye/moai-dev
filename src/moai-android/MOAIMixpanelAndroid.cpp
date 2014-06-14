#ifndef DISABLE_MIXPANEL

#include <moaiext-android/MOAIMixpanelAndroid.h>

extern JavaVM* jvm;

//----------------------------------------------------------------//
/** @name   init
    @text   Initialize Mixpanel.

    @in     string  token       Mixpanel token.
    @out    nil
 */
int  MOAIMixpanelAndroid::_init ( lua_State* L ) {
    MOAILuaState state ( L );
    MOAIMixpanelAndroid *self = &Get();
    JNI_GET_ENV(jvm, env);

    cc8* token_test = state.GetValue < cc8* >( 1, NULL );
    cc8* token_live = state.GetValue < cc8* >( 2, NULL );

    if (!token_test || !token_live) {

        lua_pushboolean ( state, false );
        return 1;
    }
#ifndef CONFIGURATION_Distribution
    JNI_GET_JSTRING(token_test, jtoken);
#else
    JNI_GET_JSTRING(token_live, jtoken);
#endif

    env->CallStaticVoidMethod(self->mClass, self->mInit, jtoken);
    return 0;
}

//----------------------------------------------------------------//
/** @name   trackEvent
    @text   Track Event.

    @in     string  event       Mixpanel event name.
    @in     table   data        Mixpanel event properties.
    @out    nil
 */
int  MOAIMixpanelAndroid::_trackEvent ( lua_State* L ) {
    MOAILuaState state ( L );
    MOAIMixpanelAndroid *self = &Get();
    JNI_GET_ENV(jvm, env);

    cc8* eventName = state.GetValue < cc8* >( 1, NULL );
    if (eventName == NULL) {
        lua_pushboolean(state, false);
        return 1;
    }
    JNI_GET_JSTRING(eventName, jeventName);
    jobject json = self->tableToJSONObject(state, 2);
    env->CallStaticVoidMethod(self->mClass, self->mTrackEvent, jeventName, json);
    return 0;
}

//----------------------------------------------------------------//
/** @name   registerSuperProperties
    @text   Attach super properties to subsequent events

    @in     table   data        properties.
    @in     boolean once        if true, does not override previous values.
    @out    nil
 */
int  MOAIMixpanelAndroid::_registerSuperProperties ( lua_State* L ) {
    MOAILuaState state ( L );
    MOAIMixpanelAndroid *self = &Get();
    JNI_GET_ENV(jvm, env);

    jobject json = self->tableToJSONObject(state, 1);

    bool once = state.GetValue<bool>(2, false);

    if (json == NULL) {
        lua_pushboolean(state, false);
        return 1;
    }

    env->CallStaticVoidMethod(self->mClass, self->mRegisterSuperProperties, json, once);

    return 0;
}


jobject MOAIMixpanelAndroid::tableToJSONObject(MOAILuaState &state, int stackLocation) {
    JNI_GET_ENV(jvm, env);

    if (!state.IsType(stackLocation, LUA_TTABLE))
        return NULL;
    jobject json = env->NewObject(mJSONClass, mJSONObjectCtor);
    if (json == NULL)
        return NULL;
    lua_pushnil(state);
    while (lua_next ( state, stackLocation )) {
        cc8* key;
        if (key = lua_tostring(state, -2)) {
            jobject value;
            if (value = parseValue(state, -1)) {
                JNI_GET_JSTRING(key, jkey);
                env->CallObjectMethod(json, mJSONPut, jkey, value);
            }
        }
        lua_pop(state, 1);
    }
    return json;
}

jobject MOAIMixpanelAndroid::parseValue(MOAILuaState &state, int idx) {
    JNI_GET_ENV(jvm, env);

    switch (lua_type(state, idx)) {
        case LUA_TSTRING:
        {
            cc8* str = lua_tostring(state, idx);
            JNI_GET_JSTRING(str, jstr);
            return jstr;
        }
        break;
        case LUA_TNUMBER:
            return env->NewObject(mDoubleClass, mDoubleCtor, state.GetValue<double>(idx, 0));
        break;
        case LUA_TBOOLEAN:
            return env->NewObject(mBooleanClass, mBooleanCtor, state.GetValue<bool>(idx, false));
        break;
        default:
            USLog::Print("Error, invalid type %d when parsing MIXPANEL parameters in %s lin %s", lua_type(state, idx), __FILE__, __LINE__);
        break;
    }
    return NULL;
}


MOAIMixpanelAndroid::MOAIMixpanelAndroid () {
    JNI_GET_ENV(jvm, env);

    mClass = findClassOrDie(env, JAVA_CLASS);
    mClass = (jclass)env->NewGlobalRef(mClass);

    mJSONClass = findClassOrDie(env, JSON_CLASS);
    mJSONClass = (jclass)env->NewGlobalRef(mJSONClass);
    mJSONObjectCtor = findJavaMethod(env, mJSONClass, "<init>", "()V");

    mDoubleClass = findClassOrDie(env, DOUBLE_CLASS);
    mDoubleClass = (jclass)env->NewGlobalRef(mDoubleClass);
    mDoubleCtor = findJavaMethod(env, mDoubleClass, "<init>", "(D)V");

    mBooleanClass = findClassOrDie(env, BOOLEAN_CLASS);
    mBooleanClass = (jclass)env->NewGlobalRef(mBooleanClass);
    mBooleanCtor = findJavaMethod(env, mBooleanClass, "<init>", "(Z)V");


    mInit = findJavaMethod(env, mClass, "initMixPanel", "(Ljava/lang/String;)V", true);
    mTrackEvent = findJavaMethod(env, mClass, "track", "(Ljava/lang/String;Lorg/json/JSONObject;)V", true);
    mRegisterSuperProperties = findJavaMethod(env, mClass, "registerSuperProperties", "(Lorg/json/JSONObject;Z)V", true);

    mJSONPut = findJavaMethod(env, mJSONClass, "put", "(Ljava/lang/String;Ljava/lang/Object;)Lorg/json/JSONObject;");
}

MOAIMixpanelAndroid::~MOAIMixpanelAndroid () {

}

void MOAIMixpanelAndroid::RegisterLuaClass ( MOAILuaState& state ) {
    luaL_Reg regTable [] = {
        { "init",                    _init },
        { "trackEvent",              _trackEvent },
        { "registerSuperProperties", _registerSuperProperties },
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}


#endif // DISABLE_MIXPANEL

