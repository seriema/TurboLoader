R"(
//#version 100
//precision mediump float;

attribute vec4 pos;

void main ()
{
	gl_Position = vec4( pos.xy, 0.5, 1.0 );
}
)"
