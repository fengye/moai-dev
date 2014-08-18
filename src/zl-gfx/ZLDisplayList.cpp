// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLDisplayList.h>

//================================================================//
// ZLDisplayList
//================================================================//

//----------------------------------------------------------------//
void ZLDisplayList::ActiveTexture ( u32 textureUnit ) {

	assert ( this->mStream );
	
	this->mStream->Write < u32 >( ACTIVE_TEXTURE );
	this->mStream->Write < u32 >( textureUnit );
}

//----------------------------------------------------------------//
void ZLDisplayList::BindBuffer ( u32 target, u32 buffer ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_BUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( buffer );
}

//----------------------------------------------------------------//
void ZLDisplayList::BindFramebuffer ( u32 target, u32 frameBuffer ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_FRAMEBUFFER );
	this->mStream->Write < u32 >( target );
	this->mStream->Write < u32 >( frameBuffer );
}

//----------------------------------------------------------------//
void ZLDisplayList::BindRenderbuffer ( u32 renderbuffer ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_RENDERBUFFER );
	this->mStream->Write < u32 >( renderbuffer );
}

//----------------------------------------------------------------//
void ZLDisplayList::BindTexture ( u32 texID ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_TEXTURE );
	this->mStream->Write < u32 >( texID );
}

//----------------------------------------------------------------//
void ZLDisplayList::BindVertexArray ( u32 vertexArrayID ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BIND_VERTEX_ARRAY );
	this->mStream->Write < u32 >( vertexArrayID );
}

//----------------------------------------------------------------//
void ZLDisplayList::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BLEND_FUNC );
	this->mStream->Write < u32 >( sourceFactor );
	this->mStream->Write < u32 >( destFactor );
}

//----------------------------------------------------------------//
void ZLDisplayList::BlendMode ( u32 mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( BLEND_MODE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
const void* ZLDisplayList::CachePointer ( u32 usage, u32 size, u32 type, u32 stride, const void* buffer ) {

	return 0;
}

//----------------------------------------------------------------//
void ZLDisplayList::Clear ( u32 mask ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CLEAR );
	this->mStream->Write < u32 >( mask );
}

//----------------------------------------------------------------//
void ZLDisplayList::ClearColor ( float r, float g, float b, float a ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CLEAR_COLOR );
	this->mStream->Write < float >( r );
	this->mStream->Write < float >( g );
	this->mStream->Write < float >( b );
	this->mStream->Write < float >( a );
}

//----------------------------------------------------------------//
void ZLDisplayList::Color ( float r, float g, float b, float a ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( COLOR );
	this->mStream->Write < float >( r );
	this->mStream->Write < float >( g );
	this->mStream->Write < float >( b );
	this->mStream->Write < float >( a );
}

//----------------------------------------------------------------//
void ZLDisplayList::ColorPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {

	assert ( this->mStream );

	pointer = this->CachePointer ( USAGE_COLOR_POINTER, size, type, stride, pointer );

	this->mStream->Write < u32 >( COLOR_POINTER );
	this->mStream->Write < u32 >( size );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < size_t >(( size_t )pointer ); // TODO: fix to use buffer object
}

//----------------------------------------------------------------//
void ZLDisplayList::CullFace ( u32 mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( CULL_FACE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
void ZLDisplayList::DepthFunc ( u32 depthFunc ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DEPTH_FUNC );
	this->mStream->Write < u32 >( depthFunc );
}

//----------------------------------------------------------------//
void ZLDisplayList::DepthMask ( bool flag ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DEPTH_MASK );
	this->mStream->Write < u32 >( flag );
}

//----------------------------------------------------------------//
void ZLDisplayList::Disable ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DISABLE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLDisplayList::DisableClientState ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DISABLE_CLIENT_STATE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLDisplayList::DisableVertexAttribArray ( u32 index ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DISABLE_VERTEX_ATTRIB_ARRAY );
	this->mStream->Write < u32 >( index );
}

//----------------------------------------------------------------//
void ZLDisplayList::Draw ( ZLDrawAPI& draw ) {

	assert ( this->mStream );

	size_t top = this->mStream->GetCursor ();
	if ( top == 0 ) return;
	
	this->mStream->Seek ( 0, SEEK_SET );
	
	while ( this->mStream->GetCursor () < top ) {
	
		u32 command = this->mStream->Read < u32 >( UNKNOWN );
	
		if ( command == UNKNOWN ) break;
	
		switch ( command ) {
	
			case ACTIVE_TEXTURE: {
				draw.ActiveTexture (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BIND_BUFFER: {
				draw.BindBuffer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BIND_FRAMEBUFFER: {
				draw.BindFramebuffer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BIND_RENDERBUFFER: {
				draw.BindRenderbuffer (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BIND_TEXTURE: {
				draw.BindTexture (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BIND_VERTEX_ARRAY: {
				draw.BindVertexArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BLEND_FUNC: {
				draw.BlendFunc (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case BLEND_MODE: {
				draw.BlendMode (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case CLEAR: {
				draw.Clear (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case CLEAR_COLOR: {
				draw.ClearColor (
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case COLOR: {
				draw.Color (
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case COLOR_POINTER: {
				draw.ColorPointer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					( void* )this->mStream->Read < size_t >( 0 ), // TODO: fix to use buffer object
					false
				);
				break;
			}
			case CULL_FACE: {
				draw.CullFace (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DEPTH_FUNC: {
				draw.DepthFunc (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DEPTH_MASK: {
				draw.DepthMask (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DISABLE: {
				draw.Disable (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DISABLE_CLIENT_STATE: {
				draw.DisableClientState (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DISABLE_VERTEX_ATTRIB_ARRAY: {
				draw.DisableVertexAttribArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case DRAW_ARRAYS: {
				draw.DrawArrays (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ENABLE: {
				draw.Enable (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ENABLE_CLIENT_STATE: {
				draw.EnableClientState (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case ENABLE_VERTEX_ATTRIB_ARRAY: {
				draw.EnableVertexAttribArray (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case FLUSH: {
				draw.Flush ();
				break;
			}
			case LINE_WIDTH: {
				draw.LineWidth (
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case LOAD_IDENTITY: {
				draw.LoadIdentity ();
				break;
			}
			case LOAD_MATRIX: {
	
				ZLMatrix4x4 mtx;
				this->mStream->ReadBytes ( &mtx, sizeof ( ZLMatrix4x4 ));
			
				draw.LoadMatrix ( mtx );
			
				break;
			}
			case MATRIX_MODE: {
				draw.MatrixMode (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case MULT_MATRIX: {

				ZLMatrix4x4 mtx;
				this->mStream->ReadBytes ( &mtx, sizeof ( ZLMatrix4x4 ));
			
				draw.LoadMatrix ( mtx );
			
				break;
			}
			case NORMAL_POINTER: {
				draw.NormalPointer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					( void* )this->mStream->Read < size_t >( 0 ), // TODO: fix to use buffer object
					false
				);
				break;
			}
			case POINT_SIZE: {
				draw.PointSize (
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case SCISSOR: {
				draw.Scissor (
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case TEX_COORD_POINTER: {
				draw.TexCoordPointer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					( void* )this->mStream->Read < size_t >( 0 ), // TODO: fix to use buffer object
					false
				);
				break;
			}
			case TEX_ENVI: {
				draw.TexEnvi (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < s32 >( 0 )
				);
				break;
			}
			case TEX_PARAMETERI: {
				draw.TexParameteri (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < s32 >( 0 )
				);
				break;
			}
			case UNIFORM_1F: {
				draw.Uniform1f (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < float >( 0 )
				);
				break;
			}
			case UNIFORM_1I: {
				draw.Uniform1i (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < s32 >( 0 )
				);
				break;
			}
			case UNIFORM_4FV: {
			
				u32 location = this->mStream->Read < u32 >( location );
				u32 count = this->mStream->Read < u32 >( count );
				
				float vec [ 4 ];
				this->mStream->ReadBytes ( vec, sizeof ( float ) * 4 );
			
				draw.Uniform4fv ( location, count, vec );
				break;
			}
			case UNIFORM_MATRIX_3FV: {
				
				u32 location = this->mStream->Read < u32 >( 0 );
				u32 count = this->mStream->Read < u32 >( 0 );
				bool transpose = this->mStream->Read < bool >( false );
				
				ZLMatrix3x3 mtx;
				this->mStream->ReadBytes ( &mtx, sizeof ( ZLMatrix3x3 ));
				
				draw.UniformMatrix3fv ( location, count, transpose, mtx );
				
				break;
			}
			case UNIFORM_MATRIX_4FV: {
				
				u32 location = this->mStream->Read < u32 >( 0 );
				u32 count = this->mStream->Read < u32 >( 0 );
				bool transpose = this->mStream->Read < bool >( false );
				
				ZLMatrix4x4 mtx;
				this->mStream->ReadBytes ( &mtx, sizeof ( ZLMatrix4x4 ));
				
				draw.UniformMatrix4fv ( location, count, transpose, mtx );
			
				break;
			}
			case USE_PROGRAM: {
				draw.UseProgram (
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
			case VERTEX_ATTRIB_POINTER: {
				draw.VertexAttribPointer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < bool >( false ),
					this->mStream->Read < u32 >( 0 ),
					( void* )this->mStream->Read < size_t >( 0 ), // TODO: fix to use buffer object
					false
				);
				break;
			}
			case VERTEX_POINTER: {
				draw.VertexPointer (
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					( void* )this->mStream->Read < size_t >( 0 ), // TODO: fix to use buffer object
					false
				);
				break;
			}
			case VIEWPORT: {
				draw.Viewport (
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < s32 >( 0 ),
					this->mStream->Read < u32 >( 0 ),
					this->mStream->Read < u32 >( 0 )
				);
				break;
			}
		}
	}
	
	this->mStream->Seek ( top, SEEK_SET );
}

//----------------------------------------------------------------//
void ZLDisplayList::DrawArrays ( u32 primType, u32 first, u32 count ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( DRAW_ARRAYS );
	this->mStream->Write < u32 >( primType );
	this->mStream->Write < u32 >( first );
	this->mStream->Write < u32 >( count );
}

//----------------------------------------------------------------//
void ZLDisplayList::Enable ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ENABLE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLDisplayList::EnableClientState ( u32 cap ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ENABLE_CLIENT_STATE );
	this->mStream->Write < u32 >( cap );
}

//----------------------------------------------------------------//
void ZLDisplayList::EnableVertexAttribArray ( u32 index ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( ENABLE_VERTEX_ATTRIB_ARRAY );
	this->mStream->Write < u32 >( index );
}

//----------------------------------------------------------------//
void ZLDisplayList::Flush () {

	assert ( this->mStream );

	this->mStream->Write < u32 >( FLUSH );
}

//----------------------------------------------------------------//
void ZLDisplayList::LineWidth ( float width ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( LINE_WIDTH );
	this->mStream->Write < float >( width );
}

//----------------------------------------------------------------//
void ZLDisplayList::LoadIdentity () {

	assert ( this->mStream );

	this->mStream->Write < u32 >( LOAD_IDENTITY );
}

//----------------------------------------------------------------//
void ZLDisplayList::LoadMatrix ( const ZLMatrix4x4& mtx ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( LOAD_MATRIX );
	this->mStream->Write < ZLMatrix4x4 >( mtx );
}

//----------------------------------------------------------------//
void ZLDisplayList::MatrixMode ( u32 mode ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( MATRIX_MODE );
	this->mStream->Write < u32 >( mode );
}

//----------------------------------------------------------------//
void ZLDisplayList::MultMatrix ( const ZLMatrix4x4& mtx ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( MULT_MATRIX );
	this->mStream->Write < ZLMatrix4x4 >( mtx );
}

//----------------------------------------------------------------//
void ZLDisplayList::NormalPointer ( u32 type, u32 stride, const void* pointer, bool cache  ) {

	assert ( this->mStream );

	pointer = this->CachePointer ( USAGE_NORMAL_POINTER, 4, type, stride, pointer );

	this->mStream->Write < u32 >( NORMAL_POINTER );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < size_t >(( size_t )pointer ); // TODO: fix to use buffer object
}

//----------------------------------------------------------------//
void ZLDisplayList::PointSize ( float size ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( POINT_SIZE );
	this->mStream->Write < float >( size );
}

//----------------------------------------------------------------//
void ZLDisplayList::Reset () {

	assert ( this->mStream );

	this->mStream->Seek ( 0, SEEK_SET );
}

//----------------------------------------------------------------//
void ZLDisplayList::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( SCISSOR );
	this->mStream->Write < s32 >( x );
	this->mStream->Write < s32 >( y );
	this->mStream->Write < u32 >( w );
	this->mStream->Write < u32 >( h );
}

//----------------------------------------------------------------//
void ZLDisplayList::TexCoordPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {

	assert ( this->mStream );

	pointer = this->CachePointer ( USAGE_TEX_COORD_POINTER, size, type, stride, pointer );

	this->mStream->Write < u32 >( TEX_COORD_POINTER );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < size_t >(( size_t )pointer ); // TODO: fix to use buffer object
}

//----------------------------------------------------------------//
void ZLDisplayList::TexEnvi ( u32 pname, s32 param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( TEX_ENVI );
	this->mStream->Write < u32 >( pname );
	this->mStream->Write < s32 >( param );
}

//----------------------------------------------------------------//
void ZLDisplayList::TexParameteri ( u32 pname, s32 param ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( TEX_PARAMETERI );
	this->mStream->Write < u32 >( pname );
	this->mStream->Write < s32 >( param );
}

//----------------------------------------------------------------//
void ZLDisplayList::Uniform1f ( u32 location, float v0 ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_1F );
	this->mStream->Write < float >( v0 );
}

//----------------------------------------------------------------//
void ZLDisplayList::Uniform1i ( u32 location, s32 v0 ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_1I );
	this->mStream->Write < s32 >( v0 );
}

//----------------------------------------------------------------//
void ZLDisplayList::Uniform4fv ( u32 location, u32 count, const float* value ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_4FV );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( count );
	this->mStream->WriteBytes ( value, sizeof ( float ) * 4 );
}

//----------------------------------------------------------------//
void ZLDisplayList::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const ZLMatrix3x3& mtx ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_4FV );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( count );
	this->mStream->Write < bool >( transpose );
	this->mStream->Write < ZLMatrix3x3 >( mtx );
}

//----------------------------------------------------------------//
void ZLDisplayList::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const ZLMatrix4x4& mtx ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_4FV );
	this->mStream->Write < u32 >( location );
	this->mStream->Write < u32 >( count );
	this->mStream->Write < bool >( transpose );
	this->mStream->Write < ZLMatrix4x4 >( mtx );
}

//----------------------------------------------------------------//
void ZLDisplayList::UseProgram ( u32 program ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( UNIFORM_4FV );
	this->mStream->Write < u32 >( program );
}

//----------------------------------------------------------------//
void ZLDisplayList::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer, bool cache  ) {

	assert ( this->mStream );

	pointer = this->CachePointer ( USAGE_VERTEX_ATTRIB_POINTER, size, type, stride, pointer );

	this->mStream->Write < u32 >( VERTEX_ATTRIB_POINTER );
	this->mStream->Write < u32 >( index );
	this->mStream->Write < u32 >( size );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < bool >( normalized );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < size_t >(( size_t )pointer ); // TODO: fix to use buffer object
}

//----------------------------------------------------------------//
void ZLDisplayList::VertexPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {

	assert ( this->mStream );

	pointer = this->CachePointer ( USAGE_VERTEX_POINTER, size, type, stride, pointer );

	this->mStream->Write < u32 >( VERTEX_POINTER );
	this->mStream->Write < u32 >( type );
	this->mStream->Write < u32 >( stride );
	this->mStream->Write < size_t >(( size_t )pointer ); // TODO: fix to use buffer object
}

//----------------------------------------------------------------//
void ZLDisplayList::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	assert ( this->mStream );

	this->mStream->Write < u32 >( VIEWPORT );
	this->mStream->Write < s32 >( x );
	this->mStream->Write < s32 >( y );
	this->mStream->Write < u32 >( w );
	this->mStream->Write < u32 >( h );
}

//----------------------------------------------------------------//
ZLDisplayList::ZLDisplayList () :
	mStream ( 0 ) {
}

//----------------------------------------------------------------//
ZLDisplayList::~ZLDisplayList () {
}
