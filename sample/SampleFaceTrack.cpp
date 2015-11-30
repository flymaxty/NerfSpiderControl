#include <iostream>
#include <math.h>
#include <signal.h>
#include "opencv2/opencv.hpp"
#include "NerfSpiderControl.hpp"

//人脸识别相关变量
cv::VideoCapture camera;
cv::Mat rawImage, grayImage;
cv::CascadeClassifier face_cascade;
std::string face_cascade_name;
std::vector<cv::Rect> faces;
bool running = false, flagA = false, flagB = false;
int flagWalk=0, flagBarbette=0;

NerfSpiderControl spiderControl;

void clear(int signo)
{
    std::cout << "exit....." << std::endl;
    running = false;
    camera.release();
    rawImage.release();
    grayImage.release();
    std::cout << "down!\n" << std::endl;;
    exit(0);
}

void *barbetteFunc(void* cs)
{
    while(running)
    {
        if(flagA)
        {
            if(flagBarbette == 1)
                spiderControl.barbetteRotate(1, 1);
            else if(flagBarbette == -1)
                spiderControl.barbetteRotate(-1, 1);
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
        if(0)
        {
            if (flagWalk == 1)
            {
                spiderControl.footWalk(0.8);
                usleep(250000);
                spiderControl.footStop();
                usleep(100000);
            }    
            else
                spiderControl.footStop();
            flagB = false;
        }
    }
}

void init()
{
    //摄像头
    camera.open(0);

    if(!camera.isOpened())
    {
        std::cout << "can not find camera!" << std::endl;
        exit(-1);
    }

    camera.set(cv::CAP_PROP_FRAME_WIDTH,160);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT,120);

    double width = camera.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = camera.get(cv::CAP_PROP_FRAME_HEIGHT);

    std::cout << "width:" << width << "\t";
    std::cout << "height:" << height << "\t" << std::endl;

    //人脸识别
    face_cascade_name = "/usr/share/OpenCV/haarcascades/haarcascade_frontalface_alt.xml";

    if(!face_cascade.load(face_cascade_name))
    {
        std::cout << "can not find face_cascade_file!" << std::endl;
        exit(-1);
    }

    running = true;
    pthread_t walkThread, barbetteThread;
    pthread_create(&walkThread, NULL, walkFunc, NULL);
    pthread_create(&barbetteThread, NULL, barbetteFunc, NULL);

    signal(SIGINT, clear);
    signal(SIGTERM, clear);
}

int main()
{
    std::cout << "Camera FaceDetect" << std::endl;
    init();

    float faceX, faceY;
    while(1)
    {
        camera >> rawImage;

        //cv::resize(rawImage,rawImage,cv::Size(128,96));

        cv::cvtColor(rawImage, grayImage, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(grayImage, grayImage);

        faces.clear();

        face_cascade.detectMultiScale(grayImage, faces, 1.1,
            2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        if(faces.empty())
        {
            std::cout << "face lost!" << std::endl;
            flagBarbette = 0;
            flagWalk = 0;
        }
        else
        {
            std::cout << "face detect!" << faces.size() << std::endl;
            faceX = faces[0].x+faces[0].width*0.5;
            faceY = faces[0].y+faces[0].height*0.5;
            std::cout << "First face location: ";
            std::cout << faceX << "\t" << faceY << std::endl;
            if(faceX < 60)
                flagBarbette = 1;
            else if(faceX > 100)
                flagBarbette = -1;
            else
                flagBarbette = 0;
            flagWalk = 1;
        }
        flagA = true;
        flagB = true;
    }

    return 0;
}