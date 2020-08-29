#ifndef __VGDE_RESOURCEMANAGER_H__
#define __VGDE_RESOURCEMANAGER_H__

#include "graphics/texture.h"

#include <map>
#include <string>

class ResourceManager {
public:
    static ResourceManager *instance();

    Texture *loadTexture(const std::string &path);
private:
    ResourceManager();
    static ResourceManager *_resourceManager;

    std::map<std::string, Texture*> _textureMap;
};


#endif //__VGDE_RESOURCEMANAGER_H__
