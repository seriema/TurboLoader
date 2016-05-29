#ifndef A_RETRO_UI_RESOURCE_SHADERCOLLECTION_H
#define A_RETRO_UI_RESOURCE_SHADERCOLLECTION_H

#include <vector>
#include <unordered_map>
#include <utility>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;

	struct ShaderCollection
	{
		unordered_map< u32, u32 >    handle_index;
		unordered_map< string, u32 > name_index;

		vector< Handle > handle;
		vector< string > name;
		vector< string > path;
		vector< char * > vert;
		vector< char * > frag;
	};
}

#endif //A_RETRO_UI_RESOURCE_SHADERCOLLECTION_H
