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

void Frame::setROI() {
	this->roi.setRange(this->frame);
}

void Frame::setRanges() {
	vector<Rect> temp = this->roi.getRanges();

	int i = 0;
	for (vector<Rect>::iterator itor = temp.begin(); itor < temp.end(); ++itor) {
		this->Ranges.push_back(temp.at(i));
		i = i + 1;
	}

	cout << "=== Current Ranges in the Frame ===" << endl;
	i = 0;
	for (vector<Rect>::iterator itor = this->Ranges.begin(); itor < this->Ranges.end(); ++itor) {
		cout << "Index " << i << ": " << this->Ranges.at(i) << endl;
		i = i + 1;
	}

	this->Ranges = this->roi.getRanges();
	this->roi.clean();

	temp = this->roi.getRanges();
	cout << "Checking Deletion: The size of the temp is " << temp.size() << endl;
}