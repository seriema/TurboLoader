R"(
//#version 100
//precision mediump float;

uniform vec2 model_pos;
attribute vec4 vert;
varying vec3 f_color;

void main ()
{
	gl_Position = vec4( model_pos.xy + vert.xy, 0.0, 1.0 );
	f_color = vert.xyz;
}
)"
