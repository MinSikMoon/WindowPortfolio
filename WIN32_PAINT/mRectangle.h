#pragma once
#include "mShape.h"

class mRectangle : public mShape {
public:
	int getName() {
		return Flag::RECTANGLE;
	}
	//1. 생성자
	mRectangle() {}

	mRectangle(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) : mShape(_upLeftX, _upLeftY, _downRightX, _downRightY) {
		setEditorWidth(_downRightX - _upLeftX - getDiff() * 2);
		setEditorHeight(_downRightY - _upLeftY - getDiff() * 2);
		setEditorX(_upLeftX + getDiff());
		setEditorY(_upLeftY + getDiff());
	}

	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		mShape::make(_upLeftX, _upLeftY, _downRightX, _downRightY);
		setEditorWidth(_downRightX - _upLeftX - getDiff() * 2);
		setEditorHeight(_downRightY - _upLeftY - getDiff() * 2);
		setEditorX(_upLeftX + getDiff());
		setEditorY(_upLeftY + getDiff());
	}


	//3. show
	void show(HDC hdc) {
		resize(getUpLeft(), getDownRight());
		Rectangle(hdc, getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
		showText(hdc);
		//printf("%d %d %d %d \n", getUpLeftX(), getUpLeftY(), getDownRightX(), getDownRightY());
	}
	void showRelative(HDC hdc, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		Rectangle(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, getDownRightX() + offsetX, getDownRightY() + offsetY);
		//showText(hdc);
	}
	void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		Rectangle(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, getDownRightX() + offsetX, getDownRightY() + offsetY);
		showZoomText(hdc, originPoint, zoomLevel);
	}
	void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel, int wordWidth) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		Rectangle(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY, getDownRightX() + offsetX, getDownRightY() + offsetY);
		showZoomText(hdc, originPoint, zoomLevel, wordWidth);
	}
	//4. showProgress
	void showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) {
		Rectangle(hdc, ulX, ulY, drX, drY);
	}
	void showZoomProgress(HDC hdc, double ulX, double ulY, double drX, double drY, POINT originPoint, double zoomLevel) {
		int offsetX = originPoint.x*zoomLevel;
		int offsetY = originPoint.y * zoomLevel;
		printf("zoomoffset xy : %d, %d \n", offsetX, offsetY);
		Rectangle(hdc, ulX + offsetX, ulY + offsetY, drX + offsetX, drY + offsetY);

	}
	void showProgress_relative(HDC hdc, int ulX, int ulY, int drX, int drY, POINT originPoint) {
		int offsetX = originPoint.x;
		int offsetY = originPoint.y;
		
		Rectangle(hdc, ulX + offsetX, ulY + offsetY, drX + offsetX, drY + offsetY);
	}
	//5. showDot
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
		mSetPixel(hdc, getUpLeftX() + offsetX, getUpLeftY() + offsetY); //좌상단
		mSetPixel(hdc, getUpLeftX() + offsetX, getDownRightY() + offsetY); //좌하단
		mSetPixel(hdc, getDownRightX() + offsetX, getUpLeftY() + offsetY); //우상단
		mSetPixel(hdc, getDownRightX() + offsetX, getDownRightY() + offsetY); // 우하단
		SelectObject(hdc, oldPen);

	}
	//6. isIn : 도형안에 마우스가 들어왔는지 판별해주는 것
	bool isIn(POINT mousePosition) {
		int tempUlx = getUpLeftX() < getDownRightX() ? getUpLeftX() : getDownRightX();
		int tempDrx = getUpLeftX() < getDownRightX() ? getDownRightX() : getUpLeftX();
		int tempUly = getUpLeftY() < getDownRightY() ? getUpLeftY() : getDownRightY();
		int tempDry = getUpLeftY() < getDownRightY() ? getDownRightY() : getUpLeftY();


		if (tempUlx <= mousePosition.x && mousePosition.x <= tempDrx
			&& tempUly <= mousePosition.y&&mousePosition.y <= tempDry) {
			return true;
		}
		else {
			return false;
		}

	
	}

	//7. rectangle resize
	//resize
	void resize(POINT newUpLeft, POINT newDownRight) {
		changeXy(newUpLeft.x, newUpLeft.y, newDownRight.x, newDownRight.y);
		int tempUlx = getUpLeftX() < getDownRightX() ? getUpLeftX() : getDownRightX();
		int tempDrx = getUpLeftX() < getDownRightX() ? getDownRightX() : getUpLeftX();
		int tempUly = getUpLeftY() < getDownRightY() ? getUpLeftY() : getDownRightY();
		int tempDry = getUpLeftY() < getDownRightY() ? getDownRightY() : getUpLeftY();
		changeXy(tempUlx, tempUly, tempDrx, tempDry);

		setEditorWidth(newDownRight.x - newUpLeft.x - getDiff() * 2);
		setEditorHeight(newDownRight.y - newUpLeft.y - getDiff() * 2);
		setEditorX(newUpLeft.x + getDiff());
		setEditorY(newUpLeft.y + getDiff());

		if (getTextHeight() > getEditorHeight()) { //글자 높이가 더 크면 조정해줘야지. 
			int midTextHeight = getTextHeight() / 2;
			int midYpos = (newDownRight.y - ((newDownRight.y - newUpLeft.y) / 2));
			int diffLineNum = ((getTextHeight() - getEditorHeight()) / getWordHeight() + 1); //몇 줄이나 튀어나갔나? 
			setEditorY(midYpos - midTextHeight + getDiff());
			setEditorHeight(getEditorHeight() + diffLineNum*getWordHeight() * 2);
		}

	}



};