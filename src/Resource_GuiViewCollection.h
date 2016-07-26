#ifndef A_RETRO_UI_RESOURCE_GUIVIEWCOLLECTION_H
#define A_RETRO_UI_RESOURCE_GUIVIEWCOLLECTION_H

#include <vector>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;
	namespace pt = boost::property_tree;

	struct GuiViewCollection
	{
		unordered_map<u32, u32> handle_index;
		unordered_map<string, u32> name_index;

		vector<Handle> handle;
		vector<pt::ptree> view;
	};
}

#endif //A_RETRO_UI_RESOURCE_GUIVIEWCOLLECTION_H
