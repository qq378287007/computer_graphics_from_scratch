#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

struct Vertice
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
#define red \
    Color { 255, 0, 0 }
#define green \
    Color { 0, 255, 0 }
#define blue \
    Color { 0, 0, 255 }
#define yellow \
    Color { 255, 255, 0 }
#define purple \
    Color { 128, 0, 128 }
#define cyan \
    Color { 0, 255, 255 }

struct Triangle
{
    int v[3];
    Color color;
};

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

static int d = 1;
Vertice ProjectVertex(const Vertice &V)
{
    Vertice V2;
    V2.x = V.x * d / V.z;
    V2.y = V.y * d / V.z;
    V2.z = d;
    return V2;
}
void DrawWireframeTriangle(const Vertice &P0, const Vertice &P1, const Vertice &P2, Color color)
{
}
void RenderTriangle(const Triangle &triangle, const vector<Vertice> &projected)
{
    DrawWireframeTriangle(projected[triangle.v[0]], projected[triangle.v[1]], projected[triangle.v[2]], triangle.color);
}
void RenderObject(const vector<Vertice> &vertices, const vector<Triangle> &triangles, const Vertice &T = {0, 0, 0})
{
    Vertice Vp;
    vector<Vertice> projected;
    for (const Vertice &V : vertices)
    {
        Vp.x = V.x + T.x;
        Vp.y = V.y + T.y;
        Vp.z = V.z + T.z;
        projected.emplace_back(ProjectVertex(Vp));
    }
    for (const Triangle &t : triangles)
        RenderTriangle(t, projected);
}

int main()
{
    vector<Vertice> vertices = {
        {1, 1, 1},
        {-1, 1, 1},
        {-1, -1, 1},
        {1, -1, 1},
        {1, 1, -1},
        {-1, 1, -1},
        {-1, -1, -1},
        {1, -1, -1},
    };
    vector<Triangle> triangles = {
        {{0, 1, 2}, red},
        {{0, 2, 3}, red},
        {{4, 0, 3}, green},
        {{4, 3, 7}, green},
        {{5, 4, 7}, blue},
        {{5, 7, 6}, blue},
        {{1, 5, 6}, yellow},
        {{1, 6, 2}, yellow},
        {{4, 5, 1}, purple},
        {{4, 1, 0}, purple},
        {{2, 6, 7}, cyan},
        {{2, 7, 3}, cyan},
    };
    RenderObject(vertices, triangles);

    return 0;
}
