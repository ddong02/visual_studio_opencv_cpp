#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;
int main(void)
{
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cerr << "Camera open failed" << endl;
		return -1;
	}

	Mat frame;
	int key;
	bool normal_mode = true;
	Mat M;
	Mat zoomout_center;
	bool zoomout_mode = false;

	while (true) {
		cap >> frame;
		if (frame.empty()) {
			cout << "frame load failed" << endl;
			break;
		}
		if (!normal_mode) {
			warpAffine(frame, frame, M, Size());
			if (zoomout_mode) {
				zoomout_center = Mat_<double>({ 2,3 }, { 1,0,frame.cols / 4.0,0,1,frame.rows / 4.0 });
				warpAffine(frame, frame, zoomout_center, Size());
			}
		}
		imshow("Frame", frame);
		key = waitKey(10);
		if (key == 'q') break;
		else if (key == 'a' || key == 'b') {
			normal_mode = false;
			if (key == 'a') {
				M = Mat_<double>({ 2,3 }, { 2.0, 0,0,0,2.0,0 });
				zoomout_mode = false;
			}
			else {
				zoomout_mode = true;
				M = Mat_<double>({ 2,3 }, { 0.5, 0,0,0,0.5,0 });
			}
		}
		else if (key == 'c') {
			zoomout_mode = false;
			normal_mode = true;
		}
	}

	return 0;
}