#include "frame.h"

Frame::Frame(Mat frame, int num, double ms) {
    this->frame = frame;
    this->num = num;
    this->ms = ms;
}

Mat Frame::getFrame() {
	return this->frame;
}

void Frame::showFrame() {
    imshow("Video Stream", frame);
    cout << "Displaying the frame of : " << this->num << "(" << this->ms << "ms)" << endl;
    waitKey();
}

void Frame::saveFrame(int index) {
	string filename = "result\\frame";
	filename.append(to_string(index));
	filename.append(".png");
	imwrite(filename, this->frame);
}

Mat Frame::binarize() {
	Mat binary;
	cvtColor(this->frame, binary, COLOR_RGB2GRAY);
	imshow("GrayScale", binary);
	threshold(binary, binary, 100, 255, THRESH_BINARY);
	return binary;
}