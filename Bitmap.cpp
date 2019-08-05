#include "stdafx.h"
#include "Bitmap.h"

#include <cmath>

using namespace std;

Color::Color() : red(0.5), green(0.5), blue(0.5)
{
}

Color::Color(double r, double g, double b, double s) : red(r), green(g), blue(b), special(s)
{

}

Color::~Color()
{

}

Color Color::ScaleColor(double scalar)
{
    return Color(red * scalar, green * scalar, blue * scalar, special);
}

Color Color::Add(Color color)
{
    return Color(red + color.GetRed(), green + color.GetGreen(), blue + color.GetBlue(), special);
}

Color Color::Multiply(Color color)
{
    return Color(red * color.GetRed(), green * color.GetGreen(), blue * color.GetBlue(), special);
}

Color Color::Average(Color color)
{
    return Color((red + color.GetRed()) / 2, (green + color.GetGreen()) / 2, (blue + color.GetBlue()) / 2, special);

}

Color Color::Clip()
{
    double lum = red + green + blue;
    double excess = lum - 3;
    if (excess > 0)
    {
        red = red + excess * (red / excess);
        green = green + excess * (green / excess);
        blue = blue + excess * (blue / excess);
    }
    red = red < 0 ? 0 :
        red > 1 ? 1 : red;
    green = green < 0 ? 0 :
        green > 1 ? 1 : green;
    blue = blue < 0 ? 0 :
        blue > 1 ? 1 : blue;
    return Color(red, green, blue, special);
}

void saveBitmap(const char* filename, int w, int h, int dpi, Pixel* data)
{
    std::ofstream outFile(filename, std::ofstream::binary);
    int k = w * h; // number of pixels
    int s = 4 * k; 
    int fileSize = 54 + s;

    double factor = 39.375;
    int m = static_cast<int>(factor);
    int ppm = dpi * m;

    unsigned char bmpFileHeader[14] = {
        'B', 'M',
        0, 0, 0, 0,
        0, 0, 0, 0,
        54, 0, 0, 0
    };

    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        1, 0, 24, 0,
    };

    bmpFileHeader[2] = (unsigned char)(fileSize);
    bmpFileHeader[3] = (unsigned char)(fileSize >> 8);
    bmpFileHeader[4] = (unsigned char)(fileSize >> 16);
    bmpFileHeader[5] = (unsigned char)(fileSize >> 24);

    bmpInfoHeader[4] = (unsigned char)(w);
    bmpInfoHeader[5] = (unsigned char)(w >> 8);
    bmpInfoHeader[6] = (unsigned char)(w >> 16);
    bmpInfoHeader[7] = (unsigned char)(w >> 24);

    bmpInfoHeader[8] = (unsigned char)(h);
    bmpInfoHeader[9] = (unsigned char)(h >> 8);
    bmpInfoHeader[10] = (unsigned char)(h >> 16);
    bmpInfoHeader[11] = (unsigned char)(h >> 24);

    bmpInfoHeader[21] = (unsigned char)(s);
    bmpInfoHeader[22] = (unsigned char)(s >> 8);
    bmpInfoHeader[23] = (unsigned char)(s >> 16);
    bmpInfoHeader[24] = (unsigned char)(s >> 24);

    bmpInfoHeader[25] = (unsigned char)(ppm);
    bmpInfoHeader[26] = (unsigned char)(ppm >> 8);
    bmpInfoHeader[27] = (unsigned char)(ppm >> 16);
    bmpInfoHeader[28] = (unsigned char)(ppm >> 24);

    bmpInfoHeader[29] = (unsigned char)(ppm);
    bmpInfoHeader[30] = (unsigned char)(ppm >> 8);
    bmpInfoHeader[31] = (unsigned char)(ppm >> 16);
    bmpInfoHeader[32] = (unsigned char)(ppm >> 24);

    outFile.write((const char*)bmpFileHeader, sizeof(bmpFileHeader));
    outFile.write((const char*)bmpInfoHeader, sizeof(bmpInfoHeader));
    
    for (int i = 0; i < k; ++i)
    {
        unsigned char color[3] = { 
            (unsigned char)data[i].b,
            (unsigned char)data[i].g,
            (unsigned char)data[i].r,
        };
        outFile.write((const char*)color, sizeof(color));
    }
    outFile.close();
}