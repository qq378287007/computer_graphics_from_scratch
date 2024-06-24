#include <cmath>
#include <vector>
using namespace std;

// i0<i1
vector<double> Interpolate(int i0, int d0, int i1, int d1)
{
    double a = 1.0 * (d1 - d0) / (i1 - i0);
    double d = d0;

    vector<double> values;
    for (int i = i0; i <= i1; i++)
    {
        values.push_back(d);
        d += a;
    }
    return values;
}

struct Canvas
{
    struct Point
    {
        int x;
        int y;
    };
    struct Color
    {
        int r;
        int g;
        int b;
    };

    void PutPixel(int x, int y, Color color)
    {
    }
    void swap(Point &P0, Point &P1)
    {
        Point temp = P0;
        P0 = P1;
        P1 = temp;
    }
    void DrawLine(Point P0, Point P1, Color color)
    {
        int dx = P1.x - P0.x;
        int dy = P1.y - P0.y;
        if (abs(dx) > abs(dy))
        {
            if (P0.x > P1.x)
                swap(P0, P1);

            int x0 = P0.x;
            int y0 = P0.y;
            int x1 = P1.x;
            int y1 = P1.y;
            vector<double> ys = Interpolate(x0, y0, x1, y1);
            for (int x = x0; x <= x1; x++)
                PutPixel(x, ys[x - x0], color);
        }
        else
        {
            if (P0.y > P1.y)
                swap(P0, P1);

            int x0 = P0.x;
            int y0 = P0.y;
            int x1 = P1.x;
            int y1 = P1.y;
            vector<double> xs = Interpolate(y0, x0, y1, x1);
            for (int y = y0; y <= y1; y++)
                PutPixel(xs[y - y0], y, color);
        }
    }
    void DrawWireframeTriangle(Point P0, Point P1, Point P2, Color color)
    {
        DrawLine(P0, P1, color);
        DrawLine(P1, P2, color);
        DrawLine(P2, P0, color);
    }
    void DrawFilledTriangle(Point P0, Point P1, Point P2, Color color)
    {
        if (P1.y < P0.y)
            swap(P1, P0);
        if (P2.y < P0.y)
            swap(P2, P0);
        if (P2.y < P1.y)
            swap(P2, P1);

        int x0 = P0.x;
        int y0 = P0.y;
        int x1 = P1.x;
        int y1 = P1.y;
        int x2 = P2.x;
        int y2 = P2.y;

        vector<double> x01 = Interpolate(y0, x0, y1, x1);
        vector<double> x12 = Interpolate(y1, x1, y2, x2);
        vector<double> x02 = Interpolate(y0, x0, y2, x2);

        x01.pop_back();
        vector<double> x012;
        x012.insert(x012.cend(), x01.cbegin(), x01.cend());
        x012.insert(x012.cend(), x12.cbegin(), x12.cend());

        vector<double> x_left;
        vector<double> x_right;
        // int m = floor(x012.size() / 2.0);
        int m = x012.size() / 2;
        if (x02[m] < x012[m])
        {
            x_left = x02;
            x_right = x012;
        }
        else
        {
            x_left = x012;
            x_right = x02;
        }
        
        for (int y = y0; y <= y2; y++)
            for (int x = x_left[y - y0]; x <= x_right[y - y0]; x++)
                PutPixel(x, y, color);
    }
};

int main()
{
    return 0;
}
