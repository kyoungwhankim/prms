#include "Shader.h"

Shader::Shader()
{
	shaderID                                = 0;
	uniformProj                             = 0;
	uniformModel                            = 0;
    uniformView                             = 0;
    uniformShininess                        = 0;
    uniformSpecularIntensity                = 0;
    uniformEyePosition                      = 0;
    uniformPointLightCount                  = 0;
    pointLightCount                         = 0;
    spotLightCount                          = 0;
    uniformSpotLightCount                   = 0;
    uniformTexture                          = 0;
    uniformDirectionalLightSpaceTransform   = 0;
    uniformDirectionalShadowMap             = 0;
    uniformOmniLightPos                     = 0;
    uniformNearPlane                        = 0;
    uniformFarPlane                         = 0;
    uniformColor                            = 0;
    memset(uniformLightMatrices, 0, sizeof(uniformLightMatrices));
}

void Shader::CreateFromString(const char* vsCode, const char* fsCode)
{
	CompileShader(vsCode, fsCode);
}

void Shader::CreateFromFiles(const char* vsPath, const char* fsPath)
{
    std::string vsFile = ReadFile(vsPath);
    std::string fsFile = ReadFile(fsPath);
    const char* vsCode = vsFile.c_str();
    const char* fsCode = fsFile.c_str();

    CompileShader(vsCode, fsCode);
}

void Shader::CreateFromFiles(const char* vsPath, const char* gsPath, const char* fsPath)
{
    std::string vsFile = ReadFile(vsPath);
    std::string gsFile = ReadFile(gsPath);
    std::string fsFile = ReadFile(fsPath);
    const char* vsCode = vsFile.c_str();
    const char* gsCode = gsFile.c_str();
    const char* fsCode = fsFile.c_str();

    CompileShader(vsCode, gsCode, fsCode);
}

std::string	Shader::ReadFile(const char* filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    while (!fileStream.is_open())
    {
        printf("Failed to read %s file! The file doesn't exist.", filePath);
        return "";
    }

    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::Validate()
{
    GLint result = 0;
    GLchar err_log[1024] = { 0 };

    // glValidateProgram is typically useful only during application development.
    glValidateProgram(shaderID); // Checks to see whether the executables contained in program can execute given the current OpenGL state.
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(err_log), NULL, err_log);
        printf("Error validating program: '%s'\n", err_log);
        return;
    }
}

void Shader::SetProjection(const glm::mat4& projection)
{
    glUniformMatrix4fv(uniformProj, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::SetView(const glm::mat4& view)
{
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
}

void Shader::SetModel(const glm::mat4& model)
{
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight, 
    GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color, glm::vec3 direction)
{
    directionalLight->UseLight(directionalLightUniforms.uniformAmbientIntensity, directionalLightUniforms.uniformColor, 
        directionalLightUniforms.uniformDiffuseIntensity, directionalLightUniforms.uniformDirection, 
        ambientIntensity, diffuseIntensity, color, direction);

    directionalLight->GetShadowMap()->Read(GL_TEXTURE2);
    glUniform1i(uniformTexture, 1);
    glUniform1i(uniformDirectionalShadowMap, 2);
}

void Shader::SetPointLight(PointLight* pointLight[], unsigned int lightCount, unsigned int textureUnit, unsigned int offset,
                           GLfloat ambientIntensity[MAX_POINT_LIGHTS], GLfloat diffuseIntensity[MAX_POINT_LIGHTS], glm::vec3 color[MAX_POINT_LIGHTS], glm::vec3 position[MAX_POINT_LIGHTS])
{
    if (lightCount > MAX_POINT_LIGHTS) 
        lightCount = MAX_POINT_LIGHTS;

    glUniform1i(uniformPointLightCount, lightCount);
    for (unsigned int i = 0; i < lightCount; i++)
    {
        pointLight[i]->UseLight(pointLightUniforms[i].uniformAmbientIntensity, pointLightUniforms[i].uniformColor,
            pointLightUniforms[i].uniformDiffuseIntensity, pointLightUniforms[i].uniformPosition,
            pointLightUniforms[i].uniformConstant, pointLightUniforms[i].uniformLinear, pointLightUniforms[i].uniformExponent,
            ambientIntensity[i], diffuseIntensity[i], color[i], position[i]);

        pointLight[i]->GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(omniShadowMapUniforms[i + offset].uniformShadowMap, textureUnit + i);
        glUniform1f(omniShadowMapUniforms[i + offset].uniformNearPlane, pointLight[i]->GetNearPlane());
        glUniform1f(omniShadowMapUniforms[i + offset].uniformFarPlane, pointLight[i]->GetFarPlane());
    }
}

void Shader::SetSpotLight(SpotLight* spotLight[], unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
    if (lightCount > MAX_SPOT_LIGHTS)
        lightCount = MAX_SPOT_LIGHTS;

    glUniform1i(uniformSpotLightCount, lightCount);
    for (unsigned int i = 0; i < lightCount; i++)
    {
        spotLight[i]->UseLight(spotLightUniforms[i].uniformAmbientIntensity, spotLightUniforms[i].uniformColor,
            spotLightUniforms[i].uniformDiffuseIntensity, spotLightUniforms[i].uniformPosition,
            spotLightUniforms[i].uniformConstant, spotLightUniforms[i].uniformLinear, spotLightUniforms[i].uniformExponent,
            spotLightUniforms[i].uniformDirection, spotLightUniforms[i].uniformEdge);

        spotLight[i]->GetShadowMap()->Read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(omniShadowMapUniforms[i + offset].uniformShadowMap, textureUnit + i);
        glUniform1f(omniShadowMapUniforms[i + offset].uniformNearPlane, spotLight[i]->GetNearPlane());
        glUniform1f(omniShadowMapUniforms[i + offset].uniformFarPlane, spotLight[i]->GetFarPlane());
    }
}

void Shader::SetColor(const glm::vec3& color)
{
    glUniform3f(uniformColor, color.r, color.g, color.b);
}

void Shader::SetTexture(GLuint textureUnit)
{
    glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightSpaceTransform(const glm::mat4& transform)
{
    glUniformMatrix4fv(uniformDirectionalLightSpaceTransform, 1, GL_FALSE, glm::value_ptr(transform));
}

void Shader::SetLightMatrices(const std::vector<glm::mat4>& lightMatrices)
{
    for (int i = 0; i < 6; ++i)
    {
        glUniformMatrix4fv(uniformLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
    }
}

void Shader::SetOmniLightPos(const glm::vec3& position)
{
    glUniform3f(uniformOmniLightPos, position.x, position.y, position.z);
}

void Shader::SetNearPlane(GLfloat nearPlane)
{
    glUniform1f(uniformNearPlane, nearPlane);
}

void Shader::SetFarPlane(GLfloat farPlane)
{
    glUniform1f(uniformFarPlane, farPlane);
}

void Shader::SetEyePosition(const glm::vec3& eyePosition)
{
    glUniform3f(uniformEyePosition, eyePosition.x, eyePosition.y, eyePosition.z);
}

void    Shader::CompileProgram()
{
    GLint result = 0;
    GLchar err_log[1024] = { 0 };

    glLinkProgram(shaderID);  // Create executables from shader codes to run on corresponding processors.
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(err_log), NULL, err_log);
        printf("Error linking program: '%s'\n", err_log);
        return;
    }

    uniformColor = glGetUniformLocation(shaderID, "color");
    uniformModel = glGetUniformLocation(shaderID, "model"); // glLinkProgram initializes uniform variables to 0
    uniformProj = glGetUniformLocation(shaderID, "proj");   // This means that glGetUniformLocation should be called
    uniformView = glGetUniformLocation(shaderID, "view");   // after linking the program via glLinkProgram.
    directionalLightUniforms.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    directionalLightUniforms.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    directionalLightUniforms.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    directionalLightUniforms.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
    for (int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { 0 };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.color", i);
        pointLightUniforms[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        pointLightUniforms[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        pointLightUniforms[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        pointLightUniforms[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        pointLightUniforms[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        pointLightUniforms[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        pointLightUniforms[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { 0 };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.base.color", i);
        spotLightUniforms[i].uniformColor = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.base.ambientIntensity", i);
        spotLightUniforms[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.base.diffuseIntensity", i);
        spotLightUniforms[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.position", i);
        spotLightUniforms[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.constant", i);
        spotLightUniforms[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.linear", i);
        spotLightUniforms[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].pointLight.exponent", i);
        spotLightUniforms[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        spotLightUniforms[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        spotLightUniforms[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
    uniformDirectionalLightSpaceTransform = glGetUniformLocation(shaderID, "directionalLightSpaceTransform");
    uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");
    uniformTexture = glGetUniformLocation(shaderID, "theTexture");

    uniformOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
    uniformNearPlane = glGetUniformLocation(shaderID, "nearPlane");
    uniformFarPlane = glGetUniformLocation(shaderID, "farPlane");
    for (int i = 0; i < 6; i++)
    {
        char locBuff[100] = { 0 };

        snprintf(locBuff, sizeof(locBuff), "lightMatrices[%d]", i);
        uniformLightMatrices[i] = glGetUniformLocation(shaderID, locBuff);
    }

    for (int i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { 0 };

        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].shadowMap", i);
        omniShadowMapUniforms[i].uniformShadowMap = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].nearPlane", i);
        omniShadowMapUniforms[i].uniformNearPlane = glGetUniformLocation(shaderID, locBuff);
        snprintf(locBuff, sizeof(locBuff), "omniShadowMaps[%d].farPlane", i);
        omniShadowMapUniforms[i].uniformFarPlane = glGetUniformLocation(shaderID, locBuff);
    }
}

void	Shader::CompileShader(const char* vsCode, const char* fsCode)
{
    GLuint vs, fs;
    
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error: Cannot create shader program.");
        return;
    }

    vs = AddShader(vsCode, GL_VERTEX_SHADER);
    fs = AddShader(fsCode, GL_FRAGMENT_SHADER);
    glAttachShader(shaderID, vs);  // Attach shaders to the program for linking process.
    glAttachShader(shaderID, fs);

    CompileProgram();
}

void	Shader::CompileShader(const char* vsCode, const char* gsCode, const char* fsCode)
{
    GLuint vs, fs, gs;

    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error: Cannot create shader program.");
        return;
    }

    vs = AddShader(vsCode, GL_VERTEX_SHADER);
    glAttachShader(shaderID, vs);  // Attach shaders to the program for linking process.
    gs = AddShader(gsCode, GL_GEOMETRY_SHADER);
    glAttachShader(shaderID, gs);
    fs = AddShader(fsCode, GL_FRAGMENT_SHADER);
    glAttachShader(shaderID, fs);

    CompileProgram();
}

GLuint	Shader::AddShader(const char* shaderCode, GLenum shaderType)
{
    GLuint new_shader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = shaderCode;

    glShaderSource(new_shader, 1, code, NULL);

    GLint result = 0;
    GLchar err_log[1024] = { 0 };

    glCompileShader(new_shader);
    glGetShaderiv(new_shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(new_shader, sizeof(err_log), NULL, err_log);
        printf("Error compiling the %d shader: '%s'\n", shaderType, err_log);
        return 0;
    }
    return new_shader;
}

void Shader::UseShader()
{
    if (shaderID != 0)
        glUseProgram(shaderID);
    else
        printf("Error: Cannot find a shader program.");
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    
    shaderID = 0;
    uniformProj = 0;
    uniformModel = 0;
    uniformView = 0;
    uniformShininess = 0;
    uniformSpecularIntensity = 0;
    uniformEyePosition = 0;
    uniformPointLightCount = 0;
    pointLightCount = 0;
    spotLightCount = 0;
    uniformSpotLightCount = 0;
    uniformTexture = 0;
    uniformDirectionalLightSpaceTransform = 0;
    uniformDirectionalShadowMap = 0;
    uniformOmniLightPos = 0;
    uniformNearPlane = 0;
    uniformFarPlane = 0;
    memset(uniformLightMatrices, 0, sizeof(uniformLightMatrices));
}

Shader::~Shader()
{
    ClearShader();
}