#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static void help()
{
	cout
		<< "\n--------------------------------------------------------------------------" << endl
		<< "This program shows how to scan image objects in OpenCV (cv::Mat). As use case"
		<< " we take an input image and divide the native color palette (255) with the " << endl
		<< "input. Shows C operator[] method, iterators and at function for on-the-fly item address calculation." << endl
		<< "Usage:" << endl
		<< "./how_to_scan_images <imageNameToUse> <divideWith> [G]" << endl
		<< "if you add a G parameter the image is processed in gray scale" << endl
		<< "--------------------------------------------------------------------------" << endl
		<< endl;
}

Mat& ScanImageAndReduceC(Mat& I, const uchar *table);
Mat& ScanImageAndReduceIterator(Mat& I, const uchar *table);
Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar *table);

int main(int argc, char* argv[])
{
	help();

	bool isGrayImg = false;

	Mat I, J;
	if (isGrayImg)
	{
		I = imread("test_gray.png", IMREAD_GRAYSCALE);
	}
	else
	{
		I = imread("test.png", IMREAD_COLOR);
	}

	if (I.empty())
	{
		cout << "The image could not be loaded." << endl;
		return -1;
	}

	imshow("test_before", I);

	//! [dividewith]
	int divideWith = 10; // convert our input string to number - C++ style
	uchar table[256];
	for (int i = 0; i < 256; ++i)
	{
		table[i] = (uchar)(divideWith * (i / divideWith));
	}
	//! [dividewith]

	// 每一种方式测试次数
	const int times = 10;
	double t;

	// getTickCount返回从操作系统启动到当前所经的计时周期数
	// getTickFrequency返回每秒的计时周期数
	t = (double)getTickCount();

	for (int i = 0; i < times; ++i)
	{
		Mat clone_i = I.clone();
		J = ScanImageAndReduceC(clone_i, table);
	}

	// 计算一共耗时多少毫秒
	t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	// 平均耗时多少毫秒
	t /= times;

	cout << "Time of reducing with the C operator [] (averaged for " << times << " runs): " << t << " milliseconds." << endl;

	t = (double)getTickCount();

	for (int i = 0; i < times; ++i)
	{
		Mat clone_i = I.clone();
		J = ScanImageAndReduceIterator(clone_i, table);
	}

	t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	t /= times;

	cout << "Time of reducing with the iterator (averaged for " << times << " runs): " << t << " milliseconds." << endl;

	t = (double)getTickCount();

	for (int i = 0; i < times; ++i)
	{
		Mat clone_i = I.clone();
		ScanImageAndReduceRandomAccess(clone_i, table);
	}

	t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	t /= times;

	cout << "Time of reducing with the on-the-fly address generation - at function (averaged for " << times << " runs): " << t << " milliseconds." << endl;

	//! [table-init]
	Mat lookUpTable(1, 256, CV_8U);
	uchar *p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
	{
		p[i] = table[i];
	}
	//! [table-init]

	t = (double)getTickCount();

	for (int i = 0; i < times; ++i)
	{
		//! [table-use]
		LUT(I, lookUpTable, J);
		//! [table-use]
	}

	t = 1000 * ((double)getTickCount() - t) / getTickFrequency();
	t /= times;

	cout << "Time of reducing with the LUT function (averaged for " << times << " runs): " << t << " milliseconds." << endl;

	waitKey(0);

	return 0;
}

//! [scan-c] C语言方式处理
Mat& ScanImageAndReduceC(Mat& I, const uchar * const table)
{
	// accept only char type matrices
	// 只接收8位无符号精度（每一个像素中每一个通道的）
	CV_Assert(I.depth() == CV_8U);

	// 获取通道数
	int channels = I.channels();

	// 获取行
	int nRows = I.rows;
	// 获取矩阵总列数，通道数乘以矩阵的列
	int nCols = I.cols * channels;

	// 判断像素点排列在内存中是否连续，如果连续转换成一行，提高执行效率
	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar *p;
	for (i = 0; i < nRows; ++i)
	{
		// 依次取出矩阵的每一行
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			// 依次取出每个通道值进行查表转换
			p[j] = table[p[j]];
		}
	}

	return I;
}
//! [scan-c]

//! [scan-iterator] 迭代器方式处理
Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() == CV_8U);

	const int channels = I.channels();
	switch (channels)
	{
		case 1:
		{
			// 单通道
			MatIterator_<uchar> it, end;
			for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
			{
				*it = table[*it];
			}
			break;
		}
		case 3:
		{
			// 3通道
			MatIterator_<Vec3b> it, end;
			for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
			{
				(*it)[0] = table[(*it)[0]];
				(*it)[1] = table[(*it)[1]];
				(*it)[2] = table[(*it)[2]];
			}
		}
	}

	return I;
}
//! [scan-iterator]

//! [scan-random]
Mat& ScanImageAndReduceRandomAccess(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() == CV_8U);

	const int channels = I.channels();
	switch (channels)
	{
		case 1:
		{
			for (int i = 0; i < I.rows; ++i)
			{
				for (int j = 0; j < I.cols; ++j)
				{
					I.at<uchar>(i, j) = table[I.at<uchar>(i, j)];
				}
			}
			break;
		}
		case 3:
		{
			Mat_<Vec3b> _I = I;

			for (int i = 0; i < I.rows; ++i)
			{
				for (int j = 0; j < I.cols; ++j)
				{
					_I(i, j)[0] = table[_I(i, j)[0]];
					_I(i, j)[1] = table[_I(i, j)[1]];
					_I(i, j)[2] = table[_I(i, j)[2]];
				}
			}
			I = _I;
			break;
		}
	}

	return I;
}
//! [scan-random]
