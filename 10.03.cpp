#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

struct Vertice
{
    int x;
    int y;
    int z;
    Vertice operator+(const Vertice &v) const
    {
        return Vertice{x + v.x, y + v.y, z + v.z};
    }
    Vertice operator*(double scale) const
    {
        return Vertice{int(x * scale), int(y * scale), int(z * scale)};
    }
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

struct Model
{
    vector<Vertice> vertices;
    vector<Triangle> triangles;
};

struct Transform
{
    double scale;
    double rotation; // 绕y轴旋转角度
    Vertice translation;
};

struct Instance
{
    Model model;
    Vertice position;
    Transform transform;
};

static int d = 1;
Vertice ProjectVertex(const Vertice &V)
{
    return Vertice{V.x * d / V.z, V.y * d / V.z, d};
}

void DrawWireframeTriangle(const Vertice &P0, const Vertice &P1, const Vertice &P2, Color color)
{
}
void RenderTriangle(const Triangle &triangle, const vector<Vertice> &projected)
{
    DrawWireframeTriangle(projected[triangle.v[0]], projected[triangle.v[1]], projected[triangle.v[2]], triangle.color);
}

Vertice Scale(const Vertice &vertex, double scale)
{
    return vertex * scale;
}
Vertice Rotate(const Vertice &vertex, double theta) // 绕y轴旋转theta弧度
{
    double x = cos(theta) * vertex.x + sin(theta) * vertex.z;
    double y = vertex.y;
    double z = -sin(theta) * vertex.x + cos(theta) * vertex.z;
    return Vertice{int(x), int(y), int(z)};
}
Vertice Translate(const Vertice &vertex, const Vertice &translation)
{
    return vertex + translation;
}

Vertice ApplyTransform(const Vertice &vertex, const Transform &transform)
{
    auto scaled = Scale(vertex, transform.scale);
    auto rotated = Rotate(scaled, transform.rotation);
    auto translated = Translate(rotated, transform.translation);
    return translated;
}
void RenderInstance(const Instance &instance)
{
    vector<Vertice> projected;
    auto model = instance.model;
    for (auto V : model.vertices)
    {
        auto V_p = ApplyTransform(V, instance.transform) + instance.position;
        projected.emplace_back(ProjectVertex(V_p));
    }
    for (auto T : model.triangles)
        RenderTriangle(T, projected);
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

    Model model{vertices, triangles};
    Vertice position{1, 2, 3};
    Transform transform{1.5, 45, Vertice{1, 2, 3}};
    Instance instance{model, position, transform};
    RenderInstance(instance);

    return 0;
}
