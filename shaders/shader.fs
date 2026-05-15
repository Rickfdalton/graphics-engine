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
    float outCutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;

    sampler2D flashLight;
};


uniform Material material;
uniform Light light;
uniform vec3 objectColor;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 light_space;

void main()
{
    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords))  ;

    //diffuse
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position-FragPos);
    float theta = dot(normalize(-light.direction), lightDir);

    //intensity
    float intensity = clamp((theta - light.outCutoff) / (light.cutoff - light.outCutoff), 0.0, 1.0);  

    vec3 projCoords = light_space.xyz / light_space.w;
    projCoords = projCoords * 0.5 + 0.5;

    vec3 cookie = vec3(1.0f);
    if(projCoords.x >= 0.0 && projCoords.x <= 1.0 &&
           projCoords.y >= 0.0 && projCoords.y <= 1.0 &&
           projCoords.z >= 0.0 && projCoords.z <= 1.0)
        {
            projCoords.y = 1-projCoords.y;
            cookie = vec3(texture(light.flashLight, projCoords.xy));
    }

    float cookieIntensity = dot(cookie, vec3(2.0,2.0,2.0));

    if(theta > light.outCutoff){
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

        diffuse  *= attenuation*intensity*cookieIntensity;
        specular *= attenuation*intensity*cookieIntensity;   

        FragColor = vec4 ( ambient+diffuse+specular,1.0) ;

    }else{
        FragColor = vec4 ( ambient,1.0) ;
    }
    

}

