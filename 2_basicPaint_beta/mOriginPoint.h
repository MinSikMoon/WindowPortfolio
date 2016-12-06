#pragma once
#include <windef.h>
#include "mMouse.h"
class mOriginPoint {
private:
	POINT origin_point;
	int lineWidth; //십자가 그려질 때 길이 

public:
	//1. constructor
	mOriginPoint() {
		origin_point.x = 0;
		origin_point.y = 0;
		lineWidth = 40; //반토막 길이 
	}
	//2. move
	void move(int xDist, int yDist) {
		origin_point.x += xDist;
		origin_point.y += yDist;
	}
	//3. getters
	POINT getOriginPoint() {
		return origin_point;
	}
	//4. 0.0으로 돌아가기 위해 x를 얼마나 움직여야 하는가 
	int getToZeroXdist() {
		return -origin_point.x;
	}
	//5. 0.0으로 돌아가기 위해 y를 얼마나 움직여야 하는가 
	int getToZeroYdist() {
		return -origin_point.y;
	}
	//6. show() : 화면상에 원점의 좌표가 표시되게끔.
	void show(HDC hdc) {
		//1. 세로선 그어주기 
		MoveToEx(hdc, origin_point.x, origin_point.y - lineWidth, NULL);
		LineTo(hdc, origin_point.x, origin_point.y + lineWidth);
		//2. 가로선 그어주기 
		MoveToEx(hdc, origin_point.x - lineWidth, origin_point.y, NULL);
		LineTo(hdc, origin_point.x + lineWidth, origin_point.y);
		//3. 0.0표시
		TextOut(hdc, origin_point.x + 2, origin_point.y - 18, TEXT("0,0"), 3);
	}

	//7. setStartPoint
	void setStartPoint(mMouse& mouse, int xDist, int yDist) {
		move(xDist, yDist);
		mouse.setOriginPos(-xDist, -yDist);
	}

	void goToZero(mMouse& mouse, int xDist, int yDist) {
		move(-origin_point.x, -origin_point.y);
		mouse.setOriginPos(origin_point.x, origin_point.y);
		move(xDist, yDist);
		mouse.setOriginPos(-xDist, -yDist);

	}

	
};
