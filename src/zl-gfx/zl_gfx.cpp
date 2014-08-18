// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-gfx/zl_gfx.h>

//================================================================//
// enums
//================================================================//

//----------------------------------------------------------------//
u32 zglGetComponentSize ( u32 type ) {

	u32 bytes;
	switch ( type ) {
	
		case ZGL_TYPE_BYTE:
		case ZGL_TYPE_UNSIGNED_BYTE:
			bytes = 1;
			break;
		
		case ZGL_TYPE_SHORT:
		case ZGL_TYPE_UNSIGNED_SHORT:
			bytes = 2;
			break;
		
		//case ZGL_FIXED:
		case ZGL_TYPE_FLOAT:
			bytes = 4;
			break;
		
		default:
			bytes = 0;
			break;		
	}
	
	return bytes;
}
