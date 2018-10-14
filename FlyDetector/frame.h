#pragma once
#include "stdafx.h"

class Frame {
    private:

    Mat frame;
    int num;
    double ms;

    public:

    Frame(Mat frame, int num, double ms);

	Mat getFrame();

    void showFrame();
	void saveFrame(int index);

	Mat binarize();
};