#include "opencv2/highgui/highgui.hpp"
#include "opencv\cv.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>

//как определить, что один контур внутри другого?
//2 списка контуров
//наложить контуры друг на друга
//иерархия контуров - еще 1 вариант

cv::Mat binarize(const char * filename, cv::Scalar low_color, cv::Scalar high_color)
{
	cv::Mat im = cv::imread(filename);
	imshow("original", im);
	/*binarisation*/
	//1
	cv::Mat hsv_im;
	cvtColor(im, hsv_im, CV_BGR2GRAY);
	cv::Mat binary;
	inRange(hsv_im, low_color, high_color, binary);
	cv::imshow("bin", binary);


	/*borders/*/
	cv::Mat mStructuringElement = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(5, 5));

	//cv::dilate(binary, binary, mStructuringElement, cv::Point(0, 0), 1, 1, 1);//more white
	cv::erode(binary, binary, mStructuringElement, cv::Point(-1, -1), 2);//more black
	cv::imshow("erode", binary);
	return binary;
}

std::vector<cv::Point> getCoordinates(const char * filename)
{
	std::fstream file(filename);
	std::vector<int> tmpVec;
	int flag = 0;
	while (file.good())
	{
		std::string str;
		if (flag == 0)
		{
			std::getline(file, str, ';');
			if (str == "") break;
			tmpVec.push_back(std::stoi(str));
			flag = 1;
		}
		else
		{
			std::getline(file, str, '\n');
			tmpVec.push_back(std::stoi(str));
			flag = 0;
		}
	}
	std::vector<cv::Point> coordinatesVector;
	for (int i = 0; i < tmpVec.size(); i+=2)
	{
		coordinatesVector.push_back(cv::Point(tmpVec[i], tmpVec[i + 1]));
	}
	return coordinatesVector;
}

void cutKernels(cv::Mat im, std::vector<cv::Point> centers, int width, int height)
{
	for (int i = 0; i < centers.size(); i++)
	{
		cv::Mat imKernel;
		int corner_x, corner_y;
		if (centers[i].x < width / 2) corner_x = centers[i].x; else corner_x = centers[i].x - width / 2;
		if (centers[i].y < height / 2) corner_y = centers[i].y; else corner_y = centers[i].y - height / 2;

		imKernel = im(cv::Rect(corner_x, corner_y, width, height));
		cv::resize(imKernel, imKernel, cv::Size(24, 24));
		cv::imwrite(std::to_string(i) + ".jpg", imKernel);
	}
}

int main(int argc, const char** argv)
{
	char* picturePath = NULL;
	char* dataPath = NULL;
	
	if (argc >= 2) {
		picturePath = (char*)argv[1];
		dataPath = (char*)argv[2];
	} else{
		picturePath = "D:\\studies\\cells\\1part\\4.jpg";
		dataPath = "D:\\studies\\cells\\1part\\4jpg.csv";
	}
	cv::Mat im = cv::imread(picturePath);
	cv::imshow("original", im);

	im = binarize(picturePath, cv::Scalar(20, 20, 0), cv::Scalar(255, 255, 0));

	std::vector<cv::Point> kernelCoordinates = getCoordinates(dataPath);

	cutKernels(im, kernelCoordinates, 65, 75);
	std::cout << "end";

	cv::waitKey(0);

	return 0;
}