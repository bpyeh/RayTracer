#include "stdafx.h"
#include "Vector.h"
#include <cmath>

Vect3::Vect3() : x(0), y(0), z(0)
{
}

Vect3::Vect3(double x, double y, double z) : x(x), y(y), z(z)
{

}

Vect3::~Vect3()
{

}

Vect3 Vect3::Normalize()
{
    double norm = Magnitude();
    return Vect3(x / norm, y / norm, z / norm);
}

double Vect3::Magnitude()
{
    return sqrt(x * x + y * y + z * z);
}

// TODO: write overloading functions

Vect3 Vect3::operator-() const
{
    return Vect3(-x, -y, -z);
}

Vect3 Vect3::operator+(const Vect3& other) const
{
    return Vect3(x + other.x, y + other.y, z + other.z);
}

Vect3 Vect3::Negative()
{
    return Vect3(-x, -y, -z);
}

double Vect3::Dot(const Vect3& other)
{
    return x * other.GetX() + y * other.GetY() + z * other.GetZ();
}

Vect3 Vect3::Cross(const Vect3& other)
{
    return Vect3(
        y * other.GetZ() - z * other.GetY(),
        z * other.GetX() - x * other.GetZ(),
        x * other.GetY() - y * other.GetX()
    );
}

Vect3 Vect3::Add(const Vect3& other)
{
    return Vect3(x + other.GetX(), y + other.GetY(), z + other.GetZ());
}

Vect3 Vect3::operator*(double c) const
{
    return Vect3(x * c, y * c, z * c);
}

Vect3 Vect3::Mult(double c)
{
    return Vect3(x * c, y * c, z * c);
}