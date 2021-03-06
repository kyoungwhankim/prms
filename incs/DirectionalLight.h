#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity,
        GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat diffuseIntensity,
        GLuint shadowWidth, GLuint shadowHeight);
    ~DirectionalLight();

    void		UseLight(GLuint ambientIntensityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation, 
        GLfloat ambientIntensity, GLfloat diffuseIntensity, glm::vec3 color, glm::vec3 direction);
    glm::mat4   CalculateLightSpaceTransform();

private:
    glm::vec3	direction;
};

