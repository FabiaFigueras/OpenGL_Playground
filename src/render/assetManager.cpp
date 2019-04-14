#include "assetManager.h"

ShaderProgram* AssetManager::addProgram(std::string name)
{
    auto it = programs.insert(std::make_pair(name, ShaderProgram()));
    return &it.first->second;
}

ShaderProgram* AssetManager::getProgram(std::string name)
{
    auto it = programs.find(name);
    return &it->second;
}

void AssetManager::reloadPrograms()
{
    for (auto it = programs.begin(); it != programs.end(); ++it) {
        it->second.reload();
    }
}