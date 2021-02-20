
// Import libraries
#include <iostream>
#include "../resource/lib/public/includeLibraries.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include "../resource/lib/public/displayImages.h"

#include "../resource/lib/public/matplotlibcpp.h"

#include <cmath>

using namespace std;

using namespace cv;

using namespace matplotlibcpp;

Mat kernel1 = (Mat_<float>(3, 3) << 0, -0.166, 0,
                                -0.166, 0.666, -0.166,
                                0, -0.166, 0);


cout << kernel1 << endl;

// Implement Variance of absolute values of Laplacian - Method 1
// Input: image
// Output: Floating point number denoting the measure of sharpness of image

// Do NOT change the function name and definition

double var_abs_laplacian(Mat image){
    Mat dst, grayImage;
    Scalar mean, stddev;
    //convert the image to GRAY
    cvtColor(image, grayImage, COLOR_BGR2GRAY );
    filter2D(grayImage, dst, CV_64F, kernel1);
    meanStdDev(dst, mean, stddev, Mat());
    double variance_abs_value = stddev.val[0] * stddev.val[0];
//     cout << variance_abs_value << endl;
    return variance_abs_value;
}

// Implement Sum Modified Laplacian - Method 2
// Input: image
// Output: Floating point number denoting the measure of sharpness of image

// Do NOT change the function name and definition
// int frame_count = 0;

double sum_modified_laplacian(Mat image){
    Mat dst_x, dst_y, dst, dst_sum, grayImage;
    Mat abs_dst_x, abs_dst_y;
    cvtColor(image, grayImage, COLOR_BGR2GRAY );
    Sobel(grayImage, dst_x, CV_64F, 2, 0, 3, 1, 0, BORDER_DEFAULT);
    Sobel(grayImage, dst_y, CV_64F, 0, 2, 3, 1, 0, BORDER_DEFAULT);
//     filter2D(grayImage, dst_y, CV_64F, kernel_y);

    convertScaleAbs(dst_x, abs_dst_x);
    convertScaleAbs(dst_y, abs_dst_y);
    addWeighted(abs_dst_x, 0.5, abs_dst_y, 0.5, 0, dst);
    
    threshold( dst, dst_sum, 5, 0, THRESH_TOZERO );
    
    double ml_sum = sum(dst)[0];
    
//     cout << "Frame:" << frame_count << "sum is:" << ml_sum << endl;
//     frame_count += 1;
    return ml_sum;
}

// Read input video filename
string filename = DATA_PATH + "videos/focus-test.mp4";

// Create a VideoCapture object
VideoCapture cap(filename);

// Read first frame from the video
Mat frame;
cap >> frame;

// Display total number of frames in the video
cout << "Total number of frames : " << (int)cap.get(CAP_PROP_FRAME_COUNT) << endl;

double maxV1 = 0;
double maxV2 = 0;

// Frame with maximum measure of focus
// Obtained using methods 1 and 2
Mat bestFrame1;
Mat bestFrame2;

// Frame ID of frame with maximum measure
// of focus
// Obtained using methods 1 and 2
int bestFrameId1 = 0;
int bestFrameId2 = 0;

// Get measures of focus from both methods
double val1 = var_abs_laplacian(frame);
double val2 = sum_modified_laplacian(frame);

// Specify the ROI for flower in the frame
// UPDATE THE VALUES BELOW
int topCorner = 40;
int leftCorner = 425;
int bottomCorner = 650;
int rightCorner = 1020;

Mat flower;
flower = frame(Range(topCorner,bottomCorner),Range(leftCorner,rightCorner));

// Iterate over all the frames present in the video
while (1){
    // Crop the flower region out of the frame
    flower = frame(Range(topCorner,bottomCorner),Range(leftCorner,rightCorner));
    // Get measures of focus from both methods
    val1 = var_abs_laplacian(flower);
    val2 = sum_modified_laplacian(flower);
    // If the current measure of focus is greater 
    // than the current maximum
    if (val1 > maxV1){
        // Revise the current maximum
        maxV1 = val1;
        // Get frame ID of the new best frame
        bestFrameId1 = (int)cap.get(CAP_PROP_POS_FRAMES);
        // Revise the new best frame
        bestFrame1 = frame.clone();
        cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << endl;
    }
    // If the current measure of focus is greater 
    // than the current maximum
    if (val2 > maxV2){
        // Revise the current maximum
        maxV2 = val2;
        // Get frame ID of the new best frame
        bestFrameId2 = (int)cap.get(CAP_PROP_POS_FRAMES);
        // Revise the new best frame
        bestFrame2 = frame.clone();
        cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << endl;
    }
    cap >> frame;
    if (frame.empty())
        break;
}

cout << "================================================" << endl;

// Print the Frame ID of the best frame
cout << "Frame ID of the best frame [Method 1]: " << bestFrameId1 << endl;
cout << "Frame ID of the best frame [Method 2]: " << bestFrameId2 << endl;

cap.release();

Mat out;
hconcat(bestFrame1, bestFrame2, out);

plt::figure_size(1200,500);
plt::imshow(out);
auto pltImg = displayImage(out);
pltImg

///
/// AUTOGRADER TEST - DO NOT REMOVE
///


///
/// AUTOGRADER TEST - DO NOT REMOVE
///

