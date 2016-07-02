#ifndef A_RETRO_UI_MATH_H
#define A_RETRO_UI_MATH_H

#include "platform.h"

namespace RetroMath
{
	// fnv-1a hash from http://www.isthe.com/chongo/tech/comp/fnv/

	const u32 FNV_PRIME_32 = 16777619u;
	const u32 FNV_OFFSET_BASIS_32  = 2166136261u;

	inline u32 fnv1a( const void* data, const u32 size )
	{
		u32 hash = FNV_OFFSET_BASIS_32;
		for ( u32 i = 0; i < size; ++i )
		{
			hash = ( hash ^ (((unsigned char*)(data))[i] ) ) * FNV_PRIME_32;
		}
		return hash;
	}

	inline u32 fnv1a( const u32 data )
	{
		return fnv1a( &data, sizeof(u32) );
	}

	template< typename T >
	inline T lerp( T a, T b, T t )
	{
		return a + t * (b - a);
	}

	template< typename T >
	inline T sign( T v )
	{
		return static_cast<T>( (T(0) < v) - (v < T(0)) );
	}
}

#endif //A_RETRO_UI_MATH_H
