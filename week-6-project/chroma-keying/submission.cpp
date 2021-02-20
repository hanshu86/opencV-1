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
int softnessFactor = 1;
int toleranceFactor = 58;
int toleranceFactorUp = 100;
int colorCast = 1;
int maxColorCast = 100;
Mat frame;

string windowNameOG = "Original Video";

string windowName = "Slider Window";
string trackbarSoftness = "Softness";
string trackbarTolerance = "Tolerance";
string trackbarClrCast = "Color Cast";

// Points to store the center of the circle and a point on the circumference
Point left_top, right_bottom, transient_right_bottom;
bool mouse_has_clicked = false;
bool patch_selected = false;
Scalar mean_background_color = 0;
Scalar upper_background_color = Scalar(0, 100, 0);//default
Scalar lower_background_color = Scalar(120, 255, 100);//default

//callbacks
void backgroundRemovalImage(int, void*);
void patchSelector(int action, int x, int y, int flags, void *userdata);

int bound(int n, int lower, int upper) {
  return std::max(lower, std::min(n, upper));
}

int main(){

    VideoCapture cap("greenscreen-demo.mp4");
    
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
        
        //create mask based on upper and lower limit
        Mat mask;
        inRange(frame, lower_background_color, upper_background_color, mask);
        imshow( windowNameOG, mask );
        
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
    
    upper_background_color = Scalar(bound((mean_background_color.val[0] + 0), 0, 255),
                                    bound((mean_background_color.val[1] + toleranceFactor), 0, 255),
                                    bound((mean_background_color.val[2] + 0), 0, 255));
    
    lower_background_color = Scalar(bound((mean_background_color.val[0] - 0), 0, 255),
                                    bound((mean_background_color.val[1] - toleranceFactor), 0, 255),
                                    bound((mean_background_color.val[2] - 0), 0, 255));

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
        rectangle(imgCopy, left_top, right_bottom, Scalar(0,0,255), 15, LINE_AA );
        imshow(windowNameOG, imgCopy);

        //lets do the crop of the selecte area
        Mat patchImg = imgCopy(Range(left_top.y, right_bottom.y),Range(left_top.x, right_bottom.x));
        Scalar mean, stddev;
        meanStdDev(patchImg, mean, stddev);
        
        mean_background_color = mean;
        mouse_has_clicked = false;
        patch_selected = true;
        
        //upper limit of background color
        upper_background_color = Scalar(120,
                                        255,
                                        100);
        
        lower_background_color = Scalar(0,
                                        mean_background_color.val[1] - toleranceFactor,
                                        0);

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
