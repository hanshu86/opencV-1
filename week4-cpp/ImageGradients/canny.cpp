#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Function for trackbar call
void applyCanny(int ,void *)
{
  // Variable to store blurred image
  Mat blurredSrc;

  // Blur the image before edge detection
  if (blurAmount > 0 )
  {
    GaussianBlur(src, blurredSrc, Size( 2 * blurAmount + 1, 2 * blurAmount + 1), 0);
  }
  else
  {
    blurredSrc = src.clone();
  }

  // Canny requires aperture size to be odd
  int apertureSize = apertureSizes[apertureIndex];

  // Apply canny to get the edges
  Canny( blurredSrc, edges, lowThreshold, highThreshold, apertureSize );

  //Display images
  imshow("Edges",edges);
}

int main(){
// Variables for source and edge images
Mat src, edges;
// Variables for low and high thresholds
int lowThreshold = 50;
int highThreshold = 100;

// Max trackbar value
int const maxThreshold = 1000;


// Sobel aperture sizes for Canny edge detector
int apertureSizes [] = {3, 5, 7};
int maxapertureIndex = 2;
int apertureIndex = 0;

// Gaussian blur size
int blurAmount = 0;
int maxBlurAmount = 20;

// Read lena image
src = imread("../data/images/sample.jpg", IMREAD_GRAYSCALE);

//Display images
imshow("Edges",src);

// Create a window to display output.
namedWindow("Edges",CV_WINDOW_AUTOSIZE);

// Trackbar to control the low threshold
createTrackbar( "Low Threshold", "Edges", &lowThreshold, maxThreshold, applyCanny);

// Trackbar to control the high threshold
createTrackbar( "High Threshold", "Edges", &highThreshold, maxThreshold, applyCanny);

// Trackbar to control the aperture size
createTrackbar( "aperture Size", "Edges", &apertureIndex, maxapertureIndex, applyCanny);

// Trackbar to control the blur
createTrackbar( "Blur", "Edges", &blurAmount, maxBlurAmount, applyCanny);

waitKey(0);

destroyAllWindows();
}
