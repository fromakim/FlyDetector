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
            cout << "Point: " << x << ", " << y << endl;
    }
}

bool pointCompare(Point a, Point b) {
	return (a.y > b.y);
}

Mat extractTip(Mat source, vector<Point> &tip) {
    vector<KeyPoint> vertex;
	vector<Point> vertexPoint;
    Mat result = source.clone();
    
    FAST(source, vertex, 50, false);

    for (vector<KeyPoint>::iterator itor = vertex.begin(); itor != vertex.end(); ++itor) {
		bool isSimilar = false;

		for (vector<Point>::iterator pointItor = vertexPoint.begin(); pointItor != vertexPoint.end(); ++pointItor) {
			Point pt = itor->pt;
			double distance = norm(pt - *pointItor);

			if (distance < EPSILON1)
				isSimilar = true;
		}
		if (!isSimilar) {
			vertexPoint.push_back(itor->pt);
		}
    }
	sort(vertexPoint.begin(), vertexPoint.end(), pointCompare);
	for (vector<Point>::iterator itor = vertexPoint.begin(); itor != vertexPoint.end(); ++itor) {
		bool isSimilar = false;
		
		for (vector<Point>::iterator tipItor = tip.begin(); tipItor != tip.end(); ++tipItor) {
			if (abs(itor->x - tipItor->x) < EPSILON2) {
				isSimilar = true;
			}
		}

		if (!isSimilar) {
			tip.push_back(*itor);
		}
	}
	cout << endl << "TIP" << endl;
	for (vector<Point>::iterator itor = tip.begin(); itor != tip.end(); ++itor) {
		circle(result, *itor, 5, Scalar(0));
		cout << *itor << " ";
	}

	cout << endl;
    return result;
}

void dummy() {
	/*
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
	if (itor->pt.y > 420)
		circle(result, itor->pt, 5, Scalar(125));
	*/
}

int main(int argc, char **argv) {
	cv::Point a(1, 3);
	cv::Point b(5, 6);
	double res = cv::norm(a - b);

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
    Mat result = target.clone();
	vector<Point> tip;
	// erode(target, result, cv::Mat());

	analyzer.setCriterion(result);
	analyzer.calculateCriterion();
	analyzer.setPointsAndTips(result);

	vector<Point> originalPoint = analyzer.getPoints();
	vector<Point> originalTip = analyzer.getTips();
	// analyzer.calculateCriterion();

	cvtColor(result, result, COLOR_GRAY2RGB);
	for (vector<Point>::iterator itor = originalPoint.begin(); itor != originalPoint.end(); ++itor) {
		circle(result, *itor, 5, Scalar(255, 0, 0));
	}
	imshow("Original", result);
	waitKey();

	for (int i = 0; i < 10; ++i) {
		Frame input = video.getFrameBySecond(i + 2);
		Mat bin = input.binarize();
		Mat color;
		cvtColor(bin, color, COLOR_GRAY2RGB);

		analyzer.setPointsAndTips(bin);
		vector<Point> newPoints = analyzer.getPoints();
		
		cout << "Original" << endl;
		for (vector<Point>::iterator itor = originalPoint.begin(); itor != originalPoint.end(); ++itor) {
			cout << *itor << endl;
			circle(color, *itor, 5, Scalar(255, 0, 0));
		}
		cout << "New" << endl;
		for (vector<Point>::iterator itor = newPoints.begin(); itor != newPoints.end(); ++itor) {
			cout << *itor << endl;
			circle(color, *itor, 5, Scalar(0, 0, 255));
		}

		imshow("Test", color);
		waitKey();
	}

    setMouseCallback("Test After", onMouseEvent, (void *)&result);
    waitKey();

    return 0;
}