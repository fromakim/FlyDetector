#include "setup.h"

void Setup::getUserInput() {
    cout << "PATH: ";
    cin >> this->path;
    cout << "FPS: ";
    cin >> this->fps;
}

string Setup::getPath() {
    return this->path;
}

double Setup::getFps() {
    return this->fps;
}
