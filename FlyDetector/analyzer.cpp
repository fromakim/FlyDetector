#include "analyzer.h"

void Analyzer::setSource(vector<Mat> source) {
	for (vector<Mat>::iterator itor = source.begin(); itor != source.end(); ++itor) {
		this->curr.push_back(*itor);
	}
}

void Analyzer::binarize() {
	for (vector<Mat>::iterator itor = this->curr.begin(); itor != this->curr.end(); ++itor) {
		cvtColor(*itor, *itor, COLOR_RGB2GRAY);
		threshold(*itor, *itor, THRESHOLD1, THRESHOLD2, THRESH_BINARY);
	}

	// DEBUG: imshow("Binarized", this->curr.at(0));
	waitKey();
}

void Analyzer::getLocation(vector<Mat> next) {
	for (vector<Mat>::iterator itor = this->curr.begin(); itor != this->curr.end(); ++itor) {
		this->prev.push_back(*itor);
	}
	cout << "Prev moved" << this->prev.size() << endl;
	this->curr.clear();			// Remove everything in current list.
	for (vector<Mat>::iterator itor = next.begin(); itor != next.end(); ++itor) {
		this->curr.push_back(*itor);
	}
	cout << "Curr moved" << this->curr.size() << endl;
	binarize();

	// imshow("Previous", this->prev.at(0));
	// imshow("Current", this->curr.at(0));
	waitKey();
}

void Analyzer::findFeature() {

}