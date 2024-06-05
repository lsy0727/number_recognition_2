# number_recognition_2
 
# 추가 내용
### 함수 : bounding_img, morph
Mat bounding_img(Mat img) : img영상의 객체부분을 추출하기 위한 함수

Mat morph(Mat img) : img영상의 객체에 모폴로지 연산을 수행하는 함수

### 기능 : contour, center
contour : 외곽선의 갯수를 구하고 예상 결과를 출력한다.

center : 무게중심의 위치를 비율(%)로 구한다.

# 함수
Mat morph(Mat img) {	//모폴로지 연산
	Mat bin;
	draw_img = img(Rect(0, 0, 500, 500));
	cvtColor(draw_img, gray, COLOR_BGR2GRAY);
	threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);


	Mat labels, stats, centroids;
	int cnt = connectedComponentsWithStats(bin, labels, stats, centroids);
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
# contour


# center
