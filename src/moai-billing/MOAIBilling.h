// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	BILLING_H
#define	BILLING_H

#if __APPLE__

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <moai-core/headers.h>

//================================================================//
// MOAIBilling
//================================================================//
/**	@name	MOAIBilling
	@text	Wrapper for in-app purchase integration on iOS devices 
			using Apple StoreKit. Exposed to Lua via MOAIBilling on 
			all mobile platforms, but API differs on iOS and Android.
	
	@const	PAYMENT_QUEUE_TRANSACTION		Event invoked when a transaction changes state.
	@const	PAYMENT_QUEUE_ERROR				Event invoked when a transaction fails.
	@const	PRODUCT_REQUEST_RESPONSE		Event invoked when a product information request completes.
	@const	PRODUCT_RESTORE_FINISHED		Event invoked when a transactions restore is finished.
	@const	PRODUCT_VERIFY_FINISHED			Event invoked when a receipt verification is finished.
	
	@const	TRANSACTION_STATE_PURCHASING	Error code indicating a transaction in progress.
	@const	TRANSACTION_STATE_PURCHASED		Error code indicating a completed transaction.
	@const	TRANSACTION_STATE_FAILED		Error code indicating a failed transaction.
	@const	TRANSACTION_STATE_RESTORED		Error code indicating a restored transaction.
	@const	TRANSACTION_STATE_CANCELLED		Error code indicating a canceled transaction.
*/
class MOAIBilling :
	public MOAIGlobalClass < MOAIBilling, MOAIGlobalEventSource > {
private:

	//----------------------------------------------------------------//
	static int	_canMakePayments				( lua_State* L );
	static int	_getTransactionInfo				( lua_State* L );
	static int	_getTransactions				( lua_State* L );
	static int	_verifyTransaction				( lua_State* L );
	static int	_finishTransaction				( lua_State* L );
	static int	_restoreCompletedTransactions	( lua_State* L );
	static int	_requestPaymentForProduct		( lua_State* L );
	static int	_requestProductIdentifiers		( lua_State* L );
	static int	_startDownloads					( lua_State* L );
	static int	_pauseDownloads					( lua_State* L );
	static int	_resumeDownloads				( lua_State* L );
	static int	_cancelDownloads				( lua_State* L );
	static int	_setListener					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIBilling )
	
	enum {
		PAYMENT_QUEUE_TRANSACTION,
		PAYMENT_QUEUE_ERROR,
		PRODUCT_REQUEST_RESPONSE,
		PAYMENT_RESTORE_FINISHED,
		RECEIPT_VERIFY_FINISHED,
		PAYMENT_QUEUE_DOWNLOAD,
		TOTAL,
	};
	
	enum {
		TRANSACTION_STATE_PURCHASING,
		TRANSACTION_STATE_PURCHASED,
		TRANSACTION_STATE_FAILED,
		TRANSACTION_STATE_RESTORED,
		TRANSACTION_STATE_CANCELLED,
	};

	enum {
		DOWNLOAD_STATE_WAITING,
		DOWNLOAD_STATE_ACTIVE,
		DOWNLOAD_STATE_PAUSED,
		DOWNLOAD_STATE_FINISHED,
		DOWNLOAD_STATE_FAILED,
		DOWNLOAD_STATE_CANCELLED,
	};

	NSMutableDictionary*	mProducts;
	BOOL					mPaymentQueueObserved;

			MOAIBilling						();
			~MOAIBilling						();
	void	DidReceivePaymentQueueError			( SKPaymentQueue* queue, NSError *error, cc8 *extraInfo );
	void	DidReceiveRestoreFinished			( SKPaymentQueue* queue );
	void	AddTransactionObserver				();
	void	RemoveTransactionObserver			();
	SKPaymentTransaction* FindTransaction		( NSString *transaction_id );
	void    ReceiptVerificationDidFinish		( SKPaymentTransaction* transaction, NSError *error );
	void	DidUpdatePaymentQueueTransactions	( SKPaymentQueue* queue, NSArray* transactions );
	void	DidUpdatePaymentQueueDownloads		( SKPaymentQueue* queue, NSArray* downloads );
	void	DidReceiveProductsRequestResponse	( NSArray *products, NSArray *invalidIdentifiers, NSError *error = nil );
	void	PushPaymentTransaction				( lua_State* L, SKPaymentTransaction* transaction );
	void	PushDownload						( lua_State* L, SKDownload* download );
	void	RegisterLuaClass					( MOAILuaState& state );
};

#endif

#endif