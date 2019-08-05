#pragma once
#include "Vector.h"
class Camera
{
private:
    Vect3 camPos, camDir, camRight, camDown;

public:
    Camera();
    Camera(Vect3, Vect3, Vect3, Vect3);

    ~Camera();
    
    Vect3 GetCamPos() const { return camPos; }
    Vect3 GetCamDir() const { return camDir; }
    Vect3 GetCamRight() const { return camRight; }
    Vect3 GetCamDown() const { return camDown; }
};

