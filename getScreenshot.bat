@echo off
::��ȡ����
::set temp=%date:~0,10%
::set myDate=%temp:/=%
::��ȡʱ��
::set temp=%time: =0%
::set temp=%temp::=%
::set myTime=%temp:.=%
::����·��
set pcSaveFile=D:/�μ�/��Ƭ��/curriculum-design/image/screenshot.png
set mobileSaveFile=/storage/emulated/0/Pictures/Screenshots/screenshot.png
::����ADB��ͼ�ϴ�
adb shell screencap -p %mobileSaveFile%
adb pull %mobileSaveFile% %pcSaveFile%
adb shell rm %mobileSaveFile%