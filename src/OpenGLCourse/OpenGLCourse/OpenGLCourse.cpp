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

using namespace std;

const float toRadians = 3.14159265359f / 100.0f;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Window mainWindow;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vertexShader = "Shaders/shader.vert";
static const char* fragmentShader = "Shaders/shader.frag";

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
		-1.f, -1.f, 0.f,
		0.0f, -1.f, 1.f,
		1.0f, -1.f, 0.f,
		0.f, 1.f, 0.f
	};

	Mesh* obj1 = new Mesh();
	obj1->Create(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFile(vertexShader, fragmentShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1290, 720);
	mainWindow.Initialize();
	
	CreateObject();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.f, 0.f, 5.0f, 1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;

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

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0, 0.f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.f));
		glUniformMatrix4fv(uniformModel, 1.f, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1.f, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1.f, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshList[0]->Render();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
