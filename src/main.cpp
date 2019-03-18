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
    const GLubyte* vendor = glGetString(GL_VENDOR);
    printf("Vendor: %s\n", vendor);
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);

    // Tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);       // Depth testing interprets a smaller value as closer
    



    GLfloat points[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
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
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        // Draw points in TRIANGLE MODE (1 triangle for every 3 points), starts at point number 0
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Update other events like input handling
        glfwPollEvents();
        // Put everything that we've been drawing on the screen
        glfwSwapBuffers(window);
    }

    // Close the GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}