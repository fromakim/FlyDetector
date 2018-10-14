#include "stdafx.h"
#include "setup.h"
#include "video.h"
#include "frame.h"

int main(int argc, char **argv) {
    Setup setup;
    Video video;

    setup.getUserInput();

    if (setup.getPath() == "none") {
        video.setVideo("fly_movie.avi");
    }
    else {
        video.setVideo(setup.getPath());
    }

    for (int i = 0; i < 20; ++i) {
        Frame f = video.getFrameBySecond(i);

        f.showFrame();
    }

    waitKey();

    return 0;
}