//
// Created by Skyler on 9/8/2020.
//

#ifndef __VGDE_VSTRING_H__
#define __VGDE_VSTRING_H__

#include "config.h"

#include <string>

class String {
public:
    String();

private:
    std::basic_string<uint32> _str;
};


#endif //__VGDE_VSTRING_H__
