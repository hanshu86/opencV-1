
#include "includeLibraries.h"
#include <opencv2/opencv.hpp>
#include "matplotlibcpp.h"
#include "displayImages.h"

using namespace std;

using namespace cv;

using namespace matplotlibcpp;

Mat pencilSketch(Mat image, int arguments=0){
    
    Mat pencilSketchImage;
    
    Mat gray_image, smooth_image, abs_image, laplacian;
    int kernelSize = 3;

    cvtColor( image, gray_image, COLOR_BGR2GRAY );
    GaussianBlur(gray_image, smooth_image, Size(3, 3), 0);
//     Laplacian(gray_image, laplacian, CV_64F, kernelSize, 1, 0);
    Laplacian(smooth_image, laplacian, CV_16U, 5);
    convertScaleAbs( laplacian, abs_image , (2+1)*0.25);
    pencilSketchImage =  cv::Scalar::all(255) - abs_image;
    
    threshold(pencilSketchImage, pencilSketchImage, 150, 255, THRESH_BINARY);

//     Mat medianBlurred;
//     medianBlur(pencilSketchImage,medianBlurred,kernelSize);
//     normalize(cartoonImage, cartoonImage, 0, 1, NORM_MINMAX, CV_32F);

    return pencilSketchImage;
}

Mat cartoonify(Mat image, int arguments=0){
    
    Mat cartoonImage;
    Mat ogImage = image.clone();
    
    Mat edgeImage = pencilSketch(image);
    
    cvtColor( edgeImage, edgeImage, COLOR_GRAY2BGR );
    bitwise_and(ogImage, edgeImage, cartoonImage);

//     cout << ogImage.size() << endl;
//     cout << edgeImage.size() << endl;
//     cout << cartoonImage.size() << endl;
    return (cartoonImage);
}

string imagePath = DATA_PATH + "images/trump.jpg";
Mat image = imread(imagePath);

Mat cartoonImage = cartoonify(image);
Mat pencilSketchImage = pencilSketch(image);

plt::figure_size(1200,500);
plt::subplot(1,3,1);
plt::imshow(image);
auto pltImg = displayImage(image);
plt::subplot(1,3,2);
plt::imshow(cartoonImage);
pltImg = displayImage(cartoonImage);
plt::subplot(1,3,3);
plt::imshow(pencilSketchImage);
pltImg = displayImage(pencilSketchImage);
pltImg


