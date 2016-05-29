#ifndef A_RETRO_UI_RESOURCE_PACKAGECOLLECTION_H
#define A_RETRO_UI_RESOURCE_PACKAGECOLLECTION_H

#include <vector>
#include <unordered_map>
#include <utility>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;

	struct Package
	{
		Handle           handle;
		string           name;
		vector< Handle > bitmaps;
		vector< Handle > shaders;
	};

	struct PackageCollection
	{
		unordered_map< u32, Package& >    handle_lookup;
		unordered_map< string, Package& > name_lookup;

		vector< Package >                 package;
	};
}

#endif //A_RETRO_UI_RESOURCE_PACKAGECOLLECTION_H
