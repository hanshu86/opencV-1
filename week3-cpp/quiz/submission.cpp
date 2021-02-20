#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;


int main()
{
    Mat image  = imread("asset.png", 1);
    if (image.empty()) //check whether the image is loaded or not
    {
        cout << "Error : Image cannot be loaded..!!" << endl;
        //system("pause"); //wait for a key press
        return -1;
    }
    Mat imageRGB;
    cvtColor(image, imageRGB, COLOR_BGR2RGB);
    // Add 200 to image
    add(imageRGB,200,imageRGB);

    cvtColor(imageRGB, image, COLOR_RGB2BGR);

    cout << "size:" << image.size() << endl;
    namedWindow("Image");
    imshow("Image", image );
    waitKey(0);

    return 0;
}
