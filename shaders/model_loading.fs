#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{    
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 ambient = 0.1* color;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color;

     // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specularTex = texture(texture_specular1, TexCoords).rgb;
    vec3 specular = spec * specularTex;
    vec3 result = (ambient + diffuse + specular) * lightColor;

    FragColor = vec4(result, 1.0);
}