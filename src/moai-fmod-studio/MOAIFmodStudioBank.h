// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFMODSTUDIOBANK_H
#define	MOAIFMODSTUDIOBANK_H

#include <moai-sim/headers.h>
#include <moai-fmod-studio/MOAIFmodStudio.h>

class MOAIDataBuffer;

//================================================================//
// MOAIFmodStudioBank
//================================================================//
/**	@name	MOAIFmodStudioBank
	@text	Fmod bank object.
 */
class MOAIFmodStudioBank :
	public virtual MOAILuaObject {
private:
		
	FMOD::Studio::Bank* mBank;
	STLString mFileName;

	//----------------------------------------------------------------//
	static int	_load				( lua_State* L );
	static int	_release			( lua_State* L );

public:

	friend class MOAIFmodStudio;

	DECL_LUA_FACTORY ( MOAIFmodStudioBank )

	//----------------------------------------------------------------//
				MOAIFmodStudioBank		();
				~MOAIFmodStudioBank		();
	const char	*GetFileName			() { return mFileName.c_str(); }
	bool		Load					( MOAIDataBuffer& data );
	bool		Load					( cc8* filename );
	void		RegisterLuaClass		( MOAILuaState& state );
	void		RegisterLuaFuncs		( MOAILuaState& state );
	void		Release					();			
	STLString	ToString				();
};

#endif
