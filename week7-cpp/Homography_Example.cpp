#include <opencv2/opencv.hpp>
#include <iostream>
#include "dataPath.hpp"

using namespace cv;
using namespace std;


int main( int argc, char** argv)
{

    // Read source image.
    Mat im_src = imread(DATA_PATH + "images/book2.jpg");
    Mat im_dst = imread(DATA_PATH + "images/book1.jpg");

    // Create a vector of source and destination points.
    vector<Point2f> pts_src, pts_dst;

    pts_src.push_back(Point2f(141, 131));
    pts_src.push_back(Point2f(480, 159));
    pts_src.push_back(Point2f(493, 630));
    pts_src.push_back(Point2f(64, 601));

    pts_dst.push_back(Point2f(318, 256));
    pts_dst.push_back(Point2f(534, 372));
    pts_dst.push_back(Point2f(316, 670));
    pts_dst.push_back(Point2f(73, 473));

    // Set data for mouse event
    Mat im_out;

    // Calculate the homography
    Mat h = findHomography(pts_src, pts_dst);

    // Warp source image to destination
    warpPerspective(im_src, im_out, h, im_out.size());

    // Show image
    imshow("Source Image", im_src);
    imshow("Destination Image", im_dst);
    imshow("Warped Source Image", im_out);
    waitKey(0);

    return 0;
}
