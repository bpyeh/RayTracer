#include "stdafx.h"
#include "Light.h"

Source::Source()
{

}

Source::~Source()
{

}

Light::Light() : position(0, 0, 0), color(1, 1, 1, 0)
{
}

Light::Light(Vect3 pos, Color c) : position(pos), color(c)
{

}

Light::~Light()
{
}
