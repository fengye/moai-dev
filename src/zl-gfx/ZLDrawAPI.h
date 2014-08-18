// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDRAWAPI_H
#define ZLDRAWAPI_H

//================================================================//
// ZLDrawAPI
//================================================================//
class ZLDrawAPI {
public:

	// api for drawing

	//----------------------------------------------------------------//
	virtual void		ActiveTexture				( u32 textureUnit ) = 0;
	virtual void		BindBuffer					( u32 target, u32 buffer ) = 0;
	virtual void		BindFramebuffer				( u32 target, u32 frameBuffer ) = 0;
	virtual void		BindRenderbuffer			( u32 renderbuffer ) = 0;
	virtual void		BindTexture					( u32 texID ) = 0;
	virtual void		BindVertexArray				( u32 vertexArrayID ) = 0;
	virtual void		BlendFunc					( u32 sourceFactor, u32 destFactor ) = 0;
	virtual void		BlendMode					( u32 mode ) = 0;
	virtual void		Clear						( u32 mask ) = 0;
	virtual void		ClearColor					( float r, float g, float b, float a ) = 0;
	virtual void		Color						( float r, float g, float b, float a ) = 0;
	virtual void		ColorPointer				( u32 size, u32 type, u32 stride, const void* pointer, bool cache ) = 0;
	virtual void		CullFace					( u32 mode ) = 0;
	virtual void		DepthFunc					( u32 depthFunc ) = 0;
	virtual void		DepthMask					( bool flag ) = 0;
	virtual void		Disable						( u32 cap ) = 0;
	virtual void		DisableClientState			( u32 cap ) = 0;
	virtual void		DisableVertexAttribArray	( u32 index ) = 0;
	virtual void		DrawArrays					( u32 primType, u32 first, u32 count ) = 0;
	virtual void		Enable						( u32 cap ) = 0;
	virtual void		EnableClientState			( u32 cap ) = 0;
	virtual void		EnableVertexAttribArray		( u32 index ) = 0;
	virtual void		Flush						() = 0;
	virtual void		LineWidth					( float width ) = 0;
	virtual void		LoadIdentity				() = 0;
	virtual void		LoadMatrix					( const ZLMatrix4x4& mtx ) = 0;
	virtual void		MatrixMode					( u32 mode ) = 0;
	virtual void		MultMatrix					( const ZLMatrix4x4& mtx ) = 0;
	virtual void		NormalPointer				( u32 type, u32 stride, const void* pointer, bool cache ) = 0;
	virtual void		PointSize					( float size ) = 0;
	virtual void		Scissor						( s32 x, s32 y, u32 w, u32 h ) = 0;
	virtual void		TexCoordPointer				( u32 size, u32 type, u32 stride, const void* pointer, bool cache ) = 0;
	virtual void		TexEnvi						( u32 pname, s32 param ) = 0;
	virtual void		TexParameteri				( u32 pname, s32 param ) = 0;
	virtual void		Uniform1f					( u32 location, float v0 ) = 0;
	virtual void		Uniform1i					( u32 location, s32 v0 ) = 0;
	virtual void		Uniform4fv					( u32 location, u32 count, const float* value ) = 0;
	virtual void		UniformMatrix3fv			( u32 location, u32 count, bool transpose, const ZLMatrix3x3& mtx ) = 0;
	virtual void		UniformMatrix4fv			( u32 location, u32 count, bool transpose, const ZLMatrix4x4& mtx ) = 0;
	virtual void		UseProgram					( u32 program ) = 0;
	virtual void		VertexAttribPointer			( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer, bool cache ) = 0;
	virtual void		VertexPointer				( u32 size, u32 type, u32 stride, const void* pointer, bool cache ) = 0;
	virtual void		Viewport					( s32 x, s32 y, u32 w, u32 h ) = 0;
						ZLDrawAPI					() {}
	virtual				~ZLDrawAPI					() {}
};

#endif
