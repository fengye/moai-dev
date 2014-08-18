// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/zl_gfx.h>
#include <zl-gfx/ZLDraw.h>

//================================================================//
// ZLDraw
//================================================================//

//----------------------------------------------------------------//
void ZLDraw::ActiveTexture ( u32 textureUnit ) {

	zglActiveTexture ( textureUnit );
}

//----------------------------------------------------------------//
void ZLDraw::BindBuffer ( u32 target, u32 buffer ) {

	zglBindBuffer ( target, buffer );
}

//----------------------------------------------------------------//
void ZLDraw::BindFramebuffer ( u32 target, u32 frameBuffer ) {

	zglBindFramebuffer ( target, frameBuffer );
}

//----------------------------------------------------------------//
void ZLDraw::BindRenderbuffer ( u32 renderbuffer ) {

	zglBindRenderbuffer ( renderbuffer );
}

//----------------------------------------------------------------//
void ZLDraw::BindTexture ( u32 texID ) {

	zglBindTexture ( texID );
}

//----------------------------------------------------------------//
void ZLDraw::BindVertexArray ( u32 vertexArrayID ) {

	zglBindVertexArray ( vertexArrayID );
}

//----------------------------------------------------------------//
void ZLDraw::BlendFunc ( u32 sourceFactor, u32 destFactor ) {

	zglBlendFunc ( sourceFactor, destFactor );
}

//----------------------------------------------------------------//
void ZLDraw::BlendMode ( u32 mode ) {

	zglBlendMode ( mode );
}

//----------------------------------------------------------------//
void ZLDraw::Clear ( u32 mask ) {

	zglClear ( mask );
}

//----------------------------------------------------------------//
void ZLDraw::ClearColor ( float r, float g, float b, float a ) {

	zglClearColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void ZLDraw::Color ( float r, float g, float b, float a ) {

	zglColor ( r, g, b, a );
}

//----------------------------------------------------------------//
void ZLDraw::ColorPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {

	zglColorPointer ( size, type, stride, pointer );
}

//----------------------------------------------------------------//
void ZLDraw::CullFace ( u32 mode ) {

	zglCullFace ( mode );
}

//----------------------------------------------------------------//
void ZLDraw::DepthFunc ( u32 depthFunc ) {

	zglDepthFunc ( depthFunc );
}

//----------------------------------------------------------------//
void ZLDraw::DepthMask ( bool flag ) {

	zglDepthMask ( flag );
}

//----------------------------------------------------------------//
void ZLDraw::Disable ( u32 cap ) {

	zglDisable ( cap );
}

//----------------------------------------------------------------//
void ZLDraw::DisableClientState ( u32 cap ) {

	zglDisableClientState ( cap );
}

//----------------------------------------------------------------//
void ZLDraw::DisableVertexAttribArray ( u32 index ) {

	zglDisableVertexAttribArray ( index );
}

//----------------------------------------------------------------//
void ZLDraw::DrawArrays ( u32 primType, u32 first, u32 count ) {

	zglDrawArrays ( primType, first, count );
}

//----------------------------------------------------------------//
void ZLDraw::Enable ( u32 cap ) {

	zglEnable ( cap );
}

//----------------------------------------------------------------//
void ZLDraw::EnableClientState ( u32 cap ) {

	zglEnableClientState ( cap );
}

//----------------------------------------------------------------//
void ZLDraw::EnableVertexAttribArray ( u32 index ) {

	zglEnableVertexAttribArray ( index );
}

//----------------------------------------------------------------//
void ZLDraw::Flush () {

	zglFlush ();
}

//----------------------------------------------------------------//
ZLDraw& ZLDraw::Get () {
	static ZLDraw draw;
	return draw;
}

//----------------------------------------------------------------//
void ZLDraw::LineWidth ( float width ) {

	zglLineWidth ( width );
}

//----------------------------------------------------------------//
void ZLDraw::LoadIdentity () {

	zglLoadIdentity ();
}

//----------------------------------------------------------------//
void ZLDraw::LoadMatrix ( const ZLMatrix4x4& mtx ) {

	zglLoadMatrix ( mtx.m );
}

//----------------------------------------------------------------//
void ZLDraw::MatrixMode ( u32 mode ) {

	zglMatrixMode ( mode );
}

//----------------------------------------------------------------//
void ZLDraw::MultMatrix ( const ZLMatrix4x4& mtx ) {

	zglMultMatrix ( mtx.m );
}

//----------------------------------------------------------------//
void ZLDraw::NormalPointer ( u32 type, u32 stride, const void* pointer, bool cache  ) {

	zglNormalPointer ( type, stride, pointer );
}

//----------------------------------------------------------------//
void ZLDraw::PointSize ( float size ) {

	zglPointSize ( size );
}

//----------------------------------------------------------------//
void ZLDraw::Scissor ( s32 x, s32 y, u32 w, u32 h ) {

	zglScissor ( x, y, w, h );
}

//----------------------------------------------------------------//
void ZLDraw::TexCoordPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {

	zglTexCoordPointer ( size, type, stride, pointer );
}

//----------------------------------------------------------------//
void ZLDraw::TexEnvi ( u32 pname, s32 param ) {

	zglTexEnvi ( pname, param );
}

//----------------------------------------------------------------//
void ZLDraw::TexParameteri ( u32 pname, s32 param ) {

	zglTexParameteri ( pname, param );
}

//----------------------------------------------------------------//
void ZLDraw::Uniform1f ( u32 location, float v0 ) {

	zglUniform1f ( location, v0 );
}

//----------------------------------------------------------------//
void ZLDraw::Uniform1i ( u32 location, s32 v0 ) {

	zglUniform1i ( location, v0 );
}

//----------------------------------------------------------------//
void ZLDraw::Uniform4fv ( u32 location, u32 count, const float* value ) {

	zglUniform4fv ( location, count, value );
}

//----------------------------------------------------------------//
void ZLDraw::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const ZLMatrix3x3& mtx ) {

	zglUniformMatrix3fv ( location, count, transpose, mtx.m );
}

//----------------------------------------------------------------//
void ZLDraw::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const ZLMatrix4x4& mtx ) {

	zglUniformMatrix4fv ( location, count, transpose, mtx.m );
}

//----------------------------------------------------------------//
void ZLDraw::UseProgram ( u32 program ) {

	zglUseProgram ( program );
}

//----------------------------------------------------------------//
void ZLDraw::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer, bool cache  ) {

	zglVertexAttribPointer ( index, size, type, normalized, stride, pointer );
}

//----------------------------------------------------------------//
void ZLDraw::VertexPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {

	zglVertexPointer ( size, type, stride, pointer );
}

//----------------------------------------------------------------//
void ZLDraw::Viewport ( s32 x, s32 y, u32 w, u32 h ) {

	zglViewport ( x, y, w, h );
}
