#ifndef SHADER_PROGRAM_H_
#define SHADER_PROGRAM_H_

#include <GL/glew.h>
#include <string>
#include <vector>

struct Shader
{
    // Shader types
    //  .vert   Vertex
    //  .frag   Fragment
    //  .tesc   Tesselation control
    //  .tese   Tesselation evaluation
    //  .geom   Geometry
    //  .comp   Compute
    GLenum type;
    std::string filename;
    GLuint id;
};

class ShaderProgram
{
private:
    GLuint id;
    std::vector<Shader> shaders;
    std::string SHADERS_PATH = "resources/shaders/";

    GLenum typeFromExtension(std::string filename);
    Shader compileShader(GLenum type, std::string filename);
    bool checkShaderCompile(GLuint shaderIndex);
    void printShaderInfoLog(GLuint shaderIndex); 

    bool checkShaderLink(GLuint programme);
    void printAllInfo(GLuint programme);
    void printProgrammeInfoLog(GLuint programme);

public:
    template <typename ...Shaders>
    void addShaders(GLenum type, std::string name, Shaders... shaders) {
        addShaders(type, name);
        addShaders(shaders...);
    }
    void addShaders(GLenum type, std::string name);
    void link();
    void reload();
    void activate();
};

#endif