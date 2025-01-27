#include "DirectionalLight.h"


DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.f, -1.f, 0.f);
}

DirectionalLight::DirectionalLight(
	GLfloat red,
	GLfloat green,
	GLfloat blue,
	GLfloat aIntensity,
	GLfloat dIntensity,
	GLfloat xDir,
	GLfloat yDir,
	GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::Use(
	GLfloat ambientIntesityLocation,
	GLfloat ambientColourLocation,
	GLfloat diffuseIntensityLocation,
	GLfloat directionLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntesityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}