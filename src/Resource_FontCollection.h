#ifndef A_RETRO_UI_RESOURCE_FONTCOLLECTION_H
#define A_RETRO_UI_RESOURCE_FONTCOLLECTION_H

#include <vector>
#include <unordered_map>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;

	struct FontMetadata
	{
		u8     w;
		u8     h;
		float  baseline;
	};

	struct FontCollection
	{
		unordered_map< u32, u32 >    handle_index;
		unordered_map< string, u32 > name_index;

		vector< Handle >        handle;
		vector< string >        name;
		vector< string >        path;
		vector< FontMetadata >  metadata;
	};
}

#endif //A_RETRO_UI_RESOURCE_FONTCOLLECTION_H
