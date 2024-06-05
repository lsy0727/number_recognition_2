#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat img(500, 900, CV_8UC3, Scalar(255, 255, 255));	//3채널 컬러영상
Mat img_size(500 / 5, 200, CV_8UC3, Scalar(255, 255, 255));
Mat contour_img;
Mat gray, bin, rev;
Point ptOld;
string file_name;

void on_mouse(int event, int x, int y, int flags, void*);	//마우스 이벤트
void img_UI(Mat& img);	//영상 UI 그리기 함수

int main() {
	img_UI(img);	//UI그리기 함수 호출

	namedWindow("img");
	setMouseCallback("img", on_mouse);	//마우스이벤트 함수 호출

	imshow("img", img);
	waitKey();
	return 0;
}

void on_mouse(int event, int x, int y, int flags, void*) {
	imshow("img", img);
	Rect rect_area[] = {
		Rect(0, 0, 500, 500),	//입력창 영역
		Rect(501, 0, 199, 99),	//save 영역
		Rect(501, 500 / 5 + 1, 199, 99),	//load 영역
		Rect(501, 500 * 2 / 5 + 1, 199, 99),	//clear 영역
		Rect(501, 500 * 3 / 5 + 1, 199, 99),	//run 영역
		Rect(501, 500 * 4 / 5 + 1, 199, 99),	//exit 영역
		Rect(700, 0, 199, 99),	//contour 영역
		Rect(700, 500 / 5 + 1, 199, 99),
		Rect(700, 500 * 2 / 5 + 1, 199, 99),
		Rect(700, 500 * 3 / 5 + 1, 199, 99),
		Rect(700, 500 * 4 / 5 + 1, 199, 99)
	};
	switch (event) {
	case EVENT_LBUTTONDOWN:
		ptOld = Point(x, y);
		if (rect_area[1].contains(ptOld)) {	//save
			cout << "save press" << endl;
			cout << "저장할 파일명을 입력 : ";
			getline(cin, file_name);
			Mat save_img = img(Rect(0, 0, 500, 500));
			imwrite(file_name, save_img);
		}
		else if (rect_area[2].contains(ptOld)) {	//load
			cout << "load press" << endl;
			cout << "불러올 파일명을 입력 : ";
			getline(cin, file_name);
			Mat load_img = imread(file_name);
			load_img.copyTo(img(Rect(0, 0, 500, 500)));
		}
		else if (rect_area[3].contains(Point(x, y))) {	//clear
			cout << "clear press" << endl;
			rectangle(img, Rect(0, 0, 501, 501), Scalar(255, 255, 255), -1);
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

			contour_img = img(Rect(0, 0, 500, 500));
			cvtColor(contour_img, gray, COLOR_BGR2GRAY);
			threshold(gray, bin, 0, 255, THRESH_BINARY | THRESH_OTSU);
			rev = ~bin;
			vector<vector<Point>> contours;
			findContours(rev, contours, RETR_LIST, CHAIN_APPROX_NONE);

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
		}
		break;
	case EVENT_MOUSEMOVE:
		if (rect_area[0].contains(Point(x, y))) {
			if (flags & EVENT_FLAG_LBUTTON) {
				line(img, ptOld, Point(x, y), Scalar(0, 0, 0), 10);
				ptOld = Point(x, y);
			}
		}
		break;
	}
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
		{"contour", "feature2", "feature3", "feature4", "feafure5"} };
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
