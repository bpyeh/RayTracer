#pragma once

class Vect3
{
private:
    double x, y, z;
public:
    Vect3();
    Vect3(double, double, double);

    ~Vect3();

    double GetX() const { return x; }
    double GetY() const { return y; }
    double GetZ() const { return z; }

    Vect3 Normalize();
    double Magnitude();
    double Dot(const Vect3&);
    Vect3 Cross(const Vect3&);
    Vect3 Negative();

    Vect3 operator -() const;
    Vect3 operator +(const Vect3&) const;
    Vect3 Add(const Vect3&);
    Vect3 operator *(double) const;
    Vect3 Mult(double);
};