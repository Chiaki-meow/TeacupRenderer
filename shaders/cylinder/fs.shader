#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    vec3 specular;
//     sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// // texture sampler
// uniform sampler2D texture1;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 norm = normalize(Normals);
//     vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//     vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
//     FragColor = vec4(1, 0, 1, 1);
// 	FragColor = texture(material.diffuse, TexCoords);
}