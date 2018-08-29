#version 330 core
out vec4 final_color;
uniform vec3 circleColor;

void main()
{    
    final_color = vec4(circleColor,1.0);
}  