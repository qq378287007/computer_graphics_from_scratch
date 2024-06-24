#include <vector>
#include <cmath>
using namespace std;

struct Vertex
{
    double x;
    double y;
    double z;
    double distance2(const Vertex &v) const
    {
        return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z - v.z);
    }
    double operator*(const Vertex &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    Vertex operator*(double t) const
    {
        return Vertex{x * t, y * t, z * t};
    }
    Vertex operator-(const Vertex &v) const
    {
        return Vertex{x - v.x, y - v.y, z - v.z};
    }
    Vertex operator+(const Vertex &v) const
    {
        return Vertex{x + v.x, y + v.y, z + v.z};
    }
};
Vertex centerVertex(const vector<Vertex> &vertexs)
{
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    for (const Vertex &vertex : vertexs)
    {
        x += vertex.x;
        y += vertex.y;
        z += vertex.z;
    }
    const int num = vertexs.size();
    return Vertex{x / num, y / num, z / num};
}
double radiusVertex(const vector<Vertex> &vertexs, const Vertex &center)
{
    double r2 = 0.0;
    for (const Vertex &vertex : vertexs)
    {
        double d2 = center.distance2(vertex);
        if (r2 < d2)
            r2 = d2;
    }
    return sqrt(r2);
}

struct Plane
{
    Vertex normal; // 单位向量
    double d;      // N*P=d

    double distance(const Vertex &vertex) const
    {
        return normal.x * vertex.x + normal.y * vertex.y + normal.z * vertex.z - d;
    }
};

struct LineSegment
{
    Vertex A;
    Vertex B;
    Vertex intersect(const Plane &p) const
    {
        double t = (p.d - p.normal * A) / (p.normal * (B - A));
        return A + (B - A) * t;
    }
};

struct Triangle
{
    int v[3];
};

struct Sphere
{
    Vertex center;
    double radius;
};

struct Instance
{
    vector<Vertex> vertexs;
    vector<Triangle> triangles;
    Sphere bound_sphere;

    void addVertex(const Vertex &vertex)
    {
        vertexs.push_back(vertex);

        Vertex center = centerVertex(vertexs);
        double radius = radiusVertex(vertexs, center);
        bound_sphere.center = center;
        bound_sphere.radius = radius;
    }
};

struct Scene
{
    vector<Instance> instances;
};

struct Camera
{
    double Vw;
    double Vh;
    double d;
    double near_distance;
    double far_distance;
    Plane nearPlane() const
    {
        Plane plane;
        plane.normal = Vertex{0, 0, -1};
        plane.d = -near_distance;
        return plane;
    }
    Plane farPlane() const
    {
        Plane plane;
        plane.normal = Vertex{0, 0, 1};
        plane.d = far_distance;
        return plane;
    }
    Plane upPlane() const
    {
        Plane plane;
        double dd = sqrt(Vh / 2 * Vh / 2 + d * d);
        plane.normal = Vertex{0, -d / dd, Vh / 2 / dd};
        plane.d = 0.0;
        return plane;
    }
    Plane downPlane() const
    {
        Plane plane;
        double dd = sqrt(Vh / 2 * Vh / 2 + d * d);
        plane.normal = Vertex{0, -d / dd, -Vh / 2 / dd};
        plane.d = 0.0;
        return plane;
    }
    Plane leftPlane() const
    {
        Plane plane;
        double dd = sqrt(Vw / 2 * Vw / 2 + d * d);
        plane.normal = Vertex{-d / dd, 0, -Vw / 2 / dd};
        plane.d = 0.0;
        return plane;
    }
    Plane rightPlane() const
    {
        Plane plane;
        double dd = sqrt(Vw / 2 * Vw / 2 + d * d);
        plane.normal = Vertex{d / dd, 0, -Vw / 2 / dd};
        plane.d = 0.0;
        return plane;
    }
};

bool ClipInstance(const Instance &instance, const vector<Plane> &planes, Instance &clipped_instance)
{
    bool flag = true;
    for (const Plane &plane : planes)
    {
        if(plane.distance(instance.bound_sphere.center) > instance.bound_sphere.radius)
        {
            flag = false;
            break;
        }
    }
    if (flag)//内部
    {
        clipped_instance = instance;
        return true;
    } 
    
    flag = true;
    for (const Plane &plane : planes)
    {
        if(plane.distance(instance.bound_sphere.center) < instance.bound_sphere.radius)
        {
            flag = false;
            break;
        }
    }
    if (flag)//外部
    {
        return false;
    }

    return false;
}

Scene ClipScene(const Scene &scene, const vector<Plane> &planes)
{
    vector<Instance> clipped_instances;
    Instance clipped_instance;
    for (const Instance &instance : scene.instances)
        if (ClipInstance(instance, planes, clipped_instance))
            clipped_instances.push_back(clipped_instance);

    Scene clipped_scene;
    clipped_scene.instances = clipped_instances;
    return clipped_scene;
}

int main()
{
    return 0;
}
