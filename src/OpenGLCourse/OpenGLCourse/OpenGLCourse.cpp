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
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include "Common.h"

using namespace std;

std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Window mainWindow;
Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

static const char* vertexShader = "Shaders/shader.vert";
static const char* fragmentShader = "Shaders/shader.frag";

void CalcAverageNormals(
    unsigned int* indices, unsigned int indiceCount,
    GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in1] - vertices[in0],
            vertices[in1 + 1] - vertices[in0 + 1],
            vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0],
            vertices[in2 + 1] - vertices[in0 + 1],
            vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++) {
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
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] = {
        // x      y     z     u     v     nx    ny    nz
        -1.f, -1.f, -0.6f,  0.0f, 0.0f,   0.f,  0.f,  0.f,
         0.f, -1.f,  1.0f,  0.5f, 0.0f,   0.f,  0.f,  0.f,
         1.f, -1.f, -0.6f,  1.0f, 0.0f,   0.f,  0.f,  0.f,
         0.f,  1.f,  0.0f,  0.5f, 1.0f,   0.f,  0.f,  0.f,
    };

    unsigned int floorIndices[] = {
       0, 2, 1,
       1, 2, 3,
    };

    GLfloat floorVertices[] = {
        -10.f, 0.f, -10.f,  0.f, 0.f,  0.f, -1.f, 0.f,
        10.f, 0.f, -10.f,   10.f, 0.0f, 0.f, -1.f, 0.f,
        -10.f, 0.f, 10.f,   0.0f, 10.f, 0.0f, -1.f, 0.f,
        10.f, 0.0f, 10.0f,  10.f, 10.f, 0.0, -1.f, 0.0f,
    };

    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh* obj1 = new Mesh();
    obj1->Create(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->Create(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh* obj3 = new Mesh();
    obj3->Create(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFile(vertexShader, fragmentShader);
    shaderList.push_back(*shader1);
}

int main()
{
    mainWindow = Window(1920, 1080);
    mainWindow.Initialize();

    CreateObject();
    CreateShaders();

    camera = Camera(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.f, 0.f,
        5.0f, 1.0f
    );

    // Load textures
    brickTexture = Texture("Textures/brick.png");
    brickTexture.Load();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.Load();

    // Materials
    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    // Light
    mainLight = DirectionalLight(1.f, 1.f, 1.f,
                                0.1f, 0.3f, 
                                0.0f, 0.0f, -1.0f);

    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(
        0.0f, 0.0f, 1.f,
        0.0f, 1.0f,
        4.0f, 0.0f, 0.0f,
        0.3f, 0.2f, 0.1f);
    pointLightCount++;

    pointLights[1] = PointLight(
        0.0f, 1.0f, 0.f,
        0.1f, 1.0f,
        -4.0f, 2.0f, 0.0f,
        0.3f, 0.1f, 0.1f);
    pointLightCount++;

    // Projection
    glm::mat4 projection = glm::perspective(
        45.f,
        (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(),
        0.1f, 100.f
    );

    // Uniform variables
    GLuint uniformProjection = 0;
    GLuint uniformModel = 0;
    GLuint uniformView = 0;
    GLuint uniformEyePosition = 0;
    GLuint uniformSpecularIntensity = 0;
    GLuint uniformShininess = 0;

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        glfwPollEvents();
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        // Clear
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our only shader
        shaderList[0].Use();

        // Get uniform locations (for shaderList[0])
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformModel = shaderList[0].GetModelLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        // Set matrix uniforms
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition,
            camera.getCameraPosition().x,
            camera.getCameraPosition().y,
            camera.getCameraPosition().z);

        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLight(pointLights, pointLightCount);


        // Render first mesh
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        brickTexture.Use();
        shinyMaterial.Use(uniformSpecularIntensity, uniformShininess);
        meshList[0]->Render();

        // Render Second Mesh 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, 3.f, -2.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        dirtTexture.Use();
        dullMaterial.Use(uniformSpecularIntensity, uniformShininess);
        meshList[1]->Render();


        // Render Third Mesh 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.f, -3.f, -2.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        dirtTexture.Use();
        dullMaterial.Use(uniformSpecularIntensity, uniformShininess);
        meshList[2]->Render();

        // Done with this shader
        glUseProgram(0);

        // Swap buffers
        mainWindow.swapBuffers();
    }

    return 0;
}
