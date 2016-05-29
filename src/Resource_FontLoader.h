#ifndef A_RETRO_UI_RESOURCE_FONTLOADER_H
#define A_RETRO_UI_RESOURCE_FONTLOADER_H

#include <algorithm>
#include <vector>

#include <msdfgen.h>
#include <msdfgen-ext.h>

#include "platform.h"
#include "Resource_Handle.h"
#include "Resource_HandleManager.h"
#include "Resource_FontCollection.h"
#include "Resource_BitmapCollection.h"

namespace RetroResource
{
	class FontLoader
	{
		HandleManager  &   _handle_manager;
		FontCollection &   _fonts;
		BitmapCollection & _bitmaps;

	public:
		FontLoader( HandleManager & handle_manager, FontCollection & fonts, BitmapCollection & bitmaps )
			: _handle_manager( handle_manager )
			, _fonts( fonts )
			, _bitmaps( bitmaps )
		{

		}

		u32 load( const std::string * names, const std::string * paths, Handle * handles, const u32 size = 1 )
		{
			msdfgen::FreetypeHandle * ft = msdfgen::initializeFreetype();
			if ( !ft )
			{
				std::cout << "[font loader] error couldnt init freetype." << std::endl;
				return 0;
			}

			u32 fonts_size0 = _fonts.handle.size();
			u32 fonts_size = fonts_size0;
			u32 bitmaps_size = _bitmaps.handle.size();

			{
				u32 fonts_new_size = fonts_size + size;
				_fonts.handle.reserve( fonts_new_size );
				_fonts.name.reserve( fonts_new_size );
				_fonts.path.reserve( fonts_new_size );
				_fonts.metadata.reserve( fonts_new_size );

				u32 bitmaps_new_size = bitmaps_size + size;
				_bitmaps.handle.reserve( bitmaps_new_size );
				_bitmaps.name.reserve( bitmaps_new_size );
				_bitmaps.path.reserve( bitmaps_new_size );
				_bitmaps.size.reserve( bitmaps_new_size );
				_bitmaps.resource.reserve( bitmaps_new_size );
				_bitmaps.resource_raw.reserve( bitmaps_new_size );
			}

			for ( int i = 0; i < size; ++i )
			{
				const std::string & name = names[ i ];
				const std::string & path = paths[ i ];

				msdfgen::FontHandle * font = msdfgen::loadFont( ft, path.c_str() );
				if ( !font )
				{
					std::cout << "[font loader] error couldnt load font '" << path << "'" << std::endl;
					continue;
				}

				std::cout << "[font loader] loading '" << path << "' ..." << std::endl;

				// TODO this should be specific per font and read from font loader!
				float baseline = 0.0f;
				u8 glyph_w = 24;
				u8 glyph_h = 32;
				double angle_threshold = 3.0;
				double range = 4.0;
				double scale = 1.0;
				msdfgen::Vector2 translation( 4.0, 6.0 );

				int atlas_w = 16; // 16 x 8 glyphs.
				int atlas_h = 8;
				int chars_n = 128;
				int channels_n = 4;
				u32 glyph_size = glyph_w * glyph_h;
				char * font_atlas = new char[ channels_n * glyph_size * chars_n ];

				for ( int c = 0; c < chars_n; ++c )
				{
					msdfgen::Shape shape;
					if ( !msdfgen::loadGlyph( shape, font, (char)c ) )
					{
						std::cout << "    couldnt find char '" << (int)c << "'" << std::endl;
						continue;
					}

					shape.normalize();
					msdfgen::edgeColoringSimple( shape, angle_threshold );
					msdfgen::Bitmap< msdfgen::FloatRGB > msdf( glyph_w, glyph_h );
					msdfgen::generateMSDF( msdf, shape, range, scale, translation );

					int glyph_y = 0;//glyph_h - 1;
					int y_step = channels_n * glyph_w * atlas_w;
					int y = y_step * glyph_h * ( c / atlas_w );
					int y_end = y + y_step * glyph_h;

					while ( y < y_end )
					{
						int glyph_x = 0;
						int x = channels_n * glyph_w * ( c % atlas_w );
						int x_end = x + channels_n * glyph_w;

						while ( x < x_end )
						{
							msdfgen::FloatRGB & rgb = msdf( glyph_x, glyph_y );
							++glyph_x;

							font_atlas[ y + x++ ] = std::max( 0, std::min( 255, int(255 * rgb.r) ) );
							font_atlas[ y + x++ ] = std::max( 0, std::min( 255, int(255 * rgb.g) ) );
							font_atlas[ y + x++ ] = std::max( 0, std::min( 255, int(255 * rgb.b) ) );
							font_atlas[ y + x++ ] = 255;
						}

						++glyph_y;
						y += y_step;
					}
				}

				msdfgen::destroyFont( font );
				msdfgen::deinitializeFreetype( ft );

				Handle handle = _handle_manager.create();

				_fonts.handle_index.insert( { handle.id, fonts_size } );
				_fonts.name_index.insert( { name, fonts_size } );

				_fonts.handle.push_back( handle );
				_fonts.name.push_back( name );
				_fonts.path.push_back( path );
				_fonts.metadata.push_back( { glyph_w, glyph_h, baseline } );

				_bitmaps.handle_index.insert( { handle.id, bitmaps_size } );
				_bitmaps.name_index.insert( { name, bitmaps_size } );

				_bitmaps.handle.push_back( handle );
				_bitmaps.name.push_back( name );
				_bitmaps.path.push_back( path );
				_bitmaps.size.push_back( { glyph_w*atlas_w, glyph_h*atlas_h } );
				_bitmaps.resource_raw.push_back( font_atlas );

				handles[ fonts_size - fonts_size0 ] = handle;

				++fonts_size;
				++bitmaps_size;

				std::cout << "[font loader] loaded: (" << handle.id << ") '" << path << "'" << std::endl;
			}

			return fonts_size - fonts_size0;
		}

		void unload( Handle * handles, const u32 size = 1 )
		{
			u32 i_last = _fonts.handle.size();

			for ( int j = 0; j < size; ++j )
			{
				Handle handle = handles[ j ];
				u32 i = _fonts.handle_index.at( handle.id );

				std::string & name = _fonts.name[ i ];
				char * font_atlas = static_cast< char* >( _bitmaps.resource_raw[ i ] );

				// TODO Move data here!

				--i_last;

				_handle_manager.destroy( handle );
				delete [] font_atlas; // TODO How do we know the size here?
			}
		}
	};
}

#endif //A_RETRO_UI_RESOURCE_FONTLOADER_H
