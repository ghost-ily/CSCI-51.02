/******************************************************************************
 * This demo draws a triangle by defining its vertices in 3 dimensions
 * (the 3rd dimension is currently ignored and is just set to 0).
 *
 * The drawing is accomplished by:
 * - Uploading the vertices to the GPU using a Vertex Buffer Object (VBO).
 * - Specifying the vertices' format using a Vertex Array Object (VAO).
 * - Using a GLSL shader program (consisting of a simple vertex shader and a
 *   simple fragment shader) to actually draw the vertices as a triangle.
 *
 * Happy hacking! - eric
 *****************************************************************************/

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <gdev.h>

// change this to your desired window attributes
#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE  "Hello Triangle"
GLFWwindow *pWindow;

// define a vertex array to hold our vertices
float vertices[] =
{
    // position (x, y, z)      color (r, g, b)       texture (s, t)
    //FRONT FACE
    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
    0.500f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  1.00f, 0.50f,
    0.383f,  0.321f, -1.000f,  0.30f, 0.30f, 0.30f,  0.88f, 0.82f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
    0.383f,  0.321f, -1.000f,  0.30f, 0.30f, 0.30f,  0.88f, 0.82f,
    0.087f,  0.492f, -1.000f,  0.30f, 0.30f, 0.30f,  0.58f, 0.99f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
    0.087f,  0.492f, -1.000f,  0.30f, 0.30f, 0.30f,  0.58f, 0.99f,
   -0.250f,  0.433f, -1.000f,  0.30f, 0.30f, 0.30f,  0.25f, 0.93f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
   -0.250f,  0.433f, -1.000f,  0.30f, 0.30f, 0.30f,  0.25f, 0.93f,
   -0.470f,  0.171f, -1.000f,  0.30f, 0.30f, 0.30f,  0.03f, 0.67f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
   -0.470f,  0.171f, -1.000f,  0.30f, 0.30f, 0.30f,  0.03f, 0.67f,
   -0.470f, -0.171f, -1.000f,  0.30f, 0.30f, 0.30f,  0.03f, 0.33f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
   -0.470f, -0.171f, -1.000f,  0.30f, 0.30f, 0.30f,  0.03f, 0.33f,
   -0.250f, -0.433f, -1.000f,  0.30f, 0.30f, 0.30f,  0.25f, 0.07f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
   -0.250f, -0.433f, -1.000f,  0.30f, 0.30f, 0.30f,  0.25f, 0.07f,
    0.087f, -0.492f, -1.000f,  0.30f, 0.30f, 0.30f,  0.58f, 0.01f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
    0.087f, -0.492f, -1.000f,  0.30f, 0.30f, 0.30f,  0.58f, 0.01f,
    0.383f, -0.321f, -1.000f,  0.30f, 0.30f, 0.30f,  0.88f, 0.18f,

    0.000f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  0.50f, 0.50f,
    0.383f, -0.321f, -1.000f,  0.30f, 0.30f, 0.30f,  0.88f, 0.18f,
    0.500f,  0.000f, -1.000f,  0.30f, 0.30f, 0.30f,  1.00f, 0.50f,

    //BACK FACE
    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
    0.383f,  0.321f, -0.800f,  1.00f, 1.00f, 1.00f,  0.88f, 0.82f,
    0.500f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  1.00f, 0.50f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
    0.087f,  0.492f, -0.800f,  1.00f, 1.00f, 1.00f,  0.59f, 0.99f,
    0.383f,  0.321f, -0.800f,  1.00f, 1.00f, 1.00f,  0.88f, 0.82f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
   -0.250f,  0.433f, -0.800f,  1.00f, 1.00f, 1.00f,  0.25f, 0.93f,
    0.087f,  0.492f, -0.800f,  1.00f, 1.00f, 1.00f,  0.58f, 0.99f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
   -0.470f,  0.171f, -0.800f,  1.00f, 1.00f, 1.00f,  0.03f, 0.67f,
   -0.250f,  0.433f, -0.800f,  1.00f, 1.00f, 1.00f,  0.25f, 0.93f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
   -0.470f, -0.171f, -0.800f,  1.00f, 1.00f, 1.00f,  0.03f, 0.33f,
   -0.470f,  0.171f, -0.800f,  1.00f, 1.00f, 1.00f,  0.03f, 0.67f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
   -0.250f, -0.433f, -0.800f,  1.00f, 1.00f, 1.00f,  0.25f, 0.07f,
   -0.470f, -0.171f, -0.800f,  1.00f, 1.00f, 1.00f,  0.03f, 0.33f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
    0.087f, -0.492f, -0.800f,  1.00f, 1.00f, 1.00f,  0.59f, 0.01f,
   -0.250f, -0.433f, -0.800f,  1.00f, 1.00f, 1.00f,  0.25f, 0.07f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
    0.383f, -0.321f, -0.800f,  1.00f, 1.00f, 1.00f,  0.88f, 0.18f,
    0.087f, -0.492f, -0.800f,  1.00f, 1.00f, 1.00f,  0.59f, 0.01f,

    0.000f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  0.50f, 0.50f,
    0.500f,  0.000f, -0.800f,  1.00f, 1.00f, 1.00f,  1.00f, 0.50f,
    0.383f, -0.321f, -0.800f,  1.00f, 1.00f, 1.00f,  0.88f, 0.18f,

    //SIDES
    
    // SIDE FACES
    // Edge 1
    0.500f, 0.000f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, 0.321f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, 0.321f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    0.500f, 0.000f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, 0.321f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.500f, 0.000f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 2
    0.383f, 0.321f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, 0.492f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, 0.492f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    0.383f, 0.321f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, 0.492f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, 0.321f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 3
    0.087f, 0.492f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, 0.433f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, 0.433f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    0.087f, 0.492f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, 0.433f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, 0.492f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 4
   -0.250f, 0.433f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, 0.171f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, 0.171f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

   -0.250f, 0.433f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, 0.171f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, 0.433f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 5
   -0.470f, 0.171f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, -0.171f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, -0.171f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

   -0.470f, 0.171f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, -0.171f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, 0.171f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 6
   -0.470f, -0.171f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, -0.433f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, -0.433f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

   -0.470f, -0.171f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, -0.433f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.470f, -0.171f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 7
   -0.250f, -0.433f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, -0.492f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, -0.492f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

   -0.250f, -0.433f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, -0.492f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
   -0.250f, -0.433f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 8
    0.087f, -0.492f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, -0.321f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, -0.321f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    0.087f, -0.492f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, -0.321f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.087f, -0.492f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    // Edge 9
    0.383f, -0.321f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.500f, 0.000f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.500f, 0.000f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

    0.383f, -0.321f, -1.000f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.500f, 0.000f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,
    0.383f, -0.321f, -0.800f, 0.50f, 0.50f, 0.50f,  0.50f, 0.50f,

};

// define OpenGL object IDs to represent the vertex array and the shader program in the GPU
GLuint vao;         // vertex array object (stores the render state for our vertex array)
GLuint vbo;         // vertex buffer object (reserves GPU memory for our vertex array)
GLuint shader;      // combined vertex and fragment shader
GLuint texture;

// called by the main function to do initial setup, such as uploading vertex
// arrays, shader programs, etc.; returns true if successful, false otherwise
bool setup()
{
    // generate the VAO and VBO objects and store their IDs in vao and vbo, respectively
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // bind the newly-created VAO to make it the current one that OpenGL will apply state changes to
    glBindVertexArray(vao);

    // upload our vertex array data to the newly-created VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // on the VAO, register the current VBO with the following vertex attribute layout:
    // - layout location 0...
    // - ... shall consist of 3 GL_FLOATs (corresponding to x, y, and z coordinates)
    // - ... its values will NOT be normalized (GL_FALSE)
    // - ... the stride length is the number of bytes of all 3 floats of each vertex (hence, 3 * sizeof(float))
    // - ... and we start at the beginning of the array (hence, (void*) 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
    (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // important: if you have more vertex arrays to draw, make sure you separately define them
    // with unique VAO and VBO IDs, and follow the same process above to upload them to the GPU

    // load our shader program
    shader = gdevLoadShader("demo1.vs", "demo1.fs");
    if (! shader)
        return false;

    texture = gdevLoadTexture("shield.png", GL_CLAMP_TO_EDGE, true, true);
    if (! texture)
        return false;

    return true;
}


float angle = 0;

// called by the main function to do rendering per frame
void render()
{
    // clear the whole frame
    glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // using our shader program...
    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glEnable(GL_DEPTH_TEST); // enable OpenGL's hidden surface removal
    
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f),
            (float) WINDOW_WIDTH / WINDOW_HEIGHT,
            0.1f,
            100.0f);

    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));
            
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 2.0f));

    float camX = sin(angle) * 10.0f;
    float camZ = cos(angle) * 10.0f;

    glm::vec3 eyePosition = glm::vec3(camX, 2.0f, camZ);
    glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(eyePosition, targetPosition, upVector);

    // Draw First Nonagon
    modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 finallMatrix = projectionMatrix * viewMatrix * modelMatrix;
    glBindVertexArray(vao);
    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"),
            1, GL_FALSE, glm::value_ptr(finallMatrix));
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));

    // Draw Second Nonagon
    modelMatrix = glm::mat4(1.0f); // Reset identity matrix
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));

    finallMatrix = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"),
            1, GL_FALSE, glm::value_ptr(finallMatrix));
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));

    // Draw Third Nonagon
    modelMatrix = glm::mat4(1.0f); // Reset identity matrix
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, 0.5f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    finallMatrix = projectionMatrix * viewMatrix * modelMatrix;
    glUniformMatrix4fv(glGetUniformLocation(shader, "matrix"),
            1, GL_FALSE, glm::value_ptr(finallMatrix));
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (8 * sizeof(float)));
}

/*****************************************************************************/

// handler called by GLFW when there is a keyboard event
void handleKeys(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    // pressing Esc closes the window
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        angle += 0.5;
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        angle -= 0.5;
}

// handler called by GLFW when the window is resized
void handleResize(GLFWwindow* pWindow, int width, int height)
{
    // tell OpenGL to do its drawing within the entire "client area" (area within the borders) of the window
    glViewport(0, 0, width, height);
}

// main function
int main(int argc, char** argv)
{
    // initialize GLFW and ask for OpenGL 3.3 core
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // create a GLFW window with the specified width, height, and title
    pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (! pWindow)
    {
        // gracefully terminate if we cannot create the window
        std::cout << "Cannot create the GLFW window.\n";
        glfwTerminate();
        return -1;
    }

    // make the window the current context of subsequent OpenGL commands,
    // and enable vertical sync and aspect-ratio correction on the GLFW window
    glfwMakeContextCurrent(pWindow);
    glfwSwapInterval(1);
    glfwSetWindowAspectRatio(pWindow, WINDOW_WIDTH, WINDOW_HEIGHT);

    // set up callback functions to handle window system events
    glfwSetKeyCallback(pWindow, handleKeys);
    glfwSetFramebufferSizeCallback(pWindow, handleResize);

    // don't miss any momentary keypresses
    glfwSetInputMode(pWindow, GLFW_STICKY_KEYS, GLFW_TRUE);

    // initialize GLAD, which acts as a library loader for the current OS's native OpenGL library
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    // if our initial setup is successful...
    if (setup())
    {
        // do rendering in a loop until the user closes the window
        while (! glfwWindowShouldClose(pWindow))
        {
            // render our next frame
            // (by default, GLFW uses double-buffering with a front and back buffer;
            // all drawing goes to the back buffer, so the frame does not get shown yet)
            render();

            // swap the GLFW front and back buffers to show the next frame
            glfwSwapBuffers(pWindow);

            // process any window events (such as moving, resizing, keyboard presses, etc.)
            glfwPollEvents();
        }
    }

    // gracefully terminate the program
    glfwTerminate();
    return 0;
}
