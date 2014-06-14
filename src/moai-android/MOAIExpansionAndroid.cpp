#include "MOAIExpansionAndroid.h"

extern JavaVM* jvm;

/**
* @name loadExpansion
* @text Loads the specified expansion
*
* @in String expansion filename
* @in Int expansion file size
* @in String mountpoint where to mount the expansion after download
* @out Bool is present -> true if expansion is already loaded. False if download is required
*/
int MOAIExpansionAndroid::_loadExpansion( lua_State* L ) {
    MOAIExpansionAndroid *self = &Get();
    MOAILuaState state(L);

    JNI_GET_ENV(jvm, env);

    const char * file = luaL_checkstring(L, 2);
    int size = luaL_checknumber(L, 3);
    const char * mountPoint = luaL_checkstring(L, 4);

    bool alreadyLoaded = self->loadExpansion(file, size, mountPoint);

    lua_pushboolean(state, alreadyLoaded);
    return 1;
}



/**
* @name getExpansionFilename
* @text Gets the expansion filename associated with this app
*
* @in Bool isMain true if expansion is the main expansion file. False for patch
* @in Int version is the version number of the expansion file
*/
int MOAIExpansionAndroid::_getExpansionFilename( lua_State* L ) {
    MOAILuaState state(L);
    JNI_GET_ENV(jvm, env);

    bool isMain = lua_toboolean(L, 2);
    int version = luaL_checknumber(L, 3);

    const char *filename = Get().getExpansionFilename(isMain, version);

    if (filename == NULL)
        return 0;

    lua_pushstring(state, filename);


    return 1;
}

/**
* @name pauseDownload
* @text pauses or resume the download process
*
* @in Bool pause : true->pause, false->resume
*/
int MOAIExpansionAndroid::_pauseDownload( lua_State* L ) {
    MOAIExpansionAndroid *self = &Get();
    MOAILuaState state(L);
    JNI_GET_ENV(jvm, env);

    bool pause = lua_toboolean(L, 2);

    self->pauseDownload(pause);
    return 0;
}


/**
* @name abortDownload
* @text cancels the current download process
*
*/
int MOAIExpansionAndroid::_abortDownload( lua_State* L ) {
    MOAIExpansionAndroid *self = &Get();
    MOAILuaState state(L);
    JNI_GET_ENV(jvm, env);

    self->abortDownload();
    return 0;
}


/**
* @name getState
* @text Retrieve the current download state. State corresponds to IDownloaderClient.java attributes
*
* @out int State -> The current download state
*/
int MOAIExpansionAndroid::_getState( lua_State* L) {
    MOAIExpansionAndroid *self = &Get();
    MOAILuaState state(L);
    JNI_GET_ENV(jvm, env);

    lua_pushnumber(state, self->mState);

    return 1;
}

/**
* @name getProgress
* @text Retrieve the current download progress.
*
* @out float speed -> download speed in KB/s
* @out long progress -> download progress in bytes
* @out long total -> total download size in bytes
* @out long timeRemaining -> remaining download time in seconds
*/
int MOAIExpansionAndroid::_getProgress( lua_State* L) {
    MOAIExpansionAndroid *self = &Get();
    MOAILuaState state(L);
    JNI_GET_ENV(jvm, env);

    lua_pushnumber(state, self->mSpeed);
    lua_pushnumber(state, self->mProgress);
    lua_pushnumber(state, self->mTotal);
    lua_pushnumber(state, self->mTimeRemaining);

    return 4;
}


bool MOAIExpansionAndroid::loadExpansion( const char* filename, long size, const char *mountPoint) {
    JNI_GET_ENV(jvm, env);
    USLog::Print("[MOAIExpansionAndroid] load expansion : %s, size : %d, mount : %s", filename, size, mountPoint);
    mState = 0;
    mSpeed = 0;
    mProgress = 0;
    mTotal = 0;
    mTimeRemaining = 0;

    jstring mount = env->NewStringUTF(mountPoint);
    jstring file = env->NewStringUTF(filename);
    jboolean ret = env->CallStaticBooleanMethod(mMoaiClass, mLoadExpansion, file, mount, size);
    env->DeleteLocalRef(mount);
    env->DeleteLocalRef(file);
    return ret == JNI_TRUE;
}

// Return value needs to be freed
const char *MOAIExpansionAndroid::getExpansionFilename( bool isMain, int expansionVersion ) {
    JNI_GET_ENV(jvm, env);

    USLog::Print("[MOAIExpansionAndroid] getExpansionFilename %s version %d env : %p", (isMain ? "MAIN" : "PATCH"), expansionVersion, env);

    freeLastString();
    jobject file = (jstring)env->CallStaticObjectMethod(mMoaiClass, mGetExpansionFilename, isMain, expansionVersion);
    if (file == NULL) {
        return NULL;
    }
    mLastString = (jstring)env->NewGlobalRef(file);
    mLastCharString = env->GetStringUTFChars(mLastString, NULL);
    return mLastCharString;
}

void MOAIExpansionAndroid::pauseDownload( bool pause ) {
    JNI_GET_ENV(jvm, env);
    USLog::Print("[MOAIExpansionAndroid] %s", pause ? "Pausing" : "Resuming");
    env->CallStaticVoidMethod(mMoaiClass, mPauseDownload, pause);
}

void MOAIExpansionAndroid::abortDownload( ) {
    JNI_GET_ENV(jvm, env);
    USLog::Print("[MOAIExpansionAndroid] abort download");
    env->CallStaticVoidMethod(mMoaiClass, mAbortDownload);
}

void MOAIExpansionAndroid::freeLastString() {
    JNI_GET_ENV(jvm, env);
    if (env == NULL || mLastString == NULL || mLastCharString == NULL)
        return;
    env->ReleaseStringUTFChars(mLastString, mLastCharString);
    env->DeleteGlobalRef(mLastString);
    mLastString = NULL;
    mLastCharString = NULL;
}

void MOAIExpansionAndroid::onDownloadProgress(float currentSpeed, long overallProgress, long overallTotal, long timeRemaining) {
    USLog::Print("[MOAIExpansionAndroid] onDownloadProgress %f %d %d %d", currentSpeed, overallProgress, overallTotal, timeRemaining);

    mSpeed = currentSpeed;
    mProgress = overallProgress;
    mTotal = overallTotal;
    mTimeRemaining = timeRemaining;
}

void MOAIExpansionAndroid::onStateChange(int downState) {
    USLog::Print("[MOAIExpansionAndroid] onStateChange %d", downState);

    mState = downState;
}


MOAIExpansionAndroid::MOAIExpansionAndroid() {

    JNI_GET_ENV(jvm, env);
    USLog::Print("[MOAIExpansionAndroid] constructed");


    mLastString = NULL;
    mLastCharString = NULL;

    mMoaiClass = findClassOrDie(env, MOAI_CLASS);

    mMoaiClass = (jclass)env->NewGlobalRef(mMoaiClass);


    mLoadExpansion = findJavaMethod(env, mMoaiClass, "loadExpansion", "(Ljava/lang/String;Ljava/lang/String;J)Z", true);
    mGetExpansionFilename = findJavaMethod(env, mMoaiClass, "getExpansionFilename", "(ZI)Ljava/lang/String;", true);
    mPauseDownload = findJavaMethod(env, mMoaiClass, "pauseDownload", "(Z)V", true);
    mAbortDownload = findJavaMethod(env, mMoaiClass, "abortDownload", "()V", true);

    RTTI_SINGLE ( MOAILuaObject )
}


MOAIExpansionAndroid::~MOAIExpansionAndroid(){
    freeLastString();
}



//----------------------------------------------------------------//
void MOAIExpansionAndroid::RegisterLuaClass ( MOAILuaState& state ) {
    JNI_GET_ENV(jvm, env);

    USLog::Print("MOAIExpansionAndroid registering enum fields");

    // Setting IDownloaderClient fields

    // keep as local ref should be okay
    jclass idownloader = findClassOrDie(env, IDOWNLOADER_CLASS);
    jclass idownloaderClass = env->GetObjectClass(idownloader);
    if (idownloaderClass == NULL)
        jniFail("MOAIExpansionAndroid", "unable to find class's" IDOWNLOADER_CLASS " class");
    jclass fieldClass = findClassOrDie(env, FIELD_CLASS);
    jclass modifierClass = findClassOrDie(env, MODIFIER_CLASS);
    jmethodID isStaticMethod = findJavaMethod(env, modifierClass, "isStatic", "(I)Z", true);
    jmethodID getModifiers = findJavaMethod(env, fieldClass, "getModifiers", "()I");
    jmethodID fieldsMethod = findJavaMethod(env, idownloaderClass, "getDeclaredFields", "()[L" FIELD_CLASS ";");
    jmethodID getFieldValue = findJavaMethod(env, fieldClass, "getInt", "(Ljava/lang/Object;)I");
    jobjectArray fieldArray = (jobjectArray)env->CallObjectMethod(idownloader, fieldsMethod);
    if (fieldArray == NULL) {
        jniFail("MOAIExpansionAndroid", "unable to retrieve " IDOWNLOADER_CLASS " class fields");
    }
    jsize len = env->GetArrayLength(fieldArray);
    for (jsize i = 0; i < len; ++i) {
        jobject field = env->GetObjectArrayElement(fieldArray , i);
        jint modifier = env->CallIntMethod(field, getModifiers);
        bool isStatic = env->CallStaticBooleanMethod(modifierClass, isStaticMethod, modifier);
        if (!isStatic)
            continue;
        jmethodID getName = env->GetMethodID(fieldClass , "getName" , "()Ljava/lang/String;");
        jstring _name = (jstring) env->CallObjectMethod(field, getName ) ;
        const char *str = env->GetStringUTFChars(_name, 0);
        if (strncmp(str, "STATE_", 6) == 0) {
            int value = env->CallIntMethod(field, getFieldValue, NULL);
            state.SetField(-1, str, value);
            USLog::Print("Added field %s=%d", str, value);
        }
        env->ReleaseStringUTFChars(_name, str);
    }

    //--


    luaL_Reg regTable [] = {

        { "loadExpansion",              _loadExpansion},
        { "getExpansionFilename",       _getExpansionFilename},
        { "pauseDownload",              _pauseDownload},
        { "abortDownload",              _abortDownload},
        { "getState",                   _getState},
        { "getProgress",                _getProgress},
        { NULL, NULL }
    };

    luaL_register ( state, 0, regTable );
}


extern "C" {

    void Java_com_ziplinegames_moai_Moai_AKUOnExpansionDownloadProgress( JNIEnv* env, jclass obj, jlong overallProgress, jlong overallTotal, jlong timeRemaining, jfloat currentSpeed) {
        MOAIExpansionAndroid::Get().onDownloadProgress(currentSpeed, overallProgress, overallTotal, timeRemaining);
    }

    void Java_com_ziplinegames_moai_Moai_AKUOnExpansionDownloadStateChanged( JNIEnv* env, jclass obj, jint downState) {
        MOAIExpansionAndroid::Get().onStateChange(downState);
    }

}