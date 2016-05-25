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
		unordered_map< u32, u32 >          handle_index;
		unordered_map< const char *, u32 > name_index;

		vector< Handle >        handle;
		vector< const char * >  name;
		vector< const char * >  path;
		vector< const char * >  vert;
		vector< const char * >  frag;
	};
}

#endif //A_RETRO_UI_RESOURCE_SHADERCOLLECTION_H
