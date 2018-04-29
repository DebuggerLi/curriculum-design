#include <iostream>
#include <cstdlib>
#include <string>
#include <opencv2/opencv.hpp>
#include "imageRecognition.h"

using namespace std;
using namespace cv;

int main() {
    while (1) {
        //��ȡ��ͼ
        getScreenshot();
        //��ȡͼ��
        Mat srcImage = imread("D:/�μ�/��Ƭ��/curriculum-design/image/screenshot.png");
        Mat tmplImage = imread("D:/�μ�/��Ƭ��/curriculum-design/bottle.png");
        //����ƿ��
        Rect bottleScope;
        Point bottleLoc = getBottleLoc(srcImage, tmplImage, bottleScope);
        //����ƽ̨
        Point platformLoc = getPlatformLoc(srcImage, bottleScope);
        //�������
        double distance = getDistance(bottleLoc, platformLoc);
        //�����ʾ
        draw(srcImage, bottleScope, bottleLoc, platformLoc);
        //���
        char command[200];
        sprintf_s(command, 200, "adb shell input swipe 540 1600 540 1600 %d", static_cast<int>(distance*1.35));
        cout << command << endl;
        system(command);
        waitKey(1000);
    }
    waitKey();
    return 0;
}