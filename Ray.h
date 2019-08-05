#pragma once

#include "Vector.h"

class Ray
{
private:
    Vect3 origin, dir;
public:
    Ray();
    Ray(Vect3, Vect3);

    ~Ray();

    Vect3 GetOrigin() const { return origin; }
    Vect3 GetDir() const { return dir; }
};
