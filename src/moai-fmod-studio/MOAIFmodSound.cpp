// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai-fmod-studio/MOAIFmodStudio.h>
#include <moai-fmod-studio/MOAIFmodSound.h>

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
/**	@name	getLength
	@text	Return the duration of the sound.
 
	@in		MOAIUntzSound self
	@out	number length
 */
int MOAIFmodSound::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	if ( self->mSound ) {
		
		unsigned int length;
		FMOD_RESULT result = self->mSound->getLength ( &length, FMOD_TIMEUNIT_MS );
		if ( result != FMOD_OK ) return 0;
		
		lua_pushnumber ( state, length / 1000.f );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getPosition
	@text	Return the position of the cursor in the sound.
 
	@in		MOAIFmodSound self
	@out	number position
 */
int MOAIFmodSound::_getPosition ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	if ( self->mChannel ) {
		
		unsigned int position;
		FMOD_RESULT result = self->mChannel->getPosition ( &position, FMOD_TIMEUNIT_MS );
		if ( result != FMOD_OK ) return 0;
		
		lua_pushnumber ( state, position / 1000.f );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getVolume
	@text	Returns the current volume of the channel.
 
	@in		MOAIFmodSound self
	@out	float Volume - the volume currently set in this channel.
 */
int MOAIFmodSound::_getVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	lua_pushnumber ( state, self->GetVolume() );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isLooping
	@text	Return the looping status if the sound.
 
	@in		MOAIFmodSound self
	@out	boolean looping
 */
int MOAIFmodSound::_isLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	lua_pushboolean ( state, self->mLooping );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPaused
	@text	Return the pause status of the sound.
 
	@in		MOAIFmodSound self
	@out	boolean paused
 */
int MOAIFmodSound::_isPaused ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	lua_pushboolean ( state, self->mPaused );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isPlaying
	@text	Returns true if sound is playing.
	
	@in		MOAIFmodSound self
	@out	boolean.
 */
int MOAIFmodSound::_isPlaying ( lua_State* L ) {
	
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	bool isPlaying = false;
	
	if ( !self->mPaused && self->mChannel ) {
		
		FMOD_RESULT result = self->mChannel->isPlaying ( &isPlaying );
		if ( result != FMOD_OK ) return 0;
		
		// isPlaying is not always accurate, rely on buffered position to know
		// when playback is finished.
		unsigned int position, length;
		self->mChannel->getPosition ( &position, FMOD_TIMEUNIT_MS );
		self->mSound->getLength ( &length, FMOD_TIMEUNIT_MS );
		if ( position >= length ) {
			isPlaying = false;
		}
	}
	
	lua_pushboolean ( state, isPlaying );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	load
	@text	Loads the specified sound from file, or from a MOAIDataBuffer.

	@overload
	
		@in		MOAIFmodSound self
		@in		string filename			The path to the sound to load from file.
		@in		boolean loadIntoMemory	Whether the sound should be streamed from the data source, rather than preloaded.
		@in		boolean	async			Whether the sound file should be loaded asynchronously.
		@out	nil

	@overload

		@in		MOAIFmodSound self
		@in		MOAIDataBuffer data		The MOAIDataBuffer that is storing sound data.  You must either provide a string or MOAIDataBuffer, but not both.
		@in		boolean loadIntoMemory	Whether the sound should be streamed from the data source, rather than preloaded.
		@out	nil
*/
int MOAIFmodSound::_load ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )

	bool loadIntoMemory	= state.GetValue < bool >( 3, true );
	bool async = state.GetValue < bool >( 4, false );

	if ( state.IsType( 2, LUA_TSTRING )) {

		cc8* filename	= state.GetValue < cc8* >( 2, "" );
		self->mFileName = filename;
		self->Load ( filename, loadIntoMemory, async );
	}
	else {
		
		MOAIDataBuffer* data = state.GetLuaObject < MOAIDataBuffer >( 2, true );
		if ( data ) {
			self->Load ( *data, loadIntoMemory );
		}
	}
	return 0;
}


//----------------------------------------------------------------//
/**	@name	moveVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.
 
	@in		MOAIFmodSound self
	@in		number target			The target volume.
	@in		number delay			The delay until the action starts.
	@in		number mode				The interpolation mode for the action.
	@out	MOAIAction action		The new action.  It is automatically started by this function.
 */
int MOAIFmodSound::_moveVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "UNN" )
	
	float delta		= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	
	if ( length > 0.0f ) {
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );
		
		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
		
		action->SetLink ( 0, self, MOAIFmodSoundAttr::Pack ( ATTR_VOLUME ), delta, mode );
		
		action->SetSpan ( length );
		action->Start ();
		action->PushLuaUserdata ( state );
		
		return 1;
	}
	
	self->SetVolume ( self->GetVolume() + delta );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	pause
	@text	Pause the channel.
 
	@in		MOAIFmodSound self
	@out	nil
 */
int MOAIFmodSound::_pause ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	self->SetPaused ( true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	play
	@text	Plays the specified sound, looping it if desired.
 
	@in		MOAIFmodSound		self
	@in		int numloops		Number of loops if sound is set to loop
	@out	nil
 */
int MOAIFmodSound::_play ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	int loopCount = state.GetValue < int >( 3, 0 );
	
	self->Play ( loopCount );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	seekVolume
	@text	Creates a new MOAIAction that will move the volume from it's current value to the value specified.
 
	@in		MOAIFmodSound self
	@in		number target			The target volume.
	@in		number length			Length of animation in seconds.
	@opt	number mode				The ease mode. One of MOAIEaseType.EASE_IN, MOAIEaseType.EASE_OUT, MOAIEaseType.FLAT MOAIEaseType.LINEAR,
	MOAIEaseType.SMOOTH, MOAIEaseType.SOFT_EASE_IN, MOAIEaseType.SOFT_EASE_OUT, MOAIEaseType.SOFT_SMOOTH. Defaults to MOAIEaseType.SMOOTH.
 
	@out	MOAIEaseDriver easeDriver
 */
int MOAIFmodSound::_seekVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "UNN" )
	
	float target	= state.GetValue < float >( 2, 0.0f );
	float length	= state.GetValue < float >( 3, 0.0f );
	
	if ( length > 0.0f ) {
		
		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		action->ReserveLinks ( 1 );
		
		action->SetLink ( 0, self, MOAIFmodSoundAttr::Pack ( ATTR_VOLUME ), target - self->mVolume, mode );
		
		action->SetSpan ( length );
		action->Start ();
		action->PushLuaUserdata ( state );
		
		return 1;
	}
	
	self->SetVolume ( target );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setVolume
	@text	Immediately sets the volume of this sound.
 
	@in		MOAIFmodSound	self
	@in		number volume	The volume of this sound.
	@out	nil
 */
int MOAIFmodSound::_setVolume ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "UN" )
	
	float volume = state.GetValue < float >( 2, 0.0f );
	self->SetVolume ( volume );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLooping
	@text	Immediately sets looping for this channel.
 
	@in		MOAIFmodSound		self
	@in		boolean looping		True if sound should loop.
	@out	nil
 */
int MOAIFmodSound::_setLooping ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "UB" )
	
	float looping = state.GetValue < bool >( 2, false );
	self->SetLooping (looping);
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	stop
	@text	Stops playing the sound on this channel.
 
	@in		MOAIFmodSound self
	@out	nil
 */
int MOAIFmodSound::_stop ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )
	
	self->Stop ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	release
	@text	Releases the sound data from memory.

	@in		MOAIFmodSound self
	@out	nil
*/
int MOAIFmodSound::_release ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIFmodSound, "U" )

	self->Release ();

	return 0;
}

//================================================================//
// MOAIFmodSound
//================================================================//
//----------------------------------------------------------------//

bool MOAIFmodSound::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {
	
	if ( MOAIFmodSoundAttr::Check ( attrID )) {
		attrID = UNPACK_ATTR ( attrID );
		
		if ( attrID == ATTR_VOLUME ) {
			
			this->SetVolume ( attrOp.Apply ( this->mVolume, op, MOAIAttrOp::ATTR_READ_WRITE ));
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
static FMOD_RESULT FMODOpenCallback(const char *name, int unicode, unsigned int *filesize, void **handle, void *userdata)
{
	ZLFILE* file = zl_fopen ( name, "rb" );
	if ( !file ) {
		return FMOD_ERR_FILE_NOTFOUND;
	}
	
	zl_fseek ( file , 0, SEEK_END );
	*filesize = zl_ftell ( file );
	zl_rewind ( file );
	*handle = ( void * )file;
	return FMOD_OK;
}

//----------------------------------------------------------------//
static FMOD_RESULT FMODCloseCallback(void *handle, void *userdata)
{
	ZLFILE* file = ( ZLFILE *) handle;
	if ( file ) {
		zl_fclose( file );
		return FMOD_OK;
	}
	return FMOD_ERR_INTERNAL;
}

//----------------------------------------------------------------//
static FMOD_RESULT FMODReadCallback(void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata)
{
	
	ZLFILE* file = ( ZLFILE *) handle;
	if ( file ) {
		*bytesread = zl_fread ( buffer, 1, sizebytes, file );
		return FMOD_OK;
	}
	return FMOD_ERR_INTERNAL;
}

//----------------------------------------------------------------//
static FMOD_RESULT FMODSeekCallback(void *handle, unsigned int pos, void *userdata)
{
	ZLFILE* file = ( ZLFILE *) handle;
	if ( file ) {
		int result = zl_fseek( file, pos, SEEK_SET );
		return result != -1 ? FMOD_OK : FMOD_ERR_FILE_EOF;
	}
	return FMOD_ERR_INTERNAL;
}

//----------------------------------------------------------------//
MOAIFmodSound::MOAIFmodSound () :
	mSound ( 0 ),
	mChannel ( 0 ),
	mVolume ( 1.0f ),
	mPaused ( false ) ,
	mLooping (false ),
	mFileName ( ) {

	RTTI_SINGLE ( MOAINode )
}

//----------------------------------------------------------------//
MOAIFmodSound::~MOAIFmodSound () {

	this->Release ();
}

//----------------------------------------------------------------//
void MOAIFmodSound::Load ( MOAIDataBuffer& data, bool loadIntoMemory ) {

	if ( this->mSound ) return;
	
	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid()) return;
	
	FMOD_RESULT result;
	FMOD::System* system;
	result = soundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
	
	void* bytes;
	size_t size;
	data.Lock ( &bytes, &size );

	FMOD_MODE mode = FMOD_OPENMEMORY;
	mode |= !loadIntoMemory ? FMOD_CREATESTREAM : FMOD_DEFAULT;

	FMOD_CREATESOUNDEXINFO info;
	memset( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
	info.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );
	info.length = size;

	FMOD::Sound* sound = 0;

	result = system->createSound (( cc8* )bytes, mode, &info, &sound );
	
	data.Unlock ();
	
	if ( result != FMOD_OK ) return;
	
	this->mSound = sound;
}

//----------------------------------------------------------------//
static bool has_extension ( char const* filename, char const* extension ) {
	
	char* pos = strrchr( filename, '.' );
	return pos && strcasecmp( pos + 1, extension ) == 0;
}

//----------------------------------------------------------------//
void MOAIFmodSound::Load ( cc8* filename, bool loadIntoMemory, bool async ) {

	if ( this->mSound ) return;
	
	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid()) return;
	
	FMOD_RESULT result;
	FMOD::System* system;
	result = soundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
	
	FMOD_MODE mode = FMOD_DEFAULT;
	mode |= !loadIntoMemory ? FMOD_CREATESTREAM : FMOD_CREATESAMPLE;
//	mode |= async ? FMOD_NONBLOCKING : 0;
//	mode |= FMOD_LOWMEM;
	
	FMOD::Sound* sound = 0;
	FMOD_CREATESOUNDEXINFO info;
	memset ( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ));
	
	// custom callbacks to be able to read sounds from mounted zip files
	info.fileuseropen = FMODOpenCallback;
	info.fileuserclose = FMODCloseCallback;
	info.fileuserread = FMODReadCallback;
	info.fileuserseek = FMODSeekCallback;
	info.cbsize = sizeof ( FMOD_CREATESOUNDEXINFO );
	
	if ( has_extension ( filename, "ogg" )) {
		info.suggestedsoundtype = FMOD_SOUND_TYPE_OGGVORBIS;
	} else if ( has_extension ( filename, "mp3" )) {
		mode = mode | FMOD_ACCURATETIME; // fix for bug introduced in 1.03.03 preventing loops for vbr files
		info.suggestedsoundtype = FMOD_SOUND_TYPE_MPEG;
	}
	
	#ifdef MOAI_OS_IPHONE
//		info.audioqueuepolicy = FMOD_AUDIOQUEUE_CODECPOLICY_SOFTWAREONLY;
	#endif

#ifdef MOAI_OS_NACL

	mode = FMOD_OPENMEMORY | FMOD_CREATESAMPLE;

	memset( &info, 0, sizeof( FMOD_CREATESOUNDEXINFO ) );
	info.cbsize = sizeof( FMOD_CREATESOUNDEXINFO );

	NaClFile *file = g_FileSystem->fopen ( filename, "r" );

	if ( file ) {
		
		info.length = file->mSize;
		result = system->createSound (( cc8* )file->mData, FMOD_SOFTWARE | mode, &info, &sound );
		g_FileSystem->fclose ( file );
	}
	else {
		
		info.length = 0;
		result = system->createSound (( cc8* )NULL, FMOD_SOFTWARE | mode, &info, &sound );
	}
#else
	result = system->createSound ( filename, mode, &info, &sound );
#endif
	
	if ( result != FMOD_OK ) return;

	this->mSound = sound;
}

//----------------------------------------------------------------//
void MOAIFmodSound::Play ( int loopCount ) {
	
	this->Stop ();
	
	if ( !this->mSound ) return;
	
	FMOD::Studio::System* soundSys = MOAIFmodStudio::Get ().GetSoundSys ();
	if ( !soundSys->isValid()) return;
	
	FMOD::System* system;
	FMOD_RESULT result;
	result = soundSys->getLowLevelSystem ( &system );
	if ( result != FMOD_OK ) return;
	
	if ( loopCount > 0 ) {
		
		this->mSound->setMode ( FMOD_LOOP_NORMAL );
		this->mSound->setLoopCount ( loopCount );
	} else {
		
		this->mSound->setMode ( mLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
		this->mSound->setLoopCount ( -1 );
	}
	
	FMOD::Channel* channel = NULL;
	
	//printf ( "PLAY SOUND %s, @ %f\n", sound->GetFileName (), USDeviceTime::GetTimeInSeconds () );
	result = system->playSound ( this->mSound, 0, true, &channel );
	if ( result != FMOD_OK ) {
		
		printf (" FMOD ERROR: Sound did not play\n" );
		return;
	}
	
	this->mChannel = channel;
	this->SetVolume ( this->mVolume );
	this->SetPaused ( false );
}

//----------------------------------------------------------------//
void MOAIFmodSound::SetPaused ( bool paused ) {
	
	this->mPaused = paused;
	
	if ( !this->mChannel ) return;
	
	this->mChannel->setPaused ( this->mPaused );
}

//----------------------------------------------------------------//
void MOAIFmodSound::SetLooping( bool looping ) {
	
	this->mLooping = looping;
	if ( !this->mChannel ) return;
	
	// set up infinite looping or looping off
	if ( this->mLooping ) {
		
		this->mChannel->setMode ( FMOD_LOOP_NORMAL );
		this->mChannel->setLoopCount ( -1 );
	}
	else {
		
		this->mChannel->setMode ( FMOD_LOOP_OFF );
	}
	
	// For streaming sounds, flush according to docs
//	unsigned int position;
//	this->mChannel->getPosition (&position, FMOD_TIMEUNIT_MS );
//	this->mChannel->setPosition (position, FMOD_TIMEUNIT_MS );
}

//----------------------------------------------------------------//
void MOAIFmodSound::SetVolume ( float volume ) {
	
	this->mVolume = volume;
	
	if ( !this->mChannel ) return;
	
	this->mChannel->setVolume ( this->mVolume );
}

//----------------------------------------------------------------//
float MOAIFmodSound::GetVolume () {
	
	return this->mVolume;
}

//----------------------------------------------------------------//
void MOAIFmodSound::Stop () {
	
	if ( !this->mChannel ) return;
	this->mChannel->stop ();
	
	// once a channel is stopped, it is not valid anymore
	this->mChannel = NULL;
}

//----------------------------------------------------------------//
void MOAIFmodSound::Release () {

	this->Stop();
	
	if ( !this->mSound ) return;
	
	if ( MOAIFmodStudio::mSoundSys ) {
		
		this->mSound->release ();
	}
	this->mSound = 0;
}

//----------------------------------------------------------------//
void MOAIFmodSound::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAINode::RegisterLuaClass ( state );
	
	state.SetField ( -1, "ATTR_VOLUME", MOAIFmodSoundAttr::Pack ( ATTR_VOLUME ));
}

//----------------------------------------------------------------//
void MOAIFmodSound::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getLength",		_getLength },
		{ "getPosition",	_getPosition },
		{ "getVolume",		_getVolume },
		{ "isLooping",		_isLooping },
		{ "isPaused",		_isPaused },
		{ "isPlaying",		_isPlaying },
		{ "load",			_load },
		{ "moveVolume",		_moveVolume },
		{ "pause",			_pause },
		{ "play",			_play },
		{ "seekVolume",		_seekVolume },
		{ "setLooping",		_setLooping },
		{ "setVolume",		_setVolume },
		{ "stop",			_stop },
		{ "release",		_release },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

