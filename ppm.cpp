#include "ppm.h"

int WriteRGBImg(const char* path, int nx, int ny, Color *pix)
{
    std::ofstream fout(path);
    fout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int i = 0; i < nx * ny; ++i)
    {
        fout << pix[i][0] << "\t" << pix[i][1] << "\t" << pix[i][2] << "\n"; 
    }
    fout.flush();
    return 0;
}