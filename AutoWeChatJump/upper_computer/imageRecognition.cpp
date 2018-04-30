#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool getScreenshot() {
    //���ô��·��
    string pcSaveFile = "D:/�μ�/��Ƭ��/curriculum-design/image/screenshot.png";
    string mobileSaveFile = "/storage/emulated/0/Pictures/Screenshots/screenshot.png";
    //����ADB��ͼ�ϴ�
    char command[200];
    sprintf_s(command, 200, "adb shell screencap -p %s", mobileSaveFile.c_str());
    system(command);
    sprintf_s(command, 200, "adb pull %s %s", mobileSaveFile.c_str(), pcSaveFile.c_str());
    system(command);
    sprintf_s(command, 200, "adb shell rm %s", mobileSaveFile.c_str());
    system(command);
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
    //����ƿ�ӵķ�Χ�͵ײ�λ��
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
    //ȥ��ƿ�������Է�ƿ�ӱ�ƽ̨��
    dstImage(bottleScope + Point(-1, -1) + Size(2, 2)) = { Scalar(0) };
    //����ƽ̨�϶�����50���ش�
    Point platformLoc;
    bool flag = 0;
    for (int i = 400; i != bottleScope.y + bottleScope.height; ++i) {
        if (flag == 1) break;   //���ٶ�����ж�
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

bool draw(Mat &srcImage, Rect bottleScope, Point bottleLoc, Point platformLoc) {
    //����
    rectangle(srcImage, bottleScope, Scalar(0, 255, 255), 8, CV_AA);
    line(srcImage, bottleLoc, platformLoc, Scalar(255, 0, 0), 10, CV_AA);
    circle(srcImage, bottleLoc, 10, Scalar(0, 0, 255), -1, CV_AA);
    circle(srcImage, platformLoc, 10, Scalar(0, 0, 255), -1, CV_AA);
    //��ʾ
    namedWindow("���", WINDOW_NORMAL);
    resizeWindow("���", 360, 640);
    imshow("���", srcImage);
    waitKey(30);
    //����
    static unsigned int n = 0;
    char srcImageSaveFile[100];
    sprintf_s(srcImageSaveFile, 100, "D:/�μ�/��Ƭ��/curriculum-design/image/screenshot_%d.jpg", ++n);
    imwrite(srcImageSaveFile, srcImage);
    return 0;
}