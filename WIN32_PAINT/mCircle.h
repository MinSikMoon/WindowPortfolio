#pragma once
#include "mShape.h"
class mCircle : public mShape {
public:
	int getName() {
		return Flag::CIRCLE;
	}
	//1. 생성자
	mCircle() {}
	mCircle(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {
		setEditorWidth((_downRightX - _upLeftX) / 10 * 8 - getDiff() * 2);
		setEditorHeight((_downRightY - _upLeftY) / 2 + getDiff());
		setEditorX(_upLeftX + (getEditorWidth() / 10) + getDiff() + 8);
		setEditorY(_upLeftY + ((_downRightY - _upLeftY) / 4) + getDiff());
	}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
		setEditorWidth((_downRightX - _upLeftX) / 10 * 8 - getDiff() * 2);
		setEditorHeight((_downRightY - _upLeftY) / 2 + getDiff());
		setEditorX(_upLeftX + (getEditorWidth() / 10) + getDiff() + 8);
		setEditorY(_upLeftY + ((_downRightY - _upLeftY) / 4) + getDiff());
	}

	//3. show
	void show(HDC hdc) {
		resize(getUpLeft(), getDownRight());
		setEdit(getUpLeft(), getDownRight());
		Ellipse(hdc, getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		showText(hdc);
	}
	void showRelative(HDC hdc, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		Ellipse(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, getDownRightX() + offsetX, getDownRightY()+offsetY);
		printf("현재 circle의 좌상단 x,y = %d, %d \n", getUpLeftX(), getUpLeftY());
		printf("현재 circle의 editor x,y = %d, %d \n", getEditorX(), getEditorY());
		//showZoomText(hdc, originPoint, 2.0);
		showText(hdc);
		
	}

	void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		Ellipse(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, getDownRightX() + offsetX, getDownRightY() + offsetY);
		//printf("현재 circle의 좌상단 x,y = %d, %d \n", getUpLeftX(), getUpLeftY());
		//printf("현재 circle의 editor x,y = %d, %d \n", getEditorX(), getEditorY());
		showZoomText(hdc, originPoint, zoomLevel);
	}
	void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel, int wordWidth) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		Ellipse(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, getDownRightX() + offsetX, getDownRightY() + offsetY);
		//printf("현재 circle의 좌상단 x,y = %d, %d \n", getUpLeftX(), getUpLeftY());
		//printf("현재 circle의 editor x,y = %d, %d \n", getEditorX(), getEditorY());
		showZoomText(hdc, originPoint, zoomLevel, wordWidth);
	}

	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		Ellipse(hdc, ulX, ulY, drX, drY);
	}
	void showProgress_relative(HDC hdc, int ulX, int ulY, int drX, int drY, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		printf("offset xy : %d, %d \n", offsetX, offsetY);
		Ellipse(hdc, ulX + offsetX, ulY + offsetY, drX + offsetX, drY + offsetY);
	}
	void showZoomProgress(HDC hdc, double ulX, double ulY, double drX, double drY, POINT originPoint, double zoomLevel) {
		int offsetX = originPoint.x*zoomLevel ;
		int offsetY = originPoint.y * zoomLevel;
		//printf("zoomoffset xy : %d, %d \n", offsetX, offsetY);
		//Ellipse(hdc, ulX + offsetX, ulY/zoomLevel + offsetY, drX/zoomLevel + offsetX, drY/zoomLevel + offsetY);
		Ellipse(hdc, ulX + offsetX, ulY + offsetY, drX + offsetX, drY + offsetY);

	}


	//5. dot 보여주기
	void  showDot(HDC hdc) {
		//setpixel 말고 lineto로 만들어보자. 
		HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX(), getUpLeftY()); //좌상단
		mSetPixel(hdc, getUpLeftX(), getDownRightY()); //좌하단
		mSetPixel(hdc, getDownRightX(), getUpLeftY()); //우상단
		mSetPixel(hdc, getDownRightX(), getDownRightY()); // 우하단
		SelectObject(hdc, oldPen);
	};

	void showDot_relative(HDC hdc, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;

		//setpixel 말고 lineto로 만들어보자. 
		//HPEN tempPen = CreatePen(PS_SOLID, 5, blueColor); //노란색
		HPEN tempPen = getDotPen();
		HPEN oldPen = (HPEN)SelectObject(hdc, tempPen);
		mSetPixel(hdc, getUpLeftX()+offsetX, getUpLeftY()+offsetY); //좌상단
		mSetPixel(hdc, getUpLeftX() + offsetX, getDownRightY() + offsetY); //좌하단
		mSetPixel(hdc, getDownRightX() + offsetX, getUpLeftY() + offsetY); //우상단
		mSetPixel(hdc, getDownRightX() + offsetX, getDownRightY() + offsetY); // 우하단
		SelectObject(hdc, oldPen);

	}
	//6. 들어왔는지 안들어왔는지 판별
	bool isIn(POINT mousePosition) {
		POINT p1 = { getUpLeftX(), getUpLeftY() };
		POINT p2 = { getDownRightX(), getDownRightY() };

		double a = (p2.x - p1.x) / 2.0;
		double b = (p2.y - p1.y) / 2;
		double alpha = p1.x + a;
		double beta = p1.y + b;
		double lValue = (mousePosition.x - alpha)*(mousePosition.x - alpha) / (a*a)
			+
			(mousePosition.y - beta)*(mousePosition.y - beta) / (b*b);

		if (lValue <= 1) {
			return true;
		}
		else {
			return false;
		}
	}

	//7. circle resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);
		int tempUlx = getUpLeftX() < getDownRightX() ? getUpLeftX() : getDownRightX();
		int tempDrx = getUpLeftX() < getDownRightX() ? getDownRightX() : getUpLeftX();
		int tempUly = getUpLeftY() < getDownRightY() ? getUpLeftY() : getDownRightY();
		int tempDry = getUpLeftY() < getDownRightY() ? getDownRightY() : getUpLeftY();
		changeXy(tempUlx, tempUly, tempDrx, tempDry);
		setEdit(newUpLeft, newDownRight);
		printf("resize!!!!!!!!!!!!!!!!!!!!111\n");

	}

	void setEdit(POINT newUpLeft, POINT newDownRight) {
		setEditorWidth((newDownRight.x - newUpLeft.x) / 10 * 8 - getDiff() * 2);
		setEditorHeight((newDownRight.y - newUpLeft.y) / 2 + getDiff());
		setEditorX(newUpLeft.x + (getEditorWidth() / 10) + getDiff() + 8);
		setEditorY(newUpLeft.y + ((newDownRight.y - newUpLeft.y) / 4) + getDiff());

		if (getTextHeight() > getEditorHeight()) {
			int midTextHeight = getTextHeight() / 2;
			int midYpos = (newDownRight.y - ((newDownRight.y - newUpLeft.y) / 2));
			int diffLineNum = ((getTextHeight() - getEditorHeight()) / getWordHeight() + 1); //몇 줄이나 튀어나갔나? 
			setEditorY(midYpos - midTextHeight + getDiff());
			setEditorHeight(getEditorHeight() + diffLineNum*getWordHeight() * 2);
		}
	}
};