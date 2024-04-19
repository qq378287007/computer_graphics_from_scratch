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
        int x0 = P0.x;
        int y0 = P0.y;
        int x1 = P1.x;
        int y1 = P1.y;
        double a = 1.0 * (y1 - y0) / (x1 - x0);
        double b = y0 - a * x0;
        for (int x = x0; x <= x1; x++)
        {
            int y = a * x + b;
            PutPixel(x, y, color);
        }
    }
    void DrawLine2(Point P0, Point P1, Color color)
    {
        int x0 = P0.x;
        int y0 = P0.y;
        int x1 = P1.x;
        int y1 = P1.y;

        double a = 1.0 * (y1 - y0) / (x1 - x0);
        double y = y0;
        for (int x = x0; x <= x1; x++)
        {
            PutPixel(x, y, color);
            y += a;
        }
    }
    void DrawLine3(Point P0, Point P1, Color color)
    {
        if (P0.x > P1.x)
            swap(P0, P1);

        int x0 = P0.x;
        int y0 = P0.y;
        int x1 = P1.x;
        int y1 = P1.y;

        double a = 1.0 * (y1 - y0) / (x1 - x0);
        double y = y0;
        for (int x = x0; x <= x1; x++)
        {
            PutPixel(x, y, color);
            y += a;
        }
    }
    void DrawLine4(Point P0, Point P1, Color color)
    {
        if (P0.y > P1.y)
            swap(P0, P1);

        int x0 = P0.x;
        int y0 = P0.y;
        int x1 = P1.x;
        int y1 = P1.y;

        double a = 1.0 * (x1 - x0) / (y1 - y0);
        double x = x0;
        for (int y = y0; y <= y1; y++)
        {
            PutPixel(x, y, color);
            x += a;
        }
    }
    void DrawLineLast(Point P0, Point P1, Color color)
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

            double a = 1.0 * (y1 - y0) / (x1 - x0);
            double y = y0;
            for (int x = x0; x <= x1; x++)
            {
                PutPixel(x, y, color);
                y += a;
            }
        }
        else
        {
            if (P0.y > P1.y)
                swap(P0, P1);

            int x0 = P0.x;
            int y0 = P0.y;
            int x1 = P1.x;
            int y1 = P1.y;

            double a = 1.0 * (x1 - x0) / (y1 - y0);
            double x = x0;
            for (int y = y0; y <= y1; y++)
            {
                PutPixel(x, y, color);
                x += a;
            }
        }
    }
    void DrawLineLast2(Point P0, Point P1, Color color)
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
};

int main()
{
    return 0;
}
