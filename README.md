# number_recognition_2
 
# 추가 내용

### 특징
외곽선의 갯수

무게중심의 위치 비율(%)

가로, 세로 비

### 함수
Mat morph(Mat img) : img영상의 객체에 모폴로지 연산을 수행하는 함수

Mat bounding_img(Mat img) : img영상의 객체부분을 추출하기 위한 함수

int getContourCount(Mat img) : 외곽선의 갯수를 반환해주는 함수

Point getCenterPt(Mat img) : 바운딩박스 크기에 대한 무게중심 x, y 좌표의 비율을 반환해주는 함수
