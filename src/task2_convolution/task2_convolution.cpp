// Task 2 - Implement a convolution filter
//        - Define a convolution filter
//        - Handle image boundary correctly as specified in assignment
//        - Correctly handle color manipulation

#include <ppgso/ppgso.h>

using namespace std;
using namespace ppgso;

const int kernel_height = 3;
const int kernel_width = 3;
//const float gaussian_blur = 256;
float kernel[kernel_height][kernel_width] {
    // TODO: Define the kernel
        {-1, -2, -1},
        {0 ,0 , 0},
        {1, 2, 1}
//        {1, 4, 6, 4, 1},
//        {4, 16, 24, 16, 4},
//        {6, 24, 36, 24, 6},
//        {4, 16, 24, 16, 4},
//        {1, 4, 6, 4, 1}
};
float kernel_factor = 1.0f;
float kernel_bias = 0.0f;

// //Clamp value to the <0, 1> range
//inline float clamp(float value) {
//  if(value > 1.0f) return 1.0f;
//  if(value < 0) return 0;
//  return value;
//}

unsigned char clamp(float color)
{
    if(color > 255)
        return 255;
    if(color < 0)
        return 0;

    return color;
}

int abs_value(int number)
{
    return number < 0 ? number * (-1) : number;
}

int main()
{
  // We will use ppgso::Image for the source and destination framebuffer this time
  ppgso::Image source = image::loadBMP("result.bmp");
    ppgso::Image source2 = image::loadBMP("transpone.bmp");
    ppgso::Image destination(source.width, source.height);

    float tr[kernel_width][kernel_height] = {0};
    for(int i = 0; i < kernel_width; ++i)
        for(int j = 0; j < kernel_height; ++j)
        {
            tr[j][i]= kernel[i][j];
        }

  // Traverse the source image, use Image::getPixel to get source samples
  for(int x = 0; x < source.width; x++) {
    for(int y = 0; y < source.height; y++) {
      // Store sum of colors
        float r = 0, g = 0, b = 0;
      // TODO: Compute convolution
      for(int k = -kernel_width/2; k <= kernel_width/2; k++)
      {
        for(int j = -kernel_height/2; j <= kernel_height/2; j++)
        {
            int mirror_x = x - j;
            int mirror_y = y - k;
            if(x - j < 0)
                mirror_x  = abs_value(x-j);
            if(x - j > source.width)
                mirror_x = source.width - j;
            if(y - k < 0)
                mirror_y  = abs_value(x-j);
            if(y - k > source.width)
                mirror_y = source.width - j;

          r += tr[j + kernel_width/2][k + kernel_height/2]*source.getPixel(mirror_x, mirror_y).r;
          g += tr[j + kernel_width/2][k + kernel_height/2]*source.getPixel(mirror_x, mirror_y).g;
          b += tr[j + kernel_width/2][k + kernel_height/2]*source.getPixel(mirror_x, mirror_y).b;
        }
      }

      // Store the result divided by factor

      destination.setPixel(x, y,
                           (source.getPixel(x, y).r) | source2.getPixel(x,y).r,
                           (source.getPixel(x, y).g) | source2.getPixel(x,y).g,
                           (source.getPixel(x, y).b) | source2.getPixel(x,y).b);
    }
  }

  // Save as result
  cout << "Generating task2_convolution.bmp file ..." << endl;
  image::saveBMP(destination, "transpone.bmp");

  cout << "Done." << endl;
  return EXIT_SUCCESS;
}
