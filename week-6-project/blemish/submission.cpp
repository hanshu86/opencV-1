#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

#define EXIT_KEY    27//escape character
#define RESTART_KEY 114//r character

#define DRAW_RECTANGLE_PATCH 0 //for debugging patch selection
#define SHOW_EACH_PATCH 0 //show each path on ROI image window
#define SHOW_LOWEST_GRADIENT_PATH 0 //show loest gradient patch selected for seamless cloning


// Points to store the center of the circle and a point on the circumference
Point blemish_point;
// Source image
Mat source, source_copy;
const int radius = 15;
const int neighbors = 8;
int patchedFound = 0;
vector<Mat> roi(9); //store 8 neighbor and itself at 0
bool l_button_clicked = false;

int bound(int n, int lower, int upper) {
  return std::max(lower, std::min(n, upper));
}

void getRegionAroundBlemish(Point center)
{
    //    cout << "Blemish Point:" << center.x << " " << center.y << endl;
    //lets get roi around the center with radius for source
    Mat region;
    int x_start, y_start, x_end, y_end;
    int index = 0;
    patchedFound = 0;
    
    //blemish itself at index 0
    x_start = bound((center.x - radius), 0, source.size().width);
    x_end = bound((center.x + radius), 0, source.size().width);
    y_start = bound((center.y - radius), 0, source.size().height);
    y_end = bound((center.y + radius), 0, source.size().height);
    
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
    
#if DRAW_RECTANGLE_PATCH
    cout << "point:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(255, 0, 0), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
#endif
    
    //left bottom neighbor
    x_start = bound((center.x - (3*radius)), 0, source.size().width);
    x_end = bound((center.x - radius), 0, source.size().width);
    y_start = bound((center.y + radius), 0, source.size().height);
    y_end = bound((center.y + (3*radius)), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Left Bottom:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(0, 255, 0), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif
    //Bottom neighbor
    x_start = bound((center.x - radius), 0, source.size().width);
    x_end = bound((center.x + radius), 0, source.size().width);
    y_start = bound((center.y + radius), 0, source.size().height);
    y_end = bound((center.y + (3*radius)), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Bottom:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(0, 0, 255), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif

    //left bottom neighbor
    x_start = bound((center.x + radius), 0, source.size().width);
    x_end = bound((center.x + (3*radius)), 0, source.size().width);
    y_start = bound((center.y + radius), 0, source.size().height);
    y_end = bound((center.y + (3*radius)), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Right Bottom:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(255, 0, 255), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif
    
    //left neighbor
    x_start = bound((center.x - (3*radius)), 0, source.size().width);
    x_end = bound((center.x - radius), 0, source.size().width);
    y_start = bound((center.y - radius), 0, source.size().height);
    y_end = bound((center.y + radius), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Left:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(0, 255, 255), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif
    
    //right neighbor
    x_start = bound((center.x + (radius)), 0, source.size().width);
    x_end = bound((center.x + (3*radius)), 0, source.size().width);
    y_start = bound((center.y - radius), 0, source.size().height);
    y_end = bound((center.y + radius), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Right:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(140, 140, 140), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif

    //Left Top neighbor
    x_start = bound((center.x - (3*radius)), 0, source.size().width);
    x_end = bound((center.x - radius), 0, source.size().width);
    y_start = bound((center.y - (3*radius)), 0, source.size().height);
    y_end = bound((center.y - radius), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Left Top:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(0, 180, 140), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif
    
    //top neighbor
    x_start = bound((center.x - radius), 0, source.size().height);
    x_end = bound((center.x + radius), 0, source.size().height);
    y_start = bound((center.y - (3* radius)), 0, source.size().height);
    y_end = bound((center.y - radius), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Top:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(180, 0, 140), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif

    //top right neighbor
    x_start = bound((center.x + radius), 0,source.size().width);
    x_end = bound((center.x + (3*radius)), 0, source.size().width);
    y_start = bound((center.y - (3* radius)), 0, source.size().height);
    y_end = bound((center.y - radius), 0, source.size().height);
    region = source(cv::Range(y_start,y_end), cv::Range(x_start, x_end));
    roi.at(index++) = region;
    if((region.size().height > 0) && (region.size().width > 0)) patchedFound++;
#if SHOW_EACH_PATCH
    imshow("ROI", region );
    waitKey(0);
#endif
#if DRAW_RECTANGLE_PATCH
    cout << "Top Right:" << x_start << "," << y_start << ":" << x_end << "," << y_end << endl;
    rectangle(source, Point(x_start, y_start), Point(x_end, y_end),Scalar(0, 0, 0), 5, LINE_8);
    imshow("Blemish Remover", source );
    waitKey(0);
    source_copy.copyTo(source);
#endif
}

int getIndexWithMinGradient(void)
{
    int index = -1;
    double variance_abs_value = DBL_MAX;
    //except index 0 since its blemish itself lets find index of region
    //with minimum gradient for next step
    cout << "patched found: " << patchedFound << endl;
    for(int i = 1; i <= patchedFound; i++)
    {
        //lets apply sobel filter on each region
        Mat region = roi.at(i);
        Mat sobelx, sobely, grad, region_gray;
        Mat abs_grad_x, abs_grad_y;
        Scalar mean, stddev;
        
        //take care of crash if empty matrix
        if((region.size().width <= 0) || (region.size().height <= 0))continue;
        
        cvtColor(region, region_gray, COLOR_BGR2GRAY);

        // Apply sobel filter with only x gradient
        Sobel(region_gray, sobelx, CV_32F, 1, 0, 3, 1, 0, BORDER_DEFAULT);

        // Apply sobel filter with only y gradient
        Sobel(region_gray, sobely, CV_32F, 0, 1, 3, 1, 0, BORDER_DEFAULT);
        
        // converting back to CV_8U
        convertScaleAbs(sobelx, abs_grad_x);
        convertScaleAbs(sobely, abs_grad_y);
        
        addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
        meanStdDev(grad, mean, stddev);
        double variance = stddev.val[0] * stddev.val[0];
//        cout << i << ":" << variance << endl;
        if(variance < variance_abs_value){
            variance_abs_value = variance;
            index = i;
        }
    }
    
    cout << "Lowest Gradient patch is at index:" << index << endl;
    return index;
}
// function which will be called on mouse input
void removeBlemish(int action, int x, int y, int flags, void *userdata)
{
    // Action to be taken when left mouse button is pressed
    if( action == EVENT_LBUTTONDOWN )
    {
        // Mark the top left point
        blemish_point = Point(x,y);
        l_button_clicked = true;
        getRegionAroundBlemish(blemish_point);
        int lowest_grad_index = getIndexWithMinGradient();
        if((lowest_grad_index > 0) && (lowest_grad_index <= 8))
        {
            Mat src = roi.at(lowest_grad_index);
            Mat src_mask = 255 * Mat::ones(src.rows, src.cols, src.depth());
    #if SHOW_LOWEST_GRADIENT_PATH
            namedWindow("LGP");
            imshow("LGP", src );
    #endif
//            cout << "src size:" << src.size() << endl;
           if( 0 <= blemish_point.x && 0 <= src.size().width && blemish_point.x + src.size().width <= source.cols && 0 <= blemish_point.y && 0 <= src.size().height && blemish_point.y + src.size().height <= source.rows)
           {
                seamlessClone(src, source, src_mask, blemish_point, source, cv::NORMAL_CLONE);
           }
            else
            {
                cout << "Error: Out of bound chosen..so no action" << endl;
            }
        }
        else
        {
            cout << "Error: No index found" << endl;
        }
    }
    else if ((action == EVENT_LBUTTONUP) && (l_button_clicked == true))
    {
        //show image with blemished nemoved
        imshow("Blemish Remover", source );
        blemish_point = Point(-1,-1);//reset blemish point
        l_button_clicked = false;
    }

}

int main()
{
    source = imread("blemish.png", IMREAD_COLOR);
    // Make a dummy image, will be useful to reset the window
    Mat dummy = source.clone();
    source_copy = source.clone();

    namedWindow("Blemish Remover");
#if SHOW_EACH_PATCH
    namedWindow("ROI");
#endif
    cout << source.size().height << " " << source.size().width << endl;
    // highgui function called when mouse events occur
    setMouseCallback("Blemish Remover", removeBlemish);
    int k = 0;
    // loop until escape character is pressed
    while(k != EXIT_KEY)
    {
        imshow("Blemish Remover", source );
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
