#include <cmath>
#include <cfloat>
#include <vector>
#include <iostream>
using namespace std;

// Cw, Ch表示画布宽高
// Vw, Vh表示视口宽高，d表示视口平面与相机距离
// Cx, Cy表示画布坐标
// Vx, Vy, Vz表示视口投影坐标
// 画布坐标转换为视口投影坐标
void canvas2viewport(int Cw, int Ch, int Cx, int Cy, int Vw, int Vh, int d,
                     int &Vx, int &Vy, int &Vz)
{
    Vx = Cx * Vw / Cw;
    Vy = Cy * Vh / Ch;
    Vz = d;
}

// d表示视口平面与相机距离
// X, Y, Z表示空间坐标
// Vx, Vy, Vz表示视口投影坐标
// 空间坐标转换为视口投影坐标
void space2viewport(int d, int X, int Y, int Z,
                    int &Vx, int &Vy, int &Vz)
{
    Vx = X * d / Z;
    Vy = Y * d / Z;
    Vz = d;
}

// Vw, Vh表示视口宽高，d表示视口平面与相机距离
// Cw, Ch表示画布宽高
// X, Y, Z表示空间坐标
// Cx, Cy表示画布坐标
// 空间坐标转换为画布坐标
void space2canvas(int Vw, int Vh, int d, int X, int Y, int Z, int Cw, int Ch,
                  int &Cx, int &Cy)
{
    int Vx;
    int Vy;
    int Vz;
    space2viewport(d, X, Y, Z, Vx, Vy, Vz);

    Cx = Vx * Cw / Vw;
    Cy = Vy * Ch / Vh;
}

struct Color
{
    int r;
    int g;
    int b;
    Color operator*(double s) const
    {
        return Color{int(r * s), int(g * s), int(b * s)};
    }
};

#define BACKGROUND_COLOR \
    Color { 0, 0, 0 }

struct Point
{
    double x;
    double y;
    double z;

    Point operator-(const Point &p) const
    {
        return Point{x - p.x, y - p.y, z - p.z};
    }
    Point operator+(const Point &p) const
    {
        return Point{x + p.x, y + p.y, z + p.z};
    }
    Point operator*(double s) const
    {
        return Point{x * s, y * s, z * s};
    }
    Point operator/(double s) const
    {
        return Point{x / s, y / s, z / s};
    }
    Point operator-() const
    {
        return Point{-x, -y, -z};
    }
};
double dot(const Point &a, const Point &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
Point canvas2viewport(int Cw, int Ch, int Cx, int Cy, int Vw, int Vh, int d)
{
    return Point{1.0 * Cx * Vw / Cw, 1.0 * Cy * Vh / Ch, 1.0 * d};
}
double length(const Point &p)
{
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}

struct Sphere
{
    Point center;
    double radius;
    Color color;
    int specular;
};

enum light_type
{
    ambient,
    point,
    directional,
};

struct Light
{
    light_type type;
    double intensity;
    Point position;
    Point direction;
};

struct Scene
{
    vector<Sphere> spheres;
    vector<Light> Lights;
};
Scene scene;

double ComputeLight(const Point &P, const Point &N, const Point &V, int s)
{
    double i = 0.0;
    for (auto light : scene.Lights)
    {
        switch (light.type)
        {
        case ambient:
            i += light.intensity;
            break;

        case point:
        {
            Point L = light.position - P;
            i += light.intensity * max(0.0, dot(N, L) / (length(N) * length(L))); // 漫反射

            if (s != -1)
            
            {
                Point R = N * dot(N, L) * 2 - L;
                double r_dot_v = dot(R, V);
                if (r_dot_v > 0.0)
                    i += light.intensity * pow(r_dot_v / (length(R) * length(V)), s); // 镜面反射
            }
        }
        break;

        case directional:
        {
            Point L = light.direction;
            i += light.intensity * max(0.0, dot(N, L) / (length(N) * length(L)));

            if (s != -1)
            {
                Point R = N * dot(N, L) * 2 - L;
                double r_dot_v = dot(R, V);
                if (r_dot_v > 0.0)
                    i += light.intensity * pow(r_dot_v / (length(R) * length(V)), s);
            }
        }
        break;
        }
    }
    return i;
}

bool IntersectRaySphere(const Point &O, const Point &D, const Sphere &sphere, double &t1, double &t2)
{
    double r = sphere.radius;
    Point CO = O - sphere.center;

    double a = dot(D, D);
    double b = 2 * dot(CO, D);
    double c = dot(CO, CO) - r * r;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0)
        return false;

    t1 = (-b + sqrt(discriminant)) / (2 * a);
    t2 = (-b - sqrt(discriminant)) / (2 * a);
    return true;
}

Color TraceRay(const Point &O, const Point &D, double t_min, double t_max)
{
    double t1;
    double t2;
    double closest_t = DBL_MAX;
    int ind = -1;
    for (int i = 0; i < scene.spheres.size(); ++i)
    {
        if (IntersectRaySphere(O, D, scene.spheres[i], t1, t2))
        {
            if (t1 >= t_min && t1 <= t_max && t1 < closest_t)
            {
                closest_t = t1;
                ind = i;
            }
            if (t2 >= t_min && t2 <= t_max && t2 < closest_t)
            {
                closest_t = t2;
                ind = i;
            }
        }
    }

    if (ind == -1)
        return BACKGROUND_COLOR;

    Point P = O + D * closest_t;             // 射线与球体的交点
    Point N = P - scene.spheres[ind].center; // 交点处的球面法线
    N = N / length(N);

    return scene.spheres[ind].color * ComputeLight(P, N, -D, scene.spheres[ind].specular);
}

struct Canvas
{
    void Putpixel(int x, int y, Color color)
    {
        if (color.r != 0 || color.g != 0 || color.b != 0)
            cout << "x: " << x << ", y: " << y << ", r: " << color.r << ", g: " << color.g << ", b: " << color.b << endl;
    }
};

int main()
{
    scene.spheres.push_back(Sphere{Point{0, -1, 3}, 1.0, Color{255, 0, 0}, 500});
    scene.spheres.push_back(Sphere{Point{2, 0, 4}, 1.0, Color{0, 0, 255}, 500});
    scene.spheres.push_back(Sphere{Point{-2, 0, 4}, 1.0, Color{0, 255, 0}, 10});
    scene.spheres.push_back(Sphere{Point{0, -5001, 0}, 5000.0, Color{255, 255, 0}, 1000});

    scene.Lights.push_back(Light{ambient, 0.2, Point{0, 0, 0}, Point{0, 0, 0}});
    scene.Lights.push_back(Light{point, 0.6, Point{2, 1, 0}, Point{0, 0, 0}});
    scene.Lights.push_back(Light{directional, 0.2, Point{0, 0, 0}, Point{1, 4, 4}});

    int Cw = 800;
    int Ch = 600;
    int Vw = 1;
    int Vh = 1;
    int d = 1;
    Point O{0, 0, 0};
    Canvas canvas;

    for (int x = -Cw / 2; x < Cw / 2; ++x)
    {
        for (int y = -Ch / 2; y < Ch / 2; ++y)
        {
            Point D = canvas2viewport(Cw, Ch, x, y, Vw, Vh, d);
            Color color = TraceRay(O, D, 1.0, DBL_MAX);
            canvas.Putpixel(x, y, color);
        }
    }

    return 0;
}
