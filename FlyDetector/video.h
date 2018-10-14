#pragma once
#include "stdafx.h"
#include "frame.h"

class Video {
    private:
    
    VideoCapture cap;
    int length;
    int width;
    int height;
    double fps;
	double duration;

    public:
    
    Video();
    void setVideo(string path = "fly_movie.avi");
	double getDuration();

    Frame getFrameBySecond(double second);

    void display();
};
