#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){

string filename = DATA_PATH + "images/girl.jpg";
Mat img = imread(filename);

// Specify scaling factor
float saturationScale = 0.01;

Mat hsvImage;

// Convert to HSV color space
cv::cvtColor(img,hsvImage,COLOR_BGR2HSV);

// Convert to float32
hsvImage.convertTo(hsvImage,CV_32F);

vector<Mat> channels(3);

// Split the channels
split(hsvImage,channels);

// Multiply S channel by scaling factor
channels[1] = channels[1] * saturationScale;

// Clipping operation performed to limit pixel values
// between 0 and 255
min(channels[1],255,channels[1]);
max(channels[1],0,channels[1]);

// Merge the channels
merge(channels,hsvImage);

// Convert back from float32
hsvImage.convertTo(hsvImage,CV_8UC3);

Mat imSat;
// Convert to BGR color space
cv::cvtColor(hsvImage,imSat,COLOR_HSV2BGR);

imshow("Original Image", img);
imshow("Desaturated Image", imSat);
waitKey(0);

return 0;
}
