#include "analyzer.h"

bool Analyzer::pointCompare(Point a, Point b) {
	return (a.y > b.y);
}

Mat Analyzer::extractTip(Mat source) {
	vector<KeyPoint> vertex;
	Mat result = source.clone();

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

	cout << endl << "TIP" << endl;
	for (vector<Point>::iterator itor = tip.begin(); itor != tip.end(); ++itor) {
		circle(result, *itor, 5, Scalar(0));
		cout << *itor << " ";
	}

	cout << endl;
	return result;
}

void Analyzer::setCriterion(Mat source) {
	this->criterion = source.clone();
	extractTip(this->criterion);
}

void Analyzer::setInput(Mat next) {
	this->input = next.clone();
}

vector<int> Analyzer::getDistance() {
	return this->distance;
}

void Analyzer::calculateCriterion() {
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

void Analyzer::calculate() {
	for (vector<Point>::iterator itor = this->tip.begin(); itor != this->tip.end(); ++itor) {

	}
}