#include <moai-cocoalumberjack/MOAICocoaLumberjack.h>

#import "DDLog.h"
#import "DDNSLoggerLogger.h"
#import "DDASLLogger.h"
#import "DDTTYLogger.h"
#import "DDASLLogCapture.h"

#define CRASHLOG_SIZE (50 * 1024)

//----------------------------------------------------------------//
@interface PZLogFormatter : NSObject <DDLogFormatter>
@end

@implementation PZLogFormatter

- ( NSString * )formatLogMessage: ( DDLogMessage * )logMessage
{
	struct ZLLog::time t;
	ZLLog::GetElapsedTime(&t);
	
	NSString *dateAndTime = [NSString stringWithFormat:@"%.2d:%.2d:%.2d.%.3d", t.h, t.m, t.s, t.ms];
	return [NSString stringWithFormat:@"%@ [%@] %@", dateAndTime, logMessage->tag, logMessage->logMsg];
}

@end

//----------------------------------------------------------------//
void MOAICocoaLumberjack::CustomLogFunc ( unsigned int level, const char* tag, cc8* format, va_list args )
{
	level = MAX ( MIN ( level, 4 ), 0 ); // MAX level is MOAILogMgr::LOG_DEBUG
	int flag = level == 0 ? 0 : 1 << ( level - 1 );
	
	[DDLog log:YES
		 level:0xFF
		  flag:flag
	   context:0
		  file:""
	  function:""
		  line:0
		   tag:[NSString stringWithUTF8String:tag]
		format:[[NSString stringWithUTF8String:format] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]]
		  args:args];
}

//----------------------------------------------------------------//
MOAICocoaLumberjack::MOAICocoaLumberjack ()
{
    RTTI_SINGLE ( MOAILuaObject )
	
	ZLLog::SetCustomLogfunc(CustomLogFunc);
	
	PZLogFormatter *formatter = [[PZLogFormatter alloc] init];
	
	// setup logging first
	mFileLogger = [[DDFileLogger alloc] init];
	
	// no rolling frequency, as we might want to access
	// older files if an app is restarted a while after the last crash
	mFileLogger.logFileManager.maximumNumberOfLogFiles = 1; // saving 1 older file
	mFileLogger.maximumFileSize = 2 * CRASHLOG_SIZE; // 50K max files
	[mFileLogger setLogFormatter:formatter];
	[DDLog addLogger:mFileLogger];
	
	[[DDTTYLogger sharedInstance] setLogFormatter:formatter];
	[DDLog addLogger:[DDTTYLogger sharedInstance]];
	[[DDASLLogger sharedInstance] setLogFormatter:formatter];
	[DDLog addLogger:[DDASLLogger sharedInstance]];
}

//----------------------------------------------------------------//
MOAICocoaLumberjack::~MOAICocoaLumberjack ()
{
}

//----------------------------------------------------------------//
void MOAICocoaLumberjack::RegisterLuaClass ( MOAILuaState& state )
{
	UNUSED(state);
}

//----------------------------------------------------------------//
void MOAICocoaLumberjack::RegisterLuaFuncs ( MOAILuaState& state )
{
	UNUSED(state);
}

//----------------------------------------------------------------//
void MOAICocoaLumberjack::Flush ()
{
	[DDLog flushLog];
	[[DDNSLoggerLogger sharedInstance] flush];
}

//----------------------------------------------------------------//
const char*	MOAICocoaLumberjack::GetCrashLog ()
{
	MOAICocoaLumberjack::Get ().Flush();
	
	NSUInteger lenTotal = 0;
	NSUInteger logIndex = 0;
	NSString *crashLog = @"";
	
	BOOL truncated = NO;
	while (lenTotal < CRASHLOG_SIZE && logIndex < [[mFileLogger.logFileManager sortedLogFileInfos] count]) {
		DDLogFileInfo* logInfo = [[mFileLogger.logFileManager sortedLogFileInfos] objectAtIndex:logIndex];
		NSData *log = [NSData dataWithContentsOfFile:logInfo.filePath
											 options:NSDataReadingMappedIfSafe
											   error:nil];
		if (log) {
			// truncate to last N KBytes
			NSUInteger len = MIN([log length], CRASHLOG_SIZE-lenTotal);
			lenTotal += len;
			if (len < [log length]) {
				log = [log subdataWithRange:NSMakeRange([log length]-len, len)];
				truncated = YES;
			}
			NSString *subCrashLog = [[NSString alloc] initWithBytes:[log bytes]
															  length:[log length]
															encoding:NSUTF8StringEncoding];
			// going back in time, so add the new one before the previous one
			crashLog = [subCrashLog stringByAppendingString:crashLog];
		} else {
			// no good, end
			break;
		}
		logIndex++;
	}
	if (truncated) {
		// to cleanup, remove the first truncated line (go to the first \n)
		NSUInteger subIndex = [crashLog rangeOfString:@"\n"].location;
		if (subIndex != NSNotFound) {
			crashLog = [crashLog substringFromIndex:subIndex+1];
		}
	}
	return [crashLog UTF8String];
}
