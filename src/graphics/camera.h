#ifndef __VGDE_CAMERA_H__
#define __VGDE_CAMERA_H__

#include "transform.h"

class Camera : public Transform {
public:
    Camera();

    void update();
private:
    float _zoom;
};


#endif //__VGDE_CAMERA_H__
