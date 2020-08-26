#include "videoMode.h"

std::ostream &operator<<(std::ostream &os, const VideoMode &mode) {
    os << mode.width << "x" << mode.height << " " << mode.refreshRate << "Hz";
    return os;
}

bool operator==(const VideoMode &left, const VideoMode &right) {
    return (left.width == right.width && left.height == right.height && left.refreshRate == right.refreshRate);
}

bool operator!=(const VideoMode &left, const VideoMode &right) {
    return !(left == right);
}
