#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;
uniform float nearPlane;

void main()
{
    float distance = length(FragPos.xyz - lightPos) - nearPlane;
    distance /= farPlane - nearPlane;
    gl_FragDepth = distance;
}