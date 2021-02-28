#include <iostream>
#include "dataPath.hpp"
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(){
// Read reference image
string refFilename(DATA_PATH + "images/form.jpg"); 
cout << "Reading reference image : " << refFilename << endl; 
Mat imReference = imread(refFilename);
imshow("Image",imReference);
waitKey(0);
// Read image to be aligned
string imFilename(DATA_PATH + "images/scanned-form.jpg");
cout << "Reading image to align : " << imFilename << endl; 
Mat img = imread(imFilename);
imshow("Image",img);
waitKey(0);

int MAX_FEATURES = 500;
float GOOD_MATCH_PERCENT = 0.15f;
// Convert images to grayscale
Mat im1Gray, im2Gray;
cvtColor(img, im1Gray, COLOR_BGR2GRAY);
cvtColor(imReference, im2Gray, COLOR_BGR2GRAY);
// Variables to store keypoints and descriptors
std::vector<KeyPoint> keypoints1, keypoints2;
Mat descriptors1, descriptors2;

// Detect ORB features and compute descriptors.
Ptr<Feature2D> orb = ORB::create(MAX_FEATURES);
orb->detectAndCompute(im1Gray, Mat(), keypoints1, descriptors1);
orb->detectAndCompute(im2Gray, Mat(), keypoints2, descriptors2);
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
drawMatches(img, keypoints1, imReference, keypoints2, matches, imMatches);
imwrite("matches.jpg", imMatches);

imshow("Matches",imMatches);
waitKey(0);

// Extract location of good matches
std::vector<Point2f> points1, points2;

for( size_t i = 0; i < matches.size(); i++ )
{
    points1.push_back( keypoints1[ matches[i].queryIdx ].pt );
    points2.push_back( keypoints2[ matches[i].trainIdx ].pt );
}

// Find homography
Mat h = findHomography( points1, points2, RANSAC );

// Use homography to warp image
Mat im1Reg;
warpPerspective(img, im1Reg, h, imReference.size());

imshow("Image",im1Reg);
waitKey(0);
// Print estimated homography
cout << "Estimated homography : \n" << h << endl;

destroyAllWindows();
return 0;
}
