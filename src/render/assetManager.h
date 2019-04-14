#ifndef ASSET_MANAGER_H_
#define ASSET_MANAGER_H_

#include <string>
#include <map>
#include "shaderProgram.h"

class AssetManager
{
private:
    std::map<std::string, ShaderProgram> programs;

public:
    ShaderProgram* addProgram(std::string name);
    ShaderProgram* getProgram(std::string name);
    void reloadPrograms();
};

#endif