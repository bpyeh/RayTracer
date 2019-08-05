#pragma once

#include "Vector.h"
#include "Bitmap.h"
#include "Object.h"

// base object for classes such as Light to inherit from
class Source
{
private:
public:
    Source();
    ~Source();

    virtual Vect3 GetPosition() const { return Vect3(0, 0, 0); }
    virtual Color GetColor() const { return Color(1, 1, 1, 0); }
};

class Light : public Source
{
private:
    Vect3 position;
    Color color;
public:
    Light();
    Light(Vect3, Color);

    ~Light();

    Vect3 GetPosition() const { return position; }
    Color GetColor() const { return color; }
};

