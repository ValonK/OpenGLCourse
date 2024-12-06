#include <stdio.h>
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 1920, HEIGHT = 1080;

// Vertex Array Object, Vertex Buffer Object ID's
GLuint VAO, VBO, shader;

using namespace std;

// Vertex Shader
static const char* vertexShader = R"(
#version 330

layout(location = 0) in vec3 pos;

void main()
{
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);
}
)";

// Fragment Shader
static const char* fragmentShader = R"(
#version 330

out vec4 colour;

void main()
{
    colour = vec4(1.0, 0.0, 0.0, 1.0);
}
)";

void AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLen[1];
    codeLen[0] = strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLen);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(program, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();
    if (!shader)
    {
        cout << "Error creating shader program" << endl;
        glfwTerminate();
        return;
    }

    AddShader(shader, vertexShader, GL_VERTEX_SHADER);
    AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

void CreateTriangle()
{
    GLfloat vertices[] =
    {
        -1.f, -1.f, 0.f,
        1.0f, -1.f, 0.f,
        0.f, 1.f, 0.f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // GL_STATIC_DRAW means not changing the array value, but we can change pos and other properties
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main()
{
    // Initialize GLFW 
    if (!glfwInit())
    {
        cout << "GLFW Initialization failed!" << endl;
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties & OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Do not use deprecated version / features, no backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        cout << "GLFW Window creation failed!" << endl;
        glfwTerminate();
        return 1;
    }

    // Get Buffer size information 
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use (also used to switch between windows)
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        cout << "GLEW Initialization failed!" << endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    CreateTriangle();
    CompileShaders();

    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + Handle user input events
        glfwPollEvents();

        // Clear window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}
