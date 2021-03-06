#ifndef A_RETRO_UI_RESOURCE_BITMAPCOLLECTION_H
#define A_RETRO_UI_RESOURCE_BITMAPCOLLECTION_H

#include <vector>
#include <unordered_map>
#include <utility>

#include <glm/vec2.hpp>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;

	struct BitmapCollection
	{
		unordered_map< u32, u32 >    handle_index;
		unordered_map< string, u32 > name_index;

		vector< Handle >    handle;
		vector< string >    name;
		vector< string >    path;
		vector< glm::vec2 > size;
		vector< void * >    resource;
	};
}

#endif //A_RETRO_UI_RESOURCE_BITMAPCOLLECTION_H
