#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <map>
#include <stdio.h>
#include <string>

typedef std::map<std::string, GLuint> Map;
typedef std::string string;

class ShaderManager {
private:
    string SHADERS_PATH = "resources/shaders/";
    Map shaders;
    Map programs;

    void loadShader(string name);
    GLuint addShader(string fileName, bool reload = false);
    bool shaderExists(string fileName);
    string readFromFile(string fileName);
    bool checkShaderCompile(GLuint shaderIndex);
    void printShaderInfoLog(GLuint shaderIndex);
    
public:
    ShaderManager();
    ~ShaderManager() {}

    void initShaders();
    void reloadShaders();
    GLuint getShader(string name);
};