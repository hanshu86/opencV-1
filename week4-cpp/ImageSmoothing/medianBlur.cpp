#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
string filename = DATA_PATH + "images/salt-and-pepper.png";
Mat image = imread(filename);

// Defining the kernel size
int kernelSize = 5;

Mat medianBlurred;
// Performing Median Blurring and store in numpy array "medianBlurred"
medianBlur(image,medianBlurred,kernelSize);

imshow("Original Image",image);
waitKey(0);
imshow("Median Blur Result : KernelSize = 5",medianBlurred);
waitKey(0);
return 0;
}
