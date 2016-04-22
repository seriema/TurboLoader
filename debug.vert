R"(
//#version 100
//precision mediump float;

attribute vec4 pos;
varying vec3 f_color;

void main ()
{
	gl_Position = vec4( pos.xyz, 1.0 );
	f_color = pos.xyz;
}
)"
