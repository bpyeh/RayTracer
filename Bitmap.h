#pragma once

#include <fstream>

struct Pixel
{
    unsigned char r, g, b;
};

void saveBitmap(const char* filename, int w, int h, int dpi, Pixel* data);

class Color
{
private:
    double red, green, blue;
    double special; // used to describe a property of surfaces, reflectivity, pattern, etc

public:
    Color();
    Color(double, double, double, double);

    ~Color();

    double GetRed() const { return red; }
    void SetRed(double r) { red = r; }
    double GetGreen() const { return green; }
    void SetGreen(double g) { green = g; }
    double GetBlue() const { return blue; }
    void SetBlue(double b) { blue = b; }
    double GetSpecial() const { return special; }
    void SetSpecial(double s) { special = s; }

    double Brightness()
    {
        return (red + green + blue) / 3;
    }

    Color ScaleColor(double scalar);
    Color Add(Color color);
    Color Multiply(Color color);
    Color Average(Color color);

    Color Clip();
};