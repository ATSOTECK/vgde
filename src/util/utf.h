//
// Created by Skyler on 9/8/2020.
//

#ifndef __VGDE_UTF_H__
#define __VGDE_UTF_H__

#include "config.h"

#include <cstdlib>

template <uint32 N>
class Utf;

template<>
class Utf<32> {
public:
    template<typename In>
    static In decode(In begin, In end, uint32 &out, uint32 replacement = 0) {
        out = *begin++;
        return begin;
    }

    template<typename Out>
    static Out encode(uint32 in, Out out, uint32 replacement = 0) {
        *out++ = in;
        return out;
    }

    template <typename In>
    static In next(In begin, In end) {
        return ++begin;
    }

    template <typename In>
    static std::size_t count(In begin, In end) {
        return end - begin;
    }
};

#endif //__VGDE_UTF_H__
