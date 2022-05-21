// Compile using g++ Test_Image.cpp -o test_image `pkg-config --cflags --libs opencv`


#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;
int lowThreshold = 0;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

static void CannyThreshold(int, void*)
{
    blur( src_gray, detected_edges, Size(3,3) );
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*3, kernel_size );
    dst = Scalar::all(0);
    src.copyTo( dst, detected_edges);
    imshow( window_name, dst );
}


 int main( int argc, char** argv )
 {
    VideoCapture cap(0); //capture the video from webcam

  if (!cap.isOpened()) {
    cerr << "ERROR: Unable to open the camera" << endl;
    return 0;
  }

  Mat frame;
  Mat contours;
  Mat grey_image;
  Mat gauss_image;
  Mat thes_image;
  cout << "Start grabbing, press a key on Live window to terminate" << endl;
  while(1) {
    cap >> frame;
    
    cvtColor(frame, grey_image, COLOR_RGB2GRAY);
    GaussianBlur(grey_image, gauss_image, Size(33,33), 0, 0);
    threshold(grey_image, thes_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
    Canny(thes_image, contours, 0, 255);
    // namedWindow("Image");
    // imshow("Image", frame);

    // namedWindow("Grey");
    // imshow("Grey", grey_image);

    namedWindow("Canny");
    imshow("Canny", thes_image);


    if (frame.empty()) {
        cerr << "ERROR: Unable to grab from the camera" << endl;
        break;
    }

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
