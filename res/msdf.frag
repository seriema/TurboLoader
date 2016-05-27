//#version 100
//precision mediump float;

uniform sampler2D texture;

const vec4 bg_color = vec4( 0 ); //uniform vec4 bg_color;
const vec4 fg_color = vec4( 1 ); //uniform vec4 fg_color;

varying vec2 f_uv;

float median( float r, float g, float b )
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec3 c = texture2D( texture, f_uv ).rgb;
	float signed_distance = median( c.r, c.g, c.b ) - 0.5;
	float opacity = clamp( signed_distance / fwidth( signed_distance ) + 0.5, 0.0, 1.0 );
	gl_FragColor = mix( bg_color, fg_color, opacity );
}
