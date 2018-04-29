#pragma once
#ifndef IMAGERECOGNITION_H
#define IMAGERECOGNITION_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool getScreenshot() {
    system("D:/�μ�/��Ƭ��/curriculum-design/getScreenshot.bat");
    return 0;
}

Point getBottleLoc(Mat &srcImage, Mat &tmplImage, Rect &bottleScope) {
    //�������ͼ��
    int retImageRows = srcImage.rows - tmplImage.rows + 1;
    int retImageCols = srcImage.cols - tmplImage.cols + 1;
    Mat retImage(retImageRows, retImageCols, CV_32FC1);
    //ģ��ƥ�䲢��һ������
    matchTemplate(srcImage, tmplImage, retImage, TM_CCOEFF_NORMED);
    normalize(retImage, retImage, 0, 1, NORM_MINMAX, -1, Mat());
    //�������ƥ��
    double minVal, maxVal;
    Point minLoc, maxLoc, matchLoc;
    minMaxLoc(retImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    matchLoc = maxLoc;
    bottleScope = Rect(matchLoc, tmplImage.size());
    Point bottleLoc = Point(bottleScope.x + bottleScope.width / 2, bottleScope.y + bottleScope.height - 20);
    return bottleLoc;
}

Point getPlatformLoc(Mat dstImage, Rect bottleScope) {
    //ת���ɻҶ�ͼ
    cvtColor(dstImage, dstImage, COLOR_BGR2GRAY);
    //��ֵģ��
    blur(dstImage, dstImage, Size(3, 3));
    //���ұ�Ե
    Canny(dstImage, dstImage, 3, 9, 3);
    //ȥ��ƿ������
    dstImage(bottleScope) = { Scalar(0) };
    //����ƽ̨�϶�����50���ش�
    Point platformLoc;
    bool flag = 0;
    for (int i = 400; i != bottleScope.y + bottleScope.height; ++i) {
        if (flag == 1) break;   //���������ж�
        uchar *pRow = dstImage.ptr<uchar>(i);
        for (int j = 0; j != dstImage.cols; ++j) {
            if (pRow[j]) {
                platformLoc = Point(j, i + 50);
                flag = 1;
                break;
            }
        }
    }
    return platformLoc;
}

double getDistance(Point bottleBottomLoc, Point platformLoc) {
    double distance = sqrt(pow(bottleBottomLoc.x - platformLoc.x, 2) + pow(bottleBottomLoc.y - platformLoc.y, 2));
    return distance;
}

bool draw(Mat &srcImage, Rect bottleScope, Point bottleLoc, Point platformLoc) {
    rectangle(srcImage, bottleScope, Scalar(0, 255, 255), 6, 8, 0);
    line(srcImage, bottleLoc, platformLoc, Scalar(255, 0, 0), 10, 8);
    circle(srcImage, bottleLoc, 10, Scalar(0, 0, 255), -1, 8);
    circle(srcImage, platformLoc, 10, Scalar(0, 0, 255), -1, 8);
    namedWindow("���", WINDOW_NORMAL);
    resizeWindow("���", 360, 640);
    imshow("���", srcImage);
    waitKey(30);
    static unsigned int n = 0;
    string srcImageSaveFile = "D:/�μ�/��Ƭ��/curriculum-design/image/screenshot_" + to_string(++n) + ".jpg";
    imwrite(srcImageSaveFile, srcImage);
    return 0;
}

#endif