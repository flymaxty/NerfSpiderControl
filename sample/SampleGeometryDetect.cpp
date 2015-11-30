#include <iostream>
#include <signal.h>
#include <math.h>
#include "opencv2/opencv.hpp"
#include "NerfSpiderControl.hpp"

using namespace std;
using namespace cv;

double area;
unsigned int i;

NerfSpiderControl spiderControl;
VideoCapture capture(0);
Mat img_cam, img_out;
Mat img_gray, img_thres, img_canny, img_contours;

vector<Point> poly;
vector<Vec3f> circles;
vector<vector<Point> > contours;

bool running = false, flagA = false, flagB = false;
int flagWalk=0, flagBarbette=0;

void *barbetteFunc(void* cs)
{
    while(running)
    {
        if(flagA)
        {
            if(flagBarbette == 1)
                spiderControl.barbetteRotate(1, 5);
            else if(flagBarbette == -1)
                spiderControl.barbetteRotate(-1, 5);
            else
                spiderControl.barbetteStop();
            flagA = false;
        }
    }
}

void *walkFunc(void* cs)
{
    while(running)
    {
        if(1)
        {
            if (flagWalk == 1)
            {
                spiderControl.footWalk(0.8);
                usleep(250000);
                spiderControl.footStop();
                usleep(10000);
            }    
            else
                spiderControl.footStop();
            flagB = false;
        }
    }
}

//程序终止回调函数
void clear(int signo)
{
    cout << "exit....." << endl;
    running = 0;
}

int main()
{
    //设置CTRL+C回调
    signal(SIGINT, clear);
    signal(SIGTERM, clear);

    //初始化摄像头，并减小分辨率
    capture.set(CAP_PROP_FRAME_WIDTH, 160);
    capture.set(CAP_PROP_FRAME_HEIGHT, 120);

    img_out = Mat::zeros(Size(320,240), CV_8UC1);

    running = true;
    pthread_t walkThread, barbetteThread;
    pthread_create(&walkThread, NULL, walkFunc, NULL);
    pthread_create(&barbetteThread, NULL, barbetteFunc, NULL);


    int faceX, faceY;
    while (running)
    {
        //初始化img_out和读取摄像头
        img_out.setTo(0);
        capture.read(img_cam);

        //前期处理（灰度、自适应二值化、边缘检测）
        cvtColor(img_cam, img_gray, COLOR_RGB2GRAY);
        adaptiveThreshold(img_gray, img_thres, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY,201,20);
        Canny(img_thres, img_canny, 150, 200);

        //检测轮廓
        findContours(img_canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
        for (i = 0; i < contours.size(); i++)
        {
            //根据轮廓面积初步筛选
            area = fabs(contourArea(contours[i]));
            if (area > 150)
            {
                //绘制轮廓并填充
                drawContours(img_out, contours, i, Scalar(255, 255, 255), FILLED);
                //检测是否有圆形
                HoughCircles(img_out, circles, HOUGH_GRADIENT, 2, img_out.rows / 2, 30, 15);
                if (circles.size() != 0)
                {
                    cout << "Find Circles! Area: " << area << endl;
                    faceX = circles[0][0];
                    faceY = circles[0][1];
                    cout << faceX << endl;
                    cout << faceY << endl;
                    if(faceX < 35)
                        flagBarbette = 1;
                    else if(faceX > 95)
                        flagBarbette = -1;
                    else
                        flagBarbette = 0;
                    flagWalk = 1;
                    flagA = true;
                    flagB = true;
                    break;
                }
            }
            flagWalk = 0;
        }
    }
    capture.release();
    cout << "Down!" << endl;
    return 0;
}