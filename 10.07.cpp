#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

struct Vertice
{
    double x;
    double y;
    double z;
    double w;
    Vertice operator*(double scale) const
    {
        return Vertice{x * scale, y * scale, z * scale, w * scale};
    }
    Vertice operator/(double scale) const
    {
        return Vertice{x / scale, y / scale, z / scale, w / scale};
    }
    Vertice one() const
    {
        double scale = w;
        return *this / scale;
    }
};
Vertice cross(const Vertice &v1, const Vertice &v2)
{
    return Vertice{
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        0,
    };
}

struct Triangle
{
    int v[3];
};

struct Matrix
{
    double m[4][4];
    Vertice operator*(const Vertice &v) const
    {
        double x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
        double y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
        double z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
        double w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
        return Vertice{x, y, z, w};
    }
    Matrix operator*(const Matrix &matrix) const
    {
        Matrix result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.m[i][j] = 0.0;
                for (int k = 0; k < 4; k++)
                    result.m[i][j] += m[i][k] * matrix.m[k][j];
            }
        }
        return result;
    }
};

Matrix scaleMatrix(double sx, double sy, double sz)
{
    return Matrix{{{sx, 0, 0, 0},
                   {0, sy, 0, 0},
                   {0, 0, sz, 0},
                   {0, 0, 0, 1}}};
}
Matrix rotateMatrix(double nx, double ny, double nz, double theta)
{
    return Matrix{{{nx * nx * (1 - cos(theta)) + cos(theta), nx * ny * (1 - cos(theta)) - nz * sin(theta), nx * nz * (1 - cos(theta)) + ny * sin(theta), 0},
                   {nx * ny * (1 - cos(theta)) + nz * sin(theta), ny * ny * (1 - cos(theta)) - cos(theta), ny * nz * (1 - cos(theta)) - nx * sin(theta), 0},
                   {nx * nz * (1 - cos(theta)) - ny * sin(theta), ny * nz * (1 - cos(theta)) + nx * sin(theta), nz * nz * (1 - cos(theta)) + cos(theta), 0},
                   {0, 0, 0, 1}}};
}
Matrix rotateMatrix(double nx, double ny, double nz, double ux, double uy, double uz)
{
    Vertice z{nx, ny, nz, 0};
    Vertice x = cross({ux, uy, uz, 0}, z);
    Vertice y = cross(z, x);
    return Matrix{{{x.x, x.y, x.z, 0},
                   {y.x, y.y, y.z, 0},
                   {z.x, z.y, z.z, 0},
                   {0, 0, 0, 1}}};
}
Matrix translateMatrix(double tx, double ty, double tz)
{
    return Matrix{{{0, 0, 0, tx},
                   {0, 0, 0, ty},
                   {0, 0, 0, tz},
                   {0, 0, 0, 1}}};
}
Matrix perspectiveProjectionMatrix(double d)
{
    return Matrix{{{d, 0, 0, 0},
                   {0, d, 0, 0},
                   {0, 0, 1, 0},
                   {0, 0, 1, 0}}};
}
Matrix viewport2canvasMappingMatrix(double vw, double vh, double cw, double ch)
{
    return Matrix{{{cw / vw, 0, 0, 0},
                   {0, ch / vh, 0, 0},
                   {0, 0, 1, 0},
                   {0, 0, 1, 0}}};
}
Matrix projectMatrix(double d, double vw, double vh, double cw, double ch)
{
    return viewport2canvasMappingMatrix(vw, vh, cw, ch) * perspectiveProjectionMatrix(d);
    /*
    return Matrix{{{d * cw / vw, 0, 0, 0},
                   {0, d * ch / vh, 0, 0},
                   {0, 0, 1, 0},
                   {0, 0, 1, 0}}};
    */
}

struct Model
{
    vector<Vertice> vertices;
    vector<Triangle> triangles;
};

struct Transform
{
    double sx, sy, sz;
    double nx, ny, nz, theta;
    double tx, ty, tz;
    Matrix modelMatrix() const
    {
        return translateMatrix(tx, ty, tz) * rotateMatrix(nx, ny, nz, theta) * scaleMatrix(sx, sy, sz);
    }
};

struct Instance
{
    Model model;
    Transform transform;
};

struct Camera
{
    Vertice position;
    Vertice orientation;
    Vertice up;
    double d;
    double vw, vh;
    double cw, ch;
    Matrix invertTranslate() const
    {
        return translateMatrix(-position.x, -position.y, -position.z);
    }
    Matrix invertRotate() const
    {
        return rotateMatrix(orientation.x, orientation.y, orientation.z, up.x, up.y, up.z);
    }

    Matrix mpMatrix() const
    {
        return projectMatrix(d, vw, vh, cw, ch);
    }
    Matrix rtMatrix() const
    {
        return invertRotate() * invertTranslate();
    }
    Matrix cameraMatrix() const
    {
        return mpMatrix() * rtMatrix();
    }
};

struct Scene
{
    vector<Instance> instances;
    Camera camera;
};

Vertice ProjectVertex(const Vertice &V)
{
    return V.one();
}
void DrawWireframeTriangle(const Vertice &P0, const Vertice &P1, const Vertice &P2)
{
}
void RenderTriangle(const Triangle &triangle, const vector<Vertice> &projected)
{
    DrawWireframeTriangle(projected[triangle.v[0]], projected[triangle.v[1]], projected[triangle.v[2]]);
}
void RenderModel(const Model &model, const Matrix &M)
{
    vector<Vertice> projected;
    for (auto V : model.vertices)
    {
        projected.emplace_back(ProjectVertex(M * V));
    }
    for (auto T : model.triangles)
        RenderTriangle(T, projected);
}
void RenderScene(const Scene &scene)
{
    Camera camera = scene.camera;
    Matrix M_camera = camera.cameraMatrix();

    for (const Instance &I : scene.instances)
    {
        Matrix M_model = I.transform.modelMatrix();
        Matrix M = M_camera * M_model;
        RenderModel(I.model, M);
    }
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
        {{0, 1, 2}},
        {{0, 2, 3}},
        {{4, 0, 3}},
        {{4, 3, 7}},
        {{5, 4, 7}},
        {{5, 7, 6}},
        {{1, 5, 6}},
        {{1, 6, 2}},
        {{4, 5, 1}},
        {{4, 1, 0}},
        {{2, 6, 7}},
        {{2, 7, 3}},
    };
    Model model{vertices, triangles};

    Transform transform{0.5, 0.5, 0.5, 0, 0, 1, 3.14 / 2, 1, 2, 3};
    Instance instance{model, transform};

    Camera camera{Vertice{0, 0, 0}, Vertice{0, 0, 1}, Vertice{0, 1, 0}, 1, 1, 800, 600};

    Scene scene{{instance}, camera};
    RenderScene(scene);

    return 0;
}
