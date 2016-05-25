#ifndef A_RETRO_UI_RESOURCE_BITMAPCOLLECTION_H
#define A_RETRO_UI_RESOURCE_BITMAPCOLLECTION_H

#include <vector>
#include <unordered_map>
#include <utility>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;

	struct BitmapCollection
	{
		unordered_map< u32, u32 >          handle_index;
		unordered_map< const char *, u32 > name_index;

		vector< Handle >        handle;
		vector< const char * >  name;
		vector< const char * >  path;
		vector< pair<u32,u32> > size;
		vector< void * >        resource;
	};
}

#endif //A_RETRO_UI_RESOURCE_BITMAPCOLLECTION_H
