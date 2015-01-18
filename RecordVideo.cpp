#pragma once
#include <opencv\cv.h>
#include <opencv\highgui.h>
using namespace std;
using namespace cv;
class RecordVideo
{
	String videoName = "MovingBall.mkv";

public:
	RecordVideo() {

	}
	~RecordVideo() {

	}
	int  recordVideo()
	{
		// Open the web camera
		VideoCapture capture(0);

		// Check if there's any error while opening camera
		if (!capture.isOpened())
		{
			cerr << "There was error while opening the web camera, kindly try again";
			return -1;
		}

		// Get the width and height property for the camera
		double width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
		double height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

		cout << " Web Camera Properties\n";
		cout << "width = " << width << endl << "height = " << height << endl;

		// Create a matrix to keep the retrieved frame
		Mat frame;

		// Create a window to show the image
		namedWindow("Moving Ball", CV_WINDOW_AUTOSIZE);

		/*
		VideoWriter
		input:
		file name - MovingBall.avi
		video codec
		FramePerSec - 30
		size of the frame
		*/
		VideoWriter video(videoName, CV_FOURCC('X', 'V', 'I', 'D'), 3, cvSize((int)width, (int)height));

		// Check if the video was opened
		if (!video.isOpened())
		{
			cerr << "Please close the video if open, inorder to start the video recording";
			return -1;
		}

		cout << "Please press Esc to stop recording." << endl;

		// Get the next frame until the user presses the escape key
		while (true)
		{
			// Get frame from capture
			capture >> frame;

			// Check if the frame was retrieved
			if (!frame.data)
			{
				cerr << "Could not retrieve frame.";
				return -1;
			}

			// Save frame to video
			video << frame;

			// Show image
			imshow("Capture", frame);

			// Exit with escape key
			if (waitKey(1) == 27)
				break;
		}
		return 0;
	}

};
