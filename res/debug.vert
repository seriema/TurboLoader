//#version 100
//precision mediump float;

uniform vec2 model_pos;

//attribute vec2 uv;
attribute vec4 vert;

varying vec2 f_uv;
varying vec3 f_color;

void main ()
{
	gl_Position = vec4( model_pos.xy + 0.5 * vert.xy, 0.0, 1.0 );
	f_uv = vert.xy;//uv;
	f_color = vert.xyz;
}
