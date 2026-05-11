#version 330 core
out vec4 FragColor;

uniform vec3 light_source_col;

void main()
{
    FragColor = vec4(light_source_col,1.0); 
}