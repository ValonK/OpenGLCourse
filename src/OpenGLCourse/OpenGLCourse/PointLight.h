#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:
	PointLight();
	PointLight(
		GLfloat red,
		GLfloat green,
		GLfloat blue,
		GLfloat aIntensity,
		GLfloat dIntensity,
		GLfloat xPos,
		GLfloat yPos,
		GLfloat zPos,
		GLfloat con,
		GLfloat lin, 
		GLfloat exp);


	void Use(
		GLfloat ambientIntesityLocation,
		GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation,
		GLfloat positionLocation,
		GLfloat constantLocation, 
		GLfloat linearLocation,
		GLfloat exponentLocation);

	~PointLight();

private:
	glm::vec3 position;
	GLfloat constant, linear, exponent;

};