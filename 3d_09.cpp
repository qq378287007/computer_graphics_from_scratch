#include <cmath>
#include <vector>
using namespace std;


// 视口坐标转换为画布坐标
// 视口坐标(Vx, Vy)和视口宽高(Vw, Vh)
// 画布坐标(Cx, Cy)和画布宽高(Cw, Ch)
void ViewPortToCanvas(int Vx, int Vy, int Vw, int Vh, int Cw, int Ch, int &Cx, int &Cy)
{
    Cx = Vx * Cw / Vw;
    Cy = Vy * Ch / Vh;
}

// 空间坐标转换为视口投影坐标
// 空间坐标(x, y, z)
// 视口平面与相机距离d
// 视口投影坐标(Vx, Vy, Vz)
void ViewPortToCanvas(int x, int y, int z, int d, int &Vx, int &Vy, int &Vz)
{
    Vx = d * x / z;
    Vy = d * y / z;
    Vz = d;
}

int main()
{
    return 0;
}
