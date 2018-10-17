#pragma once
#include "stdafx.h"

class Roi {
	private:
	
	int index;
	double width;
	double height;
	double centroid[2];
	
	static bool changed;
	static Mat origin;
	static Mat source;
	static bool select;
    static Rect range;
	static vector<Rect> ranges;
	static void onMouseEvent(int event, int x, int y, int flags, void *param);
    
	public:
    
	void setRange(Mat source);
	vector<Rect> getRanges();
	void clean();
};
