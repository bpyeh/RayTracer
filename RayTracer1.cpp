#include "stdafx.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstdlib>

#include "Bitmap.h"
#include "Vector.h"
#include "Ray.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"

using namespace std;

int FindIndexOfClosest(vector<double>& intersections)
{
    if (intersections.size() == 0)
    {
        return -1;
    }
    else if (intersections.size() == 1)
    {
        if (intersections[0] > 0)
        {
            return 0;
        }
        else
        {
            return -1; // this case happens if the ray from camera missed everything
        }
    }
    else
    {
        int out = 0;

        // could be optimized somewhat
        //double M = *max_element(intersections.begin(), intersections.end());
        double M = 0;
        for (int i = 0; i < intersections.size(); ++i)
        {
            if (M < intersections[i])
            {
                M = intersections[i];
            }
        }

        if (M > 0)
        {
            for (int i = 0; i < intersections.size(); ++i)
            {
                if (intersections[i] > 0 && intersections[i] <= M)
                {
                    M = intersections[i];
                    out = i;
                }
            }
            return out;
        }
        else
        {
            return -1; // all the intersections were negative
        }
    }
}

Color getColorAt(
    Vect3 pos, // intersection position
    Vect3 dir, // intersection ray direction
    vector<Object*>& scene_objects,
    int closestIndex,
    vector<Source*>& scene_lights,
    double eps,
    double ambientLight)
{
    Color closestObjectColor = scene_objects[closestIndex]->GetColor();
    Vect3 closestObjectNormal = scene_objects[closestIndex]->GetNormalAt(pos);
 
    if (closestObjectColor.GetSpecial() == 2)
    {
        // do a checkerboard
        int square = (int)floor(pos.GetX()) + (int)floor(pos.GetZ());
        if (square % 2 == 0)
        {
            closestObjectColor.SetRed(0);
            closestObjectColor.SetGreen(0);
            closestObjectColor.SetBlue(0);
        }
        else
        {
            closestObjectColor.SetRed(1);
            closestObjectColor.SetGreen(1);
            closestObjectColor.SetBlue(1);
        }
    }

    Color final_color = closestObjectColor.ScaleColor(ambientLight);

    if (closestObjectColor.GetSpecial() > 0 && closestObjectColor.GetSpecial() <= 1)
    {
        // reflections from objects with specular intensity
        double dot1 = closestObjectNormal.Dot(-dir);
        Vect3 scalar1 = closestObjectNormal * dot1;
        Vect3 add1 = scalar1 + dir;
        Vect3 scalar2 = add1 * 2.;
        Vect3 add2 = (-dir) + scalar2;
        Vect3 reflection_direction = add2.Normalize();
        
        Ray reflect_ray(pos, reflection_direction);

        // determine what the reflected ray intersects with first
        vector<double> reflection_intersections;
        for (int i = 0; i < scene_objects.size(); ++i)
        {
            reflection_intersections.push_back(scene_objects[i]->FindIntersection(reflect_ray));
        }
        
        int closestReflectedObjectInd = FindIndexOfClosest(reflection_intersections);

        if (closestReflectedObjectInd != -1)
        {
            if (reflection_intersections[closestReflectedObjectInd] > eps)
            {
                // determine the position and direction at the point of intersection with the reflected ray
                // the ray only affects the color if it reflected off something. 
                Vect3 reflection_intersection_pos = pos + reflection_direction.Mult(reflection_intersections[closestReflectedObjectInd]);
                Vect3 reflection_intersection_dir = reflection_direction;
                Color reflection_intersection_color = getColorAt(
                    reflection_intersection_pos, 
                    reflection_intersection_dir,
                    scene_objects,
                    closestReflectedObjectInd,
                    scene_lights,
                    eps,
                    ambientLight);
                final_color = final_color.Add(reflection_intersection_color.ScaleColor(closestObjectColor.GetSpecial()));
            }
        }
    }

    for (int i = 0; i < scene_lights.size(); ++i)
    {
        Vect3 light_dir = (scene_lights[i]->GetPosition() + (-pos)).Normalize();

        double cos = closestObjectNormal.Dot(light_dir); // cosine of angle, which is the dot product since both vectors are units
        if (cos > 0) // else the light source is behind the object
        {
            // test for shadows
            bool shadowed = false;
            Vect3 light_to_intersection = scene_lights[i]->GetPosition() + (-pos);
            double distance_to_light = light_to_intersection.Magnitude();

            Ray shadow_ray(pos, light_dir.Normalize());

            vector<double> intersections2; // holds intersection points of the shadow_ray. If this has values other than -1, the point will be in shadow
            for (int j = 0; j < scene_objects.size() && !shadowed; ++j)
            {
                intersections2.push_back(scene_objects[j]->FindIntersection(shadow_ray));
            }

            for (int j = 0; j < intersections2.size(); ++j)
            {
                if (intersections2[j] > eps)
                {
                    if (intersections2[j] <= distance_to_light)
                    {
                        shadowed = true;
                    }
                    break;
                }
            }
            
            if (!shadowed)
            {
                final_color = final_color.Add(closestObjectColor.Multiply(scene_lights[i]->GetColor().ScaleColor(cos)));

                if (closestObjectColor.GetSpecial() > 0 && closestObjectColor.GetSpecial() <= 1.0)
                {
                    double dot1 = closestObjectNormal.Dot(-dir);
                    Vect3 vec = closestObjectNormal * dot1;
                    Vect3 vec2 = vec + dir;
                    Vect3 vec3 = vec2 * 2;
                    Vect3 vec4 = (-dir) + vec3;
                    Vect3 reflect_dir = vec4.Normalize();

                    double specular = reflect_dir.Dot(light_dir);
                    if (specular > 0)
                    {
                        specular = pow(specular, 10);
                        final_color = final_color.Add(scene_lights[i]->GetColor().ScaleColor(specular * closestObjectColor.GetSpecial()));
                    }
                }
            }
        }
    }
    return final_color.Clip();
}

void makeCube(Vect3 p1, Vect3 p2, Color color)
{
    // TODO
}

int main()
{
    int dpi = 72;
    int width = 640;
    int height = 480;
    int n = width * height;
    Pixel* pixels = new Pixel[n];
    
    int aaDepth = 1; // 2 = sending out 4 rays per pixel
    double aaThreshold = 0.1;
    double aspectRatio = (double)width / height;
    double ambientLight = 0.2;
    double eps = 0.000001;

    Vect3 origin(0, 0, 0);
    Vect3 X(1, 0, 0); // X = Left/right
    Vect3 Y(0, 1, 0); // Y = Up/down
    Vect3 Z(0, 0, 1); // Z = Depth into the scene
    Vect3 second_sphere_loc(1.75, 0.25, 0);

    Vect3 camPos(3, 1.5, -4);
    Vect3 look_at(0, 0, 0); // point we want to look at
    Vect3 diff_btw(camPos.GetX() - look_at.GetX(), camPos.GetY() - look_at.GetY(), camPos.GetZ() - look_at.GetZ()); 
    Vect3 camDir = (-diff_btw).Normalize();
    Vect3 camRight = Y.Cross(camDir).Normalize();
    Vect3 camDown = camRight.Cross(camDir);
    Camera scene_camera(camPos, camDir, camRight, camDown);

    Color white(1, 1, 1, 0);
    Color green(0.5, 1.0, 0.5, 0.3);
    Color gray(0.5, 0.5, 0.5, 0);
    Color black(0, 0, 0, 0);
    Color maroon(0.75, 0.25, 0.25, 0);
    Color checkerboard(1, 1, 1, 2.);
    Color orange(0.94, 0.75, 0.31, 0);
    Vect3 light_position(-7, 10, -10);
    Light scene_light(light_position, white);
    vector<Source*> scene_lights;
    scene_lights.push_back(dynamic_cast<Source*>(&scene_light));

    // scene objects;
    Sphere scene_sphere(origin, 1, green);
    Sphere scene_sphere2(second_sphere_loc, 0.5, maroon);
    Plane scene_plane(Y, -1, checkerboard);
    Triangle scene_triangle(Vect3(3, 0, 0), Vect3(0, 3, 0), Vect3(0, 0, 3), orange);
    vector<Object*> scene_objects;
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_sphere2));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_plane));
    scene_objects.push_back(dynamic_cast<Object*>(&scene_triangle));

    double xamnt, yamnt;

    int k, aaInd;

    const int LEN = aaDepth * aaDepth;
    double* tempRed = new double[LEN];
    double* tempGreen = new double[LEN];
    double* tempBlue = new double[LEN];

    auto start = chrono::high_resolution_clock::now();
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            k = r * width + c;

            for (int subr = 0; subr < aaDepth; ++subr) // anti aliasing loops
            {
                for (int subc = 0; subc < aaDepth; ++subc)
                {
                    aaInd = subr * aaDepth + subc;
                    srand(time(0));

                    if (aaDepth == 1) // no antialiasing
                    {
                        // camera is pointing at center of screen.
                        // xamnt and yamnt are offsets from the center to pixel at (r,c)
                        // this page might have an explanation:
                        // https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays/generating-camera-rays
                        if (width > height)
                        {
                            xamnt = ((c + 0.5) / width) * aspectRatio - (((width - height) / (double)height) / 2);
                            yamnt = ((height - r) + 0.5) / height;
                        } else
                        {
                            xamnt = ((width - c) + 0.5) / width;
                            yamnt = (((height - r) + 0.5) / height) / aspectRatio - (((height - width) / (double)width) / 2);
                        }
                    }
                    else
                    {
                        if (width > height)
                        {
                            xamnt = ((c + ((double)subc/((double)aaDepth - 1))) / width) * aspectRatio - (((width - height) / (double)height) / 2);
                            yamnt = ((height - r) + ((double)subc / ((double)aaDepth - 1))) / height;
                        } else
                        {
                            xamnt = ((width - c) + ((double)subc / ((double)aaDepth - 1))) / width;
                            yamnt = (((height - r) + ((double)subc / ((double)aaDepth - 1))) / height) / aspectRatio - (((height - width) / (double)width) / 2);
                        }
                    }

                    Vect3 cam_ray_origin = scene_camera.GetCamPos();
                    Vect3 cam_ray_dir = (camDir + (camRight.Mult(xamnt - 0.5) + (camDown.Mult(yamnt - 0.5)))).Normalize();
                    Ray cam_ray(cam_ray_origin, cam_ray_dir);

                    vector<double> intersections;

                    // determine if the ray we just created intersects with any of the objects in our scene
                    for (int i = 0; i < scene_objects.size(); ++i)
                    {
                        intersections.push_back(scene_objects[i]->FindIntersection(cam_ray));
                    }

                    int closestIndex = FindIndexOfClosest(intersections);
                    if (closestIndex == -1)
                    {
                        tempRed[aaInd] = 0;
                        tempGreen[aaInd] = 0;
                        tempBlue[aaInd] = 0;
                    } else
                    {
                        if (intersections[closestIndex] > eps)
                        {
                            Vect3 intersection_pos = cam_ray_origin + cam_ray_dir.Mult(intersections[closestIndex]);
                            Vect3 intersection_dir = cam_ray_dir;

                            // color at intersection
                            Color color = getColorAt(
                                intersection_pos,
                                intersection_dir,
                                scene_objects,
                                closestIndex,
                                scene_lights,
                                eps,
                                ambientLight);
                            double r2 = color.GetRed();
                            double g2 = color.GetGreen();
                            double b2 = color.GetBlue();
                            tempRed[aaInd] = r2;
                            tempGreen[aaInd] = g2;
                            tempBlue[aaInd] = b2;
                        }
                    }
                }
            }
            
            // average the subpixel colors
            double totalRed = 0;
            double totalGreen = 0;
            double totalBlue = 0;
            for (int i = 0; i < LEN; ++i)
            {
                totalRed += tempRed[i];
                totalGreen += tempGreen[i];
                totalBlue += tempBlue[i];
            }

            pixels[k].r = (unsigned char)(totalRed * 255. / LEN);
            pixels[k].g = (unsigned char)(totalGreen * 255. / LEN);
            pixels[k].b = (unsigned char)(totalBlue * 255. / LEN);
        }
    }
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s\n";

    saveBitmap("scene.bmp", width, height, dpi, pixels);

    delete[] pixels;
    cout << "done" << endl;

    return 0;
}

