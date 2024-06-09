# number_recognition_2
 
# 추가 내용

### 특징
1. 외곽선의 갯수
2. 무게중심 x, y좌표 비
3. 객체 2분할 시 외곽선 합의 최대 개수
4. 객체를 가로, 세로로 잘랐을 때 각각의 픽셀 수의 비율


int getContourCount(Mat img) {	//외곽선 개수
	bin = morph(img);
	bin = bounding_img(bin);


	vector<vector<Point>> contours;
	findContours(bin, contours, RETR_LIST, CHAIN_APPROX_NONE);
	imshow("boundingbox", bin);
	return contours.size();
}
