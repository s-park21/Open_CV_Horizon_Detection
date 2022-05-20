#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from webcam

  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  Mat frame;
  cout << "Start grabbing, press a key on Live window to terminate" << endl;
  while(1) {
    cap >> frame;
    if (frame.empty()) {
        cerr << "ERROR: Unable to grab from the camera" << endl;
        break;
    }
    imshow("Live",frame);
    int key = cv::waitKey(20);
    key = (key==255) ? -1 : key;
    if (key>=0)
      break;
  }

  cout << "Closing the camera" << endl;
  cap.release();
  destroyAllWindows();
  cout << "bye!" <<endl;
  return 0;
	
 }
