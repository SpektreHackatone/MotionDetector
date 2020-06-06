#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

#include "MotionDetectorSingleRect.h"


using namespace cv;
using namespace std;

cv::Mat gammaCorrection(const cv::Mat& img, const double gamma_)
{
    CV_Assert(gamma_ >= 0);
    //! [changing-contrast-brightness-gamma-correction]
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    for (int i = 0; i < 256; ++i)
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, gamma_) * 255.0);

    cv::Mat res = img.clone();
    LUT(img, lookUpTable, res);
    //! [changing-contrast-brightness-gamma-correction]

    cv::Mat img_gamma_corrected;
    hconcat(img, res, img_gamma_corrected);
    return img_gamma_corrected;
    //cv::imshow("Gamma correction", img_gamma_corrected);
}

int main(int argc, char** argv)
{
    //VideoCapture cam(0);
    //Mat image;

    //MotionDetector md;

    //RectsById rects{ {0, Rect{0,0,200,200}},
    //                 {1, Rect{400,150,200,200}},
    //                 {2, Rect{0,250,200,200}}

    //};

    //int ts = 0;
    //while (true) {
    //    cam >> image;
    //    //cv::imshow("full", image);
    //    //cv::waitKey(1);
    //    md.Detect(image, ts, rects, [](size_t id) {cout << "Motion detected! id: " << id << endl; });
    //    ts += 33;
    //}

    VideoCapture cam(0);
    Mat image;

    MotionDetectorWithInterestRects md(getStandart2Rects(cv::Size(640, 480)));

    int ts = 0;
    while (true) {
        cam >> image;
        //cv::imshow("full", image);
        //cv::waitKey(1);

        md.Detect(image, ts, [](MDEventType event_type, cv::Point pt1, cv::Point pt2) {
            cout << "Event: " << EventTypeToString(event_type) << 
                ((event_type == MDEventType::OUT1 || event_type == MDEventType::OUT2) ?
                    (", pt1: (" + std::to_string(pt1.x) + ";" + std::to_string(pt1.y) +
                        ")   pt2: (" + std::to_string(pt2.x) + ";" + std::to_string(pt2.y)) + ")" : "") << std::endl;});
        ts += 33;

    }
    
    return 0;
}

