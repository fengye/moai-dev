/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 Copyright 2013 Medium Entertainment, Inc.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 PHPushRequestTest.m
 playhaven-sdk-ios

 Created by Anton Fedorchenko on 4/16/13.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHPushRequest.h"

@interface PHPushRequestTest : SenTestCase
@end

@implementation PHPushRequestTest

- (void)testCreation
{
    PHPushRequest *theRequest = [[[PHPushRequest alloc] initWithApp:@"testToken" secret:
                @"testSecret" pushNotificationDeviceToken:nil] autorelease];
    STAssertNotNil(theRequest, @"Cannot create request through designated initializer");
}

- (void)testRegistration
{
    NSString *theTestToken = @"testToken";
    PHPushRequest *theRequest = [PHPushRequest requestForApp:@"testToken" secret:@"testSecret"
                pushNotificationDeviceToken:[theTestToken dataUsingEncoding:NSUTF8StringEncoding]];
    STAssertNotNil(theRequest, @"Cannot create request instance with test parameters");

    STAssertEqualObjects(theRequest.urlPath, @"http://api2.playhaven.com/v3/publisher/push/",
                @"Unexpected request end point");

    NSString *theHexPushToken = [theRequest.additionalParameters objectForKey:@"push_token"];
    STAssertEqualObjects([[[NSString alloc] initWithData:[[self class] dataFormHexString:
                theHexPushToken] encoding:NSUTF8StringEncoding] autorelease], theTestToken,
                @"Decoded device token doesn't maches the one passed to the request");
}

- (void)testUnregistration
{
    PHPushRequest *theRequest = [PHPushRequest requestForApp:@"testToken" secret:@"testSecret"
                pushNotificationDeviceToken:nil];
    STAssertNotNil(theRequest, @"Cannot create request instance with test parameters");

    STAssertEqualObjects(theRequest.urlPath, @"http://api2.playhaven.com/v3/publisher/push/",
                @"Unexpected request end point");
    
    NSString *thePushToken = [theRequest.additionalParameters objectForKey:@"push_token"];
    STAssertEqualObjects(thePushToken, @"", @"Unregistration request should send empty "
                "device token");
}

#pragma mark - Private

+ (NSString *)hexEncodedStringForData:(NSData *)data
{
    const uint8_t   *bytes  = data.bytes;
    NSMutableString *result = [NSMutableString stringWithCapacity:2 * data.length];

    for (NSUInteger i = 0; i < data.length; i++) {
        [result appendFormat:@"%02x", bytes[i]];
    }

    return result;
}

+ (NSData *)dataFormHexString:(NSString *)aString
{
    NSMutableData *theData = [NSMutableData dataWithCapacity:[aString length] / 2];

    for (NSUInteger theIndex = 0; theIndex < [aString length]; theIndex += 2)
    {
        uint8_t theByte = (uint8_t)strtol([[aString substringWithRange:NSMakeRange(theIndex,2)]
                    UTF8String], NULL, 16);
        [theData appendBytes:&theByte length:1];
    }

    return theData;
}

@end
