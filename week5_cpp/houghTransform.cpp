#include <opencv2/opencv.hpp>
#include <iostream>
#include "dataPath.hpp"

using namespace std;
using namespace cv;

int main(){
// Read image
Mat img = imread(DATA_PATH + "images/lanes.jpg", IMREAD_COLOR);
// Convert the image to gray-scale
Mat gray;
cvtColor(img,gray,COLOR_BGR2GRAY);
// Find the edges in the image using canny detector
Mat edges;
Canny(gray,edges,50,200);
// Detect points that form a line
vector<Vec4i> lines;
HoughLinesP(edges,lines,1,CV_PI/180,100,10,250);
// Draw lines on the image
for (size_t i=0; i < lines.size(); i++){
    Vec4i line = lines[i];
    cv::line(img,Point(line[0],line[1]),Point(line[2],line[3]),Scalar(255,0,0),3,LINE_AA);
}
imshow("Hough Lines", img);
waitKey(0);

// Read image as gray-scale
img = imread(DATA_PATH + "images/circles.jpg", IMREAD_COLOR);
// Convert to gray-scale
cvtColor(img, gray, COLOR_BGR2GRAY);

// Blur the image to reduce noise
Mat img_blur;
medianBlur(gray,img_blur,5);
// Apply hough transform on the image
vector<Vec3f> circles;
HoughCircles(img_blur, circles, HOUGH_GRADIENT, 1, 50, 450, 10, 30, 40);
for( size_t i = 0; i < circles.size(); i++ )
{
  Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
  int radius = cvRound(circles[i][2]);
  // Draw the outer circle
  circle( img, center, radius, Scalar(0, 255, 0), 2);
  // Draw the center of the circle
  circle( img, center, 2, Scalar(0, 0, 255), 3);
}

imshow("Hough Circles", img);
waitKey(0);

return 0;
}
