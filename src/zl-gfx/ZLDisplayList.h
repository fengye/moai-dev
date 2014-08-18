// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLDISPLAYLIST_H
#define ZLDISPLAYLIST_H

#include <zl-gfx/ZLDrawAPI.h>

//================================================================//
// ZLPointerCacheEntry
//================================================================//
class ZLPointerCacheEntry {
private:

	friend class ZLDisplayList;

	void*		mPointer;
	u32			mSize;
	u32			mStride;
};

//================================================================//
// ZLPointerCache
//================================================================//
class ZLPointerCache {
private:

	friend class ZLDisplayList;

	enum {
		USAGE_COLOR_POINTER,
		USAGE_NORMAL_POINTER,
		USAGE_TEX_COORD_POINTER,
		USAGE_VERTEX_ATTRIB_POINTER,
		USAGE_VERTEX_POINTER,
		USAGE_TOTAL,
	};
	
	ZLPointerCacheEntry mEntries [ USAGE_TOTAL ];

	//----------------------------------------------------------------//
	void		

public:

	//----------------------------------------------------------------//
				ZLPointerCache			();
				~ZLPointerCache			();

};

//================================================================//
// ZLDisplayList
//================================================================//
class ZLDisplayList :
	public ZLDrawAPI {
private:

	enum {
		UNKNOWN,
		ACTIVE_TEXTURE,
		BIND_BUFFER,
		BIND_FRAMEBUFFER,
		BIND_RENDERBUFFER,
		BIND_TEXTURE,
		BIND_VERTEX_ARRAY,
		BLEND_FUNC,
		BLEND_MODE,
		CACHED_POINTERS,
		CLEAR,
		CLEAR_COLOR,
		COLOR,
		COLOR_POINTER,
		CULL_FACE,
		DEPTH_FUNC,
		DEPTH_MASK,
		DISABLE,
		DISABLE_CLIENT_STATE,
		DISABLE_VERTEX_ATTRIB_ARRAY,
		DRAW_ARRAYS,
		ENABLE,
		ENABLE_CLIENT_STATE,
		ENABLE_VERTEX_ATTRIB_ARRAY,
		FLUSH,
		LINE_WIDTH,
		LOAD_IDENTITY,
		LOAD_MATRIX,
		MATRIX_MODE,
		MULT_MATRIX,
		NORMAL_POINTER,
		POINT_SIZE,
		SCISSOR,
		TEX_COORD_POINTER,
		TEX_ENVI,
		TEX_PARAMETERI,
		UNIFORM_1F,
		UNIFORM_1I,
		UNIFORM_4FV,
		UNIFORM_MATRIX_3FV,
		UNIFORM_MATRIX_4FV,
		USE_PROGRAM,
		VERTEX_ATTRIB_POINTER,
		VERTEX_POINTER,
		VIEWPORT,
	};

	ZLStream* mStream;

	ZLLeanStack < ZLPointerCache, 32 > mPointerCacheStack;

	// cacheing a pointer creates a new, empty buffer cache if none exists
	// the cache gets populated with refs to the buffers to copy
	// the buffers get copied and interleaved only on a subsqeuent call to DrawArrays
	// if the buffers aren't copied and interleaved, the original calls to bind the pointers get skipped

	//----------------------------------------------------------------//
	const void*		CachePointer				( u32 usage, u32 size, u32 type, u32 stride, const void* buffer );

public:

	// write to buffer
	// decode from buffer and call
	// or just call
	// stream model

	GET_SET ( ZLStream*, Stream, mStream )

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
	void		Draw						( ZLDrawAPI& draw );
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
	void		Reset						();
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
				ZLDisplayList				();
				~ZLDisplayList				();
};

#endif
