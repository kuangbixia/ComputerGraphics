#version 400 core
out vec4 FragColor;

#define POINTLIGHT 4

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    int diffuseId;
    int specularId;
    int ambientId;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
    sampler2D ambientTexture;
};

struct ParallelLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};



in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  

uniform vec3 viewPos;
uniform Material material;

uniform ParallelLight parallelLight;
uniform PointLight pointLight[POINTLIGHT];
uniform SpotLight spotLight;

vec3 doParallelLight(ParallelLight light,vec3 norm,vec3 fragPos,vec3 viewDir);
vec3 doPointLight(PointLight light,vec3 norm,vec3 fragPos,vec3 viewDir);
vec3 doSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir);


void main(){    
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result=vec3(0.0,0.0,0.0);
    for(int i=0;i<POINTLIGHT;i++){
        result+=doPointLight(pointLight[i],norm,FragPos,viewDir);
    }
    result+=doSpotLight(spotLight,norm,FragPos,viewDir);
    result+=doParallelLight(parallelLight,norm,FragPos,viewDir);
    FragColor = vec4(result,1.0);
}

vec3 doParallelLight(ParallelLight light,vec3 norm,vec3 fragPos,vec3 viewDir){
    // 环境
    vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb;
    // 漫反射
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoords).rgb;
    return (ambient + diffuse + specular);
}


vec3 doPointLight(PointLight light,vec3 norm,vec3 fragPos,vec3 viewDir){
    // 环境光照
    vec3 ambient = light.ambient *texture(material.ambientTexture, TexCoords).rgb* material.ambient;
    // 漫反射 
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff *texture(material.diffuseTexture, TexCoords).rgb* material.diffuse);
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular;
    if(material.specularId !=0)
        specular = light.specular * (spec * texture(material.specularTexture, TexCoords).rgb * material.specular);
    else
        specular = light.specular * spec * material.specular; 
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 
    vec3 result = ambient + diffuse + specular;
    result*=attenuation;
    return result;
}


vec3 doSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射
    float diff = max(dot(norm, lightDir), 0.0);
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // 聚光强度
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoords).rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}