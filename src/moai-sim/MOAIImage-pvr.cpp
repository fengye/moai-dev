// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <moai-core/host.h>
#include <moai-core/MOAILogMessages.h>
#include <moai-sim/MOAIImage.h>
#include "PVRTTexture.h"
#include "PVRTDecompress.h"

#if MOAI_WITH_LIBPVR
//----------------------------------------------------------------//
void MOAIImage::LoadPVR	( ZLStream& stream, u32 transform )
{
	// for now, no transform is done
	transform = 0;
	
	PVR_Texture_Header header;
	
	// Read the file header
	stream.ReadBytes((char*)&header, sizeof(header));
	
	// Perform checks for old PVR psPVRHeader
	if(header.dwHeaderSize != sizeof(PVR_Texture_Header)) {
		MOAIPrint( MOAILogMgr::LOG_ERROR, (char*)"Bad PVR file");
		return -1;
	}
	
	mWidth = header.dwWidth;
	mHeight = header.dwHeight;
	
	switch(header.dwpfFlags & PVRTEX_PIXELTYPE)
	{
		case OGL_RGBA_8888:
			this->mPixelFormat = USPixel::TRUECOLOR;
			this->mColorFormat = ZLColor::RGBA_8888;
			break;
		case OGL_RGB_888:
			this->mPixelFormat = USPixel::TRUECOLOR;
			this->mColorFormat = ZLColor::RGB_888;
			break;
		case OGL_PVRTC2:
		case OGL_PVRTC4:
			this->mPixelFormat = USPixel::TRUECOLOR;
			this->mColorFormat = ZLColor::RGBA_8888;
			break;
		default:
			// TODO: Support more formats
			MOAIPrint( MOAILogMgr::LOG_ERROR, (char*)"MOAIImage-PVR", "Unsupported texture format");
			return;
	}

	// TODO: dwTextureDataSize only specify for size of one surface, Support multiple surface,
	Byte *imageData = new Byte[header.dwTextureDataSize];
	
	if(!imageData) {
		MOAIPrint( MOAILogMgr::LOG_ERROR, (char*)"Corruption of file or not enough memory, operation aborted");
		return;
	}
	
	;
	if(stream.ReadBytes((char*)imageData, header.dwTextureDataSize) != size_t(header.dwTextureDataSize)) {
		MOAIPrint( MOAILogMgr::LOG_WARNING, (char*)"End of file, pixel data incomplete");
		delete imageData;
		return;
	}
	
	// Decompress the PVRTC format on platforms without native hardware support
	const int pixelFormat = header.dwpfFlags & PVRTEX_PIXELTYPE;
	if(pixelFormat == OGL_PVRTC2 || pixelFormat == OGL_PVRTC4)
	{
		const bool twoBitMode = (pixelFormat == OGL_PVRTC2);
		
		// Calculate the total memory required for all the mip-map levels
		size_t memSize = 0;
	
		size_t i=0;
		while (i == 0 || i<header.dwMipMapCount) {
			const size_t factor = 1 << i;
			memSize += (mWidth * mHeight * 4) / (factor * factor);
			++i;
		}
		
		Byte* deCompressed = new Byte[memSize];
		
		Byte* srcPtr = imageData;
		Byte* destPtr = deCompressed;
		size_t sizeX = mWidth, sizeY = mHeight;
		
		i=0;
		while (i == 0 || i<header.dwMipMapCount) {
			PVRTDecompressPVRTC(srcPtr, twoBitMode, sizeX, sizeY, destPtr);
			
			const size_t minTexWidth = twoBitMode ? PVRTC2_MIN_TEXWIDTH : PVRTC4_MIN_TEXWIDTH;
			const size_t minTexHeight = twoBitMode ? PVRTC2_MIN_TEXHEIGHT : PVRTC4_MIN_TEXHEIGHT;
			
			const size_t compressedSize =
			MAX(sizeX, minTexWidth) *
			MAX(sizeY, minTexHeight) * header.dwBitCount / 8;
			
			srcPtr += compressedSize;
			
			const size_t deCompressedSize = sizeX * sizeY * 4;
			destPtr += deCompressedSize;

			++i;
			sizeX = MAX(sizeX/2, 1u), sizeY = MAX(sizeY/2, 1u);
		}
		
		if ( this->mData ) {
			free ( this->mData );
			this->mData = 0;
		}
		
		if ( this->mPalette ) {
			free ( this->mPalette );
			this->mPalette = 0;
		}
		this->mData = this->mBitmap = deCompressed;

		// <-Plumzi Addition
#ifdef MACOSX
		// Color correction of the PNGs for proper color matching on OSX
		// between a video encoded in the sRGB colorspace and a PNG (which is sRGB by default apparently)
		
		// correcting the gamma & desaturating a bit brings the rendering close enough
		// those are totally empirical values based on my eye
		for ( u32 y = 0; y < mHeight; ++y ) {
			void* row = this->GetRowAddr ( y );
			ZLColor::GammaCorrection ( row, this->mColorFormat, mWidth, 1.15f );
			ZLColor::Desaturate ( row, this->mColorFormat, mWidth, 0.1f );
		}
#endif
	} else {
		MOAIPrint( MOAILogMgr::LOG_ERROR, (char*)"PVR format not supported (only PVRTC2 or PVRTC4 are)");
	}
	delete imageData;
//	return header.dwTextureDataSize;
	return;
}

#endif
