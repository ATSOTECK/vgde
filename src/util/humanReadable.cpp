#include "humanReadable.h"

#include "config.h"
#include <iostream>

std::string secondsToHHMMSS(int seconds) {
    //TODO(Skyler): Formatting.
    int h = seconds / 3600;
    int m = (seconds - h * 3600) / 60;
    int s = (seconds - h * 3600) - m * 60;

    return std::to_string(h) + ":" + std::to_string(m) + ":" + std::to_string(s);
}
