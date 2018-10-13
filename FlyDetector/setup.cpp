#include "setup.h"

void Setup::getUserInput() {
    cout << "PATH: ";
    cin >> this->path;
    cout << "FPS: ";
    cin >> this->fps;
}