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

 PHPushProviderTest.m
 playhaven-sdk-ios

 Created by Anton Fedorchenko on 4/26/13
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#import <SenTestingKit/SenTestingKit.h>
#import "PHPushProvider.h"
#import "PHPublisherContentRequest.h"
#import "PHError.h"

@interface PHPushProviderTest : SenTestCase <PHPushRegistrationObserver, PHPushProviderDelegate>
@property (nonatomic, retain) NSError *registrationError;
@property (nonatomic, retain) PHPublisherContentRequest *contentRequest;
@property (nonatomic, assign) BOOL shouldOpenURLCalled;
@property (nonatomic, retain) NSURL *pushTriggeredURL;
@end

@implementation PHPushProviderTest

- (void)setUp
{
    self.contentRequest = nil;
    self.shouldOpenURLCalled = NO;
    self.pushTriggeredURL = nil;
}

- (void)testProviderInstance
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    STAssertNotNil(theProvider, @"");
    STAssertEqualObjects(theProvider, [PHPushProvider sharedInstance], @"");
}

- (void)testRegistration
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationSecret = nil;
    
    [theProvider addObserver:self];
    
    NSData *theTestToken = [@"testToken" dataUsingEncoding:NSUTF8StringEncoding];
    [theProvider registerAPNSDeviceToken:theTestToken];
    STAssertNotNil(self.registrationError, @"Registration should fail if provider's app token or"
                " secret is nil");
    STAssertEqualObjects([self.registrationError code], PHErrorIncompleteWorkflow, @"");
    
    [theProvider removeObserver:self];
    
    self.registrationError = nil;
    [theProvider registerAPNSDeviceToken:theTestToken];
    STAssertNil(self.registrationError, @"");
}

- (void)testPushNotificationHandlingCase1
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSString *theTestContentID = @"5767235784";
    NSNumber *theTestMessageID = @(438744);

    // Check that push handling works with content ID that is a string
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"ci" :
                theTestContentID}];

    STAssertNotNil(self.contentRequest, @"Delegate method was not called during push notification "
                "handling.");
    STAssertTrue([self.contentRequest isKindOfClass:[PHPublisherContentRequest class]], @"");
    
    STAssertEqualObjects([[self.contentRequest additionalParameters] objectForKey:@"content_id"],
                theTestContentID, @"The content request sent as a result of push handling does not"
                " contain expected content unit ID");
    STAssertEqualObjects([[self.contentRequest additionalParameters] objectForKey:@"message_id"],
                [theTestMessageID stringValue], @"The content request sent as a result of push "
                "handling does not contain expected message ID");
}

- (void)testPushNotificationHandlingCase2
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSUInteger theTestContentID = 3238;
    NSNumber *theTestMessageID = @(43844657678);
    
    // Check that push handling works with content ID that is a integer
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"ci" :
                @(theTestContentID)}];

    STAssertNotNil(self.contentRequest, @"Delegate method was not called during push notification "
                "handling.");
    STAssertTrue([self.contentRequest isKindOfClass:[PHPublisherContentRequest class]], @"");
    
    STAssertEqualObjects([[self.contentRequest additionalParameters] objectForKey:@"content_id"],
                [@(theTestContentID) stringValue], @"The content request sent as a result of push "
                "handling does not contain expected content unit ID");
    STAssertEqualObjects([[self.contentRequest additionalParameters] objectForKey:@"message_id"],
                [theTestMessageID stringValue], @"The content request sent as a result of push "
                "handling does not contain expected message ID");
}

- (void)testPushNotificationHandlingCase3
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSUInteger theTestContentID = 3238;
    NSString *theTestMessageID = @"43844657678";
    
    // Check that push handler dismisses push notification if message id (mi) field contains
    // non-numeric value
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"ci" :
                @(theTestContentID)}];

    STAssertNil(self.contentRequest, @"Content request should not be sent if notification payload"
                "is not valid");
}

- (void)testPushNotificationHandlingCase4
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSUInteger theTestContentID = 3238;
    
    // Check that push handler dismisses push notification if message id (mi) field is missed
    [theProvider handleRemoteNotificationWithUserInfo:@{@"ci" : @(theTestContentID)}];

    STAssertNil(self.contentRequest, @"Content request should not be sent if notification payload"
                "is not valid");
}

- (void)testPushNotificationHandlingCase5
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSNumber *theTestMessageID = @(43844657678);
    NSURL *theURLToOpen = [NSURL URLWithString:
                @"https://itunes.apple.com/ru/app/sol-runner/id566179205?l=en&mt=8"];
    
    // Check that URL specified in the push notification payload is opened
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"uri" :
                [theURLToOpen absoluteString]}];
    
    STAssertTrue(self.shouldOpenURLCalled, @"Expected delegate method was called.");
    STAssertEqualObjects(theURLToOpen, self.pushTriggeredURL, @"Requested URL should match "
                "specified");
}

- (void)testPushNotificationHandlingCase6
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSNumber *theTestMessageID = @(43844657678);
    
    // Check that push handling doesn't try to open malformed URL
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"uri" :
                @"https//#$%^&*) (*itunes.apple.com"}];
    
    STAssertFalse(self.shouldOpenURLCalled, @"The delegate method asking about URL opening should "
                "not be called for malformed URL.");
}

- (void)testPushNotificationHandlingCase7
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];

    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSUInteger theTestContentID = 3238;
    NSNumber *theTestMessageID = @(43844657678);
    
    // Check that if push handling contains both content ID and uri then the last one is ignored
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"ci" :
                @(theTestContentID), @"uri" : @"https://itunes.apple.com"}];

    STAssertNotNil(self.contentRequest, @"Delegate method was not called during push notification "
                "handling.");
    
    STAssertEqualObjects([[self.contentRequest additionalParameters] objectForKey:@"content_id"],
                [@(theTestContentID) stringValue], @"The content request sent as a result of push "
                "handling does not contain expected content unit ID");
    STAssertEqualObjects([[self.contentRequest additionalParameters] objectForKey:@"message_id"],
                [theTestMessageID stringValue], @"The content request sent as a result of push "
                "handling does not contain expected message ID");
    
    STAssertFalse(self.shouldOpenURLCalled, @"The delegate method asking about URL opening should "
                "not be called if content id (ci) is provided in the push payload");
}

- (void)testPushNotificationHandlingCase8
{
    PHPushProvider *theProvider = [PHPushProvider sharedInstance];
    
    theProvider.applicationToken = @"testToken";
    theProvider.applicationSecret = @"testSecret";
    
    theProvider.delegate = self;
    
    NSNumber *theTestMessageID = @(43844657678);
    NSURL *theURLToOpen = [NSURL URLWithString:@"https%3A%2F%2Fitunes.apple.com%2Fru%2Fapp%2F"
                "sol-runner%2Fid566179205%3Fl%3Den%26mt%3D8"];
    
    NSURL *theDecodedURL = [NSURL URLWithString:[[theURLToOpen absoluteString]
                stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    STAssertNotNil(theDecodedURL, @"Cannot decode the given URL: %@", theURLToOpen);
    
    // Check that URL specified in the push notification payload is opened
    [theProvider handleRemoteNotificationWithUserInfo:@{@"mi" : theTestMessageID, @"uri" :
                [theURLToOpen absoluteString]}];
    
    STAssertTrue(self.shouldOpenURLCalled, @"Expected delegate method was called.");
    STAssertEqualObjects(theDecodedURL, self.pushTriggeredURL, @"Requested URL should be URL "
                "decoded");
}

#pragma mark - PHPushRegistrationObserver

- (void)provider:(PHPushProvider *)aProvider
            didFailToRegisterAPNSDeviceTokenWithError:(NSError *)anError
{
    self.registrationError = anError;
}

#pragma mark - PHPushProviderDelegate

- (BOOL)pushProvider:(PHPushProvider *)aProvider
            shouldSendRequest:(PHPublisherContentRequest *)aRequest
{
    self.contentRequest = aRequest;
    return NO;
}

- (BOOL)pushProvider:(PHPushProvider *)aProvider shouldOpenURL:(NSURL *)anURL
{
    self.shouldOpenURLCalled = YES;
    self.pushTriggeredURL = anURL;
    return NO;
}

#pragma mark - Private

- (void)dealloc
{
    [_contentRequest release];
    [_registrationError release];
    [_pushTriggeredURL release];
    
    [super dealloc];
}

@end
