#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(){
// read the image in BGR format
Mat bgr = imread(DATA_PATH+"images/capsicum.jpg");

imshow("Original Image",bgr);
waitKey(0);

Mat bgrChannels[3];
split(bgr,bgrChannels);

imshow("Blue Channel",bgrChannels[0]);
imshow("Green Channel",bgrChannels[1]);
imshow("Red Channel",bgrChannels[2]);
waitKey(0);

return 0;
}
