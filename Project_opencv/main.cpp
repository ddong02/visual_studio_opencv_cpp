#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void onTrackbar(int, void*);

struct myData {
	Point center;
	Mat& org;
	Mat& target;
};

int main(void)
{
	Mat img = imread("images/rotateda.bmp", IMREAD_GRAYSCALE);
	if (img.empty()) cout << "image load failed" << endl;

	Mat bin;
	threshold(img, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	Mat dst_org;
	cvtColor(img, dst_org, COLOR_GRAY2BGR);
	Mat dst_affined = dst_org.clone();

	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);

	RotatedRect rrct = minAreaRect(contours[1]);
	Point2f pts[4];
	rrct.points(pts);

	Mat roi = dst_affined(Range(pts[1].y, pts[3].y), Range(pts[0].x, pts[2].x));
	myData my_data = { rrct.center, dst_org, dst_affined};

	namedWindow("dst_affined");
	createTrackbar("angle", "dst_affined", 0, 360, onTrackbar, &my_data);

	while (true) {
		circle(dst_affined, rrct.center, 2, Scalar(0, 0, 255), -1);

		imshow("roi", roi);
		imshow("dst_affined", dst_affined);

		if (waitKey(5) == 'q')
			break;
	}
	return 0;
}

void onTrackbar(int pos, void* userdata)
{
	static int prev_pos = 0;
	Mat m;

	myData* my_data = (myData*)userdata;
	if (prev_pos != pos) {
		m = getRotationMatrix2D(my_data->center, pos, 1);
		prev_pos = pos;
	}

	warpAffine(my_data->org, my_data->target, m, Size(), 1, 0, Scalar(255, 255, 255));
}