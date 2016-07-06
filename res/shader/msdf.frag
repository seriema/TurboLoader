#version 120
//precision mediump float;

uniform sampler2D texture;

const vec4 bg_color = vec4( 0 ); //uniform vec4 bg_color;
const vec4 fg_color = vec4( 1 ); //uniform vec4 fg_color;
const vec4 outline_color = vec4(0, 0, 0, 1); //uniform vec4 outline_color;

varying vec2 f_uv;
//varying vec4 f_color; // TODO Replace fg_color with this!

float median( float r, float g, float b )
{
	return max(min(r, g), min(max(r, g), b));
}

float aa_step( float edge_threshold, float distance )
{
	float anti_alias = 0.7 * length( vec2(dFdx(distance), dFdy(distance)) );
	return smoothstep( edge_threshold - anti_alias, edge_threshold + anti_alias, distance );
}

void msdf( vec2 uv )
{
	const float edge_threshold = 0.5;

	vec3 c = texture2D( texture, uv ).rgb;
	float distance = median( c.r, c.g, c.b );
	float alpha = aa_step( edge_threshold, distance );
	gl_FragColor = vec4( fg_color.rgb, alpha );
}

void msdf_outline( vec2 uv )
{
	const float outline_threshold_min = 0.7;
	const float outline_threshold_max = 0.5;
	const float edge_threshold = 0.25;

	vec3 c = texture2D( texture, uv ).rgb;
	float distance = median( c.r, c.g, c.b );
	float outline_factor = smoothstep( outline_threshold_min, outline_threshold_max, distance );
	gl_FragColor = mix( fg_color, outline_color, outline_factor );
	gl_FragColor.a *= aa_step( edge_threshold, distance );
}

void main()
{
	//vec2 uv = gl_PointCoord;
	vec2 uv = (f_uv + vec2(gl_PointCoord.x, 1.0-gl_PointCoord.y)) * vec2( 0.0625, 0.125 );
	//vec2 uv = f_uv + gl_PointCoord / vec2(16.0, 8.0);

	//msdf( uv );
	msdf_outline( uv );
}
