#include <opencv2/opencv.hpp>
#include <iostream>
#include "dataPath.hpp"

using namespace std;
using namespace cv;

int main(){
// Read images
Mat dst = imread(DATA_PATH + "images/trump.jpg");

Mat src = imread(DATA_PATH + "images/obama.jpg");

Mat src_mask = imread(DATA_PATH + "images/obama-mask.jpg", IMREAD_GRAYSCALE);

imshow("Obama", src);
imshow("Trump", dst);
imshow("Mask", src_mask);
waitKey(0);

Mat alpha;
cvtColor(src_mask,alpha,COLOR_GRAY2BGR);
alpha.convertTo(alpha,CV_32FC3);
alpha = alpha/255.0;

src.convertTo(src,CV_32FC3);
dst.convertTo(dst,CV_32FC3);

vector<Mat> srcChannels(3), dstChannels(3), alphaChannels(3), outputBlendChannels(3);
split(src,srcChannels);
split(dst,dstChannels);
split(alpha,alphaChannels);
for (int i=0; i<3; i++){
    outputBlendChannels[i] = srcChannels[i].mul(alphaChannels[i]) + dstChannels[i].mul(1-alphaChannels[i]);
}
Mat output_blend;
merge(outputBlendChannels,output_blend);

output_blend.convertTo(output_blend,CV_8UC3);

imshow("Using Normal Blending",output_blend);
waitKey(0);

// Find blob centroid
Mat src_mask_bin;
threshold(src_mask,src_mask_bin,128,255,THRESH_BINARY);

Moments m = moments(src_mask_bin);
Point center(m.m01/m.m00, m.m10/m.m00);

// Clone seamlessly
Mat output_clone;
src.convertTo(src,CV_8UC3);
dst.convertTo(dst,CV_8UC3);
src_mask.convertTo(src_mask,CV_8UC3);
seamlessClone(src,dst,src_mask,center,output_clone,NORMAL_CLONE);

imshow("Using Seamless Cloning",output_clone);
waitKey(0);

destroyAllWindows();

return 0;
}
