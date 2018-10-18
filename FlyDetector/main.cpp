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
	cout << "Move to Set Range" << endl;
	waitKey();
	f.setRanges();

	analyzer.setSource(f.getRanges());
	analyzer.binarize();

	for (int i = 0; i < 1; ++i) {
		Frame f1 = video.getFrameBySecond(i + 2);
		vector<Rect> newRect = f.getRects();

		cout << "Rect has no problem " << newRect.size() << endl;

		vector<Mat> newRange = f1.getRanges(newRect);
		
		imshow("New Range", newRange.at(0));
		waitKey();

		analyzer.getLocation(newRange);
	}

    waitKey();

    return 0;
}