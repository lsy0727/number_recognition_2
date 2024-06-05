#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat img(500, 900, CV_8UC3, Scalar(255, 255, 255));	//3채널 컬러영상
Mat img_size(500 / 5, 200, CV_8UC3, Scalar(255, 255, 255));
Mat draw_img;
Mat gray, bin;
Point ptOld, startpt1, startpt2, endpt;
string file_name;
void on_mouse(int event, int x, int y, int flags, void*);	//마우스 이벤트
void img_UI(Mat& img);	//영상 UI 그리기 함수
Mat morph(Mat img, Point staratpt1, Point startpt2, Point endpt, int count);	//모폴로지 연산
Mat bounding_img(Mat img);	//바운딩박스

int main() {
	img_UI(img);	//UI그리기 함수 호출

	namedWindow("img");
	setMouseCallback("img", on_mouse);	//마우스이벤트 함수 호출

	imshow("img", img);
	waitKey();
	return 0;
}

void on_mouse(int event, int x, int y, int flags, void*) {
	static int count = 0;	//획 수를 카운팅하기 위한 변수
	imshow("img", img);
	Rect rect_area[] = {
		Rect(0, 0, 500, 500),	//0. 입력창 영역
		Rect(501, 0, 199, 99),	//1. save 영역
		Rect(501, 500 / 5 + 1, 199, 99),	//2. load 영역
		Rect(501, 500 * 2 / 5 + 1, 199, 99),	//3. clear 영역
		Rect(501, 500 * 3 / 5 + 1, 199, 99),	//4. run 영역
		Rect(501, 500 * 4 / 5 + 1, 199, 99),	//5. exit 영역
		Rect(700, 0, 199, 99),	//6. contour 영역
		Rect(700, 500 / 5 + 1, 199, 99),	//7. center 영역
		Rect(700, 500 * 2 / 5 + 1, 199, 99),	//8. stroke 영역
		Rect(700, 500 * 3 / 5 + 1, 199, 99),	//9.
		Rect(700, 500 * 4 / 5 + 1, 199, 99)	//10.
	};
	switch (event) {
	startpt1, startpt2, endpt;
	case EVENT_LBUTTONDOWN:
		ptOld = Point(x, y);
		if (count == 0) startpt1 = Point(x, y);
		else if (count == 1) startpt2 = Point(x, y);
		if (rect_area[1].contains(Point(x, y))) {	//save
			cout << "save press" << endl;
			cout << "저장할 파일명을 입력 : ";
			getline(cin, file_name);
			Mat save_img = img(Rect(0, 0, 500, 500));
			imwrite(file_name, save_img);
		}
		else if (rect_area[2].contains(Point(x, y))) {	//load
			cout << "load press" << endl;
			cout << "불러올 파일명을 입력 : ";
			getline(cin, file_name);
			Mat load_img = imread(file_name);
			load_img.copyTo(img(Rect(0, 0, 500, 500)));
		}
		else if (rect_area[3].contains(Point(x, y))) {	//clear
			cout << "clear press" << endl;
			rectangle(img, Rect(0, 0, 501, 501), Scalar(255, 255, 255), -1);
			count = 0;
		}
		else if (rect_area[4].contains(Point(x, y))) {	//run
			cout << "run press" << endl;

		}
		else if (rect_area[5].contains(Point(x, y))) {	//exit
			cout << "exit press" << endl;
			cout << "프로그램 종료" << endl;
			exit(0);	//종료
		}
		else if (rect_area[6].contains(Point(x, y))) {	//contour
			cout << "contour press" << endl;

			bin = morph(img, startpt1, startpt2, endpt, count);
			vector<vector<Point>> contours;
			findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);
			cout << "외곽선 갯수 : " << contours.size() << endl;
			if (contours.size() == 1) {
				cout << "예상 결과 : 1, 2, 3, 5, 7" << endl;
			}
			else if (contours.size() == 2) {
				cout << "예상 결과 : 0, 4, 6, 9" << endl;
			}
			else if (contours.size() == 3) {
				cout << "예상 결과 : 8" << endl;
			}
			imshow("contour", bin);
		}
		else if (rect_area[7].contains(Point(x, y))) {	//무게중심
			cout << "center press" << endl;

			Mat bin = bounding_img(img);

			Mat labels, stats, centroids;
			int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
			int width = stats.at<int>(1, 2);	//width
			int height = stats.at<int>(1, 3);	//height
			int center_x = centroids.at<double>(1, 0);	//무게중심 x좌표
			int center_y = centroids.at<double>(1, 1);	//무게중심 y좌표
			int per_x = (double)center_x / (width + center_x) * 100;
			int per_y = (double)center_y / (height + center_y) * 100;
			cout << per_x << "%" << endl << per_y << "%" << endl;
			imshow("bounding box", bin);
		}
		else if (rect_area[8].contains(Point(x, y))) {	//획
			cout << "stroke press" << endl;

			cout << "전체 획 수 : " << count;
			if (count == 1) {
				cout << "예상 결과 : 0, 1, 2, 3, 6, 7, 8, 9" << endl;
			}
			else if (count == 2) {
				cout << "예상 결과 : 4, 5, 7" << endl;
			}
		}
		break;
	case EVENT_LBUTTONUP:
		if (rect_area[0].contains(Point(x, y))) {
			endpt = Point(x, y);
			count++;
		}
		break;
	case EVENT_MOUSEMOVE:
		if (rect_area[0].contains(Point(x, y))) {
			if (flags & EVENT_FLAG_LBUTTON) {
				line(img, ptOld, Point(x, y), Scalar(0), 10);
				ptOld = Point(x, y);
			}
		}
		break;
	}
}
Mat morph(Mat img, Point startpt1, Point startpt2, Point endpt, int count) {	//모폴로지 연산
	Mat bin;
	draw_img = img(Rect(0, 0, 500, 500));
	cvtColor(draw_img, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	if (count == 1) {	//1획이면 시작점과 끝점 거리 비교
		double distance = sqrt(pow(endpt.x - startpt1.x, 2) + pow(endpt.y - startpt1.y, 2));
		if (distance < 150) {
			line(bin, startpt1, endpt, Scalar(255), 5);
		}
	}
	else if (count == 2) {	//2획이면 각 획의 시작점 거리 비교
		double distance = sqrt(pow(startpt2.x - startpt1.x, 2) + pow(startpt2.y - startpt1.y, 2));
		if (distance < 150) {
			line(bin, startpt1, startpt2, Scalar(255), 5);
		}
	}

	int morph_size = 10;
	if (cnt > 2) {
		while (true) {
			morphologyEx(bin, bin, MORPH_CLOSE, Mat(morph_size, morph_size, CV_8UC1));
			cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
			if (cnt <= 2) break;
			morph_size += 3;
		}
	}
	

	return bin;
}

Mat bounding_img(Mat img) {	//바운딩 박스
	Mat bin = morph(img, startpt1, startpt2, endpt, 0);
	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);

	int* p = stats.ptr<int>(1);

	bin = bin(Rect(p[0], p[1], p[2], p[3]));
	return bin;
}

void img_UI(Mat& img) {
	//칸 나누기
	line(img, Point(502, 0), Point(502, 500), Scalar(0, 0, 0), 2);
	line(img, Point(700, 0), Point(700, 500), Scalar(0, 0, 0), 2);
	for (int i = 1; i < 5; i++) {
		line(img, Point(502, 500 * i / 5), Point(700, 500 * i / 5), Scalar(0, 0, 0), 2);
		line(img, Point(700, 500 * i / 5), Point(900, 500 * i / 5), Scalar(0, 0, 0), 2);
	}

	//UI설계
	vector<vector<string>> text = { {"Save", "Load", "Clear", "Run", "Exit"},
		{"contour", "center", "stroke", "feature4", "feafure5"} };
	int fontface = FONT_HERSHEY_SIMPLEX;	//폰트 종류
	double fontscale = 1.0;	//폰트 크기
	int thickness = 2;	//글씨 두께
	for (int i = 0; i < text.size(); i++) {
		for (int j = 0; j < text[i].size(); j++) {
			Size TextSize = getTextSize(text[i][j], fontface, fontscale, thickness, 0);	//글씨 크기
			Size imgsize = img_size.size();	//객체 사이즈
			Point org(500 + i * 200 + (imgsize.width - TextSize.width) / 2,
				500 * j / 5 + (imgsize.height + TextSize.height) / 2);
			putText(img, text[i][j], org, fontface, fontscale, Scalar(0, 0, 0), thickness);
		}
	}
}
