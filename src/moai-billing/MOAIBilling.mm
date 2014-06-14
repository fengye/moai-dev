// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#import <StoreKit/StoreKit.h>
#import <moai-billing/MOAIBilling.h>
#import <moai-iphone/NSData+MOAILib.h>
#import <moai-iphone/NSDate+MOAILib.h>
#import <moai-iphone/NSDictionary+MOAILib.h>
#import <moai-iphone/NSError+MOAILib.h>
#import <moai-iphone/NSString+MOAILib.h>
#import "CargoBay.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canMakePayments
	@text	Determines whether or not the app has permission to 
			request payments for in-app purchases.
	
	@out	bool enabled
*/
int MOAIBilling::_canMakePayments ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	BOOL result = [ SKPaymentQueue canMakePayments ];
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	requestPaymentForProduct
	@text	Request payment for an in-app purchase.
	
	@in		string 	sku				The SKU to purchase.
	@opt	number 	quantity		Default value is 1.
	@out	boolean result			False if product or quantity is invalid. True if payment is queued.
*/
int MOAIBilling::_requestPaymentForProduct ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	if ( ![ SKPaymentQueue canMakePayments ] ) {
		
		lua_pushboolean( L, false );
		return 1;
	}
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	int quantity = state.GetValue < int >( 2, 1 );
	
	SKProduct* product = [ MOAIBilling::Get ().mProducts objectForKey:[ NSString stringWithUTF8String:identifier ]];
	if ( !product || !quantity ) {
		
		lua_pushboolean ( state, false );
		return 1;
	}
	
	SKMutablePayment* payment = [ SKMutablePayment paymentWithProduct:product ];
		payment.quantity = quantity;
		[[ SKPaymentQueue defaultQueue ] addPayment:payment ];
	
	lua_pushboolean ( state, true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	requestProductIdentifiers
	@text	Verify the validity of a set of products and retrieve
			relevant information thereabout.
	
	@in		table skus				A list of SKUs.
	@out	nil
*/
int MOAIBilling::_requestProductIdentifiers ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSMutableSet* productSet = [[[ NSMutableSet alloc ] init ] autorelease ];
	
	int top = state.GetTop ();
	for ( int i = 1; i <= top; ++i ) {
	
		if ( state.IsType ( i, LUA_TSTRING )) {
			
			cc8* identifier = state.GetValue < cc8* >( i, "" );
			[ productSet addObject :[ NSString stringWithUTF8String:identifier ]];
		}
	
		if ( state.IsType ( i, LUA_TTABLE )) {
			
			int itr = state.PushTableItr ( i );
			while ( state.TableItrNext ( itr )) {
				
				cc8* identifier = state.GetValue < cc8* >( -1, "" );
				[ productSet addObject :[ NSString stringWithUTF8String:identifier ]];
			}
		}
	}
	
    [[ CargoBay sharedManager ] productsWithIdentifiers:productSet
												success:^(	NSArray *products, NSArray *invalidIdentifiers	) {
													
													dispatch_async(dispatch_get_main_queue(), ^{
	
														MOAIBilling::Get ().DidReceiveProductsRequestResponse ( products, invalidIdentifiers );
													});
												}
												failure:^(	__unused NSError *error	) {
													
													dispatch_async(dispatch_get_main_queue(), ^{
														
														MOAIBilling::Get ().DidReceiveProductsRequestResponse ( nil, nil, error );
													});
												}];
	return 0;
}

//----------------------------------------------------------------//
/** @name	restoreCompletedTransactions
	@text	Request the restoration of any previously purchased items.

	@out 	nil
*/
int MOAIBilling::_restoreCompletedTransactions( lua_State* L ) {
	
	UNUSED ( L );
	
	[[ SKPaymentQueue defaultQueue ] restoreCompletedTransactions ];

	return 0;
}

//----------------------------------------------------------------//
/**	@name	verifyTransaction
	@text	Verify a transaction is valid.
 
	@in		string id				Transaction id.
	@opt	string password			Optional password to communicate with Apple servers.
	@out	bool   result			False if transaction was not found or in an invalid state.
 */
int MOAIBilling::_verifyTransaction( lua_State* L ) {
	
	MOAILuaState state ( L );
	
    cc8* identifier = state.GetValue < cc8* >( 1, "" );
    cc8* password = state.GetValue < cc8* >( 2, "" );
	
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction( [ NSString stringWithUTF8String:identifier ] );
	if ( transaction &&
		( transaction.transactionState == SKPaymentTransactionStatePurchased ||
		  transaction.transactionState == SKPaymentTransactionStateRestored )) {
				
		[[ CargoBay sharedManager ] verifyTransaction:transaction
											 password:[ NSString stringWithUTF8String:password ]
											  success:^( __unused NSDictionary *receipt ) {
												  
												  dispatch_async(dispatch_get_main_queue(), ^{
													  
													  MOAIBilling::Get ().ReceiptVerificationDidFinish( transaction, nil );
												  });
											} failure:^( __unused NSError *error ) {
												
												dispatch_async(dispatch_get_main_queue(), ^{
													
													MOAIBilling::Get ().ReceiptVerificationDidFinish( transaction, error );
												});
											}];
			
		lua_pushboolean( L, true );
		return 1;
	}
		
	lua_pushboolean( L, false );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	finishTransaction
	@text	Mark a transaction as finished after being purchased
			or restored.
 
	@in		string id				Transaction id.
	@out	bool result				False if transaction was not found or in an invalid state.
 */
int MOAIBilling::_finishTransaction( lua_State* L ) {
	
	MOAILuaState state ( L );
	
    cc8* identifier = state.GetValue < cc8* >( 1, "" );
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction( [ NSString stringWithUTF8String:identifier ] );
	
	if ( transaction &&
		( transaction.transactionState == SKPaymentTransactionStatePurchased ||
		  transaction.transactionState == SKPaymentTransactionStateRestored )) {
				
		[[ SKPaymentQueue defaultQueue] finishTransaction:transaction];
		
		lua_pushboolean( L, true );
		return 1;
	}
	
	lua_pushboolean( L, false );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	startDownloads
	@text	Start downloads of content associated to a transaction

	@in		string id				Transaction id.
	@out	bool   result			False if transaction was not found or in an invalid state.
*/
int MOAIBilling::_startDownloads( lua_State* L ) {
			
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction( [ NSString stringWithUTF8String:identifier ] );
	
	// Verify transaction is valid and has downloads
	if ( transaction == nil || [transaction.downloads count] == 0 ) {

		lua_pushboolean( L, false );
		return 1;
	}
	
	// Queue downloads
	[[ SKPaymentQueue defaultQueue] startDownloads:transaction.downloads ];
	
	lua_pushboolean( L, true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	resumeDownloads
	@text	Resume downloads of content associated to a transaction
 
	@in		string id				Transaction id.
	@out	bool   result			False if transaction was not found or in an invalid state.
 */
int MOAIBilling::_resumeDownloads( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction( [ NSString stringWithUTF8String:identifier ] );
	
	// Verify transaction is valid and has downloads
	if ( transaction == nil || [transaction.downloads count] == 0 ) {
		
		lua_pushboolean( L, false );
		return 1;
	}
	
	// Queue downloads
	[[ SKPaymentQueue defaultQueue] resumeDownloads:transaction.downloads ];
	
	lua_pushboolean( L, true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	pauseDownloads
	@text	Pause downloads of content associated to a transaction
 
	@in		string id				Transaction id.
	@out	bool   result			False if transaction was not found or in an invalid state.
 */
int MOAIBilling::_pauseDownloads( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction( [ NSString stringWithUTF8String:identifier ] );
	
	// Verify transaction is valid and has downloads
	if ( transaction == nil || [transaction.downloads count] == 0 ) {
		
		lua_pushboolean( L, false );
		return 1;
	}
	
	// Queue downloads
	[[ SKPaymentQueue defaultQueue] pauseDownloads:transaction.downloads ];
	
	lua_pushboolean( L, true );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	cancelDownloads
	@text	Cancel downloads of content associated to a transaction
 
	@in		string id				Transaction id.
	@out	bool   result			False if transaction was not found or in an invalid state.
 */
int MOAIBilling::_cancelDownloads( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction( [ NSString stringWithUTF8String:identifier ] );
	
	// Verify transaction is valid and has downloads
	if ( transaction == nil || [transaction.downloads count] == 0 ) {
		
		lua_pushboolean( L, false );
		return 1;
	}
	
	// Queue downloads
	[[ SKPaymentQueue defaultQueue] cancelDownloads:transaction.downloads ];
	
	lua_pushboolean( L, true );
	return 1;
}

//----------------------------------------------------------------//
int MOAIBilling::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	MOAIGlobalEventSource::_setListener < MOAIBilling > ( L );
	
	// dynamically start observing payment queue as soon as one listener is set
	// and stop observing payment queue when all listeners have been removed
	if ( MOAIBilling::Get ().PushListenerTable( state )) {
		
		int itr = state.PushTableItr ( -1 );
		if ( state.TableItrNext( itr )) {
			
			MOAIBilling::Get ().AddTransactionObserver();
		} else {
			
			MOAIBilling::Get ().RemoveTransactionObserver();
		}
		state.Pop( 1 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getTransactionInfo
	@text	Request transaction information
 
	@in		string 	tid				The transaction id.
	@out	table	resuld			Transaction info if found.
 */
int MOAIBilling::_getTransactionInfo( lua_State* L ) {
	
	MOAILuaState state ( L );
	
    cc8* identifier = state.GetValue < cc8* >( 1, "" );
	SKPaymentTransaction *transaction =
		MOAIBilling::Get ().FindTransaction([NSString stringWithUTF8String:identifier]);
	if (transaction) {
		
		MOAIBilling::Get().PushPaymentTransaction( L, transaction );
		return 1;
	}
	
	lua_pushnil( L );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTransactions
	@text	Request all transactions information
 
	@out	table	resuld			Table of Transaction info tables.
 */
int MOAIBilling::_getTransactions( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	lua_newtable ( state );
	int count = 1;
	for (SKPaymentTransaction *transaction in [ SKPaymentQueue defaultQueue ].transactions) {
		
		lua_pushnumber ( state, count++ );
		MOAIBilling::Get().PushPaymentTransaction ( state, transaction );
		lua_settable ( state, -3 );
	}
	
	return 1;
}

//================================================================//
// MOAIBilling
//================================================================//

//----------------------------------------------------------------//
MOAIBilling::MOAIBilling () :
	mPaymentQueueObserved(FALSE) {

	RTTI_SINGLE ( MOAIGlobalEventSource )
	
	this->mProducts = [[ NSMutableDictionary dictionaryWithCapacity:5 ] retain ];

	// allow transactions with same ID for now, this is really for consumable only
	[[ CargoBay sharedManager ] setTransactionIDUniquenessVerificationWithBlock:^BOOL( __unused NSString *transactionID ) {
		
		return YES;
	}];
		
	// Transaction updates observer
	[[ CargoBay sharedManager ] setPaymentQueueUpdatedTransactionsBlock:^( SKPaymentQueue *queue, NSArray *transactions ) {
		
		dispatch_async(dispatch_get_main_queue(), ^{
			
			MOAIBilling::Get ().DidUpdatePaymentQueueTransactions ( queue, transactions );
		});
	}];
		
	// Restore process
	[[ CargoBay sharedManager ] setPaymentQueueRestoreCompletedTransactionsWithSuccess:^( SKPaymentQueue *queue ) {
		
		dispatch_async(dispatch_get_main_queue(), ^{
			
			MOAIBilling::Get ().DidReceiveRestoreFinished ( queue );
		});
	} failure:^( SKPaymentQueue *queue, NSError *error ) {
		
		dispatch_async(dispatch_get_main_queue(), ^{
			
			MOAIBilling::Get ().DidReceivePaymentQueueError( queue, error, "restoreCompletedTransactionsFailedWithError");
		});
	}];
		
	// Download progress
	[[ CargoBay sharedManager ] setPaymentQueueUpdatedDownloadsBlock:^(SKPaymentQueue *queue, NSArray *downloads) {
		
		dispatch_async(dispatch_get_main_queue(), ^{
			
			MOAIBilling::Get ().DidUpdatePaymentQueueDownloads ( queue, downloads );
		});
	}];
}

//----------------------------------------------------------------//
MOAIBilling::~MOAIBilling () {

	if ( mPaymentQueueObserved ) {
		
		[[ SKPaymentQueue defaultQueue ] removeTransactionObserver:[ CargoBay sharedManager ]];
	}
	
	[ this->mProducts release ];
}

//----------------------------------------------------------------//
void MOAIBilling::AddTransactionObserver () {
	
	if ( !mPaymentQueueObserved ) {
		
		[[ SKPaymentQueue defaultQueue ] addTransactionObserver:[ CargoBay sharedManager ] ];
		mPaymentQueueObserved = TRUE;
	}
}


//----------------------------------------------------------------//
void MOAIBilling::RemoveTransactionObserver () {
	
	if ( mPaymentQueueObserved ) {
		
		[[ SKPaymentQueue defaultQueue ] removeTransactionObserver:[ CargoBay sharedManager ] ];
		mPaymentQueueObserved = FALSE;
	}
}

//----------------------------------------------------------------//
SKPaymentTransaction* MOAIBilling::FindTransaction ( NSString *transaction_id ) {
	
	for (SKPaymentTransaction *transaction in [ SKPaymentQueue defaultQueue ].transactions) {
		
		if ([transaction.transactionIdentifier isEqualToString:transaction_id]) {
			return transaction;
		}
	}
	
	return nil;
}

//----------------------------------------------------------------//
void MOAIBilling::ReceiptVerificationDidFinish ( SKPaymentTransaction* transaction, NSError* error ) {
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( RECEIPT_VERIFY_FINISHED, state )) {

		MOAIBilling::Get().PushPaymentTransaction( state, transaction );
		
		if ( error ) {
			
			[ error toLua:state ];
			state.DebugCall ( 2, 0 );
			return;
		}
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::DidReceivePaymentQueueError ( SKPaymentQueue *queue, NSError* error, cc8* extraInfo ) {
	
	UNUSED ( queue );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PAYMENT_QUEUE_ERROR, state )) {
	
		[ error toLua:state ];
		lua_pushstring(state, extraInfo);
		
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::DidReceiveRestoreFinished ( SKPaymentQueue* queue ) {
	
	UNUSED ( queue );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PAYMENT_RESTORE_FINISHED, state )) {
	
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::DidUpdatePaymentQueueTransactions ( SKPaymentQueue* queue, NSArray* transactions ) {
	
	UNUSED ( queue );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	for ( SKPaymentTransaction* transaction in transactions ) {
	
		if ( this->PushListener ( PAYMENT_QUEUE_TRANSACTION, state )) {
		
			this->PushPaymentTransaction ( state, transaction );
			state.DebugCall ( 1, 0 );
		}
		
		// automatically finish failed transactions or transactions without downloads
		// otherwise user must call finishTransaction manually once downloads are completed.
		if ( transaction.transactionState == SKPaymentTransactionStateFailed ||
			( transaction.transactionState != SKPaymentTransactionStatePurchasing && !transaction.downloads )) {
			
			[[ SKPaymentQueue defaultQueue ] finishTransaction:transaction ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIBilling::DidUpdatePaymentQueueDownloads ( SKPaymentQueue* queue, NSArray* downloads ) {
	
	UNUSED ( queue );
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		
	if ( this->PushListener ( PAYMENT_QUEUE_DOWNLOAD, state )) {
		
		lua_newtable ( state );
		int count = 1;
		for ( SKDownload* download in downloads ) {
			
			lua_pushnumber ( state, count++ );
			PushDownload ( state, download );
			lua_settable ( state, -3 );
		}

		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::DidReceiveProductsRequestResponse ( NSArray *products, NSArray *invalidIdentifiers, NSError *error ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	if ( this->PushListener ( PRODUCT_REQUEST_RESPONSE, state )) {
		
		//  products table
		lua_newtable ( state );
		
		if ( products.count ) {
			NSNumberFormatter *formatter = [[[NSNumberFormatter alloc] init] autorelease];
			[formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
			[formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
			
			int count = 1;
			for ( SKProduct* product in products ) {
			
				// Keep product info around
				[ mProducts setObject:product forKey:product.productIdentifier ];

				lua_pushnumber ( state, count++ );
				lua_newtable ( state );
			
				[formatter setLocale:product.priceLocale];
				NSString * formattedString = [ formatter stringFromNumber:product.price];
				
				state.SetField ( -1, "localizedTitle", [ product.localizedTitle UTF8String ]);
				state.SetField ( -1, "localizedDescription", [ product.localizedDescription UTF8String ]);
				state.SetField ( -1, "price", [ product.price floatValue ]);
				state.SetField ( -1, "localizedPrice", [ formattedString UTF8String]);
				state.SetField ( -1, "priceLocale", [ product.priceLocale.localeIdentifier UTF8String ]);
				state.SetField ( -1, "productIdentifier", [ product.productIdentifier UTF8String ]);
				
				state.SetField ( -1, "downloadable", product.downloadable);
				
	#if !defined(MACOSX)
				state.SetField ( -1, "downloadContentVersion", [ product.downloadContentVersion UTF8String ]);
				NSArray* contentLengths = product.downloadContentLengths;
	#else
				state.SetField ( -1, "downloadContentVersion", [ product.contentVersion UTF8String ]);
				NSArray* contentLengths = product.contentLengths;
	#endif
				if ( contentLengths ) {
					NSNumber * sum = [ contentLengths valueForKeyPath:@"@sum.self" ];
					state.SetField ( -1, "downloadContentLength", (double) [ sum longLongValue ]);
				}
				
				lua_settable ( state, -3 );
			}
		}
		
		// invalid identifiers table
		lua_newtable ( state );
		
		if ( invalidIdentifiers.count ) {
			
			int count = 1;
			// Note: If you're testing in-app purchases, chances are your product Id
			// will take time to propagate into Apple's sandbox/test environment and
			// thus the id's will be invalid for hours(?) (at the time of writing this).
			for ( NSString *invalidProductId in invalidIdentifiers ) {
				
				NSLog(@"StoreKit: Invalid product id: %@" , invalidProductId);
				lua_pushnumber ( state, count++ );
				lua_pushstring( state, [invalidProductId UTF8String] );
				lua_settable ( state, -3 );
			}
		}
		
		// error (if any)
		if ( error ) {
			
			[ error toLua:state ];
			state.DebugCall ( 3, 0 );
		}
		else {
			
			state.DebugCall ( 2, 0 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIBilling::PushPaymentTransaction ( lua_State* L, SKPaymentTransaction* transaction ) {

	lua_newtable ( L );
	
	lua_pushstring ( L, "transactionState" );
	
	switch ( transaction.transactionState ) {
		
		case SKPaymentTransactionStatePurchasing: {
			
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASING );
			break;
		}
		case SKPaymentTransactionStatePurchased: {
			
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASED );
			break;
		}
		case SKPaymentTransactionStateFailed: {
			
			NSInteger code = transaction.error.code;
			if ( code == SKErrorPaymentCancelled ) {
				
				lua_pushnumber ( L, TRANSACTION_STATE_CANCELLED );
			}
			else {
				
				lua_pushnumber ( L, TRANSACTION_STATE_FAILED );
			}
			break;
		}
		case SKPaymentTransactionStateRestored: {
			
			lua_pushnumber ( L, TRANSACTION_STATE_RESTORED );
			break;
		}
		default: {
			
			lua_pushnil ( L );
			break;
		}
	}
	
	lua_settable ( L, -3 );
	
	if ( transaction.payment ) {
		
		lua_pushstring ( L, "payment" );
		lua_newtable ( L );
		
		lua_pushstring ( L, "productIdentifier" );
		[ transaction.payment.productIdentifier toLua:L ];
		lua_settable ( L, -3 );
		
		lua_pushstring ( L, "quantity" );
		lua_pushnumber ( L, transaction.payment.quantity );
		lua_settable ( L, -3 );
		
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStateFailed && transaction.error) {
		
		lua_pushstring ( L, "error" );
		[ transaction.error toLua:L ];
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStateRestored && transaction.originalTransaction) {
		
		lua_pushstring ( L, "originalTransaction" );
		this->PushPaymentTransaction ( L, transaction.originalTransaction );
		lua_settable ( L, -3 );
	}
	
	if (( transaction.transactionState == SKPaymentTransactionStatePurchased ) || ( transaction.transactionState == SKPaymentTransactionStateRestored )) {
		
#if !defined(MACOSX)
		lua_pushstring ( L, "transactionReceipt" );
		if( transaction.transactionReceipt != nil ) {
			
			[ transaction.transactionReceipt toLua:L ];
		}
		else {
			
			lua_pushnil ( L );
		}
		lua_settable ( L, -3 );
#endif
				
		lua_pushstring ( L, "transactionDate" );
		[ transaction.transactionDate toLua:L ];
		lua_settable ( L, -3 );
		
		lua_pushstring ( L, "transactionIdentifier" );
		[ transaction.transactionIdentifier toLua:L ];
		lua_settable ( L, -3 );
		
		if ( transaction.downloads ) {
			
			lua_pushstring ( L, "downloads" );
			
			// downloads
			lua_newtable ( L );
			int count = 1;
			for ( SKDownload* download in transaction.downloads ) {
				
				lua_pushnumber ( L, count++ );
				PushDownload ( L, download );
				lua_settable ( L, -3 );
			}
			
			lua_settable ( L, -3 );
		}
	}
}

//----------------------------------------------------------------//
void MOAIBilling::PushDownload ( lua_State* L, SKDownload* download ) {
	
	lua_newtable ( L );
	
	lua_pushstring ( L, "downloadState" );
	
#if !defined(MACOSX)
	SKDownloadState state = download.downloadState;
#else
	SKDownloadState state = download.state;
#endif
	
	switch ( state ) {
			
		case SKDownloadStateWaiting: {
			
			lua_pushnumber ( L, DOWNLOAD_STATE_WAITING );
			break;
		}
		case SKDownloadStateActive: {
			
			lua_pushnumber ( L, DOWNLOAD_STATE_ACTIVE );
			break;
		}
		case SKDownloadStatePaused: {
			
			lua_pushnumber ( L, DOWNLOAD_STATE_PAUSED );
			break;
		}
		case SKDownloadStateFinished: {
			
			lua_pushnumber ( L, DOWNLOAD_STATE_FINISHED );
			break;
		}
		case SKDownloadStateFailed: {
			
			lua_pushnumber ( L, DOWNLOAD_STATE_FAILED );
			break;
		}
		case SKDownloadStateCancelled: {
			
			lua_pushnumber ( L, DOWNLOAD_STATE_CANCELLED );
			break;
		}
		default: {
			
			lua_pushnil ( L );
			break;
		}
	}
	lua_settable ( L, -3 );
	
	lua_pushstring ( L, "contentLength" );
#if !defined(MACOSX)
	lua_pushnumber ( L, download.contentLength );
#else
	lua_pushnumber ( L, [download.contentLength longLongValue]);
#endif
	lua_settable ( L, -3 );
	
#if !defined(MACOSX)
	lua_pushstring ( L, "transactionIdentifier" );
	[ download.transaction.transactionIdentifier toLua:L ];
	lua_settable ( L, -3 );
#endif
	
	lua_pushstring ( L, "version" );
	[ download.contentVersion toLua:L ];
	lua_settable ( L, -3 );
	
	lua_pushstring ( L, "contentIdentifier" );
	[ download.contentIdentifier toLua:L ];
	lua_settable ( L, -3 );
	
	if ( [download.contentURL path ] ) {

		lua_pushstring ( L, "path" );
		[ [download.contentURL path ] toLua:L ];
		lua_settable ( L, -3 );
	}
	
	if ( download.error ) {
		
		lua_pushstring ( L, "error" );
		[ download.error toLua:L ];
		lua_settable ( L, -3 );
	}
	
	lua_pushstring ( L, "progress" );
	lua_pushnumber( L, download.progress );
	lua_settable ( L, -3 );
	
	lua_pushstring ( L, "timeRemaining" );
	lua_pushnumber( L, download.timeRemaining );
	lua_settable ( L, -3 );
}

//----------------------------------------------------------------//
void MOAIBilling::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIGlobalEventSource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "PAYMENT_QUEUE_TRANSACTION",	( u32 )PAYMENT_QUEUE_TRANSACTION );
	state.SetField ( -1, "PRODUCT_REQUEST_RESPONSE",	( u32 )PRODUCT_REQUEST_RESPONSE );
	state.SetField ( -1, "PAYMENT_QUEUE_ERROR",			( u32 )PAYMENT_QUEUE_ERROR );
	state.SetField ( -1, "PAYMENT_RESTORE_FINISHED",	( u32 )PAYMENT_RESTORE_FINISHED );
	state.SetField ( -1, "RECEIPT_VERIFY_FINISHED",		( u32 )RECEIPT_VERIFY_FINISHED );
	state.SetField ( -1, "PAYMENT_QUEUE_DOWNLOAD",		( u32 )PAYMENT_QUEUE_DOWNLOAD );

	state.SetField ( -1, "TRANSACTION_STATE_PURCHASING",( u32 )TRANSACTION_STATE_PURCHASING );
	state.SetField ( -1, "TRANSACTION_STATE_PURCHASED", ( u32 )TRANSACTION_STATE_PURCHASED );
	state.SetField ( -1, "TRANSACTION_STATE_FAILED",    ( u32 )TRANSACTION_STATE_FAILED );
	state.SetField ( -1, "TRANSACTION_STATE_RESTORED",  ( u32 )TRANSACTION_STATE_RESTORED );
	state.SetField ( -1, "TRANSACTION_STATE_CANCELLED", ( u32 )TRANSACTION_STATE_CANCELLED );
	
	state.SetField ( -1, "DOWNLOAD_STATE_WAITING",		( u32 )DOWNLOAD_STATE_WAITING );
	state.SetField ( -1, "DOWNLOAD_STATE_ACTIVE",		( u32 )DOWNLOAD_STATE_ACTIVE );
	state.SetField ( -1, "DOWNLOAD_STATE_PAUSED",		( u32 )DOWNLOAD_STATE_PAUSED );
	state.SetField ( -1, "DOWNLOAD_STATE_FINISHED",		( u32 )DOWNLOAD_STATE_FINISHED );
	state.SetField ( -1, "DOWNLOAD_STATE_FAILED",		( u32 )DOWNLOAD_STATE_FAILED );
	state.SetField ( -1, "DOWNLOAD_STATE_CANCELLED",	( u32 )DOWNLOAD_STATE_CANCELLED );
	
	luaL_Reg regTable [] = {
		{ "canMakePayments",				_canMakePayments },
		{ "getTransactionInfo",				_getTransactionInfo },
		{ "getTransactions",				_getTransactions },
		{ "verifyTransaction",				_verifyTransaction },
		{ "finishTransaction",				_finishTransaction },
		{ "restoreCompletedTransactions",	_restoreCompletedTransactions },
		{ "requestPaymentForProduct",		_requestPaymentForProduct },
		{ "requestProductIdentifiers",		_requestProductIdentifiers },
		{ "startDownloads",					_startDownloads },
		{ "pauseDownloads",					_pauseDownloads },
		{ "resumeDownloads",				_resumeDownloads },
		{ "cancelDownloads",				_cancelDownloads },
		{ "setListener",					_setListener },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
