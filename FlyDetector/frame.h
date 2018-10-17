#pragma once
#include "stdafx.h"
#include "roi.h"

class Frame {
    private:

    Mat frame;
    int num;
    double ms;
	Roi roi;

	vector<Rect> Ranges;

    public:

    Frame(Mat frame, int num, double ms);

	Mat getFrame();

    void showFrame();
	void saveFrame(int index);
	
	void setROI();
	void setRanges();

	Mat binarize();
};