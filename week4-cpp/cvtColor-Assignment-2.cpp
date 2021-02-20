
// Include libraries
#include <iostream>
#include "includeLibraries.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "matplotlibcpp.h"
#include "displayImages.h"

using namespace std;

using namespace cv;

using namespace matplotlibcpp;

Mat img = imread(DATA_PATH + "images/sample.jpg");

plt::figure_size(1200,500);
plt::imshow(img);
auto pltImg = displayImage(img);
pltImg

// Convert BGR image to Grayscale image
// Input: image in BGR mode
// Output: image in grayscale mode

// Reference: https://docs.opencv.org/4.1.0/de/d25/imgproc_color_conversions.html

Mat convertBGRtoGray(Mat image){
    
    Mat imgChannels[3];
    split(image, imgChannels);
    Mat gray = 0.299*imgChannels[2] +0.587* imgChannels[1] +0.114*imgChannels[0];
    return gray;
}

Mat gray = convertBGRtoGray(img);

Mat gray_cv;
cvtColor(img,gray_cv,COLOR_BGR2GRAY);

Mat diffGray;
absdiff(gray,gray_cv,diffGray);

plt::figure_size(1200,300);
plt::subplot(1,3,1);
plt::title("Result from custom function");
plt::imshow(gray);
pltImg = displayImage(gray);
plt::subplot(1,3,2);
plt::title("Result from OpenCV function");
plt::imshow(gray_cv);
pltImg = displayImage(gray_cv);
plt::subplot(1,3,3);
plt::title("Difference");
plt::imshow(diffGray);
pltImg = displayImage(diffGray);
pltImg

///
/// AUTOGRADER TEST - DO NOT REMOVE
///


Mat convertBGRtoHSV(Mat image);

float max_custom(float a, float b, float c) 
{
   return ((a > b)? (a > c ? a : c) : (b > c ? b : c));
}

float min_custom(float a, float b, float c) 
{
   return ((a < b)? (a < c ? a : c) : (b < c ? b : c));
}

void rgb_to_hsv(int red, int green, int blue, uchar *hue, uchar *sat, uchar *value) 
{
    // R, G, B values are divided by 255
    // to change the range from 0..255 to 0..1:
    float h, s, v;
    float r, g, b;
    r = ((float)red/255.0f);
    g = ((float)green/255.0f);
    b = ((float)blue/255.0f);
//     r = red;
//     g = green;
//     b = blue;
    float cmax = max_custom(r, g, b); // maximum of r, g, b
    float cmin = min_custom(r, g, b); // minimum of r, g, b
    float diff = cmax-cmin; // diff of cmax and cmin.
    
    if (cmax == r)
        //60(G−B)/(V−min(R,G,B))
//         h = ((60.0f*(g - b))/(cmax - cmin));
        h = 60 * fmod(((g - b)/diff) , 6);
    else if (cmax == g)
        //120+60(B−R)/(V−min(R,G,B))
//         h = ((120.0f + (60.0f* (b - r)))/(cmax - cmin)) ;
        h = 60 * (((b - r)/diff) + 2);
    else if (cmax == b)
        //240+60(R−G)/(V−min(R,G,B))
//         h = ((240.0f + (60.0f* (r - g)))/(cmax - cmin)) ;
        h = 60 * (((r - g)/diff) + 4);
    
    if (h < 0.0f) h = (h + 360.0f);
    if (h > 360.0f) h = h - (int)((int)h/(int)360)*360;
//     if(h > 360) h = 360;
    
    // if cmax equal zero
      if (cmax == 0)
         s = 0.0f;
      else
         s = (diff / cmax);
    // compute v
    v = cmax ;
    
//     cout << "h=" << h << "s=" << s << "v=" << v <<endl ;
    *hue = (uchar)((h/2.0f) + 0.5);
    *sat = (uchar)((s*255.0f) + 0.5);
    *value = (uchar)((v*255.0f) + 0.5);
}

// Convert BGR image to HSV image
// Input: image in BGR mode
// Output: image in HSV mode

// Reference: https://docs.opencv.org/4.1.0/de/d25/imgproc_color_conversions.html

Mat convertBGRtoHSV(Mat image_rgb){
    Mat image = image_rgb.clone();
#if 0
    Mat imgChannels[3];
    Mat imgChannelsHSV[3];
    split(image, imgChannels);
    Mat hsv, value;
    
    value = (max(imgChannels[2], max(imgChannels[1], imgChannels[0])));//value
    
    //calculate saturation
    Mat sat, minimum;
    minimum = (min(imgChannels[2], min(imgChannels[1], imgChannels[0])));
    if( countNonZero(value) == 0)
    {
        cout << "sat in zeros" << endl;
        sat = Mat::zeros(value.size(), value.type()) ;
    }
    else
    {
        cout << "sat not in zeros" << endl;
        sat = (value - minimum)/value;
    }
    
    //calculate hue
    Mat hue, diffR, diffG, diffB;
    absdiff(value,imgChannels[2],diffR);
    absdiff(value,imgChannels[1],diffG);
    absdiff(value,imgChannels[0],diffB);
    bool eqR = (cv::countNonZero(diffR) == 0);
    bool eqG = (cv::countNonZero(diffG) == 0);
    bool eqB = (cv::countNonZero(diffB) == 0);
    cout << "Diff R=" << countNonZero(diffR) << endl;
    cout << "Diff G=" << countNonZero(diffG) << endl;
    cout << "Diff B=" << countNonZero(diffB) << endl;
    if(eqR)//R
    {
        //60(G−B)/(V−min(R,G,B))
        cout << "eqR" << endl;
        hue = (60*(imgChannels[1] - imgChannels[0]))/(value - minimum);
        
    }
    else if(eqG)//G
    {
        cout << "eqG" << endl;
        //120+60(B−R)/(V−min(R,G,B))
        hue = (Mat(value.size(), value.type(), cv::Scalar::all(120)) + 60 *(imgChannels[0] - imgChannels[2]))/ (value - minimum);
    }
    else if(eqB)//B
    {
        cout << "eqB" << endl;
       //240+60(R−G)/(V−min(R,G,B)) 
        hue = (Mat(value.size(), value.type(), cv::Scalar::all(240)) + 60 *(imgChannels[2] - imgChannels[1]))/ (value - minimum);
    }
    else
    {
        cout << "error" << endl;
    }
    
    //based on image type
    uchar depth = (image.type() & CV_MAT_DEPTH_MASK);
    
    switch ( depth ) {
        case CV_8U:
        case CV_8S:
            cout << "type=8" << endl;
            value = 255*value;
            sat = 255*sat;
            hue = hue/2;
            break;
        case CV_16U:
        case CV_16S: 
            cout << "type=16" << endl;
            value = 65535*value;
            sat = 65535*sat;
            hue = hue;
            break;
        case CV_32S:
        case CV_32F:
        case CV_64F:
            cout << "type=32" << endl;
            break;
        default: 
            cout << "type=default" << endl;
            break;
    }
    
    cout << "hue size:" << hue.size() << endl;
    cout << "sat size:" << sat.size() << endl;
    cout << "value size:" << value.size() << endl;
    //merge
//     imgChannelsHSV[0] = hue;
//     imgChannelsHSV[1] = sat;
//     imgChannelsHSV[2] = value;
//     vector<Mat> channels;

//     merge(imgChannelsHSV, hsv);
        return hsv;
#else
    double scalingFactor = 1/255.0;
    double shift = 0;
    Mat hsv;
    uchar hue, sat, value;

    //Converting from unsigned char to float(32bit)
//     image.convertTo(image, CV_32FC3, scalingFactor, shift);
    
    for (int i = 0; i < image.cols; i++) {
        for (int j = 0; j < image.rows; j++) {
            Vec3b intensity = image.at<Vec3b>(j, i);
            Vec3b &intensityChange = image.at<Vec3b>(j, i);
            rgb_to_hsv(intensity.val[2], intensity.val[1], intensity.val[0], &hue, &sat, &value);
            for(int k = 0; k < image.channels(); k++) {
                // calculate pixValue
                if(k == 0)intensityChange.val[k] = (int)(hue);
                if(k == 1)intensityChange.val[k] = (int)sat;
                if(k == 2)intensityChange.val[k] = (int)value;
            }
        }
    }
    
//     image.convertTo(image, CV_8UC3, 255.0, shift);
    
    return image;
#endif
}

Mat hsv = convertBGRtoHSV(img);

Mat hsv_cv;
cvtColor(img,hsv_cv,COLOR_BGR2HSV);

Mat diffHSV;
absdiff(hsv,hsv_cv,diffHSV);

// cout << hsv << endl;

// cout << hsv_cv << endl;

plt::figure_size(1200,300);
plt::subplot(1,3,1);
plt::title("Result from custom function");
plt::imshow(hsv);
pltImg = displayImage(hsv);
plt::subplot(1,3,2);
plt::title("Result from OpenCV function");
plt::imshow(hsv_cv);
pltImg = displayImage(hsv_cv);
plt::subplot(1,3,3);
plt::title("Difference");
plt::imshow(diffHSV);
pltImg = displayImage(diffHSV);
pltImg

///
/// AUTOGRADER TEST - DO NOT REMOVE
///


// Mat diffHSVChanells[3];
// split(diffHSV, diffHSVChanells);
// cout << diffHSVChanells[0] << endl;


