#include <cmath>
#include <vector>
using namespace std;

// i0<i1
vector<double> Interpolate(int i0, double d0, int i1, double d1)
{
    double a = (d1 - d0) / (i1 - i0);
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
        double h;
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

        // int m = floor(x012.size() / 2.0);
        vector<double> x_left;
        vector<double> x_right;
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
    void DrawShadedTriangle(Point P0, Point P1, Point P2, Color color)
    {
        if (P1.y < P0.y)
            swap(P1, P0);
        if (P2.y < P0.y)
            swap(P2, P0);
        if (P2.y < P1.y)
            swap(P2, P1);

        int x0 = P0.x;
        int y0 = P0.y;
        double h0 = P0.h;
        int x1 = P1.x;
        int y1 = P1.y;
        double h1 = P1.h;
        int x2 = P2.x;
        int y2 = P2.y;
        double h2 = P2.h;

        vector<double> x01 = Interpolate(y0, x0, y1, x1);
        vector<double> h01 = Interpolate(y0, h0, y1, h1);

        vector<double> x12 = Interpolate(y1, x1, y2, x2);
        vector<double> h12 = Interpolate(y1, h1, y2, h2);

        vector<double> x02 = Interpolate(y0, x0, y2, x2);
        vector<double> h02 = Interpolate(y0, h0, y2, h2);

        x01.pop_back();
        vector<double> x012;
        x012.insert(x012.cend(), x01.cbegin(), x01.cend());
        x012.insert(x012.cend(), x12.cbegin(), x12.cend());

        h01.pop_back();
        vector<double> h012;
        h012.insert(h012.cend(), h01.cbegin(), h01.cend());
        h012.insert(h012.cend(), h12.cbegin(), h12.cend());

        vector<double> x_left;
        vector<double> h_left;
        vector<double> x_right;
        vector<double> h_right;
        int m = x012.size() / 2;
        if (x02[m] < x012[m])
        {
            x_left = x02;
            h_left = h02;

            x_right = x012;
            h_right = h012;
        }
        else
        {
            x_left = x012;
            h_left = h012;

            x_right = x02;
            h_right = h02;
        }

        Color shaded_color;
        for (int y = y0; y <= y2; y++)
        {
            int x_l = x_left[y - y0];
            int x_r = x_right[y - y0];
            vector<double> h_segment = Interpolate(x_l, h_left[y - y0], x_r, h_right[y - y0]);
            for (int x = x_l; x <= x_r; x++)
            {
                shaded_color.r = color.r * h_segment[x - x_l];
                shaded_color.g = color.g * h_segment[x - x_l];
                shaded_color.b = color.b * h_segment[x - x_l];
                PutPixel(x, y, shaded_color);
            }
        }
    }
};

int main()
{
    return 0;
}
