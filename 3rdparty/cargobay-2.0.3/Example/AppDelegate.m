// AppDelegate.m
//
// Copyright (c) 2012 Mattt Thompson (http://mattt.me/)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "AppDelegate.h"

#import "ViewController.h"

#import "CargoBay.h"

@implementation AppDelegate

- (BOOL)application:(__unused UIApplication *)application didFinishLaunchingWithOptions:(__unused NSDictionary *)launchOptions
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:[CargoBay sharedManager]];
    
    [[CargoBay sharedManager] setTransactionIDUniquenessVerificationWithBlock:^BOOL(__unused NSString *transactionID) {
        return YES;
    }];
    
    [[CargoBay sharedManager] setPaymentQueueUpdatedTransactionsBlock:^(SKPaymentQueue *queue, NSArray *transactions) {
        for (SKPaymentTransaction *transaction in transactions)
        {
            switch (transaction.transactionState)
            {
                case SKPaymentTransactionStatePurchased:
                case SKPaymentTransactionStateRestored: {
                    [[CargoBay sharedManager] verifyTransaction:transaction
                                                       password:nil
                                                        success:^(__unused NSDictionary *receipt) {
                                                            if (transaction.downloads) {
                                                                [queue startDownloads:transaction.downloads];
                                                            } else {
                                                                [queue finishTransaction:transaction];
                                                            }
                                                        } failure:^(__unused NSError *error) {
                                                            
                                                        }];
                    }
                    break;
                case SKPaymentTransactionStateFailed:
                    if (transaction.error.code != SKErrorPaymentCancelled) {
                        
                    }
                    [queue finishTransaction:transaction];
                    break;
                default:
                    break;
            }
        }
    }];
    [[CargoBay sharedManager] setPaymentQueueUpdatedDownloadsBlock:^(SKPaymentQueue *queue, NSArray *downloads) {
        for (SKDownload* download in downloads) {
            if (download.downloadState == SKDownloadStateFinished) {
                // check if all other downloads associated to the same transaction have finished
                // before finishing the transaction
                for (SKDownload* transactionDownload in download.transaction.downloads) {
                    if (transactionDownload.downloadState != SKDownloadStateFinished) {
                        return;
                    }
                }
                NSLog(@"Path: %@", [download.contentURL absoluteString]);
                // last finished download finishes the transaction
                if ([queue.transactions containsObject:download.transaction]) {
                    [queue finishTransaction:download.transaction];
                }
            }
        }
    }];
    
    [[CargoBay sharedManager] productsWithIdentifiers:[NSSet setWithObject:@"com.plumzi.host.testnonconsumable1"]
                                              success:^(__unused NSArray *products, __unused NSArray *invalidIdentifiers) {
                                                  [[SKPaymentQueue defaultQueue] addPayment:[SKPayment paymentWithProduct:[products objectAtIndex:0]]];
                                              }
                                              failure:^(__unused NSError *error) {
                            
                                              }];
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.viewController = [[ViewController alloc] initWithNibName:@"ViewController" bundle:nil];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];
    
    return YES;
}

@end
