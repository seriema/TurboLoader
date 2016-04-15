R"(
//#version 100
//precision mediump float;

varying vec3 f_color;

void main ()
{
	gl_FragColor = vec4( f_color + 0.5, 1.0 );
}
)"
