#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	direction = glm::vec3(0.f, -1.f, 0.f);
	edge = 0.f;
	procEdge = cosf(glm::radians(edge));
}

SpotLight::SpotLight(
	GLfloat red,
	GLfloat green,
	GLfloat blue,
	GLfloat aIntensity, 
	GLfloat dIntensity,
	GLfloat xPos,
	GLfloat yPos,
	GLfloat zPos,
	GLfloat xDir,
	GLfloat yDir, 
	GLfloat zDir,
	GLfloat con,
	GLfloat lin,
	GLfloat exp,
	GLfloat edg) : PointLight(red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
	direction = glm::vec3(xDir, yDir, zDir);
	edge = edg;
	procEdge = cosf(glm::radians(edge));
}

void SpotLight::Use(GLuint ambientIntesityLocation,
	GLuint ambientColourLocation, 
	GLuint diffuseIntensityLocation,
	GLuint positionLocation,
	GLuint directionLocation,
	GLuint constantLocation,
	GLuint linearLocation,
	GLuint exponentLocation,
	GLuint edgeLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntesityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}

SpotLight::~SpotLight()
{
}
