//#version 100
//precision mediump float;

uniform float timestamp;
uniform vec2 model_pos;

attribute vec4 vert;

varying vec2 f_uv;

void main ()
{
	float scale = 1.0 + 0.5 * sin( 2.0 * timestamp );

	gl_Position = vec4( model_pos.xy + scale * vert.xy, 0.0, 1.0 );
	f_uv = vert.xy;//uv;
	//f_color = vert.xyz;
}
