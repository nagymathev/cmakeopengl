#version 460 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;

	sampler2D emission;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	// Ambient Lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	// ------
	
	// Diffuse Lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	// ------

	// Specular Lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(light.position - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	// ------

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

//	ambient  *= attenuation;
	diffuse  *= attenuation;
	specular *= attenuation;

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0, 1.0);

	diffuse *= intensity;
	specular *= intensity;

	vec3 result = ambient + diffuse + specular + (vec3(texture(material.emission, TexCoords)) * 0.5 * intensity * attenuation);
	FragColor = vec4(result, 1.0);

}