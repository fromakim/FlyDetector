#pragma once
#include "stdafx.h"

class Analyzer {
private:
	vector<Mat> prev;
	vector<Mat> curr;
public:
	void setSource(vector<Mat> source);
	void binarize();
	void getLocation(vector<Mat> next);
    void findFeature();
};