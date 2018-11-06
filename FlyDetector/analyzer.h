#pragma once
#include "stdafx.h"

class Analyzer {
private:
	Mat criterion;
	Mat flylocation;
	vector<Point> fly;
	vector<Point> point;
	vector<Point> tip;
	Mat input;
	vector<int> distance;

	static bool pointCompare(Point a, Point b);
	
public:
	void setCriterion(Mat source);
	void setInput(Mat next);
	Mat setPointsAndTips(Mat source);
    void setDistanceZero();
	
	vector<Point> getPoints();
	vector<Point> getTips();
	vector<int> getDistance();
    vector<Point> getFlies();

	void calculateCriterion();
	void calculate(Mat color, vector<Point> origin, vector<Point> surface1, vector<Point> surface2);

	Point getClosestPoint(Point fly, Point top, Point bottom);

    vector<double> getDistanceMoved(vector<double> flysize);

	friend void onMouse(int event, int x, int y, int flag, void *param);
};

void onMouse(int event, int x, int y, int flag, void *param);