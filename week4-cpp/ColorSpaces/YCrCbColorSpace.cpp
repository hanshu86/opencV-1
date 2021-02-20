#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main(){
// read the image in BGR format
vector<Mat> channels(3);
Mat bgr = imread(DATA_PATH+"images/capsicum.jpg");
Mat ycbImage;
cvtColor(bgr, ycbImage, COLOR_BGR2YCrCb);
split(ycbImage,channels);

imshow("Y Channel",channels[0]);
imshow("Cr Channel",channels[1]);
imshow("Cb Channel",channels[2]);
waitKey(0);

return 0;
}
