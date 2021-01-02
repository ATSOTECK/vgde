/*
 * VGDE - Video Game Development Environment
 * Copyright (c) 2020-2021 Skyler Burwell
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifndef __VGDE_VCONTAINER_H__
#define __VGDE_VCONTAINER_H__

#include "config.h"

#include <tuple>

namespace {

struct AbstractLContainer {
    virtual void call() const = 0;
    
    virtual ~AbstractLContainer() = default;
};

template <class Fn, class ...Args>
struct LContainerWrapper : AbstractLContainer {
    explicit LContainerWrapper(Fn &&fn, Args &&...args):
            _fn(std::move(fn)),
            _args(args...)
    {
        //
    }
    
    void call() const override {
        std::apply(_fn, _args);
    }

private:
    Fn _fn;
    std::tuple<Args...> _args;
};

} //namespace

struct LContainer {
    LContainer() {
        _ptr = null;
    }
    
    template <class Fn, class ...Args>
    LContainer(Fn fn, Args ...args) {
        _ptr = new LContainerWrapper<Fn, Args...>(std::move(fn), args...);
    }
    
    void operator()() const {
        _ptr->call();
    }

private:
    AbstractLContainer *_ptr;
};

#endif //__VGDE_VCONTAINER_H__
