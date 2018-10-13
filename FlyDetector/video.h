#pragma once
#include "stdafx.h"

class Video {
    private:
    
    VideoCapture cap;
    int length;
    
    public:
    
    Video(string path = "C:/Users/FromaKim/Downloads/fly_movie.avi/fly_movie.avi"); // Dev
};

class Capture {
    Mat capture;
};