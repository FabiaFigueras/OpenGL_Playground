#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <stdio.h>

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: Could not start GLFW3\n");
    }

    fprintf(stdout, "PROGRAM IS WORKING");

    glfwTerminate();

    return 0;
}