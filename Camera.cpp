#include "stdafx.h"
#include "Camera.h"


Camera::Camera() :
    camPos(0, 0, 0),
    camDir(0, 0, 1), // looking in z direction
    camRight(0, 0, 0),
    camDown(0, 0, 0)
{
}

Camera::Camera(Vect3 pos, Vect3 dir, Vect3 right, Vect3 down) :
    camPos(pos),
    camDir(dir),
    camRight(right),
    camDown(down)
{

}

Camera::~Camera()
{
}
