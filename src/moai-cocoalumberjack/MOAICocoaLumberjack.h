#ifndef	MOAICOCOALUMBERJACK_H
#define	MOAICOCOALUMBERJACK_H

#if __APPLE__

#include <moai-core/headers.h>
#include <moai-sim/headers.h>

#include <Foundation/Foundation.h>
#import "DDFileLogger.h"

//================================================================//
// MOAICocoaLumberjack
//================================================================//
/**	@name	MOAICocoaLumberjack
	@text	CocoaLumberjack singleton wrapper
*/
class MOAICocoaLumberjack :
	public MOAIGlobalClass< MOAICocoaLumberjack, MOAILuaObject > {

private:
	DDFileLogger	*mFileLogger;
		
public:
		
	DECL_LUA_SINGLETON ( MOAICocoaLumberjack )

					MOAICocoaLumberjack		();
					~MOAICocoaLumberjack	();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Flush					();
	const char*		GetCrashLog				();
	void			GetElapsedTime			( struct time *t );
		
	static void CustomLogFunc ( unsigned int level, const char* tag, cc8* format, va_list args );
};

#endif

#endif
