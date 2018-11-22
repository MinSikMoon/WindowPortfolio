#pragma once
#include "math.h"
class mZoom {
private:
	double zoomLevel;
	double zoomCounter;
	int onePixelCnt;
	int limitLevel = 1;

	void setOnePixelCnt() { //몇 픽셀을 움직이면 픽셀이 움직이는가?//2. zoomLevel에서는 몇 pixel을 움직여야 한칸인가? : 상대적 픽셀값 구하기
		onePixelCnt = floor(zoomLevel); //1.2배 이면 1칸, 2.0배이면 2칸을 움직여야 한 단위 움직이는 것이겠지. 
	}
public:
	//1. constructor
	mZoom() {
		zoomLevel = 1.0;
		onePixelCnt = 1;
		zoomCounter = 0.2;
	}
	//2. zoomLevel setterGetter
	void setZoomLevel(double newZoomLevel) {
		if (newZoomLevel < limitLevel) {
			zoomLevel = limitLevel;
			onePixelCnt = 1;
			return;
		}

		zoomLevel = newZoomLevel;
		setOnePixelCnt(); //픽셀 카운트 갱신
	}
	double getZoomLevel() {
		return zoomLevel;
	}
	int getOnePixelCnt() {
		return onePixelCnt;
	}
	//3. operator overloading
	void operator +=(double increase) {
		setZoomLevel(zoomLevel + increase);
	}
	void operator -=(double decrease) {
		setZoomLevel(zoomLevel - decrease);
	}
	//4. howManyPixelMove() : pixel을 넣으면 현재 몇 칸 움직여야 할지 정해준다. 
	int howManyPixelMove(int pixelCnt) {
		int moveTimes = pixelCnt / onePixelCnt; //몫만 구해진다. 
		return moveTimes*onePixelCnt;
	}

	//5. getOriginPos : 현재 줌 레벨에서의 길이가 주어졌을 때, 1단계가 증가하면 길이가 얼마나 길어지는가 조사. 
	int getIncrease(int from, int to) {
		//1. 1단위 사이에 단계가 몇 개 있는가?
		int stageNum = 1.0 / zoomCounter; //들어온 길이를 몇 단계로 나눌 것인가?
		int dist = abs(to - from); //들어온 길이

		double integerCleaner;
		double notInteger = modf(zoomLevel, &integerCleaner); //notInteger에 소수부만 남음
		double checker = 1 - notInteger;
		printf("stageNum: %d, dist: %d, checker: %lf, notInteger: %lf \n", stageNum, dist, checker, notInteger);
		if (zoomCounter + notInteger >= 1) { //정수부에 가까워 졌으면 
			return ceil(dist / stageNum);
		}
		else {
			return floor(dist / stageNum);
		}
	}

	//6. getZoomOutPos : zoomOut 할 때 길이를 어떻게 할 것인가?
	static int getZoomOutPos(int curPos, double zoomOutLevel) {
		return (int)floor((double)curPos * zoomOutLevel);
	}
	//7. setZoomCounter
	void setZoomCounter(double n) {
		zoomCounter = n;
	}
	double getZoomCounter() {
		return zoomCounter;
	}

};