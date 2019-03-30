#include "shaderManager.h"
#include "../utils/GL_Log.h"
#include <fstream>
#include <cerrno>


ShaderManager::ShaderManager() {
    shaders = Map();
}

void ShaderManager::initShaders() {
    logger::glLogOut("\nInitializing Shaders\n--------------------\n");
    
    addShader("basic.frag");
    addShader("basic.vert");

    logger::glLogOut("\n");
}

std::string getFileExtension(std::string fileName) {
    return fileName.substr(fileName.find_last_of(".") + 1);
}

bool ShaderManager::shaderExists(std::string fileName) {
    Map::iterator it = shaders.find(fileName);
    return it != shaders.end();
}

void ShaderManager::addShader(std::string fileName) {
    // Check if the shader already exists
    if (shaderExists(fileName)) {
        logger::glLogOut("Shader '%s' already exists\n", fileName.c_str());
        return;
    }

    // Read compile and add it to the map
    std::string shaderContent = readFromFile(SHADERS_PATH + fileName);
    std::string ext = getFileExtension(std::string(fileName));

    GLenum shaderType = GL_FRAGMENT_SHADER;
    if (ext.compare("vert") == 0) {
        shaderType = GL_VERTEX_SHADER;
    }
    const GLchar* source = shaderContent.c_str();
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    if (checkShaderCompile(shader)) {
        shaders.insert(Map::value_type(fileName.c_str(), shader));
    }
}

std::string ShaderManager::readFromFile(std::string fileName) {
    std::ifstream in(fileName, std::ios::in);
    if (in) {
        logger::glLogOut("Reading shader: '%s'\n", fileName.c_str());
        std::string content;
        in.seekg(0, std::ios::end);
        content.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&content[0], content.size());
        in.close();
        logger::glLogOut("Shader '%s' content:\n%s\n\n", fileName.c_str(), content.c_str());
        return content;

    }
    logger::glLogErr("Could not read file: '%s'\n", fileName.c_str());
    throw(errno);
}

void ShaderManager::printShaderInfoLog(GLuint shaderIndex) {
    int maxLength = 2058;
    int actualLength = 0;
    char log[2048];
    glGetShaderInfoLog(shaderIndex, maxLength, &actualLength, log);
    logger::glLogOut("Shader info log for GL index %u:\n%s\n", shaderIndex, log);
}

bool ShaderManager::checkShaderCompile(GLuint shaderIndex) {
    int params = -1;
    glGetShaderiv(shaderIndex, GL_COMPILE_STATUS, &params);
    if (GL_TRUE != params) {
        logger::glLogErr("ERROR: GL shader index %i did not compile\n", shaderIndex);
        printShaderInfoLog(shaderIndex);
        return false;
    }
    return true;
}