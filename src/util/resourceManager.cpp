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
        texture = _textureMap[path];
    } else {
        texture = new Texture(path);
        _textureMap[path] = texture;
    }

    return texture;
}
