#version 330 core

// Output
out vec4 FragColor;

// Input
in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
	vec4 FragPosLightSpace;
} Out;

// Struct and defines
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

struct Material
{
    bool UseColors;

    vec3 ColorDiffuse;
    vec3 ColorSpecular;

    sampler2D TextureDiffuse;
    sampler2D TextureSpecular;
};

struct Colors
{
    vec3 ColorAmbient;
    vec3 ColorDiffuse;
    vec3 ColorSpecular;
};

#define MAX_LIGHTS 64

// Uniforms
uniform Material uMaterial;
uniform Light uLights[MAX_LIGHTS];
uniform int uNumLights;
uniform vec3 uCameraPos;
uniform sampler2D uShadowMap;

// Function declarations
float ShadowCalculations(vec4 fragPosLightSpace, vec3 lightDir);
vec3 CalculateDirLight(Colors colors, Light light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(Colors colors, Light light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(Colors colors, Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

// Entrypoint
void main()
{
	vec3 result = vec3(0.0f);
	int usedLights = min(MAX_LIGHTS, uNumLights);
	vec3 norm = normalize(Out.Normal);
	vec3 viewDir = normalize(uCameraPos - Out.FragPos);

    Colors colors;
    if (uMaterial.UseColors)
    {
        colors.ColorDiffuse = uMaterial.ColorDiffuse;
        colors.ColorSpecular = uMaterial.ColorSpecular;
    }
    else
    {
        colors.ColorDiffuse = texture(uMaterial.TextureDiffuse, Out.TexCoords).xyz;
        colors.ColorSpecular = texture(uMaterial.TextureSpecular, Out.TexCoords).xyz;
    }

	for (int i = 0; i < usedLights; i++)
	{
		switch (uLights[i].Type)
		{
		case LIGHT_DIRECTIONNAL:
			result += CalculateDirLight(colors, uLights[i], norm, viewDir);
			break;

		case LIGHT_POINT:
			result += CalculatePointLight(colors, uLights[i], norm, Out.FragPos, viewDir);
			break;

		case LIGHT_SPOT:
			result += CalculateSpotLight(colors, uLights[i], norm, Out.FragPos, viewDir);
			break;
		}
	}

	// Gamma correction
	result = pow(result, vec3(1.0/2.2));

	FragColor = vec4(result, 1.0);
	// FragColor = vec4(uColor, 1.0);
}

// Function definitions
float ShadowCalculations(vec4 fragPosLightSpace, vec3 lightDir)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(uShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    return shadow;
}

vec3 CalculateDirLight(Colors colors, Light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.Direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // combine results
    vec3 ambient = light.Ambient * pow(colors.ColorDiffuse, vec3(2.2f));
    vec3 diffuse = light.Diffuse * diff * pow(colors.ColorDiffuse, vec3(2.2f));
    vec3 specular = light.Specular * spec * colors.ColorSpecular;
    // Shadow
    float shadow = ShadowCalculations(Out.FragPosLightSpace, light.Direction);
    return (ambient + (1.0 - shadow) * (diffuse + specular)) * colors.ColorDiffuse;
}

vec3 CalculatePointLight(Colors colors, Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // attenuation
    float distance = length(light.Pos - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance);    
    // combine results
    vec3 ambient = light.Ambient * pow(colors.ColorDiffuse, vec3(2.2f));
    vec3 diffuse = light.Diffuse * diff * pow(colors.ColorDiffuse, vec3(2.2f));
    vec3 specular = light.Specular * spec * colors.ColorSpecular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalculateSpotLight(Colors colors, Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // attenuation
    float distance = length(light.Pos - fragPos);
    float attenuation = 1.0 / (light.Constant + light.Linear * distance);    
    // spotlight intensity
    float theta = dot(lightDir, normalize(light.Direction)); 
    float epsilon = light.CutOff - light.OuterCutOff;
    float intensity = clamp((theta - light.OuterCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.Ambient * pow(colors.ColorDiffuse, vec3(2.2f));
    vec3 diffuse = light.Diffuse * diff * pow(colors.ColorDiffuse, vec3(2.2f));
    vec3 specular = light.Specular * spec * colors.ColorSpecular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}