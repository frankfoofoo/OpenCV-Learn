#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void mat_init_example()
{
	Mat img0, img1;
	// 读取测试图片
	img0 = imread("test.png", IMREAD_COLOR);
	// 初始化img2，只拷贝img0的结构体
	Mat img2(img0);
	// 初始化img1，只拷贝img0的结构体
	img1 = img0;
	// 初始化img3，取img0的一部分，矩形方法
	Mat img3(img0, Rect(0, 0, 480, 480));
	// 初始化img4，取img0的一部分，行和列方法
	Mat img4 = img0(Range::all(), Range(0, 480));
	// 初始化img5，clone方法
	Mat img5 = img0.clone();
	// 初始化img6，copyTo方法
	Mat img6;
	img0.copyTo(img6);

	imshow("mat_init_example0", img0);
	imshow("mat_init_example1", img1);
	imshow("mat_init_example2", img2);
	imshow("mat_init_example3", img3);
	imshow("mat_init_example4", img4);
	imshow("mat_init_example5", img5);
	imshow("mat_init_example6", img6);

	return;
}

void mat_create_example()
{
	// 创件一个矩阵，8行8列，3通道8位无符号，并初始化3通道的值（BGR排列）
	Mat img0(8, 8, CV_8UC3, Scalar(255, 0, 0));
	imshow("mat_create_example0", img0);
	cout << "img0-0 = " << endl << img0 << endl << endl;

	// 创建多维度的矩阵
	int sz[3] = { 2, 2, 2 };
	Mat img1(3, sz, CV_8UC(1), Scalar::all(0));

	// 使用create方法创件矩阵
	img0.create(4, 4, CV_8UC(3));
	imshow("mat_create_example1", img0);
	cout << "img0-1 = " << endl << img0 << endl << endl;

	Mat img2;
	img2.create(8, 8, CV_8UC(3));
	imshow("mat_create_example2", img2);
	cout << "img2 = " << endl << img2 << endl << endl;

	// Matlab风格初始化
	Mat O = Mat::ones(2, 2, CV_32F);
	cout << "O = " << endl << O << endl << endl;
	Mat Z = Mat::zeros(3, 3, CV_8UC1);
	cout << "Z = " << endl << Z << endl << endl;
	Mat E = Mat::eye(4, 4, CV_64F);
	cout << "E = " << endl << E << endl << endl;

	// 使用逗号分隔符初始化小型矩阵
	Mat C = (Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	cout << "C = " << endl << " " << C << endl << endl;
	C = (Mat_<double>({ 0, -1, 0, -1, 5, -1, 0, -1, 0 })).reshape(3);
	cout << "C = " << endl << " " << C << endl << endl;

	// 为已经存在的矩阵创见一个头结构
	Mat RowClone = C.row(0).clone();
	cout << "RowClone = " << endl << " " << RowClone << endl << endl;

	// 使用randu函数随机初始化矩阵值，并用不同格式化打印输出
	Mat img3(3, 2, CV_8UC3);
	randu(img3, Scalar::all(0), Scalar::all(255));
	cout << "img3 (default)= " << endl << img3 << endl << endl;
	cout << "img3 (python)  = " << endl << format(img3, Formatter::FMT_PYTHON) << endl << endl;
	cout << "img3 (csv)  = " << endl << format(img3, Formatter::FMT_CSV) << endl << endl;
	cout << "img3 (numpy)  = " << endl << format(img3, Formatter::FMT_NUMPY) << endl << endl;
	cout << "img3 (c)  = " << endl << format(img3, Formatter::FMT_C) << endl << endl;

	// 打印二维点坐标
	Point2f P(5.0, 1.2);
	cout << "Point (2D) = " << P << endl << endl;

	// 打印三维点坐标
	Point3f P3f(2, 6, 7);
	cout << "Point (3D) = " << P3f << endl << endl;

	vector<float> v;
	v.push_back((float)CV_PI);
	v.push_back(2);
	v.push_back(3.01f);
	cout << "Vector of floats via Mat = " << Mat(v) << endl << endl;

	vector<Point2f> vPoints(20);
	for (size_t i = 0; i < vPoints.size(); ++i)
	{
		vPoints[i] = Point2f((float)(i * 5), (float)(i % 7));
	}
	cout << "A vector of 2D Points = " << vPoints << endl << endl;

	return;
}

void image_op_example()
{
	Mat img0 = imread("test.png", IMREAD_COLOR);
	Mat img0_gray;

	// 颜色空间转换，将BGR转为灰度图
	cvtColor(img0, img0_gray, COLOR_BGR2GRAY);
	// 写本地文件
	imwrite("test_gray.png", img0_gray);

	namedWindow("img0", WINDOW_AUTOSIZE);
	namedWindow("img0_gray", WINDOW_AUTOSIZE);

	imshow("img0", img0);
	imshow("img0_gray", img0_gray);
	
	return;
}

int main(int argc, char **argv)
{
	mat_init_example();
	mat_create_example();
	image_op_example();

	waitKey(0);

	return 0;
}