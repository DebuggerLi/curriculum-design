#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

bool getScreenshot();
Point getBottleLoc(Mat &srcImage, Mat &tmplImage, Rect &bottleScope);
Point getPlatformLoc(Mat dstImage, Rect bottleScope);
bool draw(Mat &srcImage, Rect bottleScope, Point bottleLoc, Point platformLoc);
double getDistance(Point bottleLoc, Point platformLoc);
int distance2time(double distance);
bool longPress(int time, bool flag = 0);

int main() {
    unsigned int n = 0;
    while (1) {
        //��ȡ��ͼ
        getScreenshot();
        cout << "�� " << ++n << " ��" << endl;
        //��ȡͼ��
        Mat srcImage = imread("D:/�μ�/��Ƭ��/curriculum-design/image/screenshot.png");
        Mat tmplImage = imread("D:/�μ�/��Ƭ��/curriculum-design/bottle.png");
        //����ƿ��
        Rect bottleScope;
        Point bottleLoc = getBottleLoc(srcImage, tmplImage, bottleScope);
        cout << "ƿ��λ�ã�" << bottleLoc << endl;
        //����ƽ̨
        Point platformLoc = getPlatformLoc(srcImage, bottleScope);
        cout << "ƽ̨λ�ã�" << platformLoc << endl;
        //�����ʾ
        draw(srcImage, bottleScope, bottleLoc, platformLoc);
        //�������
        double distance = getDistance(bottleLoc, platformLoc);
        cout << "    ���룺" << distance << " px" << endl;
        //����ʱ��
        int time = distance2time(distance);
        cout << "��ѹʱ�䣺" << time << " ms" << endl;
        //����
        longPress(time);
        waitKey(1000);
    }
    waitKey();
    return 0;
}