#include <iostream>
#include "dataPath.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

int main(){
string filename = DATA_PATH + "images/gaussian-noise.png";
Mat image = imread(filename);

//diameter of the pixel neighbourhood used during filtering
int dia=15;

// Larger the value the distant colours will be mixed together
// to produce areas of semi equal colors
double sigmaColor=80;

// Larger the value more the influence of the farther placed pixels
// as long as their colors are close enough
double sigmaSpace=80;

Mat bilateralFiltered;

// Apply bilateral filter
bilateralFilter(image, bilateralFiltered, dia,
              sigmaColor,
              sigmaSpace);

imshow("Original Image",image);
waitKey(0);
imshow("Bilateral Blur Result",bilateralFiltered);
waitKey(0);

return 0;
}
