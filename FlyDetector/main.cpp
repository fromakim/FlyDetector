#include "stdafx.h"
#include "setup.h"
#include "video.h"
#include "frame.h"
#include "roi.h"
#include "analyzer.h"

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

    waitKey();

    return 0;
}