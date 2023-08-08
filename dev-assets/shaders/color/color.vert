#version 330 core

// Input
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Output
out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
} Out;

// Uniforms
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	Out.FragPos = vec3(uModel * vec4(aPos, 1.0));
	Out.Normal = mat3(transpose(inverse(uModel))) * aNormal; // The inverse function is costly for th GPU apparently

	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
}