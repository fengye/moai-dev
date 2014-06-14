// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodStudioBank.h>
#include <fmod_studio.hpp>

#ifdef MOAI_OS_IPHONE
	#include <fmod_ios.h>
#endif

#ifdef MOAI_OS_NACL
#include <NaClFileSystem.h>
#include <moai_nacl.h>
#endif

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads the specified bank from file, or from a MOAIDataBuffer.

	@overload
	
		@in		MOAIFmodStudioBank self
		@in		string filename			The path to the bank to load from file.
		@out	bool

	@overload

		@in		MOAIFmodStudioBank self
		@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing bank data.  You must either provide a string or MOAIDataBuffer, but not both.
		@out	nil
*/
int MOAIFmodStudioBank::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioBank, "U" )

	bool result = false;
	
	if ( state.IsType( 2, LUA_TSTRING )) {

		cc8* filename = state.GetValue < cc8* >( 2, "" );
		self->mFileName = filename;
		result = self->Load ( filename );
	}
	else {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			result = self->Load( *data );
		}
	}

	lua_pushboolean( state, result );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases the bank data from memory.

	@in		MOAIFmodStudioBank self
	@out	nil
*/
int MOAIFmodStudioBank::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodStudioBank, "U" )

	self->Release ();

	return 0;
}

//================================================================//
// MOAIFmodStudioBank
//================================================================//

//----------------------------------------------------------------//
MOAIFmodStudioBank::MOAIFmodStudioBank () :
	mBank ( 0 ),
	mFileName ( ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIFmodStudioBank::~MOAIFmodStudioBank () {

	this->Release ();
}

//--------------------------------------------`--------------------//
bool MOAIFmodStudioBank::Load ( MOAIDataBuffer& data ) {

	if ( this->mBank && this->mBank->isValid() ) return false;
	
	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid() ) return false;
	
	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );
	
	FMOD_RESULT result = soundSys->loadBankMemory ( ( cc8* )bytes, size, FMOD_STUDIO_LOAD_MEMORY, FMOD_STUDIO_LOAD_BANK_NORMAL, &this->mBank );
	
	data.Unlock ();
	
	return result == FMOD_OK;
}

//----------------------------------------------------------------//
bool MOAIFmodStudioBank::Load ( cc8* filename ) {

	if ( this->mBank && this->mBank->isValid() ) return false;
	
	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid() ) return false;
	
	FMOD_RESULT result;

#ifdef MOAI_OS_NACL

	NaClFile *file = g_FileSystem->fopen ( filename, "r" );

	if ( file ) {
		
		result = FMOD_Studio_System_LoadBankFile ( soundSys, ( cc8* )file->mData, &bank );

		g_FileSystem->fclose ( file );
	}
	else {
		result = soundSys.loadBankFile ( ( cc8* )NULL, FMOD_SOFTWARE | mode, &info, &this->mBank );
	}
#else
	result = soundSys->loadBankFile ( filename, FMOD_STUDIO_LOAD_BANK_NORMAL, &this->mBank );
#endif
	
	return result == FMOD_OK;
}

//----------------------------------------------------------------//
void MOAIFmodStudioBank::Release () {

	if ( !this->mBank || !this->mBank->isValid() ) return;
	
	FMOD_STUDIO_LOADING_STATE state;
	this->mBank->getLoadingState(&state);
	if (state == FMOD_STUDIO_LOADING_STATE_LOADING || state == FMOD_STUDIO_LOADING_STATE_LOADED) {
		
		this->mBank->unload();
	}
	this->mBank = NULL;
}

//----------------------------------------------------------------//
void MOAIFmodStudioBank::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIFmodStudioBank::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "load",			_load },
		{ "release",		_release },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

