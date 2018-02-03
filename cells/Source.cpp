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

int main(int argc, const char** argv)
{
	char* filename = NULL;
	char* outPath = NULL;
	
	if (argc >= 2) {
		filename = (char*)argv[1];
		outPath = (char*)argv[2];
	} else{
		filename = "D://studies//cells//segmentation//17_R_Prol_40x_dapi_2.jpg";
		outPath = "result.csv";
	}
	cv::Mat im = cv::imread(filename);
	imshow("original", im);

	//cv::blur(im, im, cv::Size(50, 50));
	//imshow("blurred", im);

	cv::Mat p = cv::Mat::zeros(im.cols*im.rows, 5, CV_32F);
	cv::Mat bestLabels, centers, clustered;

	std::vector<cv::Mat> bgr(3);
	cv::split(im, bgr);
	for (int i = 0; i<im.cols*im.rows; i++) {
		p.at<float>(i, 0) = (i / im.cols) / im.rows;
		p.at<float>(i, 1) = (i%im.cols) / im.cols;
		p.at<float>(i, 2) = bgr[0].data[i] / 255.0;
		p.at<float>(i, 3) = bgr[1].data[i] / 255.0;
		p.at<float>(i, 4) = bgr[2].data[i] / 255.0;
	}

	int K = 2;
	cv::kmeans(p, K, bestLabels,
		cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 10, 1.0),
		3, cv::KMEANS_PP_CENTERS);

	//int* colors = new int[K];
	std::vector<int> colors(K);
	for (int i = 0; i<K; i++) 
	{
		colors[i] = 255 / (i + 1);
	}
	
	clustered = cv::Mat(im.rows, im.cols, CV_32F);
	for (int i = 0; i<im.cols*im.rows; i++) 
	{
		clustered.at<float>(i / im.cols, i%im.cols) = (float)(colors[bestLabels.at<int>(i, 0)]);
	}

	clustered.convertTo(clustered, CV_8U);
	imshow("clustered", clustered);

	cvWaitKey(0);

	return 0;
}