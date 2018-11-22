#pragma once
#include "mShape.h"
#include "mMouse.h"
#include <vector>
class mShapeContainer {
private:
	vector<mShape*> shapeVector;
	int shapeNum;
	int focusedIdx;

public:
	//1. ������
	mShapeContainer() : shapeNum(0) {}
	~mShapeContainer() {
		for (vector<mShape*>::iterator itr = shapeVector.begin(); itr != shapeVector.end(); itr++) {
			delete *itr;
		}
	}

	//2. add
	void add(mShape* _newShape) {
		shapeVector.push_back(_newShape);
		shapeNum++;
	}

	//3. showAll
	void showAll(HDC hdc) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				mShape* temp = shapeVector[i];
				temp->show(hdc);
			}
		}
	}

	void showAll_relative(HDC hdc, POINT originPoint) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				mShape* temp = shapeVector[i];
				temp->showRelative(hdc, originPoint);
				
			}
		}
	}

	void showAllExcept(HDC hdc, int exIdx) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				if (i == exIdx)
					continue;

				mShape* temp = shapeVector[i];
				temp->show(hdc);
			}
		}
	}
	void showAllExcept_relative(HDC hdc, int exIdx, POINT ORIGIN_POINT) {
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				if (i == exIdx)
					continue;

				mShape* temp = shapeVector[i];
				temp->showRelative(hdc, ORIGIN_POINT);
			}
		}
	}
	void showAllExcept_Zoom(HDC hdc, int exIdx, POINT ORIGIN_POINT, double zoomLevel) {//----------------------
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				if (i == exIdx)
					continue;

				mShape* temp = shapeVector[i];
				temp->showZoomRelative(hdc, ORIGIN_POINT, zoomLevel);
			}
		}
	}

	void showAllExcept_Zoom(HDC hdc, int exIdx, POINT ORIGIN_POINT, double zoomLevel, int wordWidth) {//----------------------
		if (isEmpty()) {
			return;
		}
		else {
			for (int i = 0; i < shapeNum; i++) {
				if (i == exIdx)
					continue;

				mShape* temp = shapeVector[i];
				temp->showZoomRelative(hdc, ORIGIN_POINT, zoomLevel, wordWidth);
				temp->resize(temp->getUpLeft(), temp->getDownRight());
			}
		}
	}

	

	//4. procAt
	void procAt(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam, int _idx) {
		if (_idx == -1) {
			return;
		}
		mShape* temp = shapeVector[_idx];
		temp->mProc(hwnd, Message, wParam, lParam);

	}

	//5. getShapeNum
	int getShapeNum() {
		return shapeNum;
	}

	//6. isEmpty(){
	bool isEmpty() {
		return shapeNum == 0 ? true : false;
	}


	//7. WhoisIn���� �ִ� �������� ���� �ֱٿ� ���� ������ ������ �Ǵ� ��Ȳ ���� 
	int whoIsIn(POINT mousePoint, int curFocusedIdx) {
		
		if (shapeNum == 0) {
			return -1; //�ƹ��͵� ���ٴ� ��
		}

		int tempIdx = -1;
		int inCnt = 0; //�� ���� ������ ��ġ���� üũ 

		for (int i = 0; i < shapeNum; i++) {
			mShape* temp = shapeVector[i];

			if (temp->isIn(mousePoint)) {
				//printf("%d ������ ���Դ�. \n", i);
				inCnt++;
				tempIdx = i; //�̷��� �Ǹ� ���� �ֱٿ� ������ ������ �ȴ�. 
			}


		}
		if (inCnt > 1) {
			if (curFocusedIdx < tempIdx) {
				return curFocusedIdx;
			}
			else {
				return tempIdx; //�ƹ��͵� ���õ��� �ʾҴ�.
			}
		}
		else {
			return tempIdx; //�ƹ��͵� ���õ��� �ʾҴ�.
		}


	}

	//8. showDotAt
	void showDotAt(HDC hdc, int idx) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];
		temp->showDot(hdc);
	}
	void showDotAt_relative(HDC hdc, int idx, POINT originPoint) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];
		temp->showDot_relative(hdc,originPoint);
	}

	//9. showAt
	void showAt(HDC hdc, int idx) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];


		temp->show(hdc);

	}
	void showAt_relative(HDC hdc, int idx, POINT ORIGIN_POINT) {
		if (idx < 0)
			return;

		mShape* temp = shapeVector[idx];


		temp->showRelative(hdc, ORIGIN_POINT);
	}

	void showAt_zoom(HDC hdc, int exIdx, POINT ORIGIN_POINT, double zoomLevel) {
		if (exIdx < 0)
			return;

		mShape* temp = shapeVector[exIdx];


		temp->showZoomRelative(hdc, ORIGIN_POINT, zoomLevel);

		//�����
		for (int i = 0; i < shapeNum; i++) {
			mShape* temp2 = shapeVector[i];
			printf("[%d]��° ���� �»�� %d,%d ���ϴ� %d,%d \n",i, temp2->getUpLeftX(), temp2->getUpLeftY(), temp2->getDownRightX(), temp2->getDownRightY());
		}
	}

	void showAt_zoom(HDC hdc, int exIdx, POINT ORIGIN_POINT, double zoomLevel, int wordWidth) {
		if (exIdx < 0)
			return;

		mShape* temp = shapeVector[exIdx];


		temp->showZoomRelative(hdc, ORIGIN_POINT, zoomLevel, wordWidth);

		//�����
		for (int i = 0; i < shapeNum; i++) {
			mShape* temp2 = shapeVector[i];
			temp->resize(temp->getUpLeft(), temp->getDownRight());
			//printf("[%d]��° ���� �»�� %d,%d ���ϴ� %d,%d \n", i, temp2->getUpLeftX(), temp2->getUpLeftY(), temp2->getDownRightX(), temp2->getDownRightY());
		}
	}
	

	//10. moveAt
	void moveAt(int idx, int xDist, int yDist) {
		mShape* temp = shapeVector[idx];
		temp->move(xDist, yDist);
	}

	//11. showProgressAt
	void showProgressAt(HDC hdc, int idx, int luX, int luY, int rdX, int rdY) {
		mShape* temp = shapeVector[idx];
		temp->showProgress(hdc, luX, luY, rdX, rdY);
	}

	//12. getUpLeft DownRight PosAt
	POINT getUpLeftPosAt(int idx) {
		mShape* temp = shapeVector[idx];
		POINT tempPoint{ temp->getUpLeftX(), temp->getUpLeftY() };
		return tempPoint;
	}
	POINT getDownRightPosAt(int idx) {
		mShape* temp = shapeVector[idx];
		POINT tempPoint{ temp->getDownRightX(), temp->getDownRightY() };
		return tempPoint;
	}


	//13. isClosedAt
	int isClosedAt(int idx, POINT point) {
		mShape* temp = shapeVector[idx];
		return temp->isClosed(point);
	}

	//14. resizeAt
	void resizeAt(int idx, POINT ul, POINT rd) {
		mShape* temp = shapeVector[idx];
		temp->resize(ul, rd);
	}

	//15. getUpleftAt, DownRight
	POINT getUpLeftAt(int idx) {
		mShape* temp = shapeVector[idx];
		return temp->getUpLeft();
	}
	POINT getDownRightAt(int idx) {
		mShape* temp = shapeVector[idx];
		return temp->getDownRight();
	}

	//16. increase/ decrease
	void increaseAt(int idx, int xInc, int yInc) {
		mShape* temp = shapeVector[idx];
		temp->increase(xInc, yInc);
	}
	void decreaseAt(int idx, int xDec, int yDec) {
		mShape* temp = shapeVector[idx];
		temp->decrease(xDec, yDec);
	}

	//17. multiple
	void multipleAt(int idx, double multiple) {
		mShape* temp = shapeVector[idx];
		temp->multiply(multiple);
	}
	
	//proc����


	//18. shapeMaking
	int buMakeShapeAction(int orderFlag, int focusedIdx, mMouse& g_mouse) {
		int g_focusedIdx = focusedIdx;
		switch (orderFlag) {
		case Flag::CIRCLE: {
			add(new mCircle(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			//printf("%d,%d ���� %d, %d�� �׾���. \n", g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		case Flag::RECTANGLE: {
			add(new mRectangle(g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y));
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		case Flag::LINE: { //������ �Ųٷ� ������ ���� 
			add(new mLine(g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY(), g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY()));
			//printf("%d,%d ���� %d, %d�� �׾���. \n", g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		}

		return g_focusedIdx;
	}

	int buMakeZoomShapeAction(int orderFlag, int focusedIdx, mMouse& g_mouse, double zoomLevel, POINT originPoint) {
		int g_focusedIdx = focusedIdx;
		switch (orderFlag) {
		case Flag::CIRCLE: {
			add(new mCircle(g_mouse.getRelativeUpleft().x /zoomLevel, g_mouse.getRelativeUpleft().y / zoomLevel, g_mouse.getRelativeDownRight().x / zoomLevel, g_mouse.getRelativeDownRight().y / zoomLevel));
			printf("%d,%d ���� %d, %d�� �׾���. \n", g_mouse.getRelativeUpleft().x*zoomLevel, g_mouse.getRelativeUpleft().y*zoomLevel, g_mouse.getRelativeDownRight().x*zoomLevel, g_mouse.getRelativeDownRight().y / zoomLevel);
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		case Flag::RECTANGLE: {
			add(new mRectangle(g_mouse.getRelativeUpleft().x / zoomLevel, g_mouse.getRelativeUpleft().y / zoomLevel, g_mouse.getRelativeDownRight().x / zoomLevel, g_mouse.getRelativeDownRight().y / zoomLevel));
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		case Flag::LINE: { //������ �Ųٷ� ������ ���� 
			add(new mLine(g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY(), g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY()));
			printf("%d,%d ���� %d, %d�� �׾���. \n", g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			g_focusedIdx = getShapeNum() - 1;
			break;
		}
		}

		return g_focusedIdx;
	}

	//19. showProgress
	void paintShowProgressAction(HDC hdc, int g_orderFlag, mMouse g_mouse, POINT ORIGIN_POINT) {
		switch (g_orderFlag) {

		case Flag::CIRCLE: {
			mCircle().showProgress_relative(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y, ORIGIN_POINT);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showProgress_relative(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y, ORIGIN_POINT);
			break;
		}
		case Flag::LINE: {
			mLine().showProgress_relative(hdc, g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY(), g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY(), ORIGIN_POINT);
			break;
		}
		}
	}

	void paintShowProgressAction(HDC hdc, int g_orderFlag, mMouse g_mouse) {
		switch (g_orderFlag) {

		case Flag::CIRCLE: {
			mCircle().showProgress(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showProgress(hdc, g_mouse.getRelativeUpleft().x, g_mouse.getRelativeUpleft().y, g_mouse.getRelativeDownRight().x, g_mouse.getRelativeDownRight().y);
			break;
		}
		case Flag::LINE: {
			mLine().showProgress(hdc, g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY(), g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY());
			break;
		}
		}
	}
	void paintShowZoomProgressAction(HDC hdc, int g_orderFlag, mMouse g_mouse, POINT ORIGIN_POINT, double zoomLevel) {
		switch (g_orderFlag) {

		case Flag::CIRCLE: {
			mCircle().showZoomProgress(hdc, g_mouse.getRelativeUpleft().x*zoomLevel, g_mouse.getRelativeUpleft().y*zoomLevel, g_mouse.getRelativeDownRight().x*zoomLevel, g_mouse.getRelativeDownRight().y*zoomLevel, ORIGIN_POINT, zoomLevel);
			//printf("���� zoomProgress: �»�� relative: %d�� zoomlevel %lf�� ���ؼ� %lf�� �����µ�, ���⿡ ������ offset: %d�� ����ؼ� ǥ�� \n", g_mouse.getRelativeUpleft().x, zoomLevel, (g_mouse.getRelativeUpleft().x*zoomLevel), ORIGIN_POINT.x);
			break;
		}
		case Flag::RECTANGLE: {
			mRectangle().showZoomProgress(hdc, g_mouse.getRelativeUpleft().x*zoomLevel, g_mouse.getRelativeUpleft().y*zoomLevel, g_mouse.getRelativeDownRight().x*zoomLevel, g_mouse.getRelativeDownRight().y*zoomLevel, ORIGIN_POINT, zoomLevel);
			break;
		}
		case Flag::LINE: {
			mLine().showZoomProgress(hdc, g_mouse.getRelativeOldX()*zoomLevel, g_mouse.getRelativeOldY()*zoomLevel, g_mouse.getRelativeNewX()*zoomLevel, g_mouse.getRelativeNewY()*zoomLevel, ORIGIN_POINT,zoomLevel);
			break;
		}
		}
	}

	//20. moveResizeAction
	void moveResizeAction(int g_focusedIdx, int g_resizePoint, mMouse& g_mouse) {
		POINT tempUl, tempRd;
		if (g_resizePoint == 1) { //�»�ܸ� �����ش�.  
			tempUl = { g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY() };
			tempRd = g_mouse.getTempPos2();
		}
		else if (g_resizePoint == 2) {
			tempUl = { g_mouse.getTempPos1().x, g_mouse.getRelativeNewY() };
			tempRd = { g_mouse.getRelativeNewX(), g_mouse.getTempPos2().y };
		}
		else if (g_resizePoint == 3) { //���ϴܸ� ����

			tempUl = g_mouse.getTempPos1();
			tempRd = { g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY() };
		}
		else {
			tempUl = { g_mouse.getRelativeNewX(), g_mouse.getTempPos1().y };
			tempRd = { g_mouse.getTempPos2().x,  g_mouse.getRelativeNewY() };
		}
		resizeAt(g_focusedIdx, tempUl, tempRd); //������ ���� 
	}


	//21. �����׽�Ʈ�� �ӽ� getText
	TCHAR* getText(int idx) {
		mShape* temp = shapeVector[idx];
		return temp->getTextAt(0);
	}

	//22. getNameAt() //���� ���õ� ������ ���� �����ΰ�?
	int getNameAt(int idx) {
		mShape* temp = shapeVector[idx];
		return temp->getName();
	}

	//23. replaceTextAt() : �ҷ����� �׽�Ʈ�� 
	void replaceTextAt(int idx, TCHAR* _newStr) { //�Ѱ��ٶ� �������� ������ ��� �Ѱ������.
		TCHAR* cpyStr = cloneStr(_newStr);
		mShape* temp = shapeVector[idx];
		temp->replaceTextAt(0, cpyStr);
	}

	//24. deleteAll()
	void deleteAll() {
		shapeVector.clear();
		shapeNum = 0;
	}
};