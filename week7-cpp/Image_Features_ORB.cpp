#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
// Read input image
Mat img = imread(DATA_PATH + "images/book.jpeg");
// Convert to grayscale
Mat imgGray;
cvtColor(img,imgGray,COLOR_BGR2GRAY);

imshow("Image",img);
waitKey(0);
imshow("Gray",imgGray);
waitKey(0);
destroyAllWindows();

// Initiate ORB detector
Ptr<ORB> orb = ORB::create();

// find the keypoints with ORB
vector<KeyPoint> kp;
orb -> detect(imgGray,kp,Mat());
// compute the descriptors with ORB
Mat des;
orb -> compute(imgGray, kp, des);
// draw only keypoints location,not size and orientation
Mat img2;
drawKeypoints(img, kp, img2, Scalar(0,255,0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
imshow("Keypoints",img2);
waitKey(0);
destroyAllWindows();

orb = ORB::create(10);
orb -> detectAndCompute(imgGray,Mat(),kp,des);
drawKeypoints(img, kp, img2, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
imshow("Keypoints",img2);
waitKey(0);

destroyAllWindows();
return 0;
}
