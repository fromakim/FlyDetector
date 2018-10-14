#include "roi.h"

void Roi::findRoi(Frame source) {
	Mat contour;
	Canny(source.getFrame(), contour, 10, 800);
	imshow("Contour", contour);
}

void Roi::findFly(Frame source) {
	Mat bin = source.binarize();
	imshow("Gray", bin);
	waitKey();
}