#pragma once

// base class for an object to be rendered in the scene

#include "Ray.h"
#include "Bitmap.h"

// TODO: create a Point class?
class Object // TODO: make purely virtual
{
private:
public:
    Object();
    ~Object();

    virtual Color GetColor() const { return Color(0, 0, 0, 0); }
    // returns scalar which is the distance from the origin of the ray to point of intersection
    virtual double FindIntersection(Ray ray) { return 0; }
    virtual Vect3 GetNormalAt(Vect3) { return Vect3(0, 0, 0); };
};

class Sphere : public Object
{
private:
    Vect3 center;
    double radius;
    Color color;
public:
    Sphere();
    Sphere(Vect3, double, Color);

    ~Sphere();

    Vect3 GetCenter() const { return center; }
    double GetRadius() const { return radius; }
    Color GetColor() const { return color; }

    Vect3 GetNormalAt(Vect3);
    double FindIntersection(Ray);
};

class Plane : public Object
{
private:
    Vect3 normal;
    double dist; // distance from origin
    Color color;
public:
    Plane();
    Plane(Vect3, double, Color);

    ~Plane();

    Vect3 GetNormal() const { return normal; }
    double GetDistance() const { return dist; }
    Color GetColor() const { return color; }

    Vect3 GetNormalAt(Vect3);
    double FindIntersection(Ray);
};

class Triangle : public Object
{
private:
    Vect3 A, B, C;
    Vect3 normal;
    double dist; // this not distance from origin to the triangle exactly. It's the distance to the plane containing the triangle
    Color color;
public:
    Triangle();
    Triangle(Vect3, Vect3, Vect3, Color);

    Vect3 GetNormal() const { return normal; }
    double GetDistance() const { return dist; }
    Color GetColor() const { return color; }

    Vect3 GetNormalAt(Vect3);
    double FindIntersection(Ray);
};