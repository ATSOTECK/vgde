#ifndef __VGDE_RESOURCEMANAGER_H__
#define __VGDE_RESOURCEMANAGER_H__

#include "graphics/shader.h"
#include "graphics/texture.h"

#include <map>
#include <string>
#include <vector>

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

    void setImgPath(const std::string &path);

    void addShader(Shader *shader);
    void removeShader(Shader *shader);
    void updateShaderProjections();
private:
    ResourceManager();
    static ResourceManager *_resourceManager;

    std::map<std::string, ref<Texture*> > _textureMap;

    std::vector<Shader*> _shaders;

    std::string _imgPath;
};


#endif //__VGDE_RESOURCEMANAGER_H__
