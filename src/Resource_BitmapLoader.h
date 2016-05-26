#ifndef A_RETRO_UI_BITMAPLOADER_H
#define A_RETRO_UI_BITMAPLOADER_H

#include <vector>

#include <FreeImage.h>

#include "platform.h"
#include "Resource_Handle.h"
#include "Resource_HandleManager.h"
#include "Resource_BitmapCollection.h"

namespace RetroResource
{
	class BitmapLoader
	{
		HandleManager    & _handle_manager;
		BitmapCollection & _bitmaps;

	public:
		BitmapLoader( HandleManager & handle_manager, BitmapCollection & bitmaps )
			: _handle_manager( handle_manager )
			, _bitmaps( bitmaps )
		{}

		u32 load( const char ** names, const char ** paths, Handle * handles, const u32 size = 1 )
		{
			u32 size0 = _bitmaps.handle.size();
			u32 size1 = size0;

			_bitmaps.handle.reserve( size );
			_bitmaps.name.reserve( size );
			_bitmaps.path.reserve( size );
			_bitmaps.size.reserve( size );
			_bitmaps.resource.reserve( size );

			for ( int i = 0; i < size; ++i )
			{
				const char * name = names[ i ];
				const char * path = paths[ i ];

				FIBITMAP * bitmap = nullptr;
				u32 w, h;

				// TODO Assert path is valid, otherwise handle error.

				load_FreeImage( path, bitmap, w, h );
				// TODO Make sure no errors occured.

				// TODO FreeImage loads in BGR format, convert to RGB.

				printf( "Texture file: %s\n", path );
				printf( "Texture size: %d x %d\n", w, h );

				Handle handle = _handle_manager.create();

				// TODO Make sure no name collision occurs!

				_bitmaps.handle_index.insert( { handle.id, size1 } );
				_bitmaps.name_index.insert( { name, size1 } );

				_bitmaps.handle.push_back( handle );
				_bitmaps.name.push_back( name );
				_bitmaps.path.push_back( path );
				_bitmaps.size.push_back( { w, h } );
				_bitmaps.resource.push_back( bitmap );

				handles[ size1 - size0 ] = handle;

				++size1;
			}

			return size1 - size0;
		}

		void unload( Handle * handles, const u32 size = 1 )
		{
			u32 i_last = _bitmaps.handle.size();

			for ( int j = 0; j < size; ++j )
			{
				Handle handle = handles[ j ];
				u32 i = _bitmaps.handle_index[ handle.id ];

				const char * name = _bitmaps.name[ i ];
				FIBITMAP * bitmap = static_cast< FIBITMAP* >( _bitmaps.resource[ i ] );

				Handle handle_last = _bitmaps.handle[ i_last ];
				const char * name_last = _bitmaps.name[ i_last ];

				_bitmaps.handle_index[ handle_last.id ] = i;
				_bitmaps.name_index[ name_last ] = i;

				_bitmaps.handle_index.erase( handle.id );
				_bitmaps.name_index.erase( name );

				_bitmaps.handle[ i ] = _bitmaps.handle[ i_last ]; _bitmaps.handle.pop_back();
				_bitmaps.name[ i ] = _bitmaps.name[ i_last ]; _bitmaps.name.pop_back();
				_bitmaps.path[ i ] = _bitmaps.path[ i_last ]; _bitmaps.path.pop_back();
				_bitmaps.size[ i ] = _bitmaps.size[ i_last ]; _bitmaps.size.pop_back();
				_bitmaps.resource[ i ] = _bitmaps.resource[ i_last ]; _bitmaps.resource.pop_back();

				--i_last;

				_handle_manager.destroy( handle );
				FreeImage_Unload( bitmap );
			}
		}

	private:
		bool load_FreeImage( const char * path, FIBITMAP *& bitmap, u32 & w, u32 & h )
		{
			FREE_IMAGE_FORMAT format = FreeImage_GetFileType( path, 0 );
			bitmap = FreeImage_Load( format, path );

			{
				FIBITMAP* temp = bitmap;
				bitmap = FreeImage_ConvertTo32Bits( bitmap );
				FreeImage_Unload( temp );
			}

			w = FreeImage_GetWidth( bitmap );
			h = FreeImage_GetHeight( bitmap );

			return true; // TODO return false or error if loading fails.

			//imgdata = FreeImage_GetBits( img ); // TODO if img size isnt power of 2 then use FreeImage_ConvertToRawBits!
		}
	};
}

#endif //A_RETRO_UI_BITMAPLOADER_H
