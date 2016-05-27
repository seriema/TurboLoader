#include <iostream>
#include "msdfgen.h"
#include "msdfgen-ext.h"

using namespace msdfgen;
using std::cout;
using std::endl;

int main()
{
	FreetypeHandle *ft = initializeFreetype();
	if ( !ft )
	{
		std::cout << "ERR COULDNT INIT FREETYPE" << std::endl;
		return 1;
	}

	FontHandle *font = loadFont( ft, "./res/OpenSans.ttf" );
	if ( !font )
	{
		std::cout << "ERR COULDNT LOAD FONT" << std::endl;
		return 2;
	}

	Shape shape;
	if ( loadGlyph(shape, font, 'A') )
	{
		shape.normalize();
		edgeColoringSimple( shape, 3.0 ); // max. angle
		Bitmap< FloatRGB > msdf( 32, 32 ); // image width, height
		generateMSDF( msdf, shape, 4.0, 1.0, Vector2(4.0, 4.0) ); // range, scale, translation

		std::string path = "output.png";
		savePng( msdf, path.c_str() );

		cout << "WROTE OUTPUT TO: '" << path << "'." << endl;
	}

	destroyFont( font );
	deinitializeFreetype( ft );

	return 0;
}
