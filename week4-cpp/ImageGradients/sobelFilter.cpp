#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
string filename = DATA_PATH + "images/truth.png";

// Read the image
Mat image = imread(filename, IMREAD_GRAYSCALE);

Mat sobelx, sobely;

// Apply sobel filter with only x gradient
Sobel(image, sobelx, CV_32F, 1, 0);

// Apply sobel filter with only y gradient
Sobel(image, sobely, CV_32F, 0, 1);

// Normalize image for display
normalize(sobelx, sobelx, 0, 1, NORM_MINMAX);
normalize(sobely, sobely, 0, 1, NORM_MINMAX);

imshow("X Gradients", sobelx);
waitKey(0);
imshow("Y Gradients", sobely);
waitKey(0);

return 0;
}
