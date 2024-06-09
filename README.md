# number_recognition_2

https://www.youtube.com/watch?v=KV629kuFwbU
 
# 추가 내용

### 특징
1. 외곽선의 갯수

https://github.com/lsy0727/number_recognition_2/blob/c4ce281ecc4290fbed49fb7dc571d2ae7d77f99d/number_recognition.cpp#L137-L145

2. 무게중심 x, y좌표 비

검출된 객체의 무게중심의 x, y좌표를 구하고 바운딩 박스 크기에 대한 가로, 세로의 비율을 구함

https://github.com/lsy0727/number_recognition_2/blob/fe9c795239ed807e8f9f3d6c1e0906db65621c92/number_recognition.cpp#L146-L163

3. 객체 2분할 시 외곽선 합의 최대 개수

객체를 가로, 세로로 잘랐을 때 각각 외곽선의 개수가 최대가 되는 수를 반환함

https://github.com/lsy0727/number_recognition_2/blob/dbcf1b982b3b7585a35ed0007310924c2499bcef/number_recognition.cpp#L164-L190

4. 객체를 가로, 세로로 잘랐을 때 각각의 픽셀 수의 비율

객체의 중심을 기준으로 가로, 세로로 각각 잘랐을 때 두 객체의 픽셀 수의 비율을 구함

https://github.com/lsy0727/number_recognition_2/blob/9f6ab633e09e81aed46a240c4c44f9faf408d42e/number_recognition.cpp#L191-L224
