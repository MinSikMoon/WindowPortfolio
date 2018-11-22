#pragma once
#include "mShape.h"
class mLine : public mShape {
public:
	int getName() {
		return Flag::LINE;
	}
	//1. 생성자
	mLine() {}
	mLine(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
	}

	//2. show
	void show(HDC hdc) {
		MoveToEx(hdc, getUpLeftX(), getUpLeftY(), NULL);
		LineTo(hdc, getDownRightX(), getDownRightY());
	}
	void showRelative(HDC hdc, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		MoveToEx(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, NULL);
		LineTo(hdc, getDownRightX() + offsetX, getDownRightY() + offsetY);
		//showText(hdc);
	}

	void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		MoveToEx(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, NULL);
		LineTo(hdc, getDownRightX() + offsetX, getDownRightY() + offsetY);
	}
	void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel, int wordWidth) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		MoveToEx(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, NULL);
		LineTo(hdc, getDownRightX() + offsetX, getDownRightY() + offsetY);
	}
	//4. mProc
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return;
	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		MoveToEx(hdc, ulX, ulY, NULL);
		LineTo(hdc, drX, drY);
	}
	void showProgress_relative(HDC hdc, int ulX, int ulY, int drX, int drY, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		MoveToEx(hdc, ulX + offsetX, ulY + offsetY, NULL);
		LineTo(hdc, drX+offsetX, drY+offsetY);
	}
	void showZoomProgress(HDC hdc, double ulX, double ulY, double drX, double drY, POINT originPoint, double zoomLevel) {
		int offsetX = originPoint.x*zoomLevel;
		int offsetY = originPoint.y * zoomLevel;
		printf("zoomoffset xy : %d, %d \n", offsetX, offsetY);
		//Ellipse(hdc, ulX + offsetX, ulY/zoomLevel + offsetY, drX/zoomLevel + offsetX, drY/zoomLevel + offsetY);
		MoveToEx(hdc, ulX + offsetX, ulY + offsetY, NULL);
		LineTo(hdc, drX + offsetX, drY + offsetY);

	}
	//5. show Dot
	void  showDot(HDC hdc) {
		//setpixel 말고 lineto로 만들어보자. 
		HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX(), getUpLeftY()); //좌상단
		mSetPixel(hdc, getDownRightX(), getDownRightY()); // 우하단
		SelectObject(hdc, oldPen);
	};
	void showDot_relative(HDC hdc, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;

		//setpixel 말고 lineto로 만들어보자. 
		//setpixel 말고 lineto로 만들어보자. 
		//HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN tempPen = getDotPen();
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX() + offsetX, getUpLeftY()+offsetY); //좌상단
		mSetPixel(hdc, getDownRightX() + offsetX, getDownRightY() + offsetY); // 우하단
		SelectObject(hdc, oldPen);

	}

	//6. isIn
	bool isIn(POINT mousePosition) {
		int checkDist = 7;
		POINT p1 = getUpLeft();
		POINT p2 = getDownRight();
		if (p1.x == p2.x) { // l 같은 선분
			if (p1.y <= p2.y) {
				if (mousePosition.x <= p1.x + checkDist &&
					mousePosition.x >= p1.x - checkDist &&
					mousePosition.y >= p1.y - checkDist &&
					mousePosition.y <= p2.y + checkDist) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if (mousePosition.x <= p1.x + checkDist &&
					mousePosition.x >= p1.x - checkDist &&
					mousePosition.y <= p1.y + checkDist &&
					mousePosition.y >= p2.y - checkDist) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else if (p1.y == p2.y) { // ㅡ 같은 선분
			if (p1.x <= p2.x) {
				if (mousePosition.y <= p1.y + checkDist &&
					mousePosition.y >= p1.y - checkDist &&
					mousePosition.x >= p1.x - checkDist &&
					mousePosition.x <= p2.x + checkDist) {
					return true;
				}
				else {
					return false;
				}
			}
			else {
				if (mousePosition.y <= p1.y + checkDist &&
					mousePosition.y >= p1.y - checkDist &&
					mousePosition.x <= p1.x + checkDist &&
					mousePosition.x >= p2.x - checkDist) {
					return true;
				}
				else {
					return false;
				}
			}
		}
		else { // 나머지 선분
			double m1 = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
			double k1 = -m1 * p1.x + p1.y;
			double m2 = -1.0 / m1;
			double k2 = mousePosition.y - m2 * mousePosition.x;
			double ax = (k2 - k1) / (m1 - m2);
			double ay = m1 * ax + k1;

			double minPx = (p1.x <= p2.x) ? p1.x : p2.x;
			double maxPx = (p1.x <= p2.x) ? p2.x : p1.x;
			double minPy = (p1.y <= p2.y) ? p1.y : p2.y;
			double maxPy = (p1.y <= p2.y) ? p2.y : p1.y;
			if (ax >= minPx && ax <= maxPx &&
				ay >= minPy && ay <= maxPy) {
				double dis = sqrt(((ax - mousePosition.x)*(ax - mousePosition.x)) + ((ay - mousePosition.y)*(ay - mousePosition.y)));

				if (dis <= checkDist) {
					puts("isIn : line \n");
					return true;
				}
				else return false;
			}
			else {
				double dis1 = sqrt(((p1.x - mousePosition.x)*(p1.x - mousePosition.x)) + ((p1.y - mousePosition.y)*(p1.y - mousePosition.y)));
				double dis2 = sqrt(((p2.x - mousePosition.x)*(p2.x - mousePosition.x)) + ((p2.y - mousePosition.y)*(p2.y - mousePosition.y)));
				double minDis = (dis1 <= dis2) ? dis1 : dis2;

				if (minDis <= checkDist) {
					puts("isiN : LINE \n");
					return true;
				}
				else return false;
			}
		}
	}


	//7. line resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);
	}
};