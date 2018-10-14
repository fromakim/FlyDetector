#pragma once
#include "stdafx.h"

class Frame {
    private:

    Mat frame;
    int num;
    double ms;

    public:

    Frame(Mat frame, int num, double ms);

    void showFrame();
};