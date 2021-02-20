#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
string filename = DATA_PATH + "images/sample.jpg";
Mat image = imread(filename);

if (image.empty())
{
    cout << "Could not read image" << endl;
}
// Set kernel size to 5
int kernelSize = 5;

// Create a 5x5 kernel with all elements equal to 1
Mat kernel = Mat::ones(kernelSize, kernelSize, CV_32F);

// Normalize kernel so sum of all elements equals 1
kernel = kernel / (float)(kernelSize*kernelSize);

// Print kernel
cout << kernel << endl;

// Output  image
Mat result;

// Apply filter
filter2D(image, result, -1 ,
      kernel, Point(-1, -1), 0, BORDER_DEFAULT);

imshow("Original Image",image);
waitKey(0);
imshow("Convolution Result",result);
waitKey(0);

return 0;
}
