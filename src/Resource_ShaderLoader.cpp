#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

#include "Resource_ShaderLoader.h"

namespace
{
	const std::string VERT_SHADER_SUFFIX = ".vert";
	const std::string FRAG_SHADER_SUFFIX = ".frag";

	inline bool read_file( const std::string path, char *& data )
	{
		std::ifstream file( path );

		if ( !file.is_open() )
		{
			std::cout << "File not found or could not be opened: '" << path << "'." << std::endl;
			return false;
		}

		std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		data = new char[ str.length() + 1 ];
		std::strcpy( data, str.c_str() );
		return true;
	}
}

RetroResource::ShaderLoader::ShaderLoader( HandleManager & handle_manager, ShaderCollection & shaders )
	: _handle_manager( handle_manager )
	, _shaders( shaders )
{}

u32 RetroResource::ShaderLoader::load( const std::string * names, const std::string * paths, Handle * handles, const u32 size )
{
	u32 size0 = _shaders.handle.size();
	u32 size1 = size0;

	_shaders.handle.reserve( size );
	_shaders.name.reserve( size );
	_shaders.path.reserve( size );
	_shaders.vert.reserve( size );
	_shaders.frag.reserve( size );

	for ( int i = 0; i < size; ++i )
	{
		const std::string & name = names[ i ];
		const std::string & path = paths[ i ];
		char * vert;
		char * frag;

		if ( !read_file( path + VERT_SHADER_SUFFIX, vert ) )
			continue;

		if ( !read_file( path + FRAG_SHADER_SUFFIX, frag ) )
			continue;

		Handle handle = _handle_manager.create();

		_shaders.handle_index.insert( { handle.id, size1 } );
		_shaders.name_index.insert( { name, size1 } );

		_shaders.handle.push_back( handle );
		_shaders.name.push_back( name );
		_shaders.path.push_back( path );
		_shaders.vert.push_back( vert );
		_shaders.frag.push_back( frag );

		handles[ size1 - size0 ] = handle;

		++size1;

		std::cout << "[shader loader] loaded: (" << handle.id << ") '" << path << ".vert' & '" << path << ".frag'" << std::endl;
	}

	return size1 - size0;
}

void RetroResource::ShaderLoader::unload( const Handle * handles, const u32 size )
{
	u32 i_last = _shaders.handle.size() - 1;

	for ( int j = 0; j < size; ++j )
	{
		Handle handle = handles[ j ];
		u32 i = _shaders.handle_index.at( handle.id );

		std::string & name = _shaders.name[ i ];
//		const char * vert = static_cast< const char * >( _shaders.vert[ i ] );
//		const char * frag = static_cast< const char * >( _shaders.frag[ i ] );

		Handle handle_last = _shaders.handle[ i_last ];
		std::string & name_last = _shaders.name[ i_last ];

		_shaders.handle_index[ handle_last.id ] = i;
		_shaders.name_index[ name_last ] = i;

		_shaders.handle_index.erase( handle.id );
		_shaders.name_index.erase( name );

		_shaders.handle[ i ] = _shaders.handle[ i_last ]; _shaders.handle.pop_back();
		_shaders.name[ i ] = _shaders.name[ i_last ]; _shaders.name.pop_back();
		_shaders.path[ i ] = _shaders.path[ i_last ]; _shaders.path.pop_back();
		delete[] _shaders.vert[ i ]; _shaders.vert[ i ] = _shaders.vert[ i_last ]; _shaders.vert.pop_back();
		delete[] _shaders.frag[ i ]; _shaders.frag[ i ] = _shaders.frag[ i_last ]; _shaders.frag.pop_back();

		--i_last;

		_handle_manager.destroy( handle );
	}
}
