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
            cout << "Value: " << (int) source->at<unsigned char>(y, x) << endl;
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
    time_t now;
    struct tm *ptm;

    setup.getUserInput();
    fout.open("result.txt")  ;

    if (setup.getPath() == "none") {
        video.setVideo("resource.mp4");
    }
    else {
        video.setVideo(setup.getPath());
    }

    Frame f = video.getFrameBySecond(1);
    Mat target = f.getFrame();
    vector<double> flySize;

    cvtColor(target, target, COLOR_RGB2GRAY);
    imshow("TEMP", target);
    cv::setMouseCallback("TEMP", onMouseEvent, &target);
    waitKey();

    // Nomalize the intensity
    int min = 255;

    for (int i = 0; i < target.rows; ++i) {
        for (int j = 0; j < target.cols; ++j) {
            int val = target.at<unsigned char>(i, j);

            if (val < min) {
                min = val;
            }
        }
    }
    cout << "MIN: " << min << endl;
    waitKey();
    for (int i = 0; i < target.rows; ++i) {
        for (int j = 0; j < target.cols; ++j) {
            target.at<unsigned char>(i, j) = (target.at<unsigned char>(i, j) - min) * 255 / (255 - min);
        }
    }
    Mat filtered;
    

    threshold(target, filtered, 80, 255, THRESH_BINARY);

    erode(filtered, filtered, cv::Mat());
    imshow("MERGED", target);
    imshow("FILTERED", filtered);

    
    waitKey();
    // Ends
    
    setup.setFly(target);
    setup.setAngle();
    flySize = setup.getFlySize();

    // Method 1
    Mat result = target.clone();
	vector<Point> tip;
	// erode(target, result, cv::Mat());  

	analyzer.setCriterion(result);
	analyzer.calculateCriterion();
	analyzer.setPointsAndTips(result);
    
    cv::waitKey();
	vector<Point> originalPoint = analyzer.getPoints();
	vector<Point> originalTip = analyzer.getTips();
    vector<Point> flies = analyzer.getFlies();
    
    cout << "FLY: " << flies.size() << endl;

    fout << "FLY_SIZE" << " ";
    for (vector<double>::iterator itor = flySize.begin(); itor != flySize.end(); ++itor) {
        fout << *itor << " ";
    }
    fout << endl;

	for (int i = 1 + setup.getFps(); i < video.getDuration(); i = i + setup.getFps()) {
		Frame input = video.getFrameBySecond(i);
		Mat bin, color;
        vector<int> distance;
        int min = 255;

        cout << "Frame of " << i << "s is coming in." << endl;

        cvtColor(input.getFrame(), bin, COLOR_RGB2GRAY);

        for (int i = 0; i < bin.rows; ++i) {
            for (int j = 0; j < bin.cols; ++j) {
                int val = bin.at<unsigned char>(i, j);

                if (val < min) {
                    min = val;
                }
            }
        }
        cout << "MIN: " << min << endl;
        waitKey();
        for (int i = 0; i < bin.rows; ++i) {
            for (int j = 0; j < bin.cols; ++j) {
                bin.at<unsigned char>(i, j) = (bin.at<unsigned char>(i, j) - min) * 255 / (255 - min);
            }
        }

        cvtColor(bin, color, COLOR_GRAY2RGB);

		analyzer.setPointsAndTips(bin);
        analyzer.calculate(color, flies);
        distance = analyzer.getDistance();
        analyzer.setDistanceZero();

        for (vector<int>::iterator itor = distance.begin(); itor != distance.end(); ++itor) {
            cout << *itor << " ";
        }
        cout << endl;
        /*
        for (vector<Point>::iterator itor = flies.begin(); itor != flies.end(); ++itor) {
            Point newFlyPoint = analyzer.getClosestPoint(*itor);
            
            circle(color, *itor, 5, Scalar(255, 0, 0));             // Original: BLUE
            circle(color, newFlyPoint, 5, Scalar(0, 0, 255));       // New: RED

            distance.push_back(abs(newFlyPoint.y - itor->y));
        }
        */

        fout << i << "s ";
        int j = 0;
        for (vector<int>::iterator itor = distance.begin(); itor != distance.end(); ++itor) {
            fout << *itor << " ";
        }
        fout << endl;
	}

    cv::setMouseCallback("Test After", onMouseEvent, (void *)&result);
    cv::waitKey();

    return 0;
}