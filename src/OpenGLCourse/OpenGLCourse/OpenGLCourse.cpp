#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"


using namespace std;

const float toRadians = 3.14159265359f / 100.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Window mainWindow;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vertexShader = "Shaders/shader.vert";
static const char* fragmentShader = "Shaders/shader.frag";

void CalcAverageNormals(
	unsigned int* indices,
	unsigned int indiceCount,
	GLfloat* vertices,
	unsigned int verticeCount,
	unsigned int vLength, 
	unsigned int normalOffset) 
{
	for (size_t i = 0; i < indiceCount; i += 3) 
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0] += normal.x;
		vertices[in0] += normal.y;
		vertices[in0] += normal.z;

		vertices[in1] += normal.x;
		vertices[in1] += normal.y;
		vertices[in1] += normal.z;

		vertices[in2] += normal.x;
		vertices[in2] += normal.y;
		vertices[in2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObject()
{
	unsigned int indices[] =
	{
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] =
	{
	 //  x     y     z    u,    v,   nx    ny     nz
		-1.f, -1.f, 0.f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.f, 1.f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.f, 0.f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.f,   1.f, 0.f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
	};

	CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->Create(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->Create(vertices, indices, 32, 12);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vertexShader, fragmentShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFile(vertexShader, fragmentShader);
	shaderList.push_back(*shader2);
}

int main()
{
	mainWindow = Window(1290, 720);
	mainWindow.Initialize();
	
	CreateObject();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, 0.f, 5.0f, 1.0f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.Load();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.Load();

	mainLight = Light(1.f, 1.f, 1.f, 0.2f, 2.f, -1.f, -2.f, 1.f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, 
		uniformAmbientColour = 0, uniformDirection = 0, uniformDiffuseIntensity = 0;

	glm::mat4 projection = glm::perspective(45.f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		//camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].Use();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformModel = shaderList[0].GetModelLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformAmbientColour = shaderList[0].GetAmbientColourLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();

		mainLight.Use(uniformAmbientIntensity, uniformAmbientColour, 
			uniformDiffuseIntensity, uniformDirection);

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0.f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.f));
		glUniformMatrix4fv(uniformModel, 1.f, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1.f, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1.f, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		brickTexture.Use();
		meshList[0]->Render();

		model = glm::mat4();
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1.f, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1.f, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1.f, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));


		dirtTexture.Use();
		meshList[1]->Render();
		shaderList[1].Use();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
