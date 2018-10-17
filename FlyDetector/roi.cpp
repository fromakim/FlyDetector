#include "roi.h"

bool Roi::select = false;
Rect Roi::range = Rect();
Mat Roi::source = Mat();
Mat Roi::origin = Mat();
bool Roi::changed = false;
vector<Rect> Roi::ranges = vector<Rect>();

void Roi::setRange(Mat source) {
	source.copyTo(Roi::origin);
	source.copyTo(Roi::source);

	namedWindow("Set Range of Interest");
	imshow("Set Range of Interest", Roi::source);
	setMouseCallback("Set Range of Interest", this->onMouseEvent, (void *)this);

	cout << "Press any key if you pressed the Range you wanted" << endl;
}

vector<Rect> Roi::getRanges() {
	return Roi::ranges;
}

void Roi::clean() {
	Roi::select = false;
	Roi::range = Rect();
	Roi::source = Mat();
	Roi::origin = Mat();
	Roi::changed = false;
	Roi::ranges.clear();
}

void Roi::onMouseEvent(int event, int x, int y, int flags, void *param) {
	Mat temp;
	Roi::source.copyTo(temp);
	int i = 0;
	
    switch (event) {
        case CV_EVENT_LBUTTONDOWN:
            Roi::select = true;
            Roi::range.x = x;
            Roi::range.y = y;
            cout << "New Range: " << Roi::range << endl;
            break;
        case CV_EVENT_MOUSEMOVE:
            if (Roi::select == true) {
                Roi::range.width = x - Roi::range.x;
                Roi::range.height = y - Roi::range.y;

				rectangle(temp, Roi::range, Scalar(255, 0, 0));
				imshow("Set Range of Interest", temp);
            }
            break;
        case CV_EVENT_LBUTTONUP:
            Roi::select = false;
			cout << "Selected Range: " << Roi::range << endl;
			cout << "Roi change: " << Roi::changed << endl;
			
			rectangle(Roi::source, Roi::range, Scalar(255, 0, 0));
			imshow("Set Range of Interest", Roi::source);
            
			Roi::ranges.push_back(Roi::range);

			cout << "=== Current Ranges ===" << endl;
			i = 0;
			for (vector<Rect>::iterator itor = Roi::ranges.begin(); itor < Roi::ranges.end(); ++itor) {
				cout << "Index " << i << ": " << Roi::ranges.at(i) << endl;
				i = i + 1;
			}

			break;
		case CV_EVENT_RBUTTONDOWN:
			Roi::origin.copyTo(Roi::source);
			vector<Rect> saveList;

			i = 0;
			for (vector<Rect>::iterator itor = Roi::ranges.begin(); itor < Roi::ranges.end(); ++itor) {
				if (!Roi::ranges.at(i).contains(Point(x, y))) {
					cout << "The Point is not: " << i << endl;
					rectangle(Roi::source, Roi::ranges.at(i), Scalar(255, 0, 0));
					saveList.push_back(Roi::ranges.at(i));
				}
				else {
					cout << "Delete the Range of Index: " << i << endl;
				}
				i = i + 1;
			}
			Roi::ranges = saveList;

			cout << "=== Current Ranges ===" << endl;
			i = 0;
			for (vector<Rect>::iterator itor = Roi::ranges.begin(); itor < Roi::ranges.end(); ++itor) {
				cout << "Index " << i << ": " << Roi::ranges.at(i) << endl;
				i = i + 1;
			}

			imshow("Set Range of Interest", Roi::source);
			
			break;
    }
}