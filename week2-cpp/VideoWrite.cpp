// Include opencv header files
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main(){

  // Create a VideoCapture object
  VideoCapture cap(0);

  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
  int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
  VideoWriter outavi("output.avi",cv::VideoWriter::fourcc('M','J','P','G'),10, Size(frame_width,frame_height));
  VideoWriter outmp4("output.mp4",cv::VideoWriter::fourcc('X','V','I','D'),10, Size(frame_width,frame_height));

  //Read and save the feed from webcam until ESC is pressed .
  while(1){

    Mat frame;
    // Capture frame-by-frame
    cap >> frame;

    // If the frame is empty, break immediately
    if (frame.empty())
      break;

    // Write the frame
    outavi.write(frame);
    outmp4.write(frame);

    imshow("Frame",frame);
    char c=(char)waitKey(25);
    if (c == 27)
      break;
  }

  // When everything done, release the video capture object
  cap.release();
  outavi.release();
  outmp4.release();

  // Closes all the frames
  destroyAllWindows();

  return 0;
}
