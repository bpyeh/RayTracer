#include "stdafx.h"
#include "Ray.h"


Ray::Ray() : origin(), dir(1, 0, 0)
{

}

Ray::Ray(Vect3 orig, Vect3 d) : origin(orig), dir(d)
{

}

Ray::~Ray()
{
}
