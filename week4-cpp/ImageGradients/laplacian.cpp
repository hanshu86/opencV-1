#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
string filename = DATA_PATH + "images/sample.jpg";

// Read the image
Mat image = imread(filename, IMREAD_GRAYSCALE);

Mat laplacian, LOG;
int kernelSize = 3;

// Applying laplacian
Laplacian(image, laplacian, CV_32F, kernelSize, 1, 0);

Mat img1;

GaussianBlur(image, img1, Size(3,3), 0, 0);

// Normalize images
normalize(laplacian, laplacian, 0, 1, NORM_MINMAX, CV_32F);

imshow("Laplacian",laplacian);
waitKey(0);

return 0;
}
