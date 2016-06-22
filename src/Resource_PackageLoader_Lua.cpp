
#include <vector>
#include <string>
#include <iostream>

#include "platform.h"
#include "Resource_PackageLoader_Lua.h"

extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

namespace
{
	const std::string LUA_SUFFIX = ".lua";

	struct PackageData
	{
		u32                        bitmap_size;
		std::vector< std::string > bitmap_names;
		std::vector< std::string > bitmap_paths;

		u32                        shader_size;
		std::vector< std::string > shader_names;
		std::vector< std::string > shader_paths;
	};

	bool is_argc( int expected_argc, int argc )
	{
		if( argc == expected_argc )
			return true;
		printf( "ERROR wrong Lua arg count. Expected %d args, got %d args.", expected_argc, argc );
		return false;
	}

	bool is_type_key( int expected_type, int type )
	{
		if ( type == expected_type )
			return true;
		printf( "ERROR wrong Lua table type for key. Expected '%d', got '%d'.", expected_type, type );
		return false;
	}

	bool is_type_val( int expected_type, int type )
	{
		if ( type == expected_type )
			return true;
		printf( "ERROR wrong Lua table type for val. Expected '%d', got '%d'.", expected_type, type );
		return false;
	}

	int lua_read_bitmaps( lua_State* L )
	{
		if ( !is_argc(1, lua_gettop(L)) )
			return 0;

		auto package = static_cast< PackageData * >( lua_touserdata(L, lua_upvalueindex(1)) );

		lua_pushnil( L );
		while ( lua_next(L, 1) != 0 )
		{
			if ( is_type_key(LUA_TSTRING, lua_type(L, -2))
				 && is_type_val(LUA_TSTRING, lua_type(L, -1)) )
			{
				const char * name = lua_tostring( L, -2 );
				const char * path = lua_tostring( L, -1 );
				package->bitmap_names.push_back( name );
				package->bitmap_paths.push_back( path );
				++package->bitmap_size;
			}
			lua_pop( L, 1 );
		}

		return 0;
	}

	int lua_read_shaders( lua_State* L )
	{
		if ( !is_argc(1, lua_gettop(L)) )
			return 0;

		auto package = static_cast< PackageData * >( lua_touserdata(L, lua_upvalueindex(1)) );

		lua_pushnil( L );
		while ( lua_next(L, 1) != 0 )
		{
			if ( is_type_key(LUA_TSTRING, lua_type(L, -2))
				 && is_type_val(LUA_TSTRING, lua_type(L, -1)) )
			{
				const char* name = lua_tostring( L, -2 );
				const char* path = lua_tostring( L, -1 );
				package->shader_names.push_back( name );
				package->shader_paths.push_back( path );
				++package->shader_size;
			}
			lua_pop( L, 1 );
		}

		return 0;
	}
}


RetroResource::PackageLoader_Lua::PackageLoader_Lua(
		HandleManager& handle_manager,
		PackageCollection& packages,
		MeshCollection& meshes,
		BitmapCollection& bitmaps,
		ShaderCollection & shaders )
	: _handle_manager( handle_manager )
	, _packages( packages )
	, _meshes( meshes )
	, _bitmaps( bitmaps )
	, _shaders( shaders )
	, _mesh_loader( MeshLoader(handle_manager, meshes) )
	, _bitmap_loader( BitmapLoader(handle_manager, bitmaps) )
	, _shader_loader( ShaderLoader(handle_manager, shaders) )
{

}

RetroResource::PackageLoader_Lua::~PackageLoader_Lua() { }

u32 RetroResource::PackageLoader_Lua::load( const std::string name, Handle & handle )
{
	load( &name, &handle );
	return 1;
}

u32 RetroResource::PackageLoader_Lua::load( const std::string * names, Handle * handles, const u32 size )
{
	u32 size0 = _packages.package.size();
	u32 size1 = size0;

	PackageData package_data;
	package_data.bitmap_size = 0;
	package_data.shader_size = 0;

	lua_State *L = luaL_newstate();
	luaL_openlibs( L );

	//lua_register( L, "hello_world", lua_hello_world );

	lua_pushlightuserdata( L, (void*)&package_data );
	lua_pushcclosure( L, &lua_read_bitmaps, 1 );
	lua_setglobal( L, "bitmaps" );

	lua_pushlightuserdata( L, (void*)&package_data );
	lua_pushcclosure( L, &lua_read_shaders, 1 );
	lua_setglobal( L, "shaders" );

	for ( u32 i = 0; i < size; ++i )
	{
		const std::string & name = names[ i ];
		std::vector< Handle > bitmap_handles;
		std::vector< Handle > shader_handles;

		printf( "[package loader] try load: '%s'\n", name.c_str() );

		u32 err = luaL_dofile( L, ( name + LUA_SUFFIX ).c_str() );
		if ( err )
		{
			printf( "[package loader] Error calling luaL_dofile()\n" );
			printf( "      %s\n\n", lua_tostring(L,-1) );
			continue;
		}

		bitmap_handles.resize( package_data.bitmap_size );
		shader_handles.resize( package_data.shader_size );
		_bitmap_loader.load( package_data.bitmap_names.data(), package_data.bitmap_paths.data(), bitmap_handles.data(), package_data.bitmap_size );
		_shader_loader.load( package_data.shader_names.data(), package_data.shader_paths.data(), shader_handles.data(), package_data.shader_size );

		package_data.bitmap_size = 0;
		package_data.shader_size = 0;
		package_data.bitmap_names.clear();
		package_data.bitmap_paths.clear();
		package_data.shader_names.clear();
		package_data.shader_paths.clear();

		Handle handle = _handle_manager.create();

		Package package;
		package.handle = handle;
		package.name = name;
		package.bitmaps = bitmap_handles;
		package.shaders = shader_handles;
		_packages.package.push_back( package );

		_packages.handle_lookup.insert( { handle.id, _packages.package.back() } );
		_packages.name_lookup.insert( { name, _packages.package.back() } );

		handles[ size1 - size0 ] = handle;

		++size1;

		std::cout << "[package loader] loaded: (" << handle.id << ") '" << name << ".lua'" << std::endl;
	}

	lua_close( L );
	L = nullptr;

	return size1 - size0;
}

void RetroResource::PackageLoader_Lua::unload( const Handle * handles, const u32 size )
{
	// TODO
}
