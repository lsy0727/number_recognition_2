#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

Mat img(500, 900, CV_8UC3, Scalar(255, 255, 255));	//3채널 컬러영상
Mat img_size(500 / 5, 200, CV_8UC3, Scalar(255, 255, 255));
Mat draw_img;
Mat gray, bin;
Point ptOld;
string file_name;

void on_mouse(int event, int x, int y, int flags, void*);	//마우스 이벤트
void img_UI(Mat& img);	//UI 그리기 함수
int ProjectRun(Mat img);	//숫자 인식 함수
Mat morph(Mat img);	//모폴로지 연산
Mat bounding_img(Mat img);	//바운딩박스
int getContourCount(Mat img);	//외곽선 개수
Point getCenterPt(Mat img);	//무게중심 좌표 비
Point SearchCount(Mat img);	//객체 2분할 외곽선 개수
tuple<int, int, int, int> PixelCount(Mat img);	//가로, 세로로 나눴을 때 객체의 픽셀 비율

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
		Rect(0, 0, 500, 500),	//0. 입력창 영역
		Rect(501, 0, 199, 99),	//1. save 영역
		Rect(501, 500 / 5 + 1, 199, 99),	//2. load 영역
		Rect(501, 500 * 2 / 5 + 1, 199, 99),	//3. clear 영역
		Rect(501, 500 * 3 / 5 + 1, 199, 99),	//4. run 영역
		Rect(501, 500 * 4 / 5 + 1, 199, 99),	//5. exit 영역
		Rect(700, 0, 199, 99),	//6. contour 영역
		Rect(700, 500 / 5 + 1, 199, 99),	//7. center 영역
		Rect(700, 500 * 2 / 5 + 1, 199, 99),	//8. split contour 영역
		Rect(700, 500 * 3 / 5 + 1, 199, 99),	//9. split pixel 영역
		Rect(700, 500 * 4 / 5 + 1, 199, 99)	//10.
	};
	switch (event) {
	case EVENT_LBUTTONDOWN:
		ptOld = Point(x, y);

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
		}
		else if (rect_area[4].contains(Point(x, y))) {	//run
			cout << "run press" << endl;

			int num = ProjectRun(img);
			if (num == -1) cout << "recognition failed!" << endl;
			else cout << "결과 : " << num << endl;
		}
		else if (rect_area[5].contains(Point(x, y))) {	//exit
			cout << "exit press" << endl;
			cout << "프로그램 종료" << endl;
			exit(0);	//종료
		}
		else if (rect_area[6].contains(Point(x, y))) {	//contour
			cout << "contour press" << endl;

			int contour_count = getContourCount(img);

			cout << "외곽선 갯수 : " << contour_count << endl;
			if (contour_count == 1) {
				cout << "예상 결과 : 1, 2, 3, 5, 7" << endl;
			}
			else if (contour_count == 2) {
				cout << "예상 결과 : 0, 4, 6, 9" << endl;
			}
			else if (contour_count == 3) {
				cout << "예상 결과 : 8" << endl;
			}
		}
		else if (rect_area[7].contains(Point(x, y))) {	//무게중심 비
			cout << "center press" << endl;

			Point center_pt = getCenterPt(img);

			cout << "X : " << center_pt.x << "%" << endl;
			cout << "Y : " << center_pt.y << "%" << endl;
		}
		else if (rect_area[8].contains(Point(x, y))) {	//분할 객체 외곽선 최대 개수
			cout << "split contour press" << endl;

			Point count = SearchCount(img);

			cout << "y축으로 나눴을 때 최대 외곽선 : " << count.x << endl;
			cout << "x축으로 나눴을 때 최대 외곽선: " << count.y << endl;
		}
		else if (rect_area[9].contains(Point(x, y))) {	//분할 객체 픽셀 개수 비
			cout << "pixel count press" << endl;

			int u_pixel_count, d_pixel_count, l_pixel_count, r_pixel_count;
			tie(u_pixel_count, d_pixel_count, l_pixel_count, r_pixel_count) = PixelCount(img);

			cout << "위쪽 객체 픽셀 수 : " << u_pixel_count << "%" << endl;
			cout << "아래쪽 객체 픽셀 수 : " << d_pixel_count << "%" << endl;
			cout << "왼쪽 객체 픽셀 수 : " << l_pixel_count << "%" << endl;
			cout << "오른쪽 객체 픽셀 수 : " << r_pixel_count << "%" << endl;
		}
		break;
	case EVENT_MOUSEMOVE:
		if (rect_area[0].contains(Point(x, y))) {
			if (flags & EVENT_FLAG_LBUTTON) {
				line(img, ptOld, Point(x, y), Scalar(0), 5);
				ptOld = Point(x, y);
			}
		}
		break;
	}
}
int ProjectRun(Mat img) {
	bin = morph(img);
	bin = bounding_img(bin);
	int contour_count = getContourCount(img);
	Point center_pt = getCenterPt(img);
	Point count = SearchCount(img);
	int u_count, d_count, l_count, r_count;
	tie(u_count, d_count, l_count, r_count) = PixelCount(img);

	if (contour_count == 3) return 8;	//외곽선 3개 
	else if (contour_count == 2) {	//외곽선 2개 - 0,4,6,9
		if (abs(center_pt.x - center_pt.y) < 15 && center_pt.x > 40 && center_pt.y < 60
			&& abs(u_count - d_count) < 10 && abs(l_count - r_count) < 10) return 0;
		else if (center_pt.y < 50 && center_pt.x > 50 && abs(u_count - d_count) > 50) return 9;
		else if (center_pt.y > 50) return 6;
		else return 4;
	}
	else if (contour_count == 1) {	//외곽선 1개 - 1,2,3,4,5,7
		if (count.x == 2 && count.y == 2) return 1;
		else if (count.x == 4) {	//2,5,7
			if (count.y == 3) {
				if (center_pt.y > 55) return 2;
				else if (l_count - r_count > 20) return 5;
				else if (center_pt.y <= 55 && center_pt.x > 45) return 7;
			}
			else return 5;
		}
		else if (count.x == 3) return 7;
		else if (count.x == 5) {	//y축으로 나눴을 때 외곽선 최대 개수 5 - 3,5
			if (l_count > r_count) return 5;
			else if (l_count < r_count) return 3;
		}
	}
	return -1;
}
int getContourCount(Mat img) {	//외곽선 개수
	bin = morph(img);
	bin = bounding_img(bin);

	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);
	imshow("boundingbox", bin);
	return contours.size();
}
Point getCenterPt(Mat img) {	//무게중심 좌표 비
	vector<int> center_res;
	bin = morph(img);
	bin = bounding_img(bin);

	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	int width = stats.at<int>(1, 2);  // width
	int height = stats.at<int>(1, 3); // height
	int center_x = centroids.at<double>(1, 0); // 무게중심 x좌표
	int center_y = centroids.at<double>(1, 1); // 무게중심 y좌표
	//비율 계산
	int per_x = (double)center_x / width * 100;
	center_res.push_back(per_x);
	int per_y = (double)center_y / height * 100;
	center_res.push_back(per_y);
	
	cvtColor(bin, bin, COLOR_GRAY2BGR);
	circle(bin, Point(center_x, center_y), 2, Scalar(0, 0, 255), 2, -1);
	imshow("boundingbox", bin);
	return Point(per_x, per_y);
}
Point SearchCount(Mat img) {
	draw_img = img(Rect(0, 0, 500, 500));
	bin = ~draw_img;
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(5, 20, CV_8UC1));
	bin = bounding_img(~bin);

	int cols_count = 0;
	for (int i = 1; i < bin.cols; i++) {
		Mat l_dst = bin(Rect(0, 0, i, bin.rows));	//왼쪽 객체
		vector<vector<Point>> contours1, contours2;
		findContours(l_dst, contours1, RETR_LIST, CHAIN_APPROX_NONE);
		Mat r_dst = bin(Rect(i, 0, bin.cols - i, bin.rows));	//오른쪽 객체
		findContours(r_dst, contours2, RETR_LIST, CHAIN_APPROX_NONE);
		if (cols_count < contours1.size() + contours2.size()) cols_count = contours1.size() + contours2.size();
	}
	int rows_count = 0;
	for (int i = 1; i < bin.rows; i++) {
		Mat u_dst = bin(Rect(0, 0, bin.cols, i));
		vector<vector<Point>> contours1, contours2;
		findContours(u_dst, contours1, RETR_LIST, CHAIN_APPROX_NONE);
		Mat d_dst = bin(Rect(0, i, bin.cols, bin.rows - i));
		findContours(d_dst, contours2, RETR_LIST, CHAIN_APPROX_NONE);
		if (rows_count < contours1.size() + contours2.size()) rows_count = contours1.size() + contours2.size();
	}
	imshow("boundingbox", bin);
	return Point(cols_count, rows_count);
}
tuple<int, int, int, int> PixelCount(Mat img) {
	draw_img = img(Rect(0, 0, 500, 500));
	bin = ~draw_img;
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(5, 20, CV_8UC1));
	bin = bounding_img(~bin);

	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	int center_x = stats.at<int>(0, 2) / 2; // 중심 x좌표
	int center_y = stats.at<int>(0, 3) / 2; // 중심 y좌표

	int draw_pixel_count = 0, background_pixel_count = 0;
	int l_pixel_count = 0, r_pixel_count = 0, u_pixel_count = 0, d_pixel_count = 0;
	for (int i = 0; i < bin.rows; i++) {
		for (int j = 0; j < bin.cols; j++) {
			if (bin.at<uchar>(i, j) == 255) draw_pixel_count++;	//전체 픽셀 수

			if (i >= 0 && i < center_y && bin.at<uchar>(i, j) == 255) u_pixel_count++;	//중심을 기준으로 위쪽 객체의 픽셀수
			else if (i > center_y && i <= bin.rows && bin.at<uchar>(i, j) == 255) d_pixel_count++;	//중심을 기준으로 아래쪽 객체의 픽셀수

			if (j >= 0 && j < center_x && bin.at<uchar>(i, j) == 255) l_pixel_count++;	//중심을 기준으로 왼쪽 객체의 픽셀수
			else if (j > center_x && j <= bin.cols && bin.at<uchar>(i, j) == 255) r_pixel_count++;	//중심을 기준으로 오른쪽 객체의 픽셀수
		}
	}
	int u_count, d_count, l_count, r_count;
	//비율 계산
	u_count = (double)u_pixel_count / (u_pixel_count + d_pixel_count) * 100;
	d_count = (double)d_pixel_count / (u_pixel_count + d_pixel_count) * 100;
	l_count = (double)l_pixel_count / (l_pixel_count + r_pixel_count) * 100;
	r_count = (double)r_pixel_count / (l_pixel_count + r_pixel_count) * 100;
	imshow("boundingbox", bin);

	return make_tuple(u_count, d_count, l_count, r_count);
}

Mat morph(Mat img) {	//모폴로지 연산
	draw_img = img(Rect(0, 0, 500, 500));
	cvtColor(draw_img, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
	int* p = stats.ptr<int>(1);

	int rows = (p[0] + p[2]) / 8;	//10은 임의의 값
	int cols = (p[1] + p[3]) / 8;
	morphologyEx(bin, bin, MORPH_CLOSE, Mat(rows, cols, CV_8UC1));

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
	if (img.type() == CV_8UC3) {
		cvtColor(img, gray, COLOR_BGR2GRAY);
		threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
	}
	else bin = img;

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
		{"contour", "center", "split contour", "split pixel", "feature5"} };
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
