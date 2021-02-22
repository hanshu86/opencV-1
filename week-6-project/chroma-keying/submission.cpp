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


int getNewValue(int value, int percentage, bool up) {
    double newVal = 0;
    if(up) {
        newVal = (double)(value) + ((double)value * (double)percentage/(double)100);
    }
    else {
        newVal = (double)(value) - ((double)value * (double)percentage/(double)100);
    }
    cout << "for value with percentage" << value << " " << percentage << " " << newVal <<  " " << up  << endl;
    
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
        erode(mask, mask, kernel_dialation, Point(-1,-1), 6);
        
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
