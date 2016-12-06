#pragma once
#include "newTextEditor03.h"

DWORD blueColor = RGB(0, 0, 255);
DWORD yellowColor = RGB(255, 255, 224);
namespace Flag {
	enum Type {
		CIRCLE,
		RECTANGLE,
		LINE,
		MOVE,
		RESIZE,
		NOTHING
	};
}





class mShape {
private:
	POINT upLeft, downRight; //좌상단 우하단 좌표
	mTextEditor textEditor;

	int editorWidth, editorHeight, editorX, editorY; //에디터의 제한 너비// ** 현재 버젼에서 높이 추가 
	int diff = 2;
	HPEN dotPen; //점 찍을 때 쓸 펜
	
public:
	//0. 자신이 무슨 도형인지 가르쳐 주는 함수 : => 저장, 불러오기 때 필요 
	virtual int getName() = 0;
	//1. 생성자 
	mShape() {
		textEditor.make(2, 16);
		
	}
	mShape(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		textEditor.make(2, 16);
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;
		editorWidth = _downRightX - _upLeftX;
		editorHeight = _downRightY - _upLeftY;

		editorX = _upLeftX;
		editorY = _upLeftY;

		dotPen = CreatePen(PS_SOLID, 5, blueColor);

	}
	void make(int _upLeftX, int _upLeftY, int _downRightX, int _downRightY) {
		textEditor.make(2, 16);
		upLeft.x = _upLeftX;
		upLeft.y = _upLeftY;
		downRight.x = _downRightX;
		downRight.y = _downRightY;
		editorWidth = _downRightX - _upLeftX;
		editorHeight = _downRightY - _upLeftY;

		editorX = _upLeftX;
		editorY = _upLeftY;

		dotPen = CreatePen(PS_SOLID, 5, blueColor);
	
	}

	//2. 에디터 너비 조정 
	void setEditorWidth(int newEditorWidth) {
		editorWidth = newEditorWidth;
	}
	void setEditorHeight(int newEditorHeight) {
		editorHeight = newEditorHeight;
	}
	int getEditorWidth() {
		return editorWidth;
	}
	int getEditorHeight() {
		return editorHeight;
	}
	void setEditorX(int x) {
		editorX = x;
	}
	void setEditorY(int y) {
		editorY = y;
	}
	//3. set,get diff
	void setDiff(int n) {
		diff = n;
	}
	int getDiff() {
		return diff;
	}

	//get set x,y
	int getUpLeftX() {
		return upLeft.x;
	}
	int getUpLeftY() {
		return upLeft.y;
	}
	int getDownRightX() {
		return downRight.x;
	}
	int getDownRightY() {
		return downRight.y;
	}
	int getEditorX() {
		return editorX;
	}
	int getEditorY() {
		return editorY;
	}
	POINT getUpLeft() {
		return upLeft;
	}
	POINT getDownRight() {
		return downRight;
	}

	//4. mProc
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		textEditor.replaceKeyboard(textEditor.getTextAt(textEditor.getCaretNode()));
		textEditor.mProc(hwnd, Message, wParam, lParam);
		textEditor.replaceCurText();
	}

	//5. show
	virtual void show(HDC hdc) = 0; //왜 virtual 붙였는 지 알것
	virtual void showRelative(HDC hdc, POINT origin) = 0;
	virtual void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel) = 0;
	virtual void showZoomRelative(HDC hdc, POINT originPoint, double zoomLevel, int wordWidth) = 0;
	//6. showText
	void showText(HDC hdc) {

		textEditor.showAllText(hdc, editorWidth, editorX, editorY);

	}

	

	void showZoomText(HDC hdc, POINT originPoint, double zoomLevel) {
		textEditor.showAllText(hdc, editorWidth, (editorX + originPoint.x), (editorY+originPoint.y));
		//textEditor.showAllZoomText()
	}
	void showZoomText(HDC hdc, POINT originPoint, double zoomLevel, int wordWidth) {
		textEditor.showAllText(hdc, editorWidth, (editorX + originPoint.x), (editorY + originPoint.y), wordWidth);
		//textEditor.showAllZoomText()
	}



	//7. showProgress
	virtual void  showProgress(HDC hdc, int ulX, int ulY, int drX, int drY) = 0;
	virtual void showProgress_relative(HDC hdc, int ulX, int ulY, int drX, int drY, POINT originPoint) = 0;
	//8. showDot //선택같은거 할때 점찍어서 보여주는것
	virtual void  showDot(HDC hdc) = 0;
	virtual void showDot_relative(HDC hdc, POINT originPoint) = 0;

	//9. dot쉽게 찍기
	void mSetPixel(HDC hdc, int x, int y) {
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x, y);
	}

	//10. isIn : 도형안에 마우스가 들어왔는지 판별해주는 것
	virtual bool isIn(POINT mousePosition) { return false; }

	//11. move //도형을 움직여 주자.
	void move(int xDist, int yDist) {
		upLeft.x += xDist;
		upLeft.y += yDist;
		downRight.x += xDist;
		downRight.y += yDist;

		editorX += xDist;
		editorY += yDist;
	};

	//12. 두 점간의 거리 계산해주는 함수
	static double distanceChecker(POINT a, POINT b) {
		double xDist = abs(a.x - b.x);
		double yDist = abs(a.y - b.y);
		double temp = pow(xDist, 2) + pow(yDist, 2);
		return sqrt(temp);
	}
	//13. 
	int isClosed(POINT mousePoint) {
		POINT p1, p2, p3, p4; //좌상단부터 시계방향으로 // 좌상, 우상, 우하, 좌하
		double dist = 7.0;
		p1 = { upLeft.x, upLeft.y };
		p2 = { downRight.x, upLeft.y };
		p3 = { downRight.x, downRight.y };
		p4 = { upLeft.x, downRight.y };

		if (distanceChecker(mousePoint, p1) < dist) {
			return 1;
		}
		else if (distanceChecker(mousePoint, p2) < dist) {
			return 2;
		}
		else if (distanceChecker(mousePoint, p3) < dist) {
			return 3;
		}
		else if (distanceChecker(mousePoint, p4) < dist) {
			return 4;
		}
		else {
			return -1; //근접한게 없다. 
		}
	}

	//14. resize 도형별로 edit의 너비가 달라서 다시 해줘야함. 
	virtual void resize(POINT newUpLeft, POINT newDownRight) = 0;


	//15. 좌상단, 우하단 좌표 변환 
	void changeXy(int ulx, int uly, int drx, int dry) {
		upLeft.x = ulx;
		upLeft.y = uly;
		downRight.x = drx;
		downRight.y = dry;
	}

	//16. 텍스트 에디터의 글자 높이, 텍스트 높이 알아내기 
	size_m getTextHeight() {
		return textEditor.getTextHeight();
	}
	size_m getWordHeight() {
		return textEditor.getWordHeight();
	}

	//17. increase / decrease : ul과 rd에서 지정한 x,y dist만큼 증가, 감소시켜주기
	void increase(int xInc, int yInc) {
		downRight.x += xInc;
		downRight.y += yInc;
	}
	void decrease(int xDec, int yDec) {
		downRight.x -= xDec;
		downRight.y -= yDec;
	}
	//18. multiple 
	void multiply(double multi) {
		upLeft.x *= multi;
		upLeft.y *= multi;
		downRight.x *= multi;
		downRight.y *= multi;
	}

	//19. getDotPen : 점찍는 펜 가져다 쓰기 
	HPEN getDotPen() {
		return dotPen;
	}

	//20. 저장용 getText
	TCHAR* getTextAt(int idx) {
		return textEditor.getTextAt(idx);
	}

	////21. 불러오기용 setText
	//void addText(TCHAR* _inStr) {
	//	textEditor.addText( _inStr);
	//}

	//21. 이미 SHAPE가 만들어질 때부터 노드가 1개 들어가 있기에 REPLACE해줘야함
	void replaceTextAt(int idx, TCHAR* _newStr) {
		textEditor.replaceTextAt(idx, _newStr);
	}
};