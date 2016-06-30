#ifndef A_RETRO_UI_RESOURCE_MESHCOLLECTION_H
#define A_RETRO_UI_RESOURCE_MESHCOLLECTION_H

#include <vector>
#include <unordered_map>
#include <utility>

#include <glm/vec3.hpp>

#include "platform.h"
#include "Resource_Handle.h"

namespace RetroResource
{
	using namespace std;

	struct Mesh
	{
		u32    vertices_n;
		u32    indices_n;
		float* vertices; // Hmm cast this to arbitrary interleaved MeshVertex ??
		u32*   indices;
	};

	struct MeshCollection
	{
		unordered_map< u32, u32 >    handle_index;
		unordered_map< string, u32 > name_index;

		vector< Handle > handle;
		vector< string > name;
		vector< string > path;
		vector< Mesh >   mesh;
	};
}

#endif //A_RETRO_UI_RESOURCE_MESHCOLLECTION_H
