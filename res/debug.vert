//#version 100
//precision mediump float;

uniform mat4 mvp;
uniform vec2 size;

attribute vec3 pos;
attribute vec2 uv;

varying vec2 f_uv;
varying vec4 f_color;

void main ()
{
	gl_Position = mvp * vec4( size*pos.xy, pos.z, 1.0 );
	f_uv = uv;
	f_color = vec4(1);
}
