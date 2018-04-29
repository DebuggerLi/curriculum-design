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

Point getBottleBottomLoc(Mat &srcImage, Mat &tmplImage, Point &matchLoc) {
    //�������ͼ��
    int retImageRows = srcImage.rows - tmplImage.rows + 1;
    int retImageCols = srcImage.cols - tmplImage.cols + 1;
    Mat retImage(retImageRows, retImageCols, CV_32FC1);
    //ģ��ƥ�䲢��һ������
    matchTemplate(srcImage, tmplImage, retImage, TM_CCOEFF_NORMED);
    normalize(retImage, retImage, 0, 1, NORM_MINMAX, -1, Mat());
    //�������ƥ��
    double minVal, maxVal;
    Point minLoc, maxLoc;
    minMaxLoc(retImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    matchLoc = maxLoc;
    Point bottleBottomLoc = Point(matchLoc.x + tmplImage.cols / 2, matchLoc.y + tmplImage.rows - 20);
    return bottleBottomLoc;
}

Point getPlatformLoc(Mat dstImage, Point matchLoc, int tmplImageRows, int tmplImageCols) {
    //ת���ɻҶ�ͼ
    cvtColor(dstImage, dstImage, COLOR_BGR2GRAY);
    //��ֵģ��
    blur(dstImage, dstImage, Size(3, 3));
    //���ұ�Ե
    Canny(dstImage, dstImage, 3, 9, 3);
    //ȥ��ƿ������
    Mat bottle = dstImage(Rect(matchLoc.x, matchLoc.y, tmplImageCols, tmplImageRows));
    bottle = { Scalar(0) };
    //����ƽ̨�϶�����50���ش�
    Point platformLoc;
    bool flag = 0;
    for (int i = 400; i != matchLoc.y + tmplImageRows; ++i) {
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

bool draw(Mat &srcImage, Point matchLoc, int tmplImageCols, int tmplImageRows, Point bottleBottomLoc, Point platformLoc) {
    rectangle(srcImage, matchLoc, Point(matchLoc.x + tmplImageCols, matchLoc.y + tmplImageRows), Scalar(0, 255, 255), 6, 8, 0);
    line(srcImage, bottleBottomLoc, platformLoc, Scalar(255, 0, 0), 10, 8);
    circle(srcImage, bottleBottomLoc, 10, Scalar(0, 0, 255), -1, 8);
    circle(srcImage, platformLoc, 10, Scalar(0, 0, 255), -1, 8);
    namedWindow("���", WINDOW_NORMAL);
    resizeWindow("���", 360, 640);
    imshow("���", srcImage);
    static unsigned int n = 0;
    string srcImageSaveFile = "D:/�μ�/��Ƭ��/curriculum-design/image/screenshot_" + to_string(++n) + ".jpg";
    imwrite(srcImageSaveFile, srcImage);
    return 0;
}

#endif