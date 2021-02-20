#include <opencv2/photo.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include "dataPath.hpp"

using namespace std;
using namespace cv;

void readImagesAndTimes(vector<Mat> &images, vector<float> &times)
{

  int numImages = 4;

  static const float timesArray[] = {1/30.0f,0.25,2.5,15.0};
  times.assign(timesArray, timesArray + numImages);

  static const char* filenames[] = {"img_0.033.jpg", "img_0.25.jpg", "img_2.5.jpg", "img_15.jpg"};
  for(int i=0; i < numImages; i++)
  {
    Mat im = imread(DATA_PATH+"images/"+filenames[i]);
    images.push_back(im);
  }

}

int main(){
vector<Mat> images;
vector<float> times;
readImagesAndTimes(images, times);

Ptr<AlignMTB> alignMTB = createAlignMTB();
alignMTB->process(images, images);

Mat responseDebevec;
Ptr<CalibrateDebevec> calibrateDebevec = createCalibrateDebevec();
calibrateDebevec->process(images, responseDebevec, times);

Mat hdrDebevec;
Ptr<MergeDebevec> mergeDebevec = createMergeDebevec();
mergeDebevec->process(images, hdrDebevec, times, responseDebevec);
imwrite("hdrDebevec.hdr", hdrDebevec);

Mat ldrDrago;
Ptr<TonemapDrago> tonemapDrago = createTonemapDrago(1.0, 0.7);
tonemapDrago->process(hdrDebevec, ldrDrago);
ldrDrago = 3 * ldrDrago;

imshow("Drago Tonemap",ldrDrago);
waitKey(0);

Mat ldrReinhard;
Ptr<TonemapReinhard> tonemapReinhard = createTonemapReinhard(1.5, 0,0,0);
tonemapReinhard->process(hdrDebevec, ldrReinhard);

imshow("Reinhard Tonemap", ldrReinhard);
waitKey(0);

Mat ldrMantiuk;
Ptr<TonemapMantiuk> tonemapMantiuk = createTonemapMantiuk(2.2,0.85, 1.2);
tonemapMantiuk->process(hdrDebevec, ldrMantiuk);
ldrMantiuk = 3 * ldrMantiuk;

imshow("Mantiuk Tonemap", ldrMantiuk);
waitKey(0);

destroyAllWindows();
return 0;
}
