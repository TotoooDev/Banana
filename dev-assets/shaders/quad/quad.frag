#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uTexture;
uniform float uFarPlane;
uniform float uNearPlane;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * uNearPlane * uFarPlane) / (uFarPlane + uNearPlane - z * (uFarPlane - uNearPlane));
}

void main()
{
    float depthValue = texture(uTexture, TexCoords).r;
    // FragColor = vec4(vec3(depthValue), 1.0);
    FragColor = vec4(1.0);
}