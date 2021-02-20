#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define EXIT_KEY    27//escape character
#define RESTART_KEY 99//numpad 3

// Points to store the center of the circle and a point on the circumference
Point left_top, right_bottom, transient_right_bottom;
// Source image
Mat source, source_copy;
bool mouse_has_clicked = false;

// function which will be called on mouse input
void drawRectangle(int action, int x, int y, int flags, void *userdata)
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
    rectangle(source, left_top, right_bottom, Scalar(0,255,0), 2, LINE_AA );
    imshow("Window", source);

    //lets do the crop of the selecte area
    Mat crop = source_copy(Range(left_top.y, right_bottom.y),Range(left_top.x, right_bottom.x));
    imwrite("face.png",crop);

    mouse_has_clicked = false;
  }
  else if(( action == EVENT_MOUSEMOVE ) && (mouse_has_clicked))
  {
    //draw transient rectangle
    transient_right_bottom = Point(x,y);
    Mat imgCopy = source.clone();
    rectangle(imgCopy, left_top, transient_right_bottom, Scalar(255,0,0), 1, LINE_AA );
    imshow("Window", imgCopy);
  }

}

int main()
{
  source = imread("sample.jpg", IMREAD_COLOR);
  // Make a dummy image, will be useful to clear the drawing
  Mat dummy = source.clone();
  source_copy = source.clone();

  namedWindow("Crop Image");
  // highgui function called when mouse events occur
  setMouseCallback("Crop Image", drawRectangle);
  int k = 0;
  // loop until escape character is pressed
  while(k != EXIT_KEY)
  {
    imshow("Crop Image", source );
    putText(source,"Choose left top corner and Drag" ,Point(10,30), FONT_HERSHEY_SIMPLEX, 0.7,Scalar(255,255,255), 2 );
    k = waitKey(20) & 0xFF;
    if(k == RESTART_KEY)
    {
      // Another way of cloning
      dummy.copyTo(source);
    }
  }

  destroyAllWindows();
  return 0;
}
