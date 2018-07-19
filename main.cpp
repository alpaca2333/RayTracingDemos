#include "stdafx.h"
#include "ppm.h"
using namespace std;


Vector3 Color(const Ray& r)
{

}

int main()
{
    Color pix[100 * 200];
    for (int i = 0; i < 100; ++i)
    {
        for (int j = 0; j < 200; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                pix[i * 200 + j][k] = (i + 1365) * j * k % 256;
            }
        }
    }
    const char filePath[] = "d:\\a.ppm";
    WriteRGBImg(filePath, 200, 100, pix);
    system(filePath);
}