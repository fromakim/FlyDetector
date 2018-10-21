#include "stdafx.h"
#include "setup.h"
#include "video.h"
#include "frame.h"
#include "roi.h"
#include "analyzer.h"
#include "main.h"

void onMouseEvent(int event, int x, int y, int flags, void *param) {
    Mat *source = (Mat *)param;
    
    switch (event) {
        case CV_EVENT_LBUTTONDOWN:
            int number[7] = { 0, 0, 0, 0, 0, 0, 0 };

            cout << "Point: " << x << ", " << y << endl;

            for (int i = 0; i < 7; ++i) {
                for (int j = 0; j < 7; ++j) {
                    
                    if ((int)source->at<unsigned char>(Point(x - 9 + 3 * j, y - 9 + 3 * i)) == 255) {
                        cout << "WHITE ";
                    }
                    else {
                        cout << "BLACK ";
                        number[i] += 1;
                    }
                }
                cout << endl;
            }
            cout << endl;

            for (int i = 0; i < 7; ++i)
                cout << number[i] << endl;
            cout << endl;
    }
}

Mat func(Mat source) {
    vector<KeyPoint> vertex;
    Mat result = source.clone();
    
    FAST(source, vertex, 50, false);

    for (vector<KeyPoint>::iterator itor = vertex.begin(); itor != vertex.end(); ++itor) {
        int number[7] = { 0, 0, 0, 0, 0, 0, 0 };
        int near[7][7];
        bool foundBlack = false;
        bool foundWhite = false;

        if (itor->pt.x - 9 < 0 || itor->pt.y - 9 < 0 || itor->pt.x + 9 > source.cols || itor->pt.y + 9 > source.rows) {
            break;
        }

        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < 7; ++j) {
                near[i][j] = (int)source.at<unsigned char>(Point(itor->pt.x - 9 + 3 * j, itor->pt.y - 9 + 3 * i));
                
                if (near[i][j] == 0) {
                    if (foundBlack == false && foundWhite == false) {       // region starts with black
                        number[i] += 1;
                        foundBlack = true;
                    }
                    else if (foundBlack == false && foundWhite == true) {       // region starts with white and firstly find black
                        number[i] += 1;
                    }
                    else if (foundBlack == true && foundWhite == true) {       // region finds another white after black
                        number[i] = 0;
                        break;
                    }
                }
                if (near[i][j] == 255) {
                    foundWhite = true;
                }
            }
        }

        for (int i = 0; i < 7; ++i) {
            if (i != 6 && number[i] < number[i + 1])
                break;
            if (i == 6) {
                if (number[3] > 0 && itor->pt.y > 420)
                    circle(result, itor->pt, 10, Scalar(0));
            }
        }
        circle(result, itor->pt, 5, Scalar(125));
    }
    cout << source.rows << endl;
    return result;
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

    // Method 1
    /*
    Mat result = target.clone();
    erode(result, result, cv::Mat());
    result = func(result);

    namedWindow("Test Before");
    namedWindow("Test After");

    moveWindow("Test Before", 0, 300);
    moveWindow("Test After", 1000, 300);

    imshow("Test Before", target);
    imshow("Test After", result);

    setMouseCallback("Test After", onMouseEvent, (void *)&result);
    waitKey();
    */

    // Method 2
    Mat comp = target.clone();
    erode(comp, comp, cv::Mat());
    dilate(comp, comp, cv::Mat());
    imshow("Dilate", comp);
    waitKey();

    return 0;
}