#ifndef __VGDE_RENDERTARGET_H__
#define __VGDE_RENDERTARGET_H__

//#include "transform.h"
#include "vec.h"

class RenderTarget {
public:
    explicit RenderTarget(const vec2f &size);

    void bind() const;
    void unbind() const;
private:
    uint _frameBuffer;
    uint _textureID;
    vec2f _size;
};


#endif //__VGDE_RENDERTARGET_H__
