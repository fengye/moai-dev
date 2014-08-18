// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-gfx/ZLGfxContext.h>


//================================================================//
// ZLGfxContext
//================================================================//

//----------------------------------------------------------------//
void ZLGfxContext::ActiveTexture ( u32 textureUnit ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BindBuffer ( u32 target, u32 buffer ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BindFramebuffer ( u32 target, u32 frameBuffer ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BindRenderbuffer ( u32 renderbuffer ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BindTexture ( u32 texID ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BindVertexArray ( u32 vertexArrayID ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BlendFunc ( u32 sourceFactor, u32 destFactor ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::BlendMode ( u32 mode ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Clear ( u32 mask ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::ClearColor ( float r, float g, float b, float a ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Color ( float r, float g, float b, float a ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::ColorPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::CullFace ( u32 mode ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::DepthFunc ( u32 depthFunc ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::DepthMask ( bool flag ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Disable ( u32 cap ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::DisableClientState ( u32 cap ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::DisableVertexAttribArray ( u32 index ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::DrawArrays ( u32 primType, u32 first, u32 count ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Enable ( u32 cap ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::EnableClientState ( u32 cap ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::EnableVertexAttribArray ( u32 index ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Flush () {
}

//----------------------------------------------------------------//
void ZLGfxContext::LineWidth ( float width ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::LoadIdentity () {
}

//----------------------------------------------------------------//
void ZLGfxContext::LoadMatrix ( const ZLMatrix4x4& mtx ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::MatrixMode ( u32 mode ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::MultMatrix ( const ZLMatrix4x4& mtx ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::NormalPointer ( u32 type, u32 stride, const void* pointer, bool cache  ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::PointSize ( float size ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Scissor ( s32 x, s32 y, u32 w, u32 h ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::TexCoordPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::TexEnvi ( u32 pname, s32 param ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::TexParameteri ( u32 pname, s32 param ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Uniform1f ( u32 location, float v0 ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Uniform1i ( u32 location, s32 v0 ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Uniform4fv ( u32 location, u32 count, const float* value ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::UniformMatrix3fv ( u32 location, u32 count, bool transpose, const ZLMatrix3x3& mtx ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::UniformMatrix4fv ( u32 location, u32 count, bool transpose, const ZLMatrix4x4& mtx ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::UseProgram ( u32 program ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::VertexAttribPointer ( u32 index, u32 size, u32 type, bool normalized, u32 stride, const void* pointer, bool cache  ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::VertexPointer ( u32 size, u32 type, u32 stride, const void* pointer, bool cache  ) {
}

//----------------------------------------------------------------//
void ZLGfxContext::Viewport ( s32 x, s32 y, u32 w, u32 h ) {
}
