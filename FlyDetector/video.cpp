#include "video.h"

Video::Video(string path) {
    this->cap = VideoCapture(path);
    if (!cap.isOpened()) {
        throw "Cannot Find Video";
    }
}

void temp() {
    VideoCapture cap("C:/Users/FromaKim/Downloads/fly_movie.avi/fly_movie.avi");
    

    namedWindow("Video Stream", CV_WINDOW_AUTOSIZE);

    while (true) {
        Mat frame;
        bool success = cap.read(frame);

        if (!success) {
            cout << "Cannot Read Video File" << endl;;
            break;
        }

        imshow("Video Stream", frame);

        if (waitKey(30) == 27) {
            cout << "ESC pressed" << endl;
            break;
        }
        while (waitKey() == 93);
    }
}
