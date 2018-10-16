#include "roi.h"

bool Roi::select = false;
Rect Roi::range = Rect();

void Roi::setRange(Frame source) {
    namedWindow("Set Range of Interest");
    imshow("Set Range of Interest", source.getFrame());
    waitKey();

    setMouseCallback("Set Range of Interest", this->onMouseEvent, NULL);
}

void Roi::findFly(Frame source) {
	Mat bin = source.binarize();
	imshow("Gray", bin);
	waitKey();
}

void Roi::onMouseEvent(int event, int x, int y, int flags, void *param) {
    cout << "Range: " << Roi::range << endl;

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
            }
            break;
        case CV_EVENT_LBUTTONUP:
            Roi::select = false;
            break;
    }
}