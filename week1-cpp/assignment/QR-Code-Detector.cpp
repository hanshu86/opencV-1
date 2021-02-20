
#include "../resource/lib/public/includeLibraries.h"

#include <opencv2/opencv.hpp>

#include <opencv2/objdetect.hpp>

#include <opencv2/imgcodecs.hpp>

#include <opencv2/highgui/highgui.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

#include <string>

using namespace std;

using namespace cv;

// Image Path
string imgPath = DATA_PATH + "/images/IDCard-Satya.png";

// Read image and store it in variable img
Mat img = imread(imgPath);


cout << img.size().height << " " << img.size().width;

///
/// AUTOGRADER TEST - DO NOT REMOVE
///


Mat bbox, rectifiedImage;

// Create a QRCodeDetector Object
// Variable name should be qrDecoder
QRCodeDetector qrDecoder = cv::QRCodeDetector();

// Detect QR Code in the Image
// Output should be stored in opencvData
std::string opencvData = qrDecoder.detectAndDecode(img, bbox, rectifiedImage);

// Check if a QR Code has been detected
if(opencvData.length()>0)
    cout << "QR Code Detected" << endl;
else
    cout << "QR Code NOT Detected" << endl;

///
/// AUTOGRADER TEST - DO NOT REMOVE
///


///
/// AUTOGRADER TEST - DO NOT REMOVE
///


int n = bbox.rows;

// Draw the bounding box
Point top_left = Point(22, 186);
Point bottom_right = Point(126,82);
Mat imageRectangle = img.clone();
rectangle(imageRectangle, top_left, bottom_right,  Scalar(255, 0, 0), 5, LINE_8);

// Since we have already detected and decoded the QR Code
// using qrDecoder.detectAndDecode, we will directly
// use the decoded text we obtained at that step (opencvData)

cout << "QR Code Detected!" << endl;
cout << opencvData


///
/// AUTOGRADER TEST - DO NOT REMOVE
///


// Write the result image
string resultImagePath = "./QRCode-Output.png";

imwrite(resultImagePath,imageRectangle);


///
/// AUTOGRADER TEST - DO NOT REMOVE
///

