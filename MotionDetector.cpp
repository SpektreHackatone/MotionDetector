#include "MotionDetector.h"

#include <opencv2/opencv.hpp>

const double MotionDetector::kMotionAreaCoef{ 0.005 };


namespace {
	cv::Mat getImgsDiff(cv::Mat img1, cv::Mat img2, cv::Mat img3) {

		cv::cvtColor(img1, img1, cv::COLOR_BGR2GRAY);
		cv::cvtColor(img2, img2, cv::COLOR_BGR2GRAY);
		cv::cvtColor(img3, img3, cv::COLOR_BGR2GRAY);
		 

		cv::Mat d1, d2, res;

		cv::absdiff(img1, img2, d1);
		cv::absdiff(img1, img3, d2);
		cv::bitwise_and(d1, d2, res);

		cv::blur(res, res, cv::Size(3, 3));//TODO:проверить качество

		cv::threshold(res, res, MotionDetector::kThresholdForMotion, 255, cv::THRESH_BINARY);

		return res;
	}

	bool DetectMotion(cv::Mat motion_img) {
		std::vector<std::vector<cv::Point>> contours;

		double true_contour_area = motion_img.total() * MotionDetector::kMotionAreaCoef;
	    
		cv::findContours(motion_img, contours, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);

		bool motion_tracked = false;
		for (size_t i = 0; i < contours.size(); ++i) {
			if (cv::contourArea(contours[i]) > true_contour_area) {
				motion_tracked = true;

				//      cv::drawContours(show, contours, i, cv::Scalar(0, 0, 255), 3);
			}
		}

		return motion_tracked;
	}
}

MotionDetector::MotionDetector(int detection_period) : kDetectionPeriod(detection_period)
{
}

void MotionDetector::Detect(cv::Mat full_img, int ts, RectsById rects, SignalFunc func)
{
	if (!detecting_in_progress_ && (ts - last_proceesed_ts_ > kDetectionPeriod)) {
		detecting_in_progress_ = true;
		current_detection_img_num_ = 0;
	}

	if (!detecting_in_progress_)
		return;


	for (const auto& rect : rects) {

		//cv::Mat tmp;
		//cv::blur(full_img(rect.second), tmp, cv::Size(5, 5)); //TODO:проверить качество
		//last_images[rect.first].push_back(tmp);

		last_images[rect.first].push_back(full_img(rect.second).clone());

		if (current_detection_img_num_ == 2) {
			auto motion_img = getImgsDiff(last_images[rect.first][0], last_images[rect.first][1], last_images[rect.first][2]);
			if (DetectMotion(motion_img))
				func(rect.first);
		}

		cv::imshow("User" + std::to_string(rect.first), last_images[rect.first][current_detection_img_num_]);
	}
	cv::waitKey(1);



	if (current_detection_img_num_ == 2) {
		//завершение
		current_detection_img_num_ = 0;
		detecting_in_progress_ = false;
		last_proceesed_ts_ = ts;
		last_images.clear();
	}
	else
		current_detection_img_num_++;



}
