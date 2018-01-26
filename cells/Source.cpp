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
using namespace cv;
using namespace std;

std::fstream file;
std::vector<std::string> photos_of_cells = { "D://studies//cells//1part//1.jpg",
	"D://studies//cells//1part//2.jpg",
	"D://studies//cells//1part//3.jpg", 
	"D://studies//cells//1part//4.jpg", 
	"D://studies//cells//1part//8.jpg", 
	"D://studies//cells//1part//9.jpg", 
	"D://studies//cells//1part//10.jpg", 
	"D://studies//cells//1part//11.jpg", 
	"D://studies//cells//1part//12.jpg", 
	"D://studies//cells//1part//13.jpg", 
	"D://studies//cells//1part//14.jpg", 
	"D://studies//cells//1part//15.jpg", 
	"D://studies//cells//1part//16.jpg", 
	"D://studies//cells//1part//17.jpg", 
	"D://studies//cells//1part//18.jpg", 
	"D://studies//cells//1part//19.jpg", 
	"D://studies//cells//1part//20.jpg", 
	"D://studies//cells//1part//21.jpg", 
	"D://studies//cells//1part//22.jpg", 
	"D://studies//cells//1part//23.jpg", 
	"D://studies//cells//1part//24.jpg", 
	"D://studies//cells//1part//28.jpg", 
	"D://studies//cells//1part//29.jpg", 
	"D://studies//cells//1part//30.jpg", 
	"D://studies//cells//1part//31.jpg", 
	"D://studies//cells//1part//33.jpg", 
	"D://studies//cells//1part//34.jpg", 
	"D://studies//cells//1part//37.jpg",
	"D://studies//cells//1part//38.jpg", 
	"D://studies//cells//1part//39.jpg", 
	"D://studies//cells//1part//40.jpg", 
	"D://studies//cells//1part//41.jpg", 
	"D://studies//cells//1part//43.jpg", 
	"D://studies//cells//1part//44.jpg", 
	"D://studies//cells//1part//45.jpg", 
	"D://studies//cells//1part//46.jpg", 
	"D://studies//cells//1part//47.jpg", 
	"D://studies//cells//1part//48.jpg", 
	"D://studies//cells//1part//52.jpg", 
	"D://studies//cells//1part//53.jpg", 
	"D://studies//cells//1part//54.jpg", 
	"D://studies//cells//1part//55.jpg", 
	"D://studies//cells//1part//56.jpg", 
	"D://studies//cells//1part//57.jpg", 
	"D://studies//cells//1part//58.jpg", 
	"D://studies//cells//1part//59.jpg", 
	"D://studies//cells//1part//60.jpg", 
	"D://studies//cells//1part//61.jpg", 
	"D://studies//cells//1part//62.jpg", 
	"D://studies//cells//1part//63.jpg", 
	"D://studies//cells//1part//65.jpg", 
	"D://studies//cells//1part//68.jpg", 
	"D://studies//cells//1part//69.jpg", 
	"D://studies//cells//1part//71.jpg",
	"D://studies//cells//1part//72.jpg",
	"D://studies//cells//1part//73.jpg",
	"D://studies//cells//1part//75.jpg",
	"D://studies//cells//1part//76.jpg",
	"D://studies//cells//1part//77.jpg",
	"D://studies//cells//1part//79.jpg",
	"D://studies//cells//1part//85.jpg",
	"D://studies//cells//1part//86.jpg",
	"D://studies//cells//1part//87.jpg",
	"D://studies//cells//1part//88.jpg",
	"D://studies//cells//1part//89.jpg",
	"D://studies//cells//1part//90.jpg",
	"D://studies//cells//1part//91.jpg",
	"D://studies//cells//1part//92.jpg"
};

int findCells(const char * filename, cv::Scalar low_color, cv::Scalar high_color)
{
	cv::Mat im = imread(filename);
	imshow("original", im);
	/*binarisation*/
	//1
	cv::Mat hsv_im;
	cvtColor(im, hsv_im, CV_BGR2GRAY);
	cv::Mat binary;
	inRange(hsv_im, low_color, high_color, binary);
	cv::imshow("bin",binary);


	/*borders/*/
	cv::Mat mStructuringElement = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(10, 10));
	
	//cv::dilate(binary, binary, mStructuringElement, Point(0, 0), 1, 1, 1);//more white
	cv::erode(binary, binary, mStructuringElement, cv::Point(-1, -1), 2);//more black
	cv::imshow("erode", binary);
	cv::Canny(binary, binary, 50, 200);
	

	/*count cells*/
	std::vector<std::vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	//if (!file) return -1;
	//file << "imagePath;sample №;classID;centerX;centerY;leftX;topY;rightX;bottomY\n";
	cv::findContours(binary, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	for (int i = 0; i < contours.size(); ++i)
	{
		cv::drawContours(im, contours, i, cv::Scalar(0, 200, 0), 1, 8, hierarchy, 0, Point());
		cv::Rect line_rect = cv::boundingRect(contours[i]);
		cv::RotatedRect rot_rect = cv::minAreaRect(contours[i]);
		// Центр фигуры.
		cv::Point2f center = rot_rect.center;
		/*file << filename << ";" << filename << ";" << "0;" << rot_rect.center.x << ";" << rot_rect.center.y << ";"
			<< rot_rect.center.x - rot_rect.size.width / 2 << ";" << rot_rect.center.y - rot_rect.size.height / 2 << ";" 
			<< rot_rect.center.x + rot_rect.size.width / 2 << ";" << rot_rect.center.y + rot_rect.size.height / 2 << ";" << "\n";*/
	}
	imshow("contours", im);
	//file.close();
	return contours.size();
}


int main(int argc, const char** argv)
{
	char* filename = NULL;
	char* outPath = NULL;
	
	if (argc >= 2) {
		filename = (char*)argv[1];
		outPath = (char*)argv[2];
	} else{
		filename = "10.jpg";
		outPath = "result.csv";
	}
		
	//file.open(outPath);
	std::fstream file_with_colours;
	file_with_colours.open("file_with_colours.csv");
	file_with_colours << "imagePath;blue;red;green\n";
	for (int i = 0; i < photos_of_cells.size(); ++i)
	{
		int num = 0;
		std::cout << "File: " << photos_of_cells[i].c_str() << "\n";
		file_with_colours << photos_of_cells[i].c_str() << ";";

		cv::Scalar low_blue = ::Scalar(20, 20, 0); cv::Scalar high_blue = ::Scalar(255, 255, 0);
		num = findCells(photos_of_cells[i].c_str(), low_blue, high_blue);
		std::cout << "Num of blue cells: " << num << "\n";
		file_with_colours << num << ";";

		cv::Scalar low_red = ::Scalar(0, 20, 20); cv::Scalar high_red = cv::Scalar(0, 255, 255);
		num = findCells(photos_of_cells[i].c_str(), low_red, high_red);
		std::cout << "Num of red cells: " << num << "\n";
		file_with_colours << num << ";";

		cv::Scalar low_green = ::Scalar(50, 0, 100); cv::Scalar high_green = cv::Scalar(255, 0, 255);
		num = findCells(photos_of_cells[i].c_str(), low_green, high_green);
		std::cout << "Num of green cells: " << num << "\n";
		file_with_colours << num << ";\n";

	}

	file_with_colours.close();
	
	cvWaitKey(0);

	return 0;
}