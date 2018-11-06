#pragma once
#include "frame.h"
#include "stdafx.h"

class Setup {
    private:

    string path;
    double fps;

    static vector<Mat> fly;
    static vector<Rect> flyzones;
    vector<double> flyangle;

    static Mat control;
    static Mat origin;
    static Rect temp;
    static bool mousedown;

    static vector<Point> topSurface;
    static vector<Point> botSurface;

    public:

    void getUserInput();
    string getPath();
    double getFps();
    vector<Point> getTopSurface();
    vector<Point> getBotSurface();

    void setFly(Mat f);
    void setAngle();
    vector<double> getFlySize();
    void setSurface();

    static void onMouse(int event, int x, int y, int flag, void *param);
    static void surfaceMouseCallback(int event, int x, int y, int flag, void *param);
};

