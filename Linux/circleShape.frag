#version 330 core
out vec4 final_color;

uniform vec2 resolution;
uniform float time;
uniform vec3 circleColor;

void main()
{    
	vec2 st = gl_FragCoord.xy/resolution.xy;
    st.x *= resolution.x/resolution.y;

    vec3 color = circleColor;

    // Cell positions
    vec2 point[5];
    point[0] = vec2(0.83,0.75);
    point[1] = vec2(0.60,0.07);
    point[2] = vec2(0.28,0.64);
    point[3] = vec2(0.31,0.26);
    point[4] = vec2(abs(sin(time)),abs(cos(time)));

    float m_dist = 1.;  // minimun distance

    // Iterate through the points positions
    for (int i = 0; i < 5; i++) {
        float dist = distance(st, point[i]);

        // Keep the closer distance
        m_dist = min(m_dist, dist);
    }

    // Draw the min distance (distance field)
    color += m_dist;

    // Show isolines
    // color -= step(.7,abs(sin(50.0*m_dist)))*.3;

    final_color = vec4(color,1.0);
}  