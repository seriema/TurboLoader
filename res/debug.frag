//#version 100
//precision mediump float;

uniform sampler2D texture;

varying vec2 f_uv;
varying vec4 f_color;

void main ()
{
	gl_FragColor = f_color * texture2D( texture, f_uv );
}
