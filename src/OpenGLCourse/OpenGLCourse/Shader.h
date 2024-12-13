#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFile(const char* vertexLocation, const char* fragmentLocation);
	
	std::string ReadFile(const char* fileLocation);

	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void Use();
	void Clear();


	~Shader();

private: 
	GLuint shaderId, uniformProjection, uniformModel;
	void Compile(const char* vertexCode, const char* fragmentCode);
	void Add(GLuint program, const char* shaderCode, GLenum shaderType);

};

