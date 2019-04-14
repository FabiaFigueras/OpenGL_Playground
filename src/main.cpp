#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "utils/GL_Log.h"
#include "render/assetManager.h"
#include "render/shaderProgram.h"

int gWinWidth  = 640;
int gWinHeight = 480;
int gFbWidth  = 640;
int gFbHeight = 480;

double previousSeconds;
int frameCount;

AssetManager assetManager;

// Methods to resize the window and the framebuffer if the user rezises the window
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    gWinWidth = width;
    gWinHeight = height;
    logger::glLogOut("Window resize: %ix%i\n", width, height);
}

void glfwFramebufferResizeCallback(GLFWwindow* window, int width, int height) {
    gFbWidth = width;
    gFbHeight = height;
    logger::glLogOut("Framebuffer resize: %ix%i\n", width, height);
}

void glfwErrorCallback(int error, const char* description) {
    logger::glLogErr("GLFW ERROR: Code %i msg: %s\n", error, description);
}

void logGLParams() {
    GLenum params[] = {
        GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
        GL_MAX_CUBE_MAP_TEXTURE_SIZE,
        GL_MAX_DRAW_BUFFERS,
        GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
        GL_MAX_TEXTURE_IMAGE_UNITS,
        GL_MAX_TEXTURE_SIZE,
        GL_MAX_VARYING_FLOATS,
        GL_MAX_VERTEX_ATTRIBS,
        GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
        GL_MAX_VERTEX_UNIFORM_COMPONENTS,
        GL_MAX_VIEWPORT_DIMS,
        GL_STEREO
    };
    const char* names[] = {
        "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
        "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
        "GL_MAX_DRAW_BUFFERS",
        "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
        "GL_MAX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_TEXTURE_SIZE",
        "GL_MAX_VARYING_FLOATS",
        "GL_MAX_VERTEX_ATTRIBS",
        "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
        "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
        "GL_MAX_VIEWPORT_DIMS",
        "GL_STEREO"
    };
    logger::glLog("GL Context Params:\n");
    // The loop is only for the ones that have an integer return
    for (int i = 0; i < 10; i++) {
        int v = 0;
        glGetIntegerv(params[i], &v);
        logger::glLog("%s %i\n", names[i], v);
    }
    // The others
    int v[2];
    v[0] = v[1] = 0;
    glGetIntegerv(params[10], v);
    logger::glLog("%s %i %i\n", names[10], v[0], v[1]);
    unsigned char s = 0;
    glGetBooleanv(params[11], &s);
    logger::glLog("%s %u\n", names[11], (unsigned int)s);
    logger::glLog("------------------------------\n\n");
}

// We will use this function to update the window title with a frame rate
void updateFpsCounter(GLFWwindow* window) {
    double currentSeconds;
    double elapsedSeconds;
    currentSeconds = glfwGetTime();
    elapsedSeconds = currentSeconds - previousSeconds;
    // Limit text updates to 4 per second
    if (elapsedSeconds > 0.25) {
        previousSeconds = currentSeconds;
        char tmp[128];
        double fps = (double)frameCount / elapsedSeconds;
        double frameTime = 1000 / fps; 
        sprintf(tmp, "opengl @ fps: %.2f - frameTime: %.2fms", fps, frameTime);
        glfwSetWindowTitle(window, tmp);
        frameCount = 0;
    }
    frameCount++;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        assetManager.reloadPrograms();
    }
}

int main() {
    // Restart the log
    if (!logger::restartGLLog()) {
        // Exit?
        return -2;
    }
    // Start the GL Context and window using the GLFW helper library
    logger::glLog("Starting GLFW: %s\n\n", glfwGetVersionString());
    // Register the error callback function that we wrote
    glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: Could not start GLFW3\n");
        return -1;
    }

    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // We can set antialiasing (I don't want it by default, I'll implement my version)
    // glfwWindowHint(GLFW_SAMPLES, 4);    // Normal mode
    // glfwWindowHint(GLFW_SAMPLES, 16);   // Screenshots

    // Set up fullscreen mode
    // GLFWmonitor* mon = glfwGetPrimaryMonitor();
    // const GLFWvidmode* vmode = glfwGetVideoMode(mon);
    // GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", mon, NULL);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Initial Test", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: Could no open window with GLFW3\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferResizeCallback);

    logGLParams();

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // Get version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    logger::glLogOut("Vendor: %s\n", vendor);
    logger::glLogOut("Renderer: %s\n", renderer);
    logger::glLogOut("OpenGL version supported %s\n", version);

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);       // Depth testing interprets a smaller value as closer

    GLfloat points[] = {
         0.51f,  0.49f, 0.0f,
         0.51f, -0.51f, 0.0f,
        -0.49f, -0.51f, 0.0f,
        -0.51f, -0.49f, 0.0f,
        -0.51f,  0.51f, 0.0f,
         0.49f,  0.51f, 0.0f
    };

    // Create all the necessary VBO
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // Create the VAO that will allow us to manage the mesh more efficiently
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Add the 'basic' program to the Asset Manager
    ShaderProgram* basicPrg = assetManager.addProgram("basic");
    basicPrg->addShaders(GL_VERTEX_SHADER, "basic.vert", GL_FRAGMENT_SHADER, "basic.frag");
    basicPrg->link();

    glfwSetKeyCallback(window, key_callback);

    // Close the program when we click the ESC key
    while(!glfwWindowShouldClose(window)) {
        updateFpsCounter(window);
        // Wipe the surface
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, gFbWidth, gFbHeight);
        glClearColor(0.7f, 0.8f, 0.8f, 1.0f);
        assetManager.getProgram("basic")->activate();
        glBindVertexArray(vao);
        // Draw points in TRIANGLE MODE for number of vertexs, starts at point number 0
        glDrawArrays(GL_TRIANGLES, 0, sizeof(points));
        // Update other events like input handling
        glfwPollEvents();
        // Put everything that we've been drawing on the screen
        glfwSwapBuffers(window);
    }

    // (F) TODO: Delete all the programs from the AssetManager

    // Close the GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}