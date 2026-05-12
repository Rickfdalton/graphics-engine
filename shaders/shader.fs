#version 330 core
out vec4 FragColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

// Directional Light
struct Light{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 viewPos;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords))  ;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse =vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;
    vec3 specMap = vec3(texture(material.specular, TexCoords));

    //specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = specMap* spec * light.specular;  

    FragColor = vec4 ( ambient+diffuse+specular,1.0) ;
}