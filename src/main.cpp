#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>

int main() {
    // Start the GL Context and window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: Could not start GLFW3\n");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Initial Test", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: Could no open window with GLFW3\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // Get version info
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);       // Depth testing interprets a smaller value as closer
    

    // PROGRAM BODY //


    // Close the GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}