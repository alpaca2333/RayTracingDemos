#include "ppm.h"

using namespace std;
int WriteRGBImg(const char* path, int nx, int ny, Color *pix)
{
    std::ofstream fout(path);
    fout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int i = 0; i < nx * ny; ++i)
    {
        fout << (int) pix[i][0] << "\t" << (int) pix[i][1] << "\t" << (int) pix[i][2] << "\n";
    }
    fout.flush();
    return 0;
}


