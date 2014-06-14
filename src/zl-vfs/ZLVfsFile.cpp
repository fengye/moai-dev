// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai_config.h>
#ifdef NACL
	#include "NaClFile.h"
#endif
#include <zl-vfs/zl_util.h>
#include <zl-vfs/ZLVfsFile.h>
#include <zl-vfs/ZLVfsFileSystem.h>
#include <zl-vfs/ZLVfsVirtualPath.h>
#include <zl-vfs/ZLVfsZipStream.h>
#include <zl-vfs/Obfuscate.h> // TODO: factor this stuff OUT

#ifdef MOAI_COMPILER_MSVC
	#include <Share.h>
#endif

#include <sys/stat.h>

using namespace std;

//================================================================//
// ZLVfsFile
//================================================================//

//----------------------------------------------------------------//
void ZLVfsFile::ClearError () {

	if ( !this->mIsZip ) {
		clearerr ( this->mPtr.mFile );
	}
}

//----------------------------------------------------------------//
int	ZLVfsFile::Close () {

	int result = 0;

	if ( this->mIsZip ) {
		if ( this->mPtr.mZip ) {
			delete this->mPtr.mZip;
			this->mPtr.mZip = 0;
		}
	}
	else if ( this->mPtr.mFile ) {
		result = fclose ( this->mPtr.mFile );
		this->mPtr.mFile = 0;
	}
	return result;
}

//----------------------------------------------------------------//
int ZLVfsFile::CloseProcess () {
	
	FILE* stdFile = 0;
	if ( !this->mIsZip ) {
		stdFile = this->mPtr.mFile;
		this->mPtr.mFile = 0;
	}
	
	#ifdef MOAI_OS_WINDOWS
		return _pclose ( stdFile );
	#else
		return pclose ( stdFile );
	#endif
}

//----------------------------------------------------------------//
// TODO: where is Encrypt?
void ZLVfsFile::Decrypt ( void* buffer, long offset, size_t count ) {

	char* charBuffer = (char *) buffer;
	for ( int i = 0 ; i < count ; i++ ) {
		charBuffer[ i ] = DECRYPT ( offset + i, charBuffer[ i ]);
	}
}

//----------------------------------------------------------------//
int	ZLVfsFile::Flush () {
	
	if ( !this->mIsZip ) {
		return fflush ( this->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZLVfsFile::GetChar () {
	
	int result = EOF;
		
	if ( this->mIsZip ) {
		char c;
		result = ( int )this->mPtr.mZip->Read ( &c, 1 );
		
		if ( result == 1 ) {
			result = c;
		}
	}
	else {
		result = fgetc ( this->mPtr.mFile );
	}

	if ( this->mIsCrypt && result != EOF ) {
		char c = ( char )result;
		long offset = this->Tell() - 1;
		this->Decrypt ( &c, offset, 1 );
		result = ( int )c;
	}

	return result;
}

//----------------------------------------------------------------//
int ZLVfsFile::GetError () {

	return ( this->mIsZip ) ? 0 : ferror ( this->mPtr.mFile ); // TODO: error flag for zip file
}

//----------------------------------------------------------------//
int	ZLVfsFile::GetFileNum () {
	
	// TODO:
	if ( !this->mIsZip && !this->mIsCrypt ) {
		return fileno ( this->mPtr.mFile );
	}
	return -1;
}

//----------------------------------------------------------------//
int ZLVfsFile::GetPos ( fpos_t* position ) {
	(( void )position );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
char* ZLVfsFile::GetString ( char* string, int length ) {
	
	int i = 0;
	int c = 0;
	
	if ( this->mIsZip || this->mIsCrypt ) {
	
		if ( length <= 1 ) return 0;

		do {
			c = this->GetChar ();
			if ( c == ( int )EOF || c == ( int )NULL ) break;
			
			string [ i++ ] = ( char )c;
			if ( i >= length ) return 0;
			
		} while ( c && ( c != '\n' ));

		if ( i == 0 ) {
			return 0;
		}
		string [ i ] = 0;
		return string;
	}

	return fgets ( string, length, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZLVfsFile::IsCrypt () {

	return ( this->mIsCrypt ) ? 1 : 0;
}

//----------------------------------------------------------------//
int ZLVfsFile::IsEOF () {

	return ( this->mIsZip ) ? this->mPtr.mZip->IsAtEnd () : feof ( this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZLVfsFile::Open ( const char* filename, const char* mode ) {
	
	ZLVfsVirtualPath* mount;
	
	// Apply the path remapping
	string remappedFilename;
	if ( mode [ 0 ] == 'r' ) {
		
		if ( ZLVfsFileSystem::Get ().CheckFileRemapping ( filename, remappedFilename )) {
			filename = remappedFilename.c_str ();
		}
	}

	string abspath = ZLVfsFileSystem::Get ().GetAbsoluteFilePath ( filename );
	filename = abspath.c_str ();
	int result = -1;
		
	mount = ZLVfsFileSystem::Get ().FindBestVirtualPath ( filename );

	if ( mount ) {
		
		if ( mode [ 0 ] == 'r' ) {
		
			ZLVfsZipStream* zipStream = 0;
			
			filename = mount->GetLocalPath ( filename );

			if ( filename ) {
				zipStream = ZLVfsZipStream::Open ( mount->mArchive, filename );
			}
			
			if ( zipStream ) {
				this->mIsZip = 1;
				this->mPtr.mZip = zipStream;
				result = 0;
			}
		}
	}
	else {

		#ifdef MOAI_COMPILER_MSVC
			FILE* stdFile = 0;
			stdFile = _fsopen ( filename, mode, _SH_DENYNO );
		#else
			FILE* stdFile = fopen ( filename, mode );
		#endif
		
		if ( stdFile ) {
			this->mPtr.mFile = stdFile;
			result = 0;
		}
	}

	if ( result == 0 ) {
		char buffer [ CRYPT_HEADER_SIZE ];
		int count = this->Read ( buffer, 1, CRYPT_HEADER_SIZE );
		if ( count == CRYPT_HEADER_SIZE && memcmp( buffer, CRYPT_HEADER, CRYPT_HEADER_SIZE ) == 0 ) {
			this->mIsCrypt = true;
		} else {
			this->Seek( 0, SEEK_SET );
		}
	}

	return result;
}

//----------------------------------------------------------------//
int ZLVfsFile::OpenProcess ( const char *command, const char *mode ) {

	FILE* stdFile = 0;

	#ifdef MOAI_OS_WINDOWS
		stdFile = _popen ( command, mode );	
	#else
		stdFile = popen ( command, mode );
	#endif

	if ( stdFile ) {
		this->mPtr.mFile = stdFile;
		return 0;
	}
	return -1;
}

//----------------------------------------------------------------//
int ZLVfsFile::OpenTemp () {

	this->mPtr.mFile = tmpfile ();
	return this->mPtr.mFile ? 0 : -1;
}

//----------------------------------------------------------------//
int ZLVfsFile::PutChar ( int c ) {

	if ( !this->mIsZip && !this->mIsCrypt ) {
		return fputc ( c, this->mPtr.mFile );
	}
	return EOF;
}

//----------------------------------------------------------------//
int ZLVfsFile::PutString ( const char* string ) {

	if ( !this->mIsZip && !this->mIsCrypt ) {
		return fputs ( string, this->mPtr.mFile );
	}
	return EOF;
}

//----------------------------------------------------------------//
size_t ZLVfsFile::Read ( void* buffer, size_t size, size_t count ) {
	
	size_t result;
	if ( this->mIsZip ) {
		result = ( size_t )this->mPtr.mZip->Read ( buffer, size * count );
		result = ( size_t )( result / size );
	} else {
		result = fread ( buffer, size, count, this->mPtr.mFile );
	}

	if ( this->mIsCrypt && result > 0 ) {
		int byteCount = result * size;
		long offset = this->Tell () - byteCount;
		this->Decrypt ( buffer, offset, byteCount );
	}

	return result;
}

//----------------------------------------------------------------//
int ZLVfsFile::Reopen ( const char* filename, const char* mode ) {

	int result = -1;

	if ( this->mIsZip ) {
		this->Close ();
		result = this->Open ( filename, mode );
	}
	else {

		// Apply the path remapping
		string remappedFilename;
		if ( mode [ 0 ] == 'r' ) {

			if ( ZLVfsFileSystem::Get ().CheckFileRemapping ( filename, remappedFilename )) {
				filename = remappedFilename.c_str ();
			}
		}

		FILE* stdFile = freopen ( filename, mode, this->mPtr.mFile );
		
		if ( stdFile ) {
			this->mPtr.mFile = stdFile;
			result = 0;
		}
	}

	if ( result == 0 && this->mIsCrypt ) {
		this->Seek( 0, SEEK_SET );
	}

	return result;
}

//----------------------------------------------------------------//
int	ZLVfsFile::Seek ( long offset, int origin ) {
	
	if ( this->mIsCrypt && origin == SEEK_SET ) {
		offset += CRYPT_HEADER_SIZE;
	}
	return ( this->mIsZip ) ? this->mPtr.mZip->Seek ( offset, origin ) : fseek ( this->mPtr.mFile, offset, origin );
}

//----------------------------------------------------------------//
void ZLVfsFile::SetFile ( FILE* file ) {

	this->Flush ();
	this->mPtr.mFile = file;
	this->mIsZip = false;

	// TODO: make all this crypt stuff go away
	this->mIsCrypt = false;

	if ( file != 0 ) {
		int fd = fileno ( file );
		struct stat s;
		if ( fstat ( fd, &s ) == -1 ) {
			return;
		}
		if ( s.st_size < CRYPT_HEADER_SIZE ) {
			return;
		}
		char buffer [ CRYPT_HEADER_SIZE ];
		int count = fread ( buffer, 1, CRYPT_HEADER_SIZE, file );
		if ( count == CRYPT_HEADER_SIZE && memcmp( buffer, CRYPT_HEADER, CRYPT_HEADER_SIZE ) == 0 ) {
		 	this->mIsCrypt = true;
		} else {
			rewind ( file );
		}
	}
}

//----------------------------------------------------------------//
int ZLVfsFile::SetPos ( const fpos_t * pos ) {
	(( void )pos );

	assert ( 0 ); // not implemented
	return -1;
}

//----------------------------------------------------------------//
long ZLVfsFile::Tell () {

	long result = ( this->mIsZip ) ? ( long )this->mPtr.mZip->Tell () : ftell ( this->mPtr.mFile );
	if ( this->mIsCrypt ) {
		result -= CRYPT_HEADER_SIZE;
	}
	return result;
	//return ( this->mIsZip ) ? ( long )this->mPtr.mZip->Tell () : ftell ( this->mPtr.mFile );
}

//----------------------------------------------------------------//
size_t ZLVfsFile::Write ( const void* data, size_t size, size_t count ) {
	
	if ( !this->mIsZip && !this->mIsCrypt ) {
		return fwrite ( data, size, count, this->mPtr.mFile );
	}
	return 0;
}

//----------------------------------------------------------------//
void ZLVfsFile::SetBuf ( char* buffer ) {

	if ( !this->mIsZip ) {
		setbuf ( this->mPtr.mFile, buffer );
	}
}

//----------------------------------------------------------------//
int ZLVfsFile::SetVBuf ( char* buffer, int mode, size_t size ) {

	if ( !this->mIsZip ) {
		setvbuf ( this->mPtr.mFile, buffer, mode, size );
	}
	return 0;
}

//----------------------------------------------------------------//
int ZLVfsFile::UnGetChar ( int character ) {
	
	if ( this->mIsCrypt ) {
		char c = ( char )character;
		long offset = this->Tell() - 1;
		this->Decrypt ( &c, offset, 1 );
		character = ( int )c;
	}
	if ( this->mIsZip ) {
		return this->mPtr.mZip->UnGetChar (( char )character ) ? EOF : 0;
	}
	return ungetc ( character, this->mPtr.mFile );
}

//----------------------------------------------------------------//
int ZLVfsFile::VarPrintf ( const char* format, va_list arg ) {

	if ( !this->mIsZip ) {
		return vfprintf ( this->mPtr.mFile, format, arg );
	}
	return -1;
}

//----------------------------------------------------------------//
ZLVfsFile::ZLVfsFile () :
	mIsZip ( false ), 
	mIsCrypt (false)  {
	
	this->mPtr.mFile = 0;
	this->mPtr.mZip = 0;
}

//----------------------------------------------------------------//
ZLVfsFile::~ZLVfsFile () {

	this->Close ();
}
