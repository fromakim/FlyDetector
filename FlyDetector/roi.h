#pragma once
#include "stdafx.h"
#include "frame.h"

class Roi {
	private:
	
	int index;
	double width;
	double height;
	double centroid[2];

    static bool select;
    static Rect range;
    static void onMouseEvent(int event, int x, int y, int flags, void *dstImage);
    
	public:
    
	void setRange(Frame source);
	void findFly(Frame source);
};
