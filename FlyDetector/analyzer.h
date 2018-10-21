#pragma once
#include "stdafx.h"

class Analyzer {
private:
	Mat criterion;
	vector<Point> point;
	vector<Point> tip;
	Mat input;
	vector<int> distance;

	static bool pointCompare(Point a, Point b);
	Mat extractTip(Mat source);
public:
	void setCriterion(Mat source);
	void setInput(Mat next);
	
	vector<int> getDistance();

	void calculateCriterion();
	void calculate();
};