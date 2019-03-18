#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "utils/GL_Log.h"

int gWinWidth  = 640;
int gWinHeight = 480;
int gFbWidth  = 640;
int gFbHeight = 480;

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

    // Create a shader here just to not waste time
    const char* vertex_shader =
        "#version 410\n"
        "in vec3 vp;"
        "void main() {"
        "   gl_Position = vec4(vp, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 410\n"
        "out vec4 frag_colour;"
        "void main() {"
        "   frag_colour = vec4(0.9, 0.6, 0.4, 1.0);"
        "}";

    // Load the shader strings into a GLShader and compile them
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);

    // Create a program and attach the shaders
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, fs);
    glLinkProgram(shader_programme);

    // Close the program when we click the ESC key
    while(!glfwWindowShouldClose(window)) {
        // Wipe the surface
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, gFbWidth, gFbHeight);
        glClearColor(0.7f, 0.8f, 0.8f, 1.0f);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        // Draw points in TRIANGLE MODE for number of vertexs, starts at point number 0
        glDrawArrays(GL_TRIANGLES, 0, sizeof(points));
        // Update other events like input handling
        glfwPollEvents();
        // Put everything that we've been drawing on the screen
        glfwSwapBuffers(window);

        if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    // Close the GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}