//Include opencv header files
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

#define EXIT_KEY    27//escape character

int softnessFactorUp = 100;
int softnessFactor = 0;
int toleranceFactor = 51;
int toleranceFactorUp = 100;
int colorCast = 1;
int maxColorCast = 100;
Mat frame;

string windowNameOG = "Original Video";

string windowName = "Slider Window";
string trackbarSoftness = "Softness (%)";
string trackbarTolerance = "Tolerance (%)";
string trackbarClrCast = "Defringe (%)";

// Points to store the center of the circle and a point on the circumference
Point left_top, right_bottom, transient_right_bottom;
bool mouse_has_clicked = false;
bool patch_selected = false;
Scalar mean_background_color = 0;
Scalar upper_background_color = Scalar(70,255,255);//default in HSV color space
Scalar lower_background_color = Scalar(40, 40, 40);//default in HSV color space

//callbacks
void backgroundRemovalImage(int, void*);
void patchSelector(int action, int x, int y, int flags, void *userdata);
Mat defringe(Mat image);

int getNewValue(int value, int percentage, bool up) {
    double newVal = 0;
    if(up) {
        newVal = (double)(value) + ((double)value * (double)percentage/(double)100);
    }
    else {
        newVal = (double)(value) - ((double)value * (double)percentage/(double)100);
    }
    // cout << "for value with percentage" << value << " " << percentage << " " << newVal <<  " " << up  << endl;
    
    return (int)newVal;
}

int bound(int n, int lower, int upper) {
  return std::max(lower, std::min(n, upper));
}

int main(){
    
    VideoCapture cap("greenscreen-demo.mp4");
    
    Mat newBackgroundImage = imread("new-bckImage.jpg");
    Mat backgroundImageCopy = newBackgroundImage.clone();
    
    // Check if camera opened successfully and read a frame from the object cap
    if(!cap.isOpened()){
        cout << "Error opening video stream or file" << endl;
        return -1;
    }

    while(1){

        // Capture frame-by-frame
        cap >> frame;

        // If the frame is empty, break immediately
        if (frame.empty())
            break;

        namedWindow(windowNameOG, WINDOW_NORMAL);

        // Create a window to display results
//        namedWindow(windowName, WINDOW_NORMAL);

        createTrackbar(trackbarSoftness, windowNameOG, &softnessFactor, softnessFactorUp, backgroundRemovalImage);
        createTrackbar(trackbarTolerance, windowNameOG, &toleranceFactor, toleranceFactorUp, backgroundRemovalImage);
        createTrackbar(trackbarClrCast, windowNameOG, &colorCast, maxColorCast, backgroundRemovalImage);
        
    
        if(patch_selected == false)
        {
            putText(frame,"Select Patch" ,Point(10,30), FONT_HERSHEY_SIMPLEX, 0.7,Scalar(255,0,0), 12 );
            setMouseCallback(windowNameOG, patchSelector);
            // Display the resulting frame
            imshow( windowNameOG, frame );
        }
        else
        {
            setMouseCallback(windowNameOG, 0);
        }
        
        while(patch_selected == false)
        {
            waitKey(250);
        }
        
        Mat hsv;
        cvtColor( frame, hsv, COLOR_BGR2HSV );
        
        //create mask based on upper and lower limit
        Mat mask;
        inRange(hsv, lower_background_color, upper_background_color, mask);
        
        GaussianBlur(mask, mask, Size( 3, 3 ), softnessFactor, 0 );
        Mat kernel_dialation = getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
        // erode(mask, mask, kernel_dialation, Point(-1,-1), 6);
        dilate(mask, mask, kernel_dialation, Point(-1,-1), 3);
        
        Mat bckMaskNOT;
        bitwise_not(mask, bckMaskNOT);
        Mat noBackImage, bckMaskNOT3, mask3;
//        cout << bckMaskNOT.type() << endl;
//        cout << frame.type() << endl;
        cvtColor( bckMaskNOT, bckMaskNOT3, COLOR_GRAY2BGR );
        cvtColor( mask, mask3, COLOR_GRAY2BGR );
        
        bitwise_and(bckMaskNOT3, frame, noBackImage);
        backgroundImageCopy.copyTo(newBackgroundImage);
        resize(newBackgroundImage, newBackgroundImage, frame.size(), 0);
        bitwise_and(bckMaskNOT3, frame, noBackImage);
        bitwise_and(mask3, newBackgroundImage, newBackgroundImage);
        bitwise_or(newBackgroundImage, noBackImage, noBackImage);
        noBackImage = defringe(noBackImage);
        imshow( windowNameOG, noBackImage );
        // Press ESC on keyboard to exit
        int c = waitKey(25) & 0xFF;
        if(c == EXIT_KEY)
        {
            break;
        }
    }

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return 0;
}

// Callback functions
void backgroundRemovalImage(int, void*){
    cout << "Tolerance:" <<  toleranceFactor << endl;
    cout << "Softness:" << softnessFactor << endl;
    cout << "Color Cast:" << colorCast << endl;
    
    upper_background_color = Scalar(bound(getNewValue(mean_background_color[0], toleranceFactor, true), 0, 255),
                                    bound(getNewValue(mean_background_color[1], toleranceFactor, true), 0, 255),
                                    bound(getNewValue(mean_background_color[2], toleranceFactor, true), 0, 255));
    
    lower_background_color = Scalar(bound(getNewValue(mean_background_color[0], toleranceFactor, false), 0, 255),
                                    bound(getNewValue(mean_background_color[1], toleranceFactor, false), 0, 255),
                                    bound(getNewValue(mean_background_color[2], toleranceFactor, false), 0, 255));

    cout << "Mean of background color: " << mean_background_color << endl;
    cout << "Upper Limit background color: " << upper_background_color << endl;
    cout << "Lower Limit background color: " << lower_background_color << endl;
}

// function which will be called on mouse input
void patchSelector(int action, int x, int y, int flags, void *userdata)
{
    // Action to be taken when left mouse button is pressed
    if( action == EVENT_LBUTTONDOWN )
    {
        // Mark the top left point
        left_top = Point(x,y);
        mouse_has_clicked = true;
    }
    // Action to be taken when left mouse button is released
    else if( action == EVENT_LBUTTONUP)
    {
        // Mark the bottom right point
        right_bottom = Point(x,y);
        // Draw the final rectangle in green color
        Mat imgCopy = frame.clone();
        Mat imgCopyHSV;
        cvtColor(imgCopy, imgCopyHSV, COLOR_BGR2HSV);
        rectangle(imgCopyHSV, left_top, right_bottom, Scalar(0,0,255), 15, LINE_AA );
        imshow(windowNameOG, imgCopy);

        //lets do the crop of the selecte area
        Mat patchImg = imgCopyHSV(Range(left_top.y, right_bottom.y),Range(left_top.x, right_bottom.x));
        Scalar mean, stddev;
        meanStdDev(patchImg, mean, stddev);
        
        mean_background_color = mean;
        mouse_has_clicked = false;
        patch_selected = true;
        
        //upper limit of background color (Default)
        upper_background_color = Scalar(getNewValue(mean_background_color[0], toleranceFactor, true),
                                        getNewValue(mean_background_color[1], toleranceFactor, true),
                                        getNewValue(mean_background_color[2], toleranceFactor, true));
        
        //lower limit of background selector (Default)
        lower_background_color = Scalar(getNewValue(mean_background_color[0], toleranceFactor, false),
                                        getNewValue(mean_background_color[1], toleranceFactor, false),
                                        getNewValue(mean_background_color[2], toleranceFactor, false));

        cout << "Mean of background color: " << mean_background_color << endl;
        cout << "Upper Limit background color: " << upper_background_color << endl;
        cout << "Lower Limit background color: " << lower_background_color << endl;
        
    }
    else if(( action == EVENT_MOUSEMOVE ) && (mouse_has_clicked))
    {
        //draw transient rectangle
        transient_right_bottom = Point(x,y);
        Mat imgCopy = frame.clone();
        rectangle(imgCopy, left_top, transient_right_bottom, Scalar(255,0,0), 5, LINE_AA );
        imshow(windowNameOG, imgCopy);
    }

}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

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
    
    // cout << "h=" << h << "s=" << s << "v=" << v <<endl ;
    *hue = (uchar)((h/2.0f) + 0.5);
    *sat = (uchar)((s*255.0f) + 0.5);
    *value = (uchar)((v*255.0f) + 0.5);
}

Mat defringe(Mat image)
{
    Mat retImage = image.clone();

    // cvtColor(image, retImage, COLOR_BGR2HSV);
#if 0
    for(int y=0; y<image.cols; y++){
        for(int x=0; x<image.rows; x++){
            Vec3b intensity = image.at<Vec3b>(x, y);
            Vec3b hsv_intensity = retImage.at<Vec3b>(x, y);
            int b = intensity.val[0];
            int g = intensity.val[1];
            int r = intensity.val[2];
            // cout << "here" << endl;
            Vec3b bgr;
            bgr[0] = b;
            bgr[1] = g;
            bgr[2] = r;
            uchar h,s,v;
            h = hsv_intensity.val[0];
            s = hsv_intensity.val[1];
            v = hsv_intensity.val[2];
            // rgb_to_hsv(r, g, b, &h, &s, &v);
            // cout << "I am here" << endl;
            if(h >= 60 && h <= 130 && s >= 0.15 && v > 0.15){
                if((r*b) !=0 && (g*g) / (r*b) >= 1.5){
                    retImage.at<Vec3b>(x,y) = Vec3b((int)(b*1.4),(int)g,(int)(r*1.4));
                } else{
                    retImage.at<Vec3b>(x,y) = Vec3b((int)(b*1.2),(int)g,(int)(r*1.2));
                }
            }
        }
    }

#endif
    // cout << "Image Type is: " << type2str(image.type()) << endl;
    return retImage;
}

