#pragma once
#include "stdafx.h"
#include "frame.h"

class Video {
    private:
    
    VideoCapture cap;
    int length;
    int width;
    int height;
    float fps;

    public:
    
    Video();
    void setVideo(string path = "fly_movie.avi");

    Frame getFrameBySecond(double second);

    void display();
};
