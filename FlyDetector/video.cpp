#include "video.h"

Video::Video() {
    
}

void Video::setVideo(string path) {
    this->cap = VideoCapture(path);
    if (!this->cap.isOpened()) {
        throw "Cannot Find Video";
    }

    this->length = this->cap.get(CV_CAP_PROP_FRAME_COUNT);
    this->width = this->cap.get(CV_CAP_PROP_FRAME_WIDTH);
    this->height = this->cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    this->fps = this->cap.get(CV_CAP_PROP_FPS);
	this->duration = this->cap.get(CAP_PROP_FRAME_COUNT) / this->cap.get(CAP_PROP_FPS);
    cout << "Video Set with:" << endl;
    cout << "Name: " << endl;
    cout << "Frames: " << this->length << endl;
    cout << "Frame Rate: " << this->fps << endl;
    cout << "Duration: " << this->duration << endl;
}

Frame Video::getFrameBySecond(double second) {
    Mat frame;
    bool success;
    int num;

    this->cap.set(CV_CAP_PROP_POS_MSEC, second * 1000.0);
    num = this->cap.get(CV_CAP_PROP_POS_FRAMES);

    success = cap.read(frame);

    if (!success) throw "Cannot read Frame";

    return Frame(frame, num, second * 1000.0);
}

double Video::getDuration() {
	return this->duration;
}

void Video::display() {
    namedWindow("Video Stream", CV_WINDOW_AUTOSIZE);

    while (true) {
        Mat frame;
        bool success = cap.read(frame);

        if (!success) {
            cout << "Cannot Read Video File" << endl;;
            break;
        }

        imshow("Video Stream", frame);

        int key = waitKey(30);
        if (key == 27) {
            cout << "ESC pressed" << endl;
            break;
        }
        else {
            cout << "Current millisecond: " << this->cap.get(CV_CAP_PROP_POS_MSEC) << endl;
            cout << "Current FPS: " << this->cap.get(CV_CAP_PROP_POS_FRAMES) << endl;
            cout << "=====" << endl;
            system("pause");
        }
    }
}