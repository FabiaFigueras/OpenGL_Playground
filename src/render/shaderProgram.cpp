#include "shaderProgram.h"
#include "../utils/GL_Log.h"
#include <fstream>
#include <cerrno>

std::string readFromFile(std::string filename) 
{
    std::ifstream in(filename, std::ios::in);
    if (in) {
        logger::glLogOut("Reading shader: '%s'\n", filename.c_str());
        std::string content;
        in.seekg(0, std::ios::end);
        content.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&content[0], content.size());
        in.close();
        // logger::glLogOut("Shader '%s' content:\n%s\n\n", filename.c_str(), content.c_str());
        return content;

    }
    logger::glLogErr("Could not read file: '%s'\n", filename.c_str());
    throw(errno);
}

Shader ShaderProgram::compileShader(GLenum type, std::string filename)
{
    Shader result;

    // Load file content
    std::string pathFilename = SHADERS_PATH + filename;
    std::string shaderContent = readFromFile(pathFilename);

    // Create shader and compile it
    const GLchar* source = shaderContent.c_str();
    result.id = glCreateShader(type);
    glShaderSource(result.id, 1, &source, NULL);
    glCompileShader(result.id);

    // Check errors if some were found
    if (checkShaderCompile(result.id)) {
        result.type = type;
        result.filename = filename;
    }

    return result;
}

// LOGGING METHODS
bool ShaderProgram::checkShaderCompile(GLuint shaderIndex)
{
    int params = -1;
    glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        logger::glLogErr("ERROR: GL shader index %i did not compile\n", shaderIndex);
        printShaderInfoLog(shaderIndex);
        return false;
    }
    return true;
}

void ShaderProgram::printShaderInfoLog(GLuint shaderIndex) 
{
    int maxLength = 2058;
    int actualLength = 0;
    char log[2048];
    glGetShaderInfoLog(shaderIndex, maxLength, &actualLength, log);
    logger::glLogOut("Shader info log for GL index %u:\n%s\n", shaderIndex, log);
}

bool ShaderProgram::checkShaderLink(GLuint programme) {
    int params = -1;
    glGetProgramiv(programme, GL_LINK_STATUS, &params);
    if (GL_TRUE != params) {
        logger::glLogErr("ERROR: Cound not link shader programme GL index %u\n", programme);
        printProgrammeInfoLog(programme);
        return false;
    }
    return true;
}

const char* GLTypeToString(GLenum type) {
    switch (type)
    {
        case GL_BOOL: return "bool";
        case GL_INT: return "int";
        case GL_FLOAT: return "float";
        case GL_FLOAT_VEC2: return "vec2";
        case GL_FLOAT_VEC3: return "vec3";
        case GL_FLOAT_VEC4: return "vec4";
        case GL_FLOAT_MAT2: return "mat2";
        case GL_FLOAT_MAT3: return "mat3";
        case GL_FLOAT_MAT4: return "mat4";
        case GL_SAMPLER_2D: return "sampler2D";
        case GL_SAMPLER_3D: return "sampler3D";
        case GL_SAMPLER_CUBE: return "samplerCube";
        case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
        default: break;
    }
    return "other";
}

void ShaderProgram::printAllInfo(GLuint programme) {
    logger::glLogOut("\n---------------------\nShader programme %i info:\n", programme);
    int params = -1;
    glGetProgramiv(programme, GL_LINK_STATUS, &params);
    logger::glLogOut("GL_LINK_STATUS = %i\n", params);

    glGetProgramiv(programme, GL_ATTACHED_SHADERS, &params);
    logger::glLogOut("GL_ATTACHED_SHADERS = %i\n", params);

    glGetProgramiv(programme, GL_ACTIVE_ATTRIBUTES, &params);
    logger::glLogOut("GL_ACTIVE_ATTRIBUTES = %i\n", params);
    for (GLuint i = 0; i < (GLuint)params; i++) {
        char name[64];
        int maxLength = 64;
        int actualLength = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib(programme, i, maxLength, &actualLength, &size, &type, name);
        if (size > 1) {
            for (int j = 0; j < size; j++) {
                char longName[256];
                sprintf(longName, "%s[%i]", name, j);
                int location = glGetAttribLocation(programme, longName);
                logger::glLogOut("  %i) type: %s, name: %s, location: %i\n", i, GLTypeToString(type), longName, location);
            }
        } else {
            int location = glGetAttribLocation(programme, name);
            logger::glLogOut("  %i) type: %s, name: %s, location: %i\n", i, GLTypeToString(type), name, location);
        }
    }

    glGetProgramiv(programme, GL_ACTIVE_UNIFORMS, &params);
    logger::glLogOut("GL_ACTIVE_UNIFORMS = %i\n", params);
    for (GLuint i = 0; i < (GLuint)params; i++) {
        char name[64];
        int maxLength = 64;
        int actualLength = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform(programme, i, maxLength, &actualLength, &size, &type, name);
        if (size > 1) {
            for (int j = 0; j < size; j++) {
                char longName[256];
                sprintf(longName, "%s[%i]", name, j);
                int location = glGetUniformLocation(programme, longName);
                logger::glLogOut("  %i) type: %s, name: %s, location: %i\n", i, GLTypeToString(type), longName, location);
            }
        } else {
            int location = glGetUniformLocation(programme, name);
            logger::glLogOut("  %i) type: %s, name: %s, location: %i\n", i, GLTypeToString(type), name, location);
        }
    }

    printProgrammeInfoLog(programme);
}

void ShaderProgram::printProgrammeInfoLog(GLuint programme) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetProgramInfoLog(programme, maxLength, &actualLength, log);
    logger::glLogOut("Program info log for GL index %u:\n%s", programme, log);
}
// END LOGGIN METHODS

void ShaderProgram::addShaders(GLenum type, std::string name)
{
    shaders.push_back(compileShader(type, name));
}

void ShaderProgram::link()
{
    id = glCreateProgram();
    for (const auto& shader: shaders) {
        glAttachShader(id, shader.id);
    }
    glLinkProgram(id);
    checkShaderLink(id);
    printAllInfo(id);

    for (const auto& shader: shaders) {
        glDeleteShader(shader.id);
    }
}

void ShaderProgram::reload()
{
    glDeleteProgram(id);
    for (size_t i = 0; i < shaders.size(); ++i) {
        Shader newShader = compileShader(shaders[i].type, shaders[i].filename);
        if (newShader.id != 0) {
            shaders[i] = newShader;
        }
    }
    link();
}

void ShaderProgram::activate()
{
    glUseProgram(id);
}