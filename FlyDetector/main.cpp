#include "stdafx.h"
#include "setup.h"
#include "video.h"
#include "frame.h"
#include "roi.h"
#include "analyzer.h"

void onMouseEvent(int event, int x, int y, int flags, void *param) {
    switch (event) {
        case CV_EVENT_LBUTTONDOWN:
            cout << x << " " << y << endl;
            break;
    }
}

Mat func(Mat source) {
    vector<vector<Point>> contours;
    Mat result = Mat(source.rows, source.cols, CV_8UC3, Scalar(255, 255, 255));

    imshow("SOURCE", source);
    waitKey();

    erode(source, source, cv::Mat());
    findContours(source, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    drawContours(result, contours, -1, Scalar(0), 2);
    imshow("Contour", result);

    setMouseCallback("Contour", onMouseEvent, NULL);
    waitKey();
    return source;
}

int main(int argc, char **argv) {
    Setup setup;
    Video video;
	Analyzer analyzer;

    setup.getUserInput();

    if (setup.getPath() == "none") {
        video.setVideo("resource.mp4");
    }
    else {
        video.setVideo(setup.getPath());
    }

    Frame f = video.getFrameBySecond(1);
    Mat target = f.binarize();
    /*
	f.setROI();
	waitKey();
	f.setRanges();

	namedWindow("Range 0");
	moveWindow("Range 0", 0, 0);
	imshow("Range 0", f.getRanges().at(0));
    
	analyzer.setSource(f.getRanges());
	analyzer.binarize();
    
	for (int i = 0; i < 9; ++i) {
		Frame f1 = video.getFrameBySecond(i + 2);
		vector<Rect> newRect = f.getRects();

		vector<Mat> newRange = f1.getRanges(newRect);
		
		analyzer.getLocation(newRange);

		namedWindow("Range " + to_string(i + 1));
		moveWindow("Range " + to_string(i + 1), i * 100, 500);
		imshow("Range " + to_string(i + 1), newRange.at(0));
	}
    */
    namedWindow("Test Before");
    namedWindow("Test After");

    moveWindow("Test Before", 0, 300);
    moveWindow("Test After", 1000, 300);

    imshow("Test Before", target);
    imshow("Test After", func(target));

    waitKey();

    return 0;
}