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
	for (vector<Point>::iterator itor = point.begin(); itor != point.end(); ++itor) {
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

	this->tip = vector<Point>(tip.begin(), tip.begin() + NUMBER_OF_ROI);
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

void Analyzer::calculateCriterion() {
	namedWindow("Set the initial location of flies");
	moveWindow("Set the initial location of flies", 0, 0);
	imshow("Set the initial location of flies", this->criterion);

	setMouseCallback("Set the initial location of flies", onMouse, this);
}

void Analyzer::calculate(vector<Point> origin) {
	int i = 0;

	for (vector<Point>::iterator pointItor = this->point.begin(); pointItor != this->point.end(); ++pointItor) {
		Point closest;
		int xDiffMin = 99999;

		for (vector<Point>::iterator tipItor = this->tip.begin(); tipItor != this->tip.end(); ++tipItor) {
			double xdiff = abs(tipItor->x - pointItor->x);

			if (xdiff < xDiffMin) {
				xDiffMin = xdiff;
				closest = *tipItor;
			}
		}

		distance.push_back(abs(closest.y - pointItor->y));

		cout << "Point " << *pointItor << ": " << closest << ", " << abs(closest.y - pointItor->y) << endl;
	}
}

Point Analyzer::getClosestPoint(vector<Point> fly) {
	Point closest;
	int diffMin = 99999;
	
	for (vector<Point>::iterator itor = point.begin(); itor != point.end(); ++itor) {
		int xdiff = abs(x - itor->x);
		int ydiff = abs(y - itor->y);

		if (sqrt(xdiff * xdiff + ydiff * ydiff) < diffMin) {
			diffMin = sqrt(xdiff * xdiff + ydiff * ydiff);
			closest = *itor;
		}
	}
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

	
	imshow("Set the initial location of flies", analyzer->flylocation);
}