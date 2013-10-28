// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <contrib/utf8.h>
#include <moai-sim/MOAIAnimCurve.h>
#include <moai-sim/MOAIFont.h>
#include <moai-sim/MOAITextLabel.h>
#include <moai-sim/MOAITextDesigner.h>
#include <moai-sim/MOAITextDesignParser.h>
#include <moai-sim/MOAITextLayout.h>
#include <moai-sim/MOAITextStyle.h>
#include <moai-sim/MOAITextStyler.h>

//================================================================//
// MOAITextDesignParser
//================================================================//

//----------------------------------------------------------------//
void MOAITextDesignParser::AcceptLine () {

	this->mLayout->PushLine ( this->mLineSpriteID, this->mLineSize, this->mLineRect, this->mLineHeight, this->mLineAscent );
	
	// end line
	//this->mPen.mY += this->mLineRect.Height () + this->mDesigner->mLineSpacing;
	//this->mPen.mY = ZLFloat::Floor ( this->mPen.mY + 0.5f );
	//this->mLineRect.Init ( 0.0f, this->mPen.mY, 0.0f, this->mPen.mY );
	
	float lineWidth = this->mLineRect.Width ();
	
	this->mLayoutWidth = lineWidth > this->mLayoutWidth ? lineWidth : this->mLayoutWidth;
	this->mLayoutHeight += this->mLineHeight + this->mDesigner->mLineSpacing;
	
	// next line
	this->mLineSize = 0;
	this->mLineIdx = this->mTokenIdx;
	this->mLineSpriteID = this->mTokenSpriteID;
	this->mLineHeight = 0.0f;
	this->mLineAscent = 0.0f;
	
	this->mPrevGlyph = 0;
	
	if ( this->mTokenSize ){
		
		// slide the current token (if any) back to the origin
		for ( u32 i = 0; i < this->mTokenSize; ++i ) {
			MOAITextSprite& sprite = this->mLayout->mSprites [ this->mTokenSpriteID + i ];
			sprite.mX -= this->mTokenRect.mXMin;
		}
		
		this->mPen.mX -= this->mTokenRect.mXMin;
		this->mTokenRect.Init ( 0.0f, 0.0f, this->mTokenRect.Width (), this->mTokenRect.Height ());
	}
	else {
		this->mPen.mX = 0.0f;
	}
}

//----------------------------------------------------------------//
void MOAITextDesignParser::AcceptToken () {

	if ( this->mTokenSize ) {
	
		if ( this->mLineSize == 0 ) {
			this->mLineIdx = this->mTokenIdx;
			this->mLineSpriteID = this->mTokenSpriteID;
		}
		
		if ( this->mLineSize == 0 ) {
			this->mLineRect = this->mTokenRect;
		}
		else {
			this->mLineRect.Grow ( this->mTokenRect );
		}
		
		this->mLineSize += this->mTokenSize;
		this->mLineHeight = this->mTokenHeight > this->mLineHeight ? this->mTokenHeight : this->mLineHeight;
		this->mLineAscent = this->mTokenAscent > this->mLineAscent ? this->mTokenAscent : this->mLineAscent;
		
		this->mTokenSize = 0;
		this->mTokenIdx = this->mPrevIdx;
		this->mTokenSpriteID = this->mLayout->mSprites.GetTop ();
	}
}

//----------------------------------------------------------------//
void MOAITextDesignParser::Align () {

	ZLRect bounds;
	bounds.Init ( 0.0f, 0.0f, 0.0f, 0.0f );

	bool hasSprites = ( this->mLayout->mSprites.GetTop () > 0 );
	u32 totalLines = this->mLayout->mLines.GetTop ();

	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;

	float width = limitWidth ? this->mDesigner->mFrame.Width () : this->mLayoutWidth;
	float height = limitHeight ? this->mDesigner->mFrame.Height () : this->mLayoutHeight;

	float xMin = limitWidth ? this->mDesigner->mFrame.mXMin : 0.0f;
	float xMax = xMin + width;
	
	float yMin = limitHeight ? this->mDesigner->mFrame.mYMin : 0.0f;
	float yMax = yMin + height;
	
	float penY = 0.0f;
	float layoutHeight = this->mLayoutHeight;

	float yFlipOffset = this->mDesigner->mYFlip ? yMin + yMax : 0.0f;
	this->mLayout->mYOffset = limitHeight ? yFlipOffset : 0.0f;
	
	switch ( this->mDesigner->mVAlign ) {
		
		case MOAITextDesigner::CENTER_JUSTIFY:
			penY = limitHeight ? ( yMin + ( height * 0.5f )) - ( layoutHeight * 0.5f ) : -( layoutHeight * 0.5f );
			break;
			
		case MOAITextDesigner::TOP_JUSTIFY:
			penY = yMin;
			break;

		case MOAITextDesigner::BOTTOM_JUSTIFY:
			penY = limitHeight ? yMax - layoutHeight : -layoutHeight;
			break;
		
		case MOAITextDesigner::BASELINE_JUSTIFY:
			penY = totalLines ? -this->mLayout->mLines [ 0 ].mAscent : yMin;
			break;
	}
	
	MOAIAnimCurve** curves = this->mDesigner->mCurves;
	u32 totalCurves = this->mDesigner->mCurves.Size ();
	
	for ( u32 i = 0; i < totalLines; ++i ) {
		
		MOAITextLine& line = this->mLayout->mLines [ i ];
		ZLRect& lineRect = line.mRect;
		float lineWidth = line.mRect.Width ();
		
		float lineX = 0.0f;
		float lineY = penY;
		
		penY += line.mHeight + this->mDesigner->mLineSpacing;
		
		switch ( this->mDesigner->mHAlign ) {
		
			case MOAITextDesigner::CENTER_JUSTIFY:
				lineX = limitWidth ? ( xMin + ( width * 0.5f )) - ( lineWidth * 0.5f ) : -( width * 0.5f );
				break;
				
			case MOAITextDesigner::LEFT_JUSTIFY:
				lineX = xMin;
				break;

			case MOAITextDesigner::RIGHT_JUSTIFY:
				lineX = limitWidth ? xMax - lineWidth : -lineWidth;
				break;
		}

		lineX = ZLFloat::Floor ( lineX + 0.5f );
		lineY = ZLFloat::Floor ( lineY + 0.5f );
		
		float xOff = lineX - lineRect.mXMin;
		float yOff = lineY + line.mAscent;
		
		lineRect.Init ( lineX, lineY, lineX + lineWidth, lineY + line.mHeight );
		
		if ( hasSprites ) {
		
			//yOff = 0.0f;
			//float spriteYOff = yOff + line.mAscent;
			
			MOAIAnimCurve* curve = curves ? curves [ i % totalCurves ] : 0;
			
			for ( u32 j = 0; j < line.mSize; ++j ) {	
				MOAITextSprite& sprite = this->mLayout->mSprites [ line.mStart + j ];
				sprite.mX += xOff;
				sprite.mY += curve ? yOff + curve->GetValue (( sprite.mX - xMin ) / width ) : yOff;
			}
		}
		
		if ( i == 0 ) {
			bounds = lineRect;
		}
		else {
			bounds.Grow ( lineRect );
		}
	}
	
	this->mLayout->mBounds = bounds;
}

//----------------------------------------------------------------//
void MOAITextDesignParser::BuildLayout () {
	
	bool limitWidth = this->mDesigner->mLimitWidth;
	bool limitHeight = this->mDesigner->mLimitHeight;
	
	float frameWidth = this->mDesigner->mFrame.Width ();
	float frameHeight = this->mDesigner->mFrame.Height ();
	
	bool more = true;
	while ( more ) {
	
		u32 c = this->NextChar ();
		
		float scale = this->mDesigner->mGlyphScale * ( this->mStyle ? this->mStyle->mScale : 1.0f ) * this->mDeckScale;
		
		if ( MOAIFont::IsControl ( c )) {
		
			if ( c == '\n' ) {
				
				this->AcceptToken ();
				
				if ( !this->mLineRect.Height ()) {
					this->mLineRect.mYMax += this->mDeck->mHeight * scale;
				}
				
				this->AcceptLine ();	
			}
			else if ( c == 0 ) {
			
				this->AcceptToken ();
				this->AcceptLine ();
			
				this->mMore = false;
				more = false;
			}
		}
		else {
			MOAIGlyph* glyph = this->mDeck->GetGlyph ( c );
			if ( !glyph ) continue;
			if ( glyph->mCode == MOAIGlyph::NULL_CODE_ID ) continue;
			
			// apply kerning
			if ( this->mPrevGlyph ) {
				MOAIKernVec kernVec = this->mPrevGlyph->GetKerning ( glyph->mCode );
				this->mPen.mX += kernVec.mX * scale;
			}
			
			this->mPrevGlyph = glyph;
			
			if ( MOAIFont::IsWhitespace ( c )) {
				this->AcceptToken ();
			}
			else {
				
				ZLRect glyphRect = glyph->GetRect ( this->mPen.mX, 0.0f, scale );
				
				// handle new token
				if ( this->mTokenSize == 0 ) {
					this->mTokenIdx = this->mPrevIdx;
					this->mTokenSpriteID = this->mLayout->mSprites.GetTop ();
					this->mTokenRect = glyphRect;
					this->mTokenHeight = this->mDeck->mHeight * scale;
					this->mTokenAscent = this->mDeck->mAscent * scale;
				}
				
				bool overrun = false;
				
				if ( limitWidth ) {
				
					ZLRect lineRect = this->mTokenRect;
					if ( this->mLineSize ) {
						lineRect.Grow ( this->mLineRect );
					}
					overrun = frameWidth < lineRect.Width ();
				}
				
				bool discard = ( this->mLineSize == 0 ) && overrun; // this is the first token in the line *and* we have overrun
				
				// if we're the first token in a line *and* have overrun, don't attempt to split the token - just
				// discard the extra glyphs. later on this will be the place to implement fancy/custom token splitting.
				if ( !discard ) {
					// push the sprite
					this->mLayout->PushSprite ( this->mPrevIdx, *glyph, *this->mStyle, this->mPen.mX, 0.0f, scale );
					this->mTokenRect.Grow ( glyphRect );
					this->mTokenSize++;
				}
				
				if ( overrun ) {
					this->AcceptLine ();
				}
				
				if ( this->mDesigner->mWordBreak == MOAITextDesigner::WORD_BREAK_CHAR ) {
					this->AcceptToken ();
				}
			}
			
			// advance the pen
			this->mPen.mX += glyph->mAdvanceX * scale;
		}
		
		// if we overrun this->mHeight, then back up to the start of the current line
		if ( limitHeight && (( this->mLayoutHeight + this->mTokenHeight ) > frameHeight )) {
			
			this->mLayout->mSprites.SetTop ( this->mLineSpriteID );
			
			// if we're ending on an empty line (i.e. a newline) then throw it away
			// else back up so the next page will start on the line
			if ( this->mLineSize ) {
				this->mIdx = this->mLineIdx;
			}
			else if ( this->mTokenSize ) {
				this->mIdx = this->mTokenIdx;
			}
			else {
				this->mIdx = this->mIdx;
			}
			
			more = false;
		}
	}

	this->Align ();
}

//----------------------------------------------------------------//
void MOAITextDesignParser::BuildLayout ( MOAITextLayout& layout, MOAITextStyler& styler, MOAITextDesigner& designer, cc8* str, u32 idx ) {
	
	if ( styler.CountSpans () == 0 ) return;
	
	this->mLayout = &layout;
	this->mStyler = &styler;
	this->mDesigner = &designer;
	
	this->mStr = str;
	this->mIdx = idx;
	this->mPrevIdx = idx;
	
	this->mStyleSpan = 0;
	this->mStyle = 0;
	
	this->mLineIdx = this->mIdx;
	this->mLineSpriteID = 0;
	this->mLineSize = 0;
	this->mLineHeight = 0.0f;
	this->mLineAscent = 0.0f;
	
	this->mLineRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mTokenIdx = this->mIdx;
	this->mTokenSpriteID = 0;
	this->mTokenSize = 0;
	this->mTokenHeight = 0.0f;
	this->mTokenAscent = 0.0f;
	
	this->mTokenRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	
	this->mLayoutWidth = 0.0f;
	this->mLayoutHeight = 0.0f;
	
	this->mPen.Init ( 0.0f, 0.0f );
	this->mPrevGlyph = 0;
	
	this->mMore = true;
	
	this->BuildLayout ();
}

//----------------------------------------------------------------//
MOAITextDesignParser::MOAITextDesignParser () {
}

//----------------------------------------------------------------//
MOAITextDesignParser::~MOAITextDesignParser () {
}

//----------------------------------------------------------------//
bool MOAITextDesignParser::More () {

	return this->mMore;
}

//----------------------------------------------------------------//
u32 MOAITextDesignParser::NextChar () {

	bool newSpan = false;

	if ( !this->mStyleSpan ) {
		this->mStyleSpan = &this->mStyler->mStyleMap.Elem ( 0 );
		this->mSpanIdx = 0;
		newSpan = true;
	}

	if ( this->mIdx >= this->mStyleSpan->mTop ) {
		
		this->mStyleSpan = 0;
		
		u32 totalStyles = this->mStyler->mStyleMap.GetTop ();
		for ( this->mSpanIdx++; this->mSpanIdx < totalStyles; this->mSpanIdx++ ) {
			MOAITextStyleSpan& styleSpan = this->mStyler->mStyleMap.Elem ( this->mSpanIdx );
			
			if ( this->mIdx < styleSpan.mTop ) {
				this->mStyleSpan = &styleSpan;
				newSpan = true;
				break;
			}
		}
	}
	
	if ( this->mStyleSpan ) {
	
		if ( newSpan ) {
		
			if ( this->mIdx < this->mStyleSpan->mBase ) {
				this->mIdx = this->mStyleSpan->mBase;
			}
		
			this->mStyle = this->mStyleSpan->mStyle;
			assert ( this->mStyle );
			
			MOAIFont* font = this->mStyle->mFont;
			assert ( font );
			
			this->mDeck = font->GetGlyphSet ( this->mStyle->mSize );
			this->mDeckScale = this->mDeck && ( this->mStyle->mSize > 0.0f ) ? this->mStyle->mSize / this->mDeck->GetSize () : 1.0f;
		}
		
		this->mPrevIdx = this->mIdx;
		u32 c = u8_nextchar ( this->mStr, &this->mIdx );
		return c;
	}
	return 0;
}
