#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
int MAX_FEATURES = 500;
float GOOD_MATCH_PERCENT = 0.15f;

// Read reference image
string image1File(DATA_PATH + "images/scene/scene1.jpg");
cout << "Reading First Image : " << image1File << endl;
Mat im1 = imread(image1File);
string image2File(DATA_PATH + "images/scene/scene3.jpg");
cout << "Reading Second Image : " << image2File << endl;
Mat im2 = imread(image2File);

// Convert images to grayscale
Mat im1Gray, im2Gray;
cvtColor(im1, im1Gray, COLOR_BGR2GRAY);
cvtColor(im2, im2Gray, COLOR_BGR2GRAY);

// Variables to store keypoints and descriptors
std::vector<KeyPoint> keypoints1, keypoints2;
Mat descriptors1, descriptors2;

// Detect ORB features and compute descriptors.
Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);

Mat im1Keypoints;
drawKeypoints(im1,keypoints1,im1Keypoints,Scalar(0,0,255),DrawMatchesFlags::DEFAULT);
cout << "Saving Image with Keypoints";
imwrite("keypoints.jpg", im1Keypoints);

imshow("Keypoints obtained from the ORB detector",im1Keypoints);
waitKey(0);
// Match features.
std::vector<DMatch> matches;
Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
matcher->match(descriptors1, descriptors2, matches, Mat());

// Sort matches by score
std::sort(matches.begin(), matches.end());

// Remove not so good matches
const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
matches.erase(matches.begin()+numGoodMatches, matches.end());


// Draw top matches
Mat imMatches;
drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);

imshow("Matchings obtained from the descriptor matcher", imMatches);
waitKey(0);
// Extract location of good matches
std::vector<Point2f> points1, points2;

for( size_t i = 0; i < matches.size(); i++ )
{
    points1.push_back( keypoints1[ matches[i].queryIdx ].pt );
    points2.push_back( keypoints2[ matches[i].trainIdx ].pt );
}

// Find homography
Mat h = findHomography( points2, points1, RANSAC );

cout << "Homography Matrix:\n" << h;

// Use homography to warp image
int im1Height = im1.rows; int im1Width = im1.cols;
int im2Height = im2.rows; int im2Width = im2.cols;
Mat im2Aligned;
warpPerspective(im2, im2Aligned, h, Size(im2Width + im1Width, im2Height));

imshow("Second image aligned to first image obtained using homography and warping",im2Aligned);
waitKey(0);
Mat stitchedImage = im2Aligned.clone();

Rect roi (0,0,im1.cols,im1.rows);
im1.copyTo(stitchedImage(roi));

imshow("Final Stitched Image",stitchedImage);
waitKey(0);

destroyAllWindows();
return 0;
}
