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

    public:

    void getUserInput();
    string getPath();
    double getFps();

    void setFly(Mat f);
    void setAngle();
    vector<double> getFlySize();

    static void onMouse(int event, int x, int y, int flag, void *param);
};

