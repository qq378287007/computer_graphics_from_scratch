// 屏幕坐标系：原点位于屏幕左上角，x轴向右，y轴向下。Cw和Ch表示屏幕宽度和高度。坐标点Sx和Sy。[0, Cw)和[0, Ch)。
// 绘图坐标系：原点位于中心，x轴向右，y轴向上。坐标点Cx和Cy。[-Cw/2, Cw/2)和[-Ch/2, Ch/2)。

// 绘图坐标转换为屏幕坐标
void draw2screen(int Cw, int Ch, int Cx, int Cy, int &Sx, int &Sy)
{
    Sx = Cw / 2 + Cx;
    Sy = Ch / 2 - Cy;
}

// 屏幕坐标转换为绘图坐标
void screen2draw(int Cw, int Ch, int Sx, int Sy, int &Cx, int &Cy)
{
    Cx = Sx - Cw / 2;
    Cy = -Sy + Ch / 2;
}

int main()
{
    return 0;
}
