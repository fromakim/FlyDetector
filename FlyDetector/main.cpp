#include "stdafx.h"
#include "setup.h"
#include "video.h"
#include "frame.h"
#include "roi.h"

int main(int argc, char **argv) {
    Setup setup;
    Video video;
	Roi roi;

    setup.getUserInput();

    if (setup.getPath() == "none") {
        video.setVideo("resource.mp4");
    }
    else {
        video.setVideo(setup.getPath());
    }

    Frame f = video.getFrameBySecond(0);

	roi.findRoi(f);
	roi.findFly(f);

    waitKey();

    return 0;
}