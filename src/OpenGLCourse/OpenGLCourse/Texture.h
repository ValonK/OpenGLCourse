#pragma once

#include <GL/glew.h>
#include "stb_image.h"
#include <iostream>

class Texture
{
public: 
	Texture();
	Texture(const char* fileLoc);

	void Load();
	void Use();
	void Clear();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

