#ifndef VGDE_ACTOR_H
#define VGDE_ACTOR_H


class Actor {
public:
    Actor();
    virtual void act(float delta) = 0;
};


#endif //VGDE_ACTOR_H
