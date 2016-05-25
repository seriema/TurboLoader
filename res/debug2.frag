R"(
//#version 100
//precision mediump float;

uniform sampler2D texture;

varying vec2 f_uv;
varying vec3 f_color;

void main ()
{
	//gl_FragColor = vec4( f_color + 0.5, 1.0 );
	gl_FragColor = texture2D( texture, f_uv + vec2(0.55) );
}
)"
