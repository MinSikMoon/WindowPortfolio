#pragma once
#include "math.h"
class mZoom {
private:
	double zoomLevel;
	double zoomCounter;
	int onePixelCnt;
	int limitLevel = 1;

	void setOnePixelCnt() { //�� �ȼ��� �����̸� �ȼ��� �����̴°�?//2. zoomLevel������ �� pixel�� �������� ��ĭ�ΰ�? : ����� �ȼ��� ���ϱ�
		onePixelCnt = floor(zoomLevel); //1.2�� �̸� 1ĭ, 2.0���̸� 2ĭ�� �������� �� ���� �����̴� ���̰���. 
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
		setOnePixelCnt(); //�ȼ� ī��Ʈ ����
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
	//4. howManyPixelMove() : pixel�� ������ ���� �� ĭ �������� ���� �����ش�. 
	int howManyPixelMove(int pixelCnt) {
		int moveTimes = pixelCnt / onePixelCnt; //�� ��������. 
		return moveTimes*onePixelCnt;
	}

	//5. getOriginPos : ���� �� ���������� ���̰� �־����� ��, 1�ܰ谡 �����ϸ� ���̰� �󸶳� ������°� ����. 
	int getIncrease(int from, int to) {
		//1. 1���� ���̿� �ܰ谡 �� �� �ִ°�?
		int stageNum = 1.0 / zoomCounter; //���� ���̸� �� �ܰ�� ���� ���ΰ�?
		int dist = abs(to - from); //���� ����

		double integerCleaner;
		double notInteger = modf(zoomLevel, &integerCleaner); //notInteger�� �Ҽ��θ� ����
		double checker = 1 - notInteger;
		printf("stageNum: %d, dist: %d, checker: %lf, notInteger: %lf \n", stageNum, dist, checker, notInteger);
		if (zoomCounter + notInteger >= 1) { //�����ο� ����� ������ 
			return ceil(dist / stageNum);
		}
		else {
			return floor(dist / stageNum);
		}
	}

	//6. getZoomOutPos : zoomOut �� �� ���̸� ��� �� ���ΰ�?
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