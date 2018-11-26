// Task 3 - Implement Bresenham drawing alg.
//        - Draw a star using lines
//        - Make use of std::vector to define the shape
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
//#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

#include <ppgso/ppgso.h>

//using namespace std;
using namespace ppgso;

// Size of the framebuffer
const unsigned int SIZE = 512;

struct Point {
  int x,y;
};

void brosehamWithError(ppgso::Image& framebuffer, Point& from, Point& to)
{
    int x0 = from.x;
    int x1 = to.x;
    int y0 = from.y;
    int y1 = to.y;
//
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        int pom = x0;
        x0 = y0;
        y0 = pom;
        pom = x1;
        x1 = y0;
        y1 = pom;
//        std::swap(x0, y0);
//        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        int pom = x0;
        x0 = x1;
        x1 = pom;
        pom = y0;
        y0 = y1;
        y1 = pom;
//        std::swap(x0, x1);
//        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            framebuffer.setPixel(round(x), round(y), 255, 255, 255);
        } else {
            framebuffer.setPixel(round(x), round(y), 255, 255, 255);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
//    if(dy < 0)
//        i =
//     else
//
//    dx = x1 - x0;
//    dy = (int) y1 - y0;
//    int D = 2*dy - dx;
//    y = y0
//
//    for(x = x0; x < x1; x++)
//    {
//        framebuffer.setPixel(round(x), round(y), 255, 255, 255);
//        if (D > 0)
//        {
//            y = y + 1;
//            D = D - 2*dx;
//        }
//        D = D + 2*dy;
//    }

} //

// Bresenham drawing algorithm
void drawLine(ppgso::Image& framebuffer, Point& from, Point& to)
{
    // TODO: Implement Bresenham drawing algorithm
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float steps;

    if (abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    float Xincrement = dx / (float) steps;
    float Yincrement = dy / (float) steps;

    float x = from.x;
    float y = from.y;
    for(int v=0; v < steps; v++)
    {
        x = x + Xincrement;
        y = y + Yincrement;
        framebuffer.setPixel(round(x), round(y), 255, 255, 255);
    }
}

int main()
{
  // Use ppgso::Image as our framebuffer
  ppgso::Image framebuffer(SIZE, SIZE);

  // TODO: Generate star points
  std::vector<Point> points;
for(int i = 0; i < 1000; i++)
{
    Point point;
    point.x = 50 * cos(2*M_PI*i/499) + 100;
    point.y = 50 * sin(2*M_PI*i/499) + 250;
    points.push_back(point);
}

    std::vector<Point> logo;
    Point point;
    point.x = 40;
    point.y = 240;
    logo.push_back(point);
    Point point2;
    point2.x = 120;
    point2.y = 240;
    logo.push_back(point2);
    Point point3;
    point3.x = 80;
    point3.y = 260;
    logo.push_back(point3);
    Point point4;
    point4.x = 160;
    point4.y = 260;
    logo.push_back(point4);


  // Draw lines
  for(unsigned int i = 0; i < points.size()-1; i++)
  {
      drawLine(framebuffer, points[i], points[i+1]);

      //brosehamWithError(framebuffer, points[i], points[i+1]);
//      for(unsigned int j = i; j < points.size()-1; j++)
//      {
//          //brosehamWithError(framebuffer, points[i], points[i+1]);
//          //drawLine(framebuffer, points[i], points[j]);
//      }

      //drawLine(framebuffer, points[i], points[j]);
  }

    for(unsigned int i = 0; i < logo.size()-1; i++)
    {
        //drawLine(framebuffer, logo[i], logo[i+1]);
        brosehamWithError(framebuffer, logo[i], logo[i+1]);

    }


  // Save the result
  std::cout << "Generating task3_bresenham.bmp file ..." << std::endl;
  image::saveBMP(framebuffer, "task3_bresenham.bmp");

  std::cout << "Done." << std::endl;
  return EXIT_SUCCESS;
}

