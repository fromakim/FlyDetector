#pragma once
#include "stdafx.h"
#include "roi.h"

class Frame {
    private:

    Mat frame;
    int num;
    double ms;
	Roi roi;

	vector<Mat> Ranges;
	vector<Rect> rects;

    public:

    Frame(Mat frame, int num, double ms);

	Mat getFrame();

    void showFrame();
	void saveFrame(int index);
	
	void setROI();
	void setRanges();

	vector<Mat> getRanges();
	vector<Mat> getRanges(vector<Rect> rect);
	vector<Rect> getRects();

	Mat binarize();
};