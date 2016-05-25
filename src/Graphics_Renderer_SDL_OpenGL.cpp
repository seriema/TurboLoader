#include "Graphics_Renderer_SDL_OpenGL.h"

RetroGraphics::Renderer_SDL_OpenGL::Renderer_SDL_OpenGL( RetroResource::BitmapCollection * bitmaps, TextureManager_OpenGL * texture_manager ) // TODO RendererSettings settings )
	: _bitmaps( bitmaps )
	, _texture_manager( texture_manager )
	, _render_count( 0 )
	, _TEMP_shader( nullptr )
{
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );

	// fragment blending
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	// depth
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glDepthMask( GL_TRUE );

	glClearColor( 1.0f, 0.1f, 1.0f, 1.0f );

	// TODO remove temp debug shader when the time comes.
	_TEMP_shader = Shader::create( "debug" );
	_render_material[ 0 ].shader_handle = _TEMP_shader->m_program; // TODO ugly.
}

RetroGraphics::Renderer_SDL_OpenGL::~Renderer_SDL_OpenGL()
{
	delete _TEMP_shader;
}

std::string RetroGraphics::Renderer_SDL_OpenGL::calc_description() const
{
	std::ostringstream description;
	description << "OpenGL env" << std::endl;
	description << "   Version: " << glGetString( GL_VERSION ) << std::endl;
	description << "    Vendor: " << glGetString( GL_VENDOR ) << std::endl;
	description << "  Renderer: " << glGetString( GL_RENDERER ) << std::endl;
	description << "   Shading: " << glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;
	return description.str();
}

GLuint RetroGraphics::Renderer_SDL_OpenGL::add_mesh( const GLfloat* vertices, int n_vertices )
{
	GLuint vbo_handle;

	//GLuint vao_handle;
	//glGenVertexArrays( 1, &vao_handle );
	//glBindVertexArray( vao_handle );

	glGenBuffers( 1, &vbo_handle );
	glBindBuffer( GL_ARRAY_BUFFER, vbo_handle );
	glBufferData( GL_ARRAY_BUFFER, n_vertices*sizeof(GL_FLOAT), vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( (*_TEMP_shader)["vert"] );

	return vbo_handle;
}

bool RetroGraphics::Renderer_SDL_OpenGL::del_mesh( GLuint vbo_handle )
{
	glDeleteBuffers( 1, &vbo_handle );
	return true; // TODO Make sure delete went ok.
}

void RetroGraphics::Renderer_SDL_OpenGL::draw( const RenderKey& render_key, const RenderData& render_data )
{
	_render_key[ _render_count ] = render_key;
	_render_key[ _render_count ].RenderCommon.data_index = _render_count;
	_render_data[ _render_count ] = render_data;
	++_render_count;
}

void RetroGraphics::Renderer_SDL_OpenGL::render()
{
	std::sort( _render_key, _render_key + _render_count, Aux::CompareRenderKey );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for ( int i = 0; i < _render_count; ++i )
	{
		int data_i = _render_key[ i ].RenderCommon.data_index;
		// TODO get shader from _render_key[ i ].material_index.
		// TODO bind shader if not same as current.
		// TODO get object matrix from _render_data[ data_i ]; and bind to shader.

		_TEMP_shader->bind();

		glUniform2fv( (*_TEMP_shader)["model_pos"], 1, _render_data[ data_i ].pos );

		GLuint vbo_handle = _render_data[ data_i ].vbo_handle;
		glBindBuffer( GL_ARRAY_BUFFER, vbo_handle );
		glVertexAttribPointer( (*_TEMP_shader)["vert"], 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0 );


		// TODO bind relevant texture here.
		// TODO only activate and rebind if neccessary.
		int texture_i = 0;
		glActiveTexture( GL_TEXTURE0 + texture_i );
		glUniform1i( (*_TEMP_shader)["texture"], texture_i );

		u32 bitmap_i = _bitmaps->name_index.at( "img_test_a" );
		RetroResource::Handle handle = _bitmaps->handle[ bitmap_i ];
		_texture_manager->bind( handle );



		glDrawArrays( GL_TRIANGLES, 0, 3 );
	}

	//glFlush();

	_render_count = 0;
}
