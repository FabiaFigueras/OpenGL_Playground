#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <map>
#include <stdio.h>
#include <string>

typedef std::map<std::string, GLuint> Map;

class ShaderManager {
private:
    std::string SHADERS_PATH = "resources/shaders/";
    Map shaders;

    void addShader(std::string fileName);
    bool shaderExists(std::string fileName);
    std::string readFromFile(std::string fileName);
    bool checkShaderCompile(GLuint shaderIndex);
    void printShaderInfoLog(GLuint shaderIndex);
public:
    ShaderManager();
    ~ShaderManager() {}

    void initShaders();
};