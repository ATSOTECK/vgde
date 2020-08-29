#ifndef __VGDE_RESOURCEMANAGER_H__
#define __VGDE_RESOURCEMANAGER_H__

#include "graphics/texture.h"

#include <map>
#include <string>

template <typename T>
struct ref {
    T obj;
    int count;
};

class ResourceManager {
public:
    static ResourceManager *instance();

    Texture *loadTexture(const std::string &path);
    void unloadTexture(Texture *texture);
private:
    ResourceManager();
    static ResourceManager *_resourceManager;

    std::map<std::string, ref<Texture*>> _textureMap;
};


#endif //__VGDE_RESOURCEMANAGER_H__
