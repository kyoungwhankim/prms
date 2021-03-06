#version 330

in vec4 Color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpaceFragPos;

out vec4 color;

#define MAX_POINT_LIGHTS    3
#define MAX_SPOT_LIGHTS     5
#define DIR_PCF_INTENSITY   4.0f

struct Light
{
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight pointLight;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

struct OmniShadowMap
{
    samplerCube shadowMap;
    float nearPlane;
    float farPlane;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform int spotLightCount;
uniform int pointLightCount;

uniform sampler2D theTexture;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float Scale(float value, float original_max, float original_min, float target_max, float target_min)
{
    return target_min + ((value - original_min) * ((target_max - target_min) / (original_max - original_min)));
}

vec3 ScaleVec3(vec3 value, float original_max, float original_min, float target_max, float target_min)
{
    value.x = Scale(value.x, original_max, original_min, target_max, target_min);
    value.y = Scale(value.y, original_max, original_min, target_max, target_min);
    value.z = Scale(value.z, original_max, original_min, target_max, target_min);
    return value;
}

float CalculateDirectionalShadowFactor(DirectionalLight light)
{
    vec3 projCoords = DirectionalLightSpaceFragPos.xyz / DirectionalLightSpaceFragPos.w;
    projCoords = ScaleVec3(projCoords, 1, -1, 1, 0);

    float closestDepth = texture(directionalShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.direction);
    float bias = max(0.005 * (1 - dot(normal, lightDirection)), 0.005);

    float pcfIntensity = DIR_PCF_INTENSITY;
    float shadowFactor = 0.0f;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for (float x = -pcfIntensity; x <= pcfIntensity; ++x)
    {
        for (float y = -pcfIntensity; y <= pcfIntensity; ++y)
        {
            float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadowFactor += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;
        }
    }
    float pcfCount = pow(pcfIntensity * 2 + 1, 2);
    shadowFactor /= pcfCount;
    
    if (currentDepth > 1.0f)
        shadowFactor = 0.0f;

    return shadowFactor;
}

float CalculateOmniShadowFactor(PointLight light, int shadowIndex)
{
    vec3 lightToFrag = FragPos - light.position;
    float currentDepth = length(lightToFrag) - omniShadowMaps[shadowIndex].nearPlane;

    float shadowFactor = 0.0f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightToFrag);
    float bias = max(0.05f * (1 - dot(normal, lightDirection)), 0.05f);
    float viewDistance = length(eyePosition - FragPos);
    float diskRadius = Scale(viewDistance, omniShadowMaps[shadowIndex].farPlane, 0, 0.005f, 0.01f);

    for (int i = 0; i < 20; i++)
    {
        float closestDepth = texture(omniShadowMaps[shadowIndex].shadowMap, lightToFrag + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= omniShadowMaps[shadowIndex].farPlane - omniShadowMaps[shadowIndex].nearPlane;
        if (currentDepth - bias > closestDepth)
            shadowFactor += 1.0f;
    }

    shadowFactor /= 20.0f;

    return shadowFactor;
}

vec4 CalculateLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), -normalize(direction)), 0);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0, 0, 0, 0);
    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedLight = normalize(reflect(direction, normalize(Normal)));
        float specularFactor = dot(fragToEye, reflectedLight);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color, 1.0f) * material.specularIntensity * specularFactor;
        }
    }

    return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalculateDirectionalLight()
{
    float shadowFactor = CalculateDirectionalShadowFactor(directionalLight);
    return CalculateLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalculatePointLight(PointLight pointLight, int shadowIndex)
{
    vec3 direction = FragPos - pointLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalculateOmniShadowFactor(pointLight, shadowIndex);

    vec4 color = CalculateLightByDirection(pointLight.base, direction, shadowFactor);
    float attenuation = pointLight.exponent * pow(distance, 2.0) +
                        pointLight.linear * distance +
                        pointLight.constant;
    return color / attenuation;
}

vec4 CalculateSpotLight(SpotLight spotLight, int shadowIndex)
{
    vec3 spotLightToFrag = normalize(FragPos - spotLight.pointLight.position);
    float spotLightFactor = dot(spotLightToFrag, spotLight.direction);

    if (spotLightFactor > spotLight.edge)
    {
        vec4 color = CalculatePointLight(spotLight.pointLight, shadowIndex);
        return color * Scale(spotLightFactor, 1, spotLight.edge, 1, 0);
    }
    else
        return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

vec4 CalculatePointLights()
{
    vec4 pointLightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < pointLightCount; ++i)
    {
        pointLightColor += CalculatePointLight(pointLights[i], i);
    }
    return pointLightColor;
}

vec4 CalculateSpotLights()
{
    vec4 spotLightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < spotLightCount; ++i)
    {
        spotLightColor += CalculateSpotLight(spotLights[i], pointLightCount + i);
    }
    return spotLightColor;
}

void main()
{
    vec4 lightColor = CalculateDirectionalLight() + CalculatePointLights() + CalculateSpotLights();

    color = Color * texture(theTexture, TexCoord) * lightColor;
}
