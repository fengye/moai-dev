//
//  DDNSLoggerLogger.h
//  Created by Peter Steinberger on 26.10.10.
//

#import <Foundation/Foundation.h>
#import "DDLog.h"

#define PZ_BASE_CONTEXT 0x1000
#define PZ_LUA_CONTEXT		PZ_BASE_CONTEXT + 1
#define PZ_MOAI_CONTEXT		PZ_BASE_CONTEXT + 2
#define PZ_VIDEO_CONTEXT	PZ_BASE_CONTEXT + 3
#define PZ_PZ_CONTEXT		PZ_BASE_CONTEXT + 4

@interface DDNSLoggerLogger : DDAbstractLogger <DDLogger>
{
}

+ (DDNSLoggerLogger *)sharedInstance;

- (void)setupWithBonjourServiceName:(NSString *)serviceName;
- (void)flush;

// Inherited from DDAbstractLogger

// - (id <DDLogFormatter>)logFormatter;
// - (void)setLogFormatter:(id <DDLogFormatter>)formatter;

@end
