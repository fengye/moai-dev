// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIPARTICLETIMEDEMITTER_H
#define	MOAIPARTICLETIMEDEMITTER_H

#include <moai-sim/MOAIParticleEmitter.h>

//================================================================//
// MOAIParticleTimedEmitter
//================================================================//
/**	@lua	MOAIParticleTimedEmitter
	@text	Particle emitter.
*/
class MOAIParticleTimedEmitter :
	public MOAIParticleEmitter {
private:
	
	float	mTime;
	float	mEmitTime;
	float mDuration;
	
	float	mMinFrequency;
	float	mMaxFrequency;

	//----------------------------------------------------------------//
	static int		_setFrequency			( lua_State* L );
	static int		_setDuration			( lua_State* L );
	
	//----------------------------------------------------------------//
	float			GetRandomFrequency		();
	void			OnUpdate				( float step );

public:
	
	DECL_LUA_FACTORY ( MOAIParticleTimedEmitter )


	//----------------------------------------------------------------//
					MOAIParticleTimedEmitter	();
					~MOAIParticleTimedEmitter	();
	void			RegisterLuaClass			( MOAILuaState& state );
	void			RegisterLuaFuncs			( MOAILuaState& state );
	void			SetDuraion						( float duration );
	void			SetFrequencyRange			( float min, float max );
};

#endif
