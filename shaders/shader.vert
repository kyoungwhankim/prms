#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 nor;

out vec4 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpaceFragPos;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 directionalLightSpaceTransform;
uniform vec3 color;

void main()
{
    gl_Position = proj * view * model * vec4(pos, 1.0);
    DirectionalLightSpaceFragPos = directionalLightSpaceTransform * model * vec4(pos, 1.0);
    
    Color = vec4(color, 1.0f);
    TexCoord = tex;

    Normal = mat3(transpose(inverse(model))) * nor;
    FragPos = (model * vec4(pos, 1.0)).xyz;
}
