// Compile using g++ Test_Image.cpp -o test_image `pkg-config --cflags --libs opencv`


#include <iostream>
#include <math.h>
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

  float horizon_angle = 0;
  Point Avgpt1;
  Point Avgpt2;

  Mat frame;
  Mat contours;
  Mat grey_image;
  Mat gauss_image;
  Mat thes_image;
  Vec4f  line;
  char image_path[30] = "./Test_Images/Serp_Hz_5.png";
  cout << "Start grabbing, press a key on Live window to terminate" << endl;
  while(1) {
    horizon_angle = 0;

    // cap >> frame;
    frame = imread(image_path, IMREAD_COLOR);
    resize(frame, frame, cv::Size(640,480), 0.75, 0.75);

    cvtColor(frame, grey_image, COLOR_RGB2GRAY);
    GaussianBlur(grey_image, gauss_image, Size(33,33), 0, 0);
    threshold(grey_image, thes_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
    Canny(thes_image, contours, 0, 255);

    vector<Vec2f> lines; // will hold the results of the detection
    HoughLines(contours, lines, 1, CV_PI/180, 150, 0, 0 ); // runs the actual detection

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

        cv::line(frame, pt1, pt2, Scalar(255,0,0), 3, LINE_AA);

        // Avgpt1.x += pt1.x;
        // Avgpt1.y += pt1.y;
        // Avgpt2.x += pt2.x;
        // Avgpt2.y += pt2.y;

        float x1 = pt1.x, x2 = pt2.x, y1 = pt1.y, y2 = pt2.y;

        horizon_angle += 180*atan2((y2-y1), (x2-x1)) / M_PI;
    }
    // Avgpt1.x /= lines.size();
    // Avgpt2.x /= lines.size();
    // Avgpt1.y /= lines.size();
    // Avgpt2.y /= lines.size();
    // cv::line(frame, Avgpt1, Avgpt2, Scalar(255,0,0), 2, LINE_AA);

    horizon_angle /= lines.size();
    char angle_string[40];
    sprintf(angle_string, "Horizon angle: %0.2f degrees", horizon_angle);
    putText(frame, angle_string, Point(10, frame.rows / 4), FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 0, 0), 1);  

    // namedWindow("Image");
    // imshow("Image", frame);

    // namedWindow("Grey");
    // imshow("Grey", grey_image);

    namedWindow("Canny");
    imshow("Canny", frame);

    if (frame.empty()) {
        cerr << "ERROR: Unable to grab from the camera" << endl;
        break;
    }

    int key = cv::waitKey(20);
    key = (key==255) ? -1 : key;
    if (key>=0)
      break;
  }

  cout << "Horizon angle : " << horizon_angle << "degrees" << endl;
  cap.release();
  destroyAllWindows();
  return 0;
	
 }
