#include "frame.h"

Frame::Frame(Mat frame, int num, double ms) {
    this->frame = frame;
    this->num = num;
    this->ms = ms;
}

void Frame::showFrame() {
    imshow("Video Stream", frame);
    cout << "Displaying the frame of : " << this->num << "(" << this->ms << "ms)" << endl;
    waitKey();
}
