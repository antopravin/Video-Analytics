#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
/*
Global Variable Declaration
*/
vector<Point2f> centerPoint;
vector<Point> logoDetect(Mat logoDetectImage);
/*
Method Declaratiion
*/
void personDetect();
void drawGraphForTraversal();
/** @function main */
int main(int argc, char** argv)
{
	Mat videoFrameHSV;
	//Opening the camera in video mode
	VideoCapture capture(0);
	while (capture.read(videoFrameHSV) != NULL)
	{
		//writing the frame into a jpg file
		imwrite("pic.jpg", videoFrameHSV);
		namedWindow("Video", CV_WINDOW_AUTOSIZE);
		imshow("Video", videoFrameHSV);
		//Detecting the person wearing the logo
		personDetect();
		//Wait till excape key is pressed
		if (waitKey(30) == 27){
			drawGraphForTraversal();
			break;
		}
	}
	waitKey(0);
}
/*
Path Identification
centerPoint - Stores the center point of drawn bounding box.
*/
void drawGraphForTraversal() {
	Mat rectImg = imread("pic.jpg");
	//Run a loop for all the points selected
	for (int i = 5; i < centerPoint.size() - 2; i++) {
		int thickness = 2;
		int lineType = 8;
		cout << centerPoint[i] << abs(centerPoint[i].x) << abs(centerPoint[i].y);
		if (!((abs(centerPoint[i].x < 1)) && (abs(centerPoint[i].y < 1)))){
			//Draw a line connecting all the points
			line(rectImg, centerPoint[i], centerPoint[i + 1], Scalar(0, 0, 0), thickness, lineType);
			cout << centerPoint[i];
		}
	}
	imshow("Traversal Path", rectImg);
}
/*
Detecting UTD logo on the Tshirt
logoDetectImage - Captured Frame
*/
vector<Point> logoDetect(Mat logoDetectImage) {
	Mat output;
	vector<Point> pointsLogo;
	Rect boundingRectangle;
	Mat rectImg = imread("pic.jpg");
	//Cropping Rectangle box to increase the accuracy
	Rect modifyRect = Rect(0, 0, logoDetectImage.cols, logoDetectImage.rows / 2);
	logoDetectImage = logoDetectImage(modifyRect).clone();
	imshow("logo_image", logoDetectImage);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//Convert the image into HSV
	cvtColor(logoDetectImage, logoDetectImage, CV_BGR2HSV);//0 191 73 24 255 200
	//inRange(logoDetectImage, Scalar(87, 128, 50), Scalar(100, 175, 234), logoDetectImage);
	//Identify the logo
	inRange(logoDetectImage, Scalar(89, 18, 103), Scalar(180, 256, 256), logoDetectImage);
	//Blur the image
	GaussianBlur(logoDetectImage, logoDetectImage, Size(3, 3), 2.0);
	imshow("GaussianBlur", logoDetectImage);
	//Eroding and Dilating to smoothen the image for correct detection
	int erosion_type = 2;
	int erosion_size = 9;
	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	/// Apply the erosion operation
	erode(logoDetectImage, logoDetectImage, element);
	erosion_size = 20;
	element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	dilate(logoDetectImage, logoDetectImage, element);
	//imshow("dilate", logoDetectImage);
	Canny(logoDetectImage, output, 80, 80 * 2, 3);
	//imshow("image1111", output);
	//imshow("logodetect", logoDetectImage);
	//Finding contours
	findContours(output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//Checking contour size
	if (contours.size() == 0)
	{
		cout << "zero";
		return pointsLogo;
	}
	/*
	Drawing a rectangle on the logo detected
	*/
	vector<Point> points;
	Rect brect;
	for (size_t i = 0; i<contours.size(); i++) {
		for (size_t j = 0; j < contours[i].size(); j++) {
			Point p = contours[i][j];
			points.push_back(p);
		}
	}
	// for example: draws rectangle on original image.
	if (points.size() > 0){
		brect = boundingRect(Mat(points).reshape(2));
		rectangle(rectImg, brect.tl(), brect.br(), Scalar(150, 100, 100), 2, CV_AA);
	}
	Point pt1, pt2;
	pt1.x = brect.x;
	pt1.y = brect.y;
	pt2.x = brect.x + brect.width;
	pt2.y = brect.y + brect.height;
	pointsLogo.push_back(pt1);
	pointsLogo.push_back(pt2);
	// Draws the rect in the original image and show it
	rectangle(rectImg, pt1, pt2, CV_RGB(255, 0, 0), 1);

	imshow("Contours_Logo", rectImg);
	return pointsLogo;
}
/** Detect the person wearing the UTD T shirt */
void personDetect()
{
	Mat img = imread("pic.jpg");
	Mat rectImg = imread("pic.jpg");
	Mat rectImgForLogoDetection = imread("pic.jpg");
	Mat threshold_output;
	//Converting image to HSV
	cvtColor(img, img, CV_BGR2HSV);//0 191 73 24 255 200
	inRange(img, Scalar(0, 114, 135), Scalar(17, 256, 256), img);
	//inRange(img, Scalar(0, 167, 34), Scalar(35, 256, 256), img);
	//imshow("mm", img);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	Mat canny_output;
	/// Detect edges using Threshold
	//threshold(src_gray, threshold_output, thresh, 255, THRESH_BINARY);
	/// Find contours
	//Blur the image
	GaussianBlur(img, img, Size(7, 7), 2.0);
	//medianBlur(img, img, 3);
	//erode(img, img, 0, Point(-1, -1), 2, 6, 1);
	//dilate(img, img, 0, Point(-1, -1), 2, 14, 1);
	//Eroding and Dilating to smoothen the image
	int erosion_type = 2;
	int erosion_size = 6;
	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	/// Apply the erosion operation
	erode(img, img, element);
	erosion_size = 5;
	dilate(img, img, element);
	Canny(img, canny_output, 80, 80 * 2, 3);
	//Canny(edgeFrame, contour, 255, 50);
	Moments mu;
	Point2f currentPoint;
	//Find moments that is the center of the contoour
	mu = moments(canny_output, false);
	currentPoint = Point2f(mu.m10 / mu.m00, mu.m01 / mu.m00);
	Scalar color = Scalar(0, 0, 25);
	centerPoint.push_back(currentPoint);
	//This fixes and shows the center of the object
	circle(rectImg, currentPoint, 4, color, -1, 2, 0);

	//imshow("realtime", rectImg);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	if (contours.size() == 0)
	{
		return;
	}
	//Drawing rectangle around the person
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	float radius;
	Mat drawing = Mat::zeros(img.size(), CV_8UC3);
	Rect brect;
	vector<Point> points;
	for (size_t i = 0; i<contours.size(); i++) {
		for (size_t j = 0; j < contours[i].size(); j++) {
			Point p = contours[i][j];
			points.push_back(p);
		}
	}
	if (points.size() > 0){
		brect = boundingRect(Mat(points).reshape(2));
		rectangle(rectImg, brect.tl(), brect.br(), Scalar(150, 100, 100), 4, CV_AA);
		radius = brect.width / 2;
		//circle(rectImg, curre, (brect.y + brect.height) / 2), radius, color, 2, 8, 0);
		//minEnclosingCircle(rectImg, currentPoint, radius);
		//centerPoint.push_back(currentPoint);
	}
	//Cropping the rectangle
	Point pt1, pt2;
	pt1.x = brect.x; //- (radius);
	pt1.y = brect.y - (radius*.9);
	pt2.x = brect.x + brect.width;// +(radius);
	pt2.y = brect.y + brect.height + (radius * 3);
	rectangle(rectImg, pt1, pt2, Scalar(255, 255, 0), 3, 8, 0);
	Mat tshirt = rectImg(brect).clone();
	imshow("tshirt", tshirt);
	//Calling logoDetect() by passing the croppped rectangular frame containing the Logo
	vector<Point> logoPoint = logoDetect(tshirt);
	if (!logoPoint.empty()){
		Point pt1, pt2;
		pt1.x = brect.x + logoPoint[0].x;
		pt1.y = logoPoint[0].y + brect.y;
		pt2.x = brect.x + logoPoint[1].x;
		pt2.y = logoPoint[1].y + brect.y;
		rectangle(rectImg, pt1, pt2, CV_RGB(255, 0, 255), 3);
	}
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", rectImg);

}