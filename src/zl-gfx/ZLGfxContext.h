// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLGFXCONTEXT_H
#define ZLGFXCONTEXT_H

#include <zl-gfx/ZLDraw.h>

//================================================================//
// ZLGfxContext
//================================================================//
class ZLGfxContext :
	public ZLDraw {
public:

	// api for drawing plus cache
	// can be directed to display list or other means of rendering

	//----------------------------------------------------------------//
	void		ActiveTexture				( u32 textureUnit );
	void		BindBuffer					( u32 target, u32 buffer );
	void		BindFramebuffer				( u32 target, u32 frameBuffer );
	void		BindRenderbuffer			( u32 renderbuffer );
	void		BindTexture					( u32 texID );
	void		BindVertexArray				( u32 vertexArrayID );
	void		BlendFunc					( u32 sourceFactor, u32 destFactor );
	void		BlendMode					( u32 mode );
	void		Clear						( u32 mask );
	void		ClearColor					( float r, float g, float b, float a );
	void		Color						( float r, float g, float b, float a );
	void		ColorPointer				( u32 size, u32 type, u32 stride, const void* pointer, bool cache  );
	void		CullFace					( u32 mode );
	void		DepthFunc					( u32 depthFunc );
	void		DepthMask					( bool flag );
	void		Disable						( u32 cap );
	void		DisableClientState			( u32 cap );
	void		DisableVertexAttribArray	( u32 index );
	void		DrawArrays					( u32 primType, u32 first, u32 count );
	void		Enable						( u32 cap );
	void		EnableClientState			( u32 cap );
	void		EnableVertexAttribArray		( u32 index );
	void		Flush						();
	void		LineWidth					( float width );
	void		LoadIdentity				();
	void		LoadMatrix					( const ZLMatrix4x4& mtx );
	void		MatrixMode					( u32 mode );
	void		MultMatrix					( const ZLMatrix4x4& mtx );
	void		NormalPointer				( u32 type, u32 stride, const void* pointer, bool cache  );
	void		PointSize					( float size );
	void		Scissor						( s32 x, s32 y, u32 w, u32 h );
	void		TexCoordPointer				( u32 size, u32 type, u32 stride, const void* pointer, bool cache  );
	void		TexEnvi						( u32 pname, s32 param );
	void		TexParameteri				( u32 pname, s32 param );
	void		Uniform1f					( u32 location, float v0 );
	void		Uniform1i					( u32 location, s32 v0 );
	void		Uniform4fv					( u32 location, u32 count, const float* value );
	void		UniformMatrix3fv			( u32 location, u32 count, bool transpose, const ZLMatrix3x3& mtx );
	void		UniformMatrix4fv			( u32 location, u32 count, bool transpose, const ZLMatrix4x4& mtx );
	void		UseProgram					( u32 program );
	void		VertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer, bool cache  );
	void		VertexPointer				( u32 size, u32 type, u32 stride, const void* pointer, bool cache  );
	void		Viewport					( s32 x, s32 y, u32 w, u32 h );
};

#endif
