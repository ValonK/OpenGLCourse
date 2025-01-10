#include "Material.h"

Material::Material()
{
	specularIntensity = 0.f;
	shininess = 0.f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::Use(GLuint specularIntesityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntesityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

Material::~Material()
{
}
