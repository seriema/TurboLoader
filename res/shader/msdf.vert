#version 120
//precision mediump float;

//uniform float timestamp;
uniform mat4 mvp;
uniform vec2 size;

attribute float char_index; // TODO Should be int or uint but that doesn't fly for some reason :(
attribute vec2 pos;

varying vec2 f_uv;
//varying vec4 f_color;

void main ()
{
	float scale = 1.0;//2.0 + sin( 2.0 * timestamp );

	gl_Position = mvp * vec4( scale * size*pos.xy, 0.0, 1.0 );
	f_uv = vec2( mod(int(char_index),16), int(char_index)/16 );
	//f_color = vec4(1);
}
