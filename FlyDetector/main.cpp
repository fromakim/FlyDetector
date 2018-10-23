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

int main(int argc, char **argv) {
    Setup setup;
    Video video;
	Analyzer analyzer;
    ofstream fout;

    setup.getUserInput();
    fout.open("result.txt", ios::app);

    if (setup.getPath() == "none") {
        video.setVideo("resource.mp4");
    }
    else {
        video.setVideo(setup.getPath());
    }

    Frame f = video.getFrameBySecond(1);
    Mat target = f.binarize();

    // Method 1
    Mat result = target.clone();
	vector<Point> tip;
	// erode(target, result, cv::Mat());

	analyzer.setCriterion(result);
	analyzer.calculateCriterion();
	analyzer.setPointsAndTips(result);

    waitKey();
	vector<Point> originalPoint = analyzer.getPoints();
	vector<Point> originalTip = analyzer.getTips();
    vector<Point> flies = analyzer.getFlies();
    
    cout << "FLY: " << flies.size() << endl;

	for (int i = 0; i < 10; ++i) {
		Frame input = video.getFrameBySecond(i + 2);
		Mat bin = input.binarize();
		Mat color;
        vector<double> distance;

        cvtColor(bin, color, COLOR_GRAY2RGB);

		analyzer.setPointsAndTips(bin);


        for (vector<Point>::iterator itor = flies.begin(); itor != flies.end(); ++itor) {
            Point newFlyPoint = analyzer.getClosestPoint(*itor);
            
            circle(color, *itor, 5, Scalar(255, 0, 0));             // Original: BLUE
            circle(color, newFlyPoint, 5, Scalar(0, 0, 255));       // New: RED

            distance.push_back(abs(newFlyPoint.y - itor->y));
        }
        for (vector<double>::iterator itor = distance.begin(); itor != distance.end(); ++itor) {
            fout << *itor << " ";
        }
        fout << endl;

		imshow("Test", color);
		waitKey();
	}

    setMouseCallback("Test After", onMouseEvent, (void *)&result);
    waitKey();

    return 0;
}