// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>

using namespace std;

// Size of the framebuffer
const unsigned int SIZE = 512;
const float BRIGHTNESS_CHANGE = 2f;
const float CONTRAST = 100;
const unsigned char UNSIGNED_CHAR_SIZE = 255;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel {
  // TODO: Define correct data type for r, g, b channel
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};

unsigned char clamp(float colorValue)
{
  if(colorValue > UNSIGNED_CHAR_SIZE)
    return UNSIGNED_CHAR_SIZE;

  if(colorValue < 0)
    return 0;

  return colorValue;
}

int main()
{
  // Initialize a framebuffer
  //auto framebuffer = new Pixel[SIZE][SIZE];
  Pixel framebuffer[SIZE][SIZE];
  // TODO: Read data to framebuffer and close the file
  FILE *fileRead = fopen("lena.raw","rb");

  // TODO: Open file lena.raw (this is 512x512 RAW GRB format)
  if( fileRead )
  {
    fread(framebuffer, sizeof(Pixel), SIZE*SIZE, fileRead );
  }
  fclose(fileRead);

  // TODO: Read data to framebuffer and close the file

  // Traverse the framebuffer
  for (unsigned int y = 0; y < SIZE; y++) {
    for (unsigned int x = 0; x < SIZE; x++) {

      //brightness
      framebuffer[y][x].r = clamp(framebuffer[y][x].r * BRIGHTNESS_CHANGE);
      framebuffer[y][x].g = clamp(framebuffer[y][x].g * BRIGHTNESS_CHANGE);
      framebuffer[y][x].b = clamp(framebuffer[y][x].b * BRIGHTNESS_CHANGE);

      //contrast
      float contr_factor = (259.0 * (CONTRAST + 255.0 )) / (255.0 * ( 259.0 - CONTRAST));
      framebuffer[y][x].r = clamp(contr_factor * (framebuffer[y][x].r - 128) + 128);
      framebuffer[y][x].g = clamp(contr_factor * (framebuffer[y][x].g - 128) + 128);
      framebuffer[y][x].b = clamp(contr_factor * (framebuffer[y][x].b - 128) + 128);

    }
  }

  // TODO: Open file result.raw
  FILE * pFile;
  pFile = fopen ("C:\\Users\\HP\\Desktop\\5. semester\\PPGSO\\ppgso-public\\ppgso-public\\data\\result.raw", "wb");

  cout << "Generating result.raw file ..." << endl;

  // TODO: Write the framebuffer to the file and close it
  fwrite(framebuffer, sizeof(Pixel), SIZE*SIZE, pFile);
  fclose(pFile);
  
  cout << "Done." << endl;
  delete[] framebuffer;
  return EXIT_SUCCESS;
}
