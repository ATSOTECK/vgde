#include "resourceManager.h"

ResourceManager *ResourceManager::_resourceManager = null;

ResourceManager::ResourceManager() {
    //
}

ResourceManager *ResourceManager::instance() {
    if (_resourceManager == null) {
        _resourceManager = new ResourceManager();
    }

    return _resourceManager;
}

Texture *ResourceManager::loadTexture(const std::string &path) {
    Texture *texture = null;

    auto idx = _textureMap.find(path);
    if (idx != _textureMap.end()) {
        texture = _textureMap[path].obj;
        ++_textureMap[path].count;
    } else {
        texture = new Texture(path);
        _textureMap[path].obj = texture;
        _textureMap[path].count = 1;
    }

    return texture;
}

void ResourceManager::unloadTexture(Texture *texture) {
    for (auto &pair : _textureMap) {
        if (pair.second.obj == texture) {
            if (pair.second.count == 1) {
                delete texture;
                _textureMap.erase(pair.first);
            } else {
                --pair.second.count;
            }
        }
    }
}
