#ifndef MOAIEXPANSIONANDROID_H_
# define MOAIEXPANSIONANDROID_H_

#include <moaicore/moaicore.h>
#include <jni.h>

#include <moaiext-android/moaiext-jni.h>

#define MOAI_CLASS "com/ziplinegames/moai/Moai"
#define IDOWNLOADER_CLASS "com/google/android/vending/expansion/downloader/IDownloaderClient"
#define FIELD_CLASS "java/lang/reflect/Field"
#define MODIFIER_CLASS "java/lang/reflect/Modifier"

class MOAIExpansionAndroid : public MOAIGlobalClass < MOAIExpansionAndroid, MOAILuaObject > {
private:
    jclass mMoaiClass;

    // enum {
    //     PROGRESS_CHANGE,
    //     STATE_CHANGE,
    //     TOTAL
    // };

    jmethodID mLoadExpansion, mGetExpansionFilename, mPauseDownload, mAbortDownload, mSetListener;

    //MOAILuaRef mListeners[TOTAL];


    jstring     mLastString;
    const char *mLastCharString;
    int         mState;
    float       mSpeed;
    long        mProgress;
    long        mTotal;
    long        mTimeRemaining;


    static int _loadExpansion( lua_State* L );
    static int _getExpansionFilename( lua_State* L );
    static int _pauseDownload( lua_State* L );
    static int _abortDownload( lua_State* L );
    //static int _setListener(lua_State* L);
    static int _getState(lua_State* L);
    static int _getProgress(lua_State* L);


    bool loadExpansion( const char* filename, long size, const char *mountPoint);
    const char *getExpansionFilename( bool isMain, int expansionVersion);
    void pauseDownload( bool pause );
    void abortDownload( );

    void freeLastString();

public:

    DECL_LUA_SINGLETON ( MOAIExpansionAndroid )


    void onDownloadProgress(float currentSpeed, long overallProgress, long overallTotal, long timeRemaining);
    void onStateChange(int state);


    //----------------------------------------------------------------//
                    MOAIExpansionAndroid          ();
                    ~MOAIExpansionAndroid         ();
    void            RegisterLuaClass    ( MOAILuaState& state );
};
#endif // MOAIEXPANSIONANDROID_H_
