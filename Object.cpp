#include "stdafx.h"
#include "Object.h"
#include <cmath>

Object::Object()
{
}

Object::~Object()
{
}

Sphere::Sphere() :
    center(0, 0, 0),
    radius(1),
    color(0.5, 0.5, 0.5, 0)
{
}

Sphere::Sphere(Vect3 c, double r, Color col) :
    center(c),
    radius(r),
    color(col)
{

}

Sphere::~Sphere()
{
}

// note normal always points away from center of sphere
Vect3 Sphere::GetNormalAt(Vect3 point)
{
    Vect3 normal = point.Add(center.Negative()).Normalize();
    return normal;
}

// reference: 
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
double Sphere::FindIntersection(Ray ray)
{
    Vect3 ray_origin = ray.GetOrigin();
    double o_x = ray_origin.GetX();
    double o_y = ray_origin.GetY();
    double o_z = ray_origin.GetZ();

    Vect3 ray_dir = ray.GetDir();
    double d_x = ray_dir.GetX();
    double d_y = ray_dir.GetY();
    double d_z = ray_dir.GetZ();

    double s_x = center.GetX();
    double s_y = center.GetY();
    double s_z = center.GetZ();

    double a = 1; // normalized
    double b = (2 * (o_x - s_x) * d_x) + 
               (2 * (o_y - s_y) * d_y) +
               (2 * (o_z - s_z) * d_z);
    double c = pow(o_x - s_x, 2) +
               pow(o_y - s_y, 2) +
               pow(o_z - s_z, 2) - 
               (radius * radius);
    double discriminant = b * b - 4 * c;
    if (discriminant > 0)
    {
        // the ray intersects the sphere
        double root1 = ((-b - sqrt(discriminant)) / 2) - 0.000001;
        // return smallest positive root
        if (root1 > 0)
        {
            return root1;
        }
        else
        {
            double root2 = ((-b + sqrt(discriminant)) / 2) - 0.000001;
            return root2;
        }
    }
    else
    {
        // the ray doesn't intersect the sphere
        return -1;
    }
}

Plane::Plane() :
    normal(1, 0, 0), // in the X direction 
    dist(0),
    color(0.5, 0.5, 0.5, 0)
{
}

Plane::Plane(Vect3 norm, double d, Color c) :
    normal(norm),
    dist(d),
    color(c)
{

}

Vect3 Plane::GetNormalAt(Vect3 point)
{
    return normal;
}

double Plane::FindIntersection(Ray ray)
{
    Vect3 ray_direction = ray.GetDir();
    double a = ray_direction.Dot(normal);
    if (abs(a) < 0.00000001) // parallel
    {
        return -1;
    }
    else
    {
        // look on this page for explanation
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
        double b = normal.Dot(ray.GetOrigin().Add(normal.Mult(dist).Negative()));
        return -b / a;
    }
}

Plane::~Plane()
{
}

Triangle::Triangle() :
    A(1, 0, 0),
    B(0, 1, 0),
    C(0, 0, 1),
    color(0.5, 0.5, 0.5, 0)
{
}

// specify the points so that the normal is for the triangle p1p3p2
Triangle::Triangle(Vect3 p1, Vect3 p2, Vect3 p3, Color c) :
    A(p1),
    B(p2),
    C(p3),
    color(c)
{
    Vect3 AC(C.GetX() - A.GetX(), C.GetY() - A.GetY(), C.GetZ() - A.GetZ());
    Vect3 CB(B.GetX() - C.GetX(), B.GetY() - C.GetY(), B.GetZ() - C.GetZ());

    normal = AC.Cross(CB).Normalize();
    dist = normal.Dot(A); // any point on the triangle will do
}

Vect3 Triangle::GetNormalAt(Vect3 point)
{
    return normal;
}


double Triangle::FindIntersection(Ray ray)
{
    Vect3 ray_direction = ray.GetDir();
    Vect3 ray_orig = ray.GetOrigin();

    double a = ray_direction.Dot(normal);
    if (abs(a) < 0.00000001) // parallel
    {
        return -1;
    } else
    {
        // look on this page for explanation
        // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
        double b = normal.Dot(ray.GetOrigin().Add(normal.Mult(dist).Negative()));
        double distanceToPlane = -b / a;

        Vect3 Q = ray_direction.Mult(distanceToPlane).Add(ray_orig);
        Vect3 CA(C.GetX() - A.GetX(), C.GetY() - A.GetY(), C.GetZ() - A.GetZ());
        Vect3 AB(A.GetX() - B.GetX(), A.GetY() - B.GetY(), A.GetZ() - B.GetZ());
        Vect3 BC(B.GetX() - C.GetX(), B.GetY() - C.GetY(), B.GetZ() - C.GetZ());
        Vect3 QA(Q.GetX() - A.GetX(), Q.GetY() - A.GetY(), Q.GetZ() - A.GetZ());
        Vect3 QB(Q.GetX() - B.GetX(), Q.GetY() - B.GetY(), Q.GetZ() - B.GetZ());
        Vect3 QC(Q.GetX() - C.GetX(), Q.GetY() - C.GetY(), Q.GetZ() - C.GetZ());
        if (CA.Cross(QA).Dot(normal) >= 0
            && BC.Cross(QC).Dot(normal) >= 0
            && AB.Cross(QB).Dot(normal) >= 0)
        {
            return -b / a;
        }
        else
        {
            return -1;
        }
    }
}
