#include "setup.h"

vector<Mat> Setup::fly;
vector<Rect> Setup::flyzones;
Mat Setup::control;
Mat Setup::origin;
Rect Setup::temp;
bool Setup::mousedown = false;

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

void Setup::setFly(Mat f) {
    Setup::control = f.clone();
    Setup::origin = Setup::control.clone();

    imshow("Set the fly size", Setup::control);

    setMouseCallback("Set the fly size", onMouse, &Setup::control);

    waitKey();
    destroyWindow("Set the fly size");
}

void Setup::onMouse(int event, int x, int y, int flag, void *param) {
    Mat screen = Setup::control.clone();
    
    switch (event) {
        case CV_EVENT_LBUTTONDOWN:
            temp.x = x;
            temp.y = y;
            temp.width = 1;
            temp.height = 1;
            cout << Setup::temp << endl;

            rectangle(screen, Setup::temp, Scalar(0, 0, 255, 0));
            Setup::mousedown = true;

            imshow("Set the fly size", screen);
            break;
        case CV_EVENT_MOUSEMOVE:
            if (mousedown) {
                temp.width = x - temp.x;
                temp.height = y - temp.y;
                rectangle(screen, Setup::temp, Scalar(0, 0, 255));

                imshow("Set the fly size", screen);
            }
            break;
        case CV_EVENT_LBUTTONUP:
            temp.width = x - temp.x;
            temp.height = y - temp.y;
            rectangle(screen, Setup::temp, Scalar(0, 0, 255));
            Setup::mousedown = false;

            Setup::control = screen.clone();
            Setup::flyzones.push_back(temp);
            Setup::fly.push_back(Mat(Setup::control, temp));

            cout << temp << endl;

            imshow("Set the fly size", screen);
            break;
        case CV_EVENT_RBUTTONDOWN:
            cout << "Before: ";
            for (vector<Rect>::iterator itor = Setup::flyzones.begin(); itor != Setup::flyzones.end(); ++itor) {
                cout << *itor << " ";
                if (itor->contains(Point(x, y))) {
                    Setup::flyzones.erase(itor);
                    break;
                }
            }
            cout << endl;
            screen = Setup::origin.clone();
            cout << "Drawing ";
            for (vector<Rect>::iterator itor = Setup::flyzones.begin(); itor != Setup::flyzones.end(); ++itor) {
                cout << *itor << endl;
                rectangle(screen, Setup::temp, Scalar(0, 0, 255));
            }
            cout << endl;

            imshow("Set the fly size", screen);
    }
}

void Setup::setAngle() {
    vector<double> angle;

    for (vector<Rect>::iterator itor = flyzones.begin(); itor != flyzones.end(); ++itor) {
        Mat area = Mat(Setup::origin, *itor);
        Point centroid;
        threshold(area, area, 50, THRESHOLD2, THRESH_BINARY);
        
        Point ymax(0, 0);
        Point ymin(0, area.rows);

        Point xmax(0, 0);
        Point xmin(area.cols, 0);

        cout << area.rows << " " << area.cols << endl;

        for (int i = 0; i < area.rows; ++i) {
            for (int j = 0; j < area.cols; ++j) {
                int point = area.at<unsigned char>(i, j);
                
                if (point == 0) {
                    cout << "бр";
                }
                else {
                    cout << "бс";
                }
                
                if (point == 0) {
                    if (ymax.y < i) {
                        ymax.y = i;
                        ymax.x = j;
                    }
                    if (ymin.y > i) {
                        ymin.y = i;
                        ymin.x = j;
                    }
                    if (xmin.x > j) {
                        xmin.x = j;
                        xmin.y = i;
                    }
                    if (xmax.x < j) {
                        xmax.x = j;
                        xmax.y = i;
                    }
                }

            }
            cout << endl;
        }

        cout << "X: MAX(" << xmax << "), MIN(" << xmin << ")" << endl;
        cout << "Y: MAX(" << ymax << "), MIN(" << ymin << ")" << endl;

        bool isPositive;

        if (xmax.y > xmin.y) {
            isPositive = true;
        }
        else {
            isPositive = false;
        }
        Point p1, p2;

        if (isPositive) {
            p1 = Point((xmax.x + ymax.x) / 2, (xmax.y + ymax.y) / 2);
            p2 = Point((xmin.x + ymin.x) / 2, (xmin.y + ymin.y) / 2);
        }
        else {
            p2 = Point((xmin.x + ymax.x) / 2, (xmin.y + ymax.y) / 2);
            p1 = Point((xmax.x + ymin.x) / 2, (xmax.y + ymin.y) / 2);
        }

        cout << "POINTS: " << p1 << p2 << endl;
        angle.push_back(atan((-1) * (p1.y - p2.y) / (p1.x - p2.x)));

        imshow("AREA", area);

        waitKey();
        
    }

    cout << "ANGLES" << endl;
    for (vector<double>::iterator itor = angle.begin(); itor != angle.end(); ++itor) {
        cout << *itor << " ";
    }
    cout << endl;

    this->flyangle = angle;
}

vector<double> Setup::getFlySize() {
    vector<double> size;
    
    cout << "Fly Size" << endl;
    for (int i = 0; i < flyzones.size(); ++i) {
        size.push_back(flyzones.at(i).width / cos(this->flyangle.at(i)));
        cout << (flyzones.at(i).width / cos(this->flyangle.at(i))) << " ";
    }
    cout << endl;

    return size;
}