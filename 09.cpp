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
void SpaceToViewPort(int x, int y, int z, int d, int &Vx, int &Vy, int &Vz)
{
    Vx = d * x / z;
    Vy = d * y / z;
    Vz = d;
}

struct Point
{
    int x;
    int y;
    int z;
};
struct Color
{
    int r;
    int g;
    int b;
};
Point ViewPortToCanvas(Point v, int Vw, int Vh, int Cw, int Ch)
{
    return Point{v.x * Cw / Vw, v.y * Ch / Vh, v.z};
}
Point ProjectVertex(Point v, int d = 1, int Vw = 1, int Vh = 1, int Cw = 800, int Ch = 600)
{
    Point vp{v.x * d / v.z, v.y * d / v.z, d};
    return ViewPortToCanvas(vp, Vw, Vh, Cw, Ch);
}
struct Canvas
{
    void DrawLine(Point a, Point b, Color r)
    {
    }
};

#define BLUE \
    Color { 0, 0, 255 }
#define RED \
    Color { 255, 0, 0 }
#define GREEN \
    Color { 0, 255, 0 }

int main()
{
    //"前面"4个顶点
    Point vAf{-1, 1, 1};
    Point vBf{1, 1, 1};
    Point vCf{1, -1, 1};
    Point vDf{-1, -1, 1};

    //"背面"4个顶点
    Point vAb{-1, 1, 2};
    Point vBb{1, 1, 2};
    Point vCb{1, -1, 2};
    Point vDb{-1, -1, 2};

    Canvas canvas;

    // 前面
    canvas.DrawLine(ProjectVertex(vAf), ProjectVertex(vBf), BLUE);
    canvas.DrawLine(ProjectVertex(vBf), ProjectVertex(vCf), BLUE);
    canvas.DrawLine(ProjectVertex(vCf), ProjectVertex(vDf), BLUE);
    canvas.DrawLine(ProjectVertex(vDf), ProjectVertex(vAf), BLUE);

    // 背面
    canvas.DrawLine(ProjectVertex(vAb), ProjectVertex(vBb), RED);
    canvas.DrawLine(ProjectVertex(vBb), ProjectVertex(vCb), RED);
    canvas.DrawLine(ProjectVertex(vCb), ProjectVertex(vDb), RED);
    canvas.DrawLine(ProjectVertex(vDb), ProjectVertex(vAb), RED);

    // 连接前、后面的4条边
    canvas.DrawLine(ProjectVertex(vAf), ProjectVertex(vAb), GREEN);
    canvas.DrawLine(ProjectVertex(vBf), ProjectVertex(vBb), GREEN);
    canvas.DrawLine(ProjectVertex(vCf), ProjectVertex(vCb), GREEN);
    canvas.DrawLine(ProjectVertex(vDf), ProjectVertex(vDb), GREEN);

    return 0;
}
