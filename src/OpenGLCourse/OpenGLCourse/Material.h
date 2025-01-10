#pragma once

#include <GL/glew.h>

class Material
{

public: 
	Material();
	Material(GLfloat sIntensity, GLfloat shine);
	
	void Use(GLuint specularIntesityLocation, GLuint shininessLocation);
	
	~Material();


private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

