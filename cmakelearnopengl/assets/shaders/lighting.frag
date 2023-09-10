#version 460 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

void main()
{
	// Ambient Lighting
	vec3 ambient = light.ambient * material.ambient;
	// ------
	
	// Diffuse Lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	// ------

	// Specular Lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	// ------

	// Result
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
};