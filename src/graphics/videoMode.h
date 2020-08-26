#ifndef __VGDE_VIDEOMODE_H__
#define __VGDE_VIDEOMODE_H__

#include <iostream>

struct VideoMode {
    int width;
    int height;
    int refreshRate;
};

std::ostream &operator<<(std::ostream &os, const VideoMode &mode);

bool operator==(const VideoMode &left, const VideoMode &right);
bool operator!=(const VideoMode &left, const VideoMode &right);

#endif //__VGDE_VIDEOMODE_H__
