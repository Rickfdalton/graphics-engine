#version 330 core
out vec4 FragColor;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 direction;
    float cutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
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

    vec3 lightDir = normalize(light.position-FragPos);
    float theta = dot(normalize(-light.direction), lightDir);

    if(theta > light.cutoff){
        float distance = length(light.position-FragPos);
        float attenuation = 1.0/(light.constant + light.linear * distance + light.quadratic*(distance*distance));
        float diff = max(dot(norm, lightDir),0.0);
        vec3 diffuse =vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;
        vec3 specMap = vec3(texture(material.specular, TexCoords));

        //specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir,norm); 
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 specular = specMap* spec * light.specular;  

        diffuse  *= attenuation;
        specular *= attenuation;   

        FragColor = vec4 ( ambient+diffuse+specular,1.0) ;

    }else{
        FragColor = vec4 ( ambient,1.0) ;
    }
    

}