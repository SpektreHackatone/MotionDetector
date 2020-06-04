#pragma once

#include <opencv2/core.hpp>

#include <unordered_map>
#include <queue>

using RectsById = std::vector<std::pair<size_t, cv::Rect>>;

using SignalFunc = void(size_t);

class MotionDetector
{
public:
	/// <param name="detection_period"> - определяет периодичность детектирования движения, в миллисекундах</param>
	MotionDetector(int detection_period = 100);
	
	/// <param name="ts"> - в миллисекундах</param>
	void Detect(cv::Mat full_img, int ts, RectsById rects, SignalFunc func);

	static const int kThresholdForMotion{ 3 };
	static const double kMotionAreaCoef;

	const int kDetectionPeriod; //в ms

private:
	std::unordered_map<size_t, std::vector<cv::Mat>> last_images;

	int last_proceesed_ts_{ -1000 };
	bool detecting_in_progress_{ false };
	size_t current_detection_img_num_{ 0 };


};

