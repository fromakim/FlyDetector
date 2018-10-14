#pragma once
#include "stdafx.h"
#include "frame.h"

class Roi {
	private:
	
	int index;
	double width;
	double height;
	double centroid[2];

	public:

	void findRoi(Frame source);
	void findFly(Frame source);
};