#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
    // FragColor = vec4(vec3(1.0f - texture(screenTexture, TexCoords)),1.0f); // negate
    FragColor = texture(screenTexture, TexCoords);
    float average = (FragColor.r + FragColor.g + FragColor.b)/3.0f;
    FragColor = vec4(average, average, average, 1.0f);
}