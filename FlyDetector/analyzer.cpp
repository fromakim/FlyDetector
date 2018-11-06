#include "analyzer.h"

bool Analyzer::pointCompare(Point a, Point b) {
	return (a.y > b.y);
}

Mat Analyzer::setPointsAndTips(Mat source) {
	vector<KeyPoint> vertex;
	Mat result = source.clone();

	this->point.clear();
	this->tip.clear();

	FAST(source, vertex, 50, false);

	for (vector<KeyPoint>::iterator itor = vertex.begin(); itor != vertex.end(); ++itor) {
		bool isSimilar = false;

		for (vector<Point>::iterator pointItor = point.begin(); pointItor != point.end(); ++pointItor) {
			Point pt = itor->pt;
			double distance = norm(pt - *pointItor);

			if (distance < EPSILON1)
				isSimilar = true;
		}
		if (!isSimilar) {
			point.push_back(itor->pt);
		}
	}
	sort(point.begin(), point.end(), pointCompare);

	//this->tip = vector<Point>(tip.begin(), tip.begin() + NUMBER_OF_ROI);
	/*
	cout << endl << "TIP" << endl;
	for (vector<Point>::iterator itor = tip.begin(); itor != tip.end(); ++itor) {
		circle(result, *itor, 10, Scalar(0));
		cout << *itor << " ";
	}
	cout << endl << endl;

	cout << endl << "Points" << endl;
	for (vector<Point>::iterator itor = point.begin(); itor != point.end(); ++itor) {
		circle(result, *itor, 5, Scalar(0));
		cout << *itor << " ";
	}
	cout << endl << endl;
	*/
	return result;
}

void Analyzer::setCriterion(Mat source) {
	this->criterion = source.clone();
	cvtColor(this->criterion, this->flylocation, COLOR_GRAY2RGB);
}

void Analyzer::setInput(Mat next) {
	this->input = next.clone();
}

vector<Point> Analyzer::getPoints() {
	return this->point;
}

vector<Point> Analyzer::getTips() {
	return this->tip;
}

vector<int> Analyzer::getDistance() {
	return this->distance;
}

vector<Point> Analyzer::getFlies() {
    return this->fly;
}

void Analyzer::calculateCriterion() {
	namedWindow("Set the initial location of flies");
	moveWindow("Set the initial location of flies", 0, 0);
	cv::imshow("Set the initial location of flies", this->criterion);

	setMouseCallback("Set the initial location of flies", onMouse, this);
}

Point Analyzer::getClosestPoint(Point fly, Point top, Point bottom) {
    Point closest;
    int diffMin = 99999;
    
    for (vector<Point>::iterator itor = this->point.begin(); itor != this->point.end(); ++itor) {
        int xdiff = abs(fly.x - itor->x);
        int ydiff = abs(fly.y - itor->y);

        if (xdiff > EPSILON2) {
            continue;
        }

        if (top.y > itor->y || bottom.y < itor->y) {
            continue;
        }

        if (ydiff < diffMin) {
            diffMin = ydiff;
            closest = *itor;
        }
    }
    return closest;
}

void onMouse(int event, int x, int y, int flag, void *param) {
	Analyzer *analyzer = (Analyzer *)param;
	Point closest;
	int diffMin = 99999;
    
	for (vector<Point>::iterator itor = analyzer->point.begin(); itor != analyzer->point.end(); ++itor) {
		int xdiff = abs(x - itor->x);
		int ydiff = abs(y - itor->y);

		if (sqrt(xdiff * xdiff + ydiff * ydiff) < diffMin) {
			diffMin = sqrt(xdiff * xdiff + ydiff * ydiff);
			closest = *itor;
		}
	}
	
	switch (event) {
	case CV_EVENT_LBUTTONDOWN:
		circle(analyzer->flylocation, closest, 5, Scalar(0, 0, 255));
		analyzer->fly.push_back(closest);

		cout << "Current Fly Loc" << endl;
		for (vector<Point>::iterator itor = analyzer->fly.begin(); itor != analyzer->fly.end(); ++itor) {
			cout << *itor << " ";
		}
		cout << endl;
		break;

	case CV_EVENT_RBUTTONDOWN:
		vector<Point> temp;
		cvtColor(analyzer->criterion, analyzer->flylocation, COLOR_GRAY2RGB);
		
		for (vector<Point>::iterator itor = analyzer->fly.begin(); itor != analyzer->fly.end(); ++itor) {
			if (closest == *itor) {
				analyzer->fly.erase(itor);
				break;
			}
		}
		for (vector<Point>::iterator itor = analyzer->fly.begin(); itor != analyzer->fly.end(); ++itor) {
			circle(analyzer->flylocation, *itor, 5, Scalar(0, 0, 255));
		}
		cout << "Current Fly Loc" << endl;
		for (vector<Point>::iterator itor = analyzer->fly.begin(); itor != analyzer->fly.end(); ++itor) {
			cout << *itor << " ";
		}
		cout << endl;
		break;
	}

	cv::imshow("Set the initial location of flies", analyzer->flylocation);
}

vector<double> Analyzer::getDistanceMoved(vector<double> flysize) {
    return vector<double>();
}

void Analyzer::calculate(Mat color, vector<Point> origin, vector<Point> surface1, vector<Point> surface2) {
    for (vector<Point>::iterator itor = origin.begin(); itor != origin.end(); ++itor) {
        Point topLimit, botLimit;
        double min = 99999;

        for (vector<Point>::iterator tItor = surface1.begin(); tItor != surface1.end(); ++tItor) {
            Point fly = *itor;
            Point temp = *tItor;
            Point vec = fly - temp;

            if (sqrt(vec.x * vec.x + vec.y * vec.y) < min) {
                min = sqrt(vec.x * vec.x + vec.y * vec.y);
                topLimit = temp;
            }
        }
        min = 99999;
        for (vector<Point>::iterator bItor = surface2.begin(); bItor != surface2.end(); ++bItor) {
            Point fly = *itor;
            Point temp = *bItor;
            Point vec = fly - temp;

            if (sqrt(vec.x * vec.x + vec.y * vec.y) < min) {
                min = sqrt(vec.x * vec.x + vec.y * vec.y);
                botLimit = temp;
            }
        }
        
        Point newFlyPoint = this->getClosestPoint(*itor, topLimit, botLimit);

        cout << "OLD: " << *itor << endl;
        cout << "NEW: " << newFlyPoint << endl;
        
        circle(color, *itor, 5, Scalar(255, 0, 0));             // Original: BLUE
        circle(color, newFlyPoint, 5, Scalar(0, 0, 255));       // New: RED

        if (newFlyPoint != Point(0, 0))
            distance.push_back(newFlyPoint.y - itor->y);
        else
            distance.push_back(99999);
    }

    imshow("PROCEDURE", color);
    waitKey();
}

void Analyzer::setDistanceZero() {
    this->distance.clear();
}