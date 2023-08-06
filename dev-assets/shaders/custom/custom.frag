#version 330 core

// Output
out vec4 FragColor;

// Input
in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
} Out;

// Struct and defines
struct Material
{
	sampler2D Diffuse;
	sampler2D Specular;
	sampler2D Normal;
	float Shininess;
};

#define LIGHT_DIRECTIONNAL 0
#define LIGHT_POINT 1
#define LIGHT_SPOT 2

struct Light
{
	int Type;

	vec3 Pos;
	vec3 Direction;
	
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;

	float Constant;
	float Linear;
	float Quadratic;

	float CutOff;
	float OuterCutOff;
};

#define MAX_LIGHTS 64

// Uniforms
uniform Material uMaterial;
uniform Light uLights[MAX_LIGHTS];
uniform int uNumLights;
uniform vec3 uCameraPos;

// Function declarations
vec3 CalculateDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Entrypoint
void main()
{
	vec3 result = vec3(0.0f);
	int usedLights = min(MAX_LIGHTS, uNumLights);
	vec3 norm = normalize(Out.Normal);
	vec3 viewDir = normalize(uCameraPos - Out.FragPos);

	for (int i = 0; i < usedLights; i++)
	{
		switch (uLights[i].Type)
		{
		case LIGHT_DIRECTIONNAL:
			result += CalculateDirLight(uLights[i], norm, viewDir);
			break;

		case LIGHT_POINT:
			result += CalculatePointLight(uLights[i], norm, Out.FragPos, viewDir);
			break;

		case LIGHT_SPOT:
			result += CalculateSpotLight(uLights[i], norm, Out.FragPos, viewDir);
			break;
		}
	}

	// Gamma correction
	result = pow(result, vec3(1.0/2.2));

	// FragColor = vec4(result, 1.0f);
	FragColor = vec4(1.0f);
}

// Function definitions
vec3 CalculateDirLight(Light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.Direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.Shininess);
    // combine results
    vec3 ambient = light.Ambient * pow(texture(uMaterial.Diffuse, Out.TexCoords).rgb, vec3(2.2f));
    vec3 diffuse = light.Diffuse * diff * pow(texture(uMaterial.Diffuse, Out.TexCoords).rgb, vec3(2.2f));
    vec3 specular = light.Specular * spec * texture(uMaterial.Specular, Out.TexCoords).rgb;
    return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.Shininess);
    // attenuation
    float distance = length(light.Pos - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance);    
    // combine results
    vec3 ambient = light.Ambient * pow(texture(uMaterial.Diffuse, Out.TexCoords).rgb, vec3(2.2f));
    vec3 diffuse = light.Diffuse * diff * pow(texture(uMaterial.Diffuse, Out.TexCoords).rgb, vec3(2.2f));
    vec3 specular = light.Specular * spec * texture(uMaterial.Specular, Out.TexCoords).rgb;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.Shininess);
    // attenuation
    float distance = length(light.Pos - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance);    
    // spotlight intensity
    float theta = dot(lightDir, normalize(light.Direction)); 
    float epsilon = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.Ambient * pow(texture(uMaterial.Diffuse, Out.TexCoords).rgb, vec3(2.2f));
    vec3 diffuse = light.Diffuse * diff * pow(texture(uMaterial.Diffuse, Out.TexCoords).rgb, vec3(2.2f));
    vec3 specular = light.Specular * spec * texture(uMaterial.Specular, Out.TexCoords).rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}