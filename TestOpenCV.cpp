#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#include "MotionDetector.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    VideoCapture cam(0);
    Mat image;

    MotionDetector md;

    RectsById rects{ {0, Rect{0,0,200,200}},
                     {1, Rect{400,150,200,200}},
                     {2, Rect{0,250,200,200}}

    };

    int ts = 0;
    while (true) {
        cam >> image;
        //cv::imshow("full", image);
        //cv::waitKey(1);
        md.Detect(image, ts, rects, [](size_t id) {cout << "Motion detected! id: " << id << endl; });
        ts += 33;
    }
    
    return 0;
}