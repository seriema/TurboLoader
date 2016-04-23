#ifndef A_RETRO_UI_IRENDERER_H
#define A_RETRO_UI_IRENDERER_H

#include <cstdint>
#include <algorithm>
#include <utility>

#include "platform.h"
#include "shader.h"

#define u32 std::uint32_t

#define RENDER_INPUT_BITS_DATA_INDEX 13
#define RENDER_INPUT_BITS_DEPTH 8

#define RENDER_INPUT_ARRAY_SIZE 1 << RENDER_INPUT_BITS_DATA_INDEX

#define RENDER_INPUT_DATA_INDEX        u32 data_index        : RENDER_INPUT_BITS_DATA_INDEX;
#define RENDER_INPUT_MATERIAL_INDEX    u32 material_index    : 8;
#define RENDER_INPUT_DEPTH             u32 depth             : RENDER_INPUT_BITS_DEPTH;
#define RENDER_INPUT_IS_COMMAND        u32 is_command        : 1;
#define RENDER_INPUT_TRANSLUCENCY_TYPE u32 translucency_type : 2;



/* TODO
 * Add a factory for render keys?
 * include glm?
 * Before including glm lets make sure we can draw several tris!
 *
 */

union RenderKey
{
	u32 raw;

	struct
	{
		RENDER_INPUT_DATA_INDEX
		u32 padding : 16;
		RENDER_INPUT_IS_COMMAND
		RENDER_INPUT_TRANSLUCENCY_TYPE
	} RenderCommon;

	struct
	{
		RENDER_INPUT_DATA_INDEX
		RENDER_INPUT_DEPTH
		RENDER_INPUT_MATERIAL_INDEX
		RENDER_INPUT_IS_COMMAND
		RENDER_INPUT_TRANSLUCENCY_TYPE
	} RenderOpaque;

	struct
	{
		RENDER_INPUT_DATA_INDEX
		RENDER_INPUT_MATERIAL_INDEX
		RENDER_INPUT_DEPTH
		RENDER_INPUT_IS_COMMAND
		RENDER_INPUT_TRANSLUCENCY_TYPE
	} RenderTranslucent;

	struct
	{
		RENDER_INPUT_DATA_INDEX
		u32 command : 10;
		u32 sequence : 6;
		RENDER_INPUT_IS_COMMAND
		RENDER_INPUT_TRANSLUCENCY_TYPE
	} RenderCommand;
};

struct RenderData
{
	u32 vbo_handle; // vbo contains attributes such as vert,norm,color, these must then also exist in shader.
	//glm::mat4 model_transform;
	float x;
	float y;
};

struct RenderMaterial
{
	u32 shader_handle;

	//uniforms for shader:
	// u32 texture_handle;
	// glm::vec4 color;
};

bool CompareRenderKey ( RenderKey a, RenderKey b )
{
	return a.raw < b.raw;
}

class IRenderer
{
public:
	virtual ~IRenderer() {}

	virtual u_int32_t add_mesh( const float* vertices, int n_vertices ) = 0;
	virtual bool del_mesh( u_int32_t vbo_handle ) = 0;

	virtual void draw( const RenderKey& render_key, const RenderData& render_data ) = 0;
	virtual void render() = 0;
};

struct RendererSettings
{
};

class RendererOpenGL : public IRenderer
{
public:
	RendererOpenGL() // TODO RendererSettings settings )
		: _render_count( 0 )
		, _TEMPshader( NULL )
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

		_TEMPshader = Shader::create( "debug" );

		_render_material[ 0 ].shader_handle = _TEMPshader->m_program; // TODO ugly.
	}

	virtual ~RendererOpenGL() override
	{
		delete _TEMPshader;
	}

	virtual GLuint add_mesh( const GLfloat* vertices, int n_vertices ) override
	{
		GLuint vbo_handle;

		//GLuint vao_handle;
		//glGenVertexArrays( 1, &vao_handle );
		//glBindVertexArray( vao_handle );

		glGenBuffers( 1, &vbo_handle );
		glBindBuffer( GL_ARRAY_BUFFER, vbo_handle );
		glBufferData( GL_ARRAY_BUFFER, n_vertices*sizeof(GL_FLOAT), vertices, GL_STATIC_DRAW );

		glEnableVertexAttribArray( (*_TEMPshader)["pos"] );

		// TODO remove these?
//		glBindBuffer( GL_ARRAY_BUFFER, vbo_handle );
//		glVertexAttribPointer( (*_TEMPshader)["pos"], 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0 );

		return vbo_handle;
	}

	virtual bool del_mesh( GLuint vbo_handle ) override
	{
		glDeleteBuffers( 1, &vbo_handle );
		return true; // TODO Make sure delete went ok.
	}

	virtual void draw( const RenderKey& render_key, const RenderData& render_data ) override
	{
		_render_key[ _render_count ] = render_key;
		_render_key[ _render_count ].RenderCommon.data_index = _render_count;
		_render_data[ _render_count ] = render_data;
		++_render_count;
	}

	virtual void render() override
	{
		std::sort( _render_key, _render_key + _render_count, CompareRenderKey );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		for ( int i = 0; i < _render_count; ++i )
		{
			int data_i = _render_key[ i ].RenderCommon.data_index;
			// TODO get shader from _render_key[ i ].material_index.
			// TODO bind shader if not same as current.
			// TODO get object matrix from _render_data[ data_i ]; and bind to shader.



			_TEMPshader->bind(); // TODO only bind if not same as current.
			// TODO change uniforms if needed.

			GLuint vbo_handle = _render_data[ data_i ].vbo_handle;
//			glEnableVertexAttribArray( (*_TEMPshader)["pos"] ); // TODO remove this?
			glBindBuffer( GL_ARRAY_BUFFER, vbo_handle );
			glVertexAttribPointer( (*_TEMPshader)["pos"], 3, GL_FLOAT, GL_FALSE, 3*sizeof(GL_FLOAT), 0 );

			glDrawArrays( GL_TRIANGLES, 0, 3 );
		}

		//glFlush();

		_render_count = 0;
	}
private:
	RenderMaterial  _render_material[ 1 ];

	RenderKey       _render_key[ RENDER_INPUT_ARRAY_SIZE ];
	RenderData      _render_data[ RENDER_INPUT_ARRAY_SIZE ];
	int             _render_count;

	Shader*         _TEMPshader;
};

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

void shutdown_sdl_gl()
{
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

std::pair< SDL_GLattr, int > GL_ATTRIBUTES[] =
{
	{ SDL_GL_RED_SIZE,     8 },
	{ SDL_GL_GREEN_SIZE,   8 },
	{ SDL_GL_BLUE_SIZE,    8 },
	{ SDL_GL_ALPHA_SIZE,   8 },
	{ SDL_GL_DEPTH_SIZE,   8 },
	{ SDL_GL_DOUBLEBUFFER, 1 },

#if defined(__APPLE__) || defined(_WIN32)
	//{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE }
	{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_COMPATIBILITY },
	{ SDL_GL_CONTEXT_MAJOR_VERSION, 2 },
	{ SDL_GL_CONTEXT_MINOR_VERSION, 1 },
#else // RPI
	{ SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_ES },
	{ SDL_GL_CONTEXT_MAJOR_VERSION, 2 },
	{ SDL_GL_CONTEXT_MINOR_VERSION, 0 },
#endif
};

int init_sdl_gl()
{
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//hide mouse cursor early
//	SDL_ShowCursor(0);

	for ( auto attr : GL_ATTRIBUTES )
	{
		if ( SDL_GL_SetAttribute( attr.first, attr.second ) != 0 )
		{
			printf( "SDL_GL_SetAttribute(%d, %d) failed: %s\n", attr.first, attr.second, SDL_GetError() );
			return false;
		}
	}

	SDL_DisplayMode dispMode;
	//SDL_GetDesktopDisplayMode(0, &dispMode);
	SDL_GetCurrentDisplayMode(0, &dispMode);

	printf("Screen size: %d %d\n", dispMode.w, dispMode.h);

	gWindow = SDL_CreateWindow(
		"SDL Tutorial",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
#if defined(__APPLE__) || defined(_WIN32)
		960, 480,
		SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS
#else
		dispMode.w, dispMode.h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN
#endif
	);
	if( gWindow == NULL )
	{
		printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	//Create context
	gContext = SDL_GL_CreateContext( gWindow );
	if( gContext == NULL )
	{
		printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
		return false;
	}

	SDL_GL_MakeCurrent( gWindow, gContext );

	gladLoadGLLoader( SDL_GL_GetProcAddress );

	//Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 )
	{
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	return true;
}

#endif //A_RETRO_UI_IRENDERER_H
