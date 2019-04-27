#version 330 core
out vec4 final_color;

uniform vec2 resolution;
uniform float time;
uniform vec3 circleColor;
uniform vec2 center;

vec3 colorA = vec3(1,0,0);
vec3 colorB = vec3(0,1,0);

void main()
{    
	vec2 pos = center;
	pos.y = resolution.y - pos.y;
	pos.x *= resolution.x/resolution.y;

	pos = pos/resolution.xy;
	vec2 st = gl_FragCoord.xy/resolution.xy;
	st.x *= resolution.x/resolution.y;


    vec3 color = circleColor;

	//color.r = abs(sin(distance(pos,st)*400+time*20));
	//color.g = abs(sin(distance(pos,st)*50+time));

	float pct = abs(sin(distance(pos,st)*400+time*20));

	color = mix(colorA, colorB, pct);

    final_color = vec4(color,1.0);
}  