# number_recognition_2
 
# 추가 내용

### 특징
contour : 외곽선의 갯수

center : 무게중심의 위치 비율(%)

stroke : 획 수

### 함수
Mat morph(Mat img, Point startpt1, Point startpt2, Point endpt, int count) : img영상의 객체에 모폴로지 연산을 수행하는 함수

Mat bounding_img(Mat img, int count) : img영상의 객체부분을 추출하기 위한 함수

int getContourCount(Mat img, int count) : 외곽선의 갯수를 반환해주는 함수

Point getCenterPt(Mat img, int count) : 바운딩박스 크기에 대한 무게중심 x, y 좌표의 비율을 반환해주는 함수

int getStrokeCount(Mat img, int count) : 숫자를 몇획에 완성했는지 획수를 반환해주는 함수  ex) 4 - 2획에 걸쳐 쓰기 때문에 2반환
