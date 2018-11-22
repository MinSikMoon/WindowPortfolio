#pragma once
#include <Windows.h>
class mMouse {
private:
	POINT oldPos;
	POINT newPos;
	bool grapped;
	bool pulled;
	POINT ORIGIN_POS;
	POINT tempPos1,tempPos2; //임시로 쓸 수 있는 편한 저장소 

public:
	mMouse() {
		oldPos.x = 0;
		oldPos.y = 0;
		newPos.x = 0;
		newPos.y = 0;
		grapped = false;
		pulled = false;
		ORIGIN_POS = { 0,0 };
	}
	//. 
	void setTemPos1(POINT temp) {
		tempPos1 = temp;
	}
	void setZoomTempPos1(POINT temp, double zoomLevel) {
		tempPos1.x = temp.x*zoomLevel;
		tempPos1.y = temp.y*zoomLevel;
	}
	void setZoomTempPos2(POINT temp, double zoomLevel) {
		tempPos2.x = temp.x*zoomLevel;
		tempPos2.y = temp.y*zoomLevel;
	}
	POINT getTempPos1() {
		return tempPos1;
	}
	void setTemPos2(POINT temp) {
		tempPos2 = temp;
	}
	POINT getTempPos2() {
		return tempPos2;
	}



	//1. 좌상단, 우하단 알아서 뱉어주는 함수
	POINT getUpLeft() {
		int tempLeftX;
		int tempUpY;
		POINT tempPoint;

		//1. x정해주기
		if (oldPos.x < newPos.x) {
			tempLeftX = oldPos.x;
		}
		else if (oldPos.x == newPos.x) {
			tempLeftX = oldPos.x;
		}
		else {
			tempLeftX = newPos.x;
		}

		//2. y정해주기
		if (oldPos.y < newPos.y) {
			tempUpY = oldPos.y;
		}
		else if (oldPos.y == newPos.y) {
			tempUpY = oldPos.x;
		}
		else {
			tempUpY = newPos.y;
		}

		tempPoint.x = tempLeftX;
		tempPoint.y = tempUpY;

		return tempPoint;
	}
	POINT getRightDown() {
		int tempRightX;
		int tempDownY;
		POINT tempPoint;

		//1. x정해주기
		if (oldPos.x < newPos.x) {
			tempRightX = newPos.x;
		}
		else if (oldPos.x == newPos.x) {
			tempRightX = oldPos.x;
		}
		else {
			tempRightX = oldPos.x;
		}

		//2. y정해주기
		if (oldPos.y < newPos.y) {
			tempDownY = newPos.y;
		}
		else if (oldPos.y == newPos.y) {
			tempDownY = oldPos.x;
		}
		else {
			tempDownY = oldPos.y;
		}

		tempPoint.x = tempRightX;
		tempPoint.y = tempDownY;
		return tempPoint;
	}

	void setOldX(int _x) {
		oldPos.x = _x;
	}
	void setOldY(int _y) {
		oldPos.y = _y;
	}
	void setNewX(int _x) {
		newPos.x = _x;
	}
	void setNewY(int _y) {
		newPos.y = _y;
	}
	void setOldPos(int _x, int _y) {
		oldPos.x = _x;
		oldPos.y = _y;
	}
	void setOldZoomPos(int _x, int _y, double zoomLevel) {
		double tempX = (double)_x / zoomLevel;
		double tempY = (double)_y / zoomLevel;
		setOldPos(tempX, tempY);
	}
	void setNewPos(int _x, int _y) {
		newPos.x = _x;
		newPos.y = _y;
	}
	void setNewZoomPos(int _x, int _y, double zoomLevel) {
		double tempX = (double)_x / zoomLevel;
		double tempY = (double)_y / zoomLevel;
		//printf(" newpos에 %lf, %lf \n", tempX, tempY);
		setNewPos(tempX, tempY);
	}
	void setNewPos(POINT _newPos) {
		newPos = _newPos;
	}
	void setOldPos(POINT _oldPos) {
		oldPos = _oldPos;
	}

	int getNewZoomX(double zoomLevel) {
		return (double)getNewPos().x / zoomLevel;
	}
	int getOldZoomX(double zoomLevel) {
		return (double)getOldPos().x / zoomLevel;
	}

	POINT getOldPos() {
		return oldPos;
	}
	POINT getNewPos() {
		return newPos;
	}

	void setGrap(bool grap) {
		grapped = grap;
	}

	bool getGrapped() {
		return grapped;
	}
	void setPulled(bool pull) {
		pulled = pull;
	}
	bool getPulled() {
		return pulled;
	}
	void setOriginPos(POINT newOriginPos) {
		ORIGIN_POS = newOriginPos;
	}
	void setOriginPos(int newX, int newY) {
		ORIGIN_POS.x = newX;
		ORIGIN_POS.y = newY;
	}
	int getXdist() {
		return newPos.x - oldPos.x;
	}

	int getYdist() {
		return newPos.y - oldPos.y;
	}
	int getZoomXdist(double zoomLevel) {
		return (double)getXdist()*zoomLevel;
	}
	int getZoomYdist(double zoomLevel) {
		return (double)getYdist()*zoomLevel;
	}


	POINT getOriginPos() {
		return ORIGIN_POS;
	}

	POINT getZoomOriginPos(double zoomLevel) {
		int tempX = (double)ORIGIN_POS.x/zoomLevel;
		int tempY = (double)ORIGIN_POS.y/zoomLevel;
		return{ tempX, tempY };
	}

	int getRelativeNewX() {
		return ORIGIN_POS.x + newPos.x;
	}
	
	int getRelativeNewY() {
		return ORIGIN_POS.y + newPos.y;
	}
	POINT getRelativeNewPos() {
		POINT temp;
		temp.x = getRelativeNewX();
		temp.y = getRelativeNewY();
		return { temp.x, temp.y };
	}
	int getRelativeOldX() {
		return ORIGIN_POS.x + oldPos.x;
	}
	int getRelativeOldY() {
		return ORIGIN_POS.y + oldPos.y;
	}

	POINT getRelativeOldPos() {
		POINT temp;
		temp.x = getRelativeOldX();
		temp.y = getRelativeOldY();
		return{ temp.x, temp.y };
	}
	POINT getRelativeUpleft() {
		POINT oldPos = getRelativeOldPos();
		POINT newPos = getRelativeNewPos();
		int tempLeftX;
		int tempUpY;
		POINT tempPoint;

		//1. x정해주기
		if (oldPos.x < newPos.x) {
			tempLeftX = oldPos.x;
		}
		else if (oldPos.x == newPos.x) {
			tempLeftX = oldPos.x;
		}
		else {
			tempLeftX = newPos.x;
		}

		//2. y정해주기
		if (oldPos.y < newPos.y) {
			tempUpY = oldPos.y;
		}
		else if (oldPos.y == newPos.y) {
			tempUpY = oldPos.x;
		}
		else {
			tempUpY = newPos.y;
		}

		tempPoint.x = tempLeftX;
		tempPoint.y = tempUpY;

		return tempPoint;
	}
	POINT getRelativeDownRight() {
		POINT oldPos = getRelativeOldPos();
		POINT newPos = getRelativeNewPos();
		int tempRightX;
		int tempDownY;
		POINT tempPoint;

		//1. x정해주기
		if (oldPos.x < newPos.x) {
			tempRightX = newPos.x;
		}
		else if (oldPos.x == newPos.x) {
			tempRightX = oldPos.x;
		}
		else {
			tempRightX = oldPos.x;
		}

		//2. y정해주기
		if (oldPos.y < newPos.y) {
			tempDownY = newPos.y;
		}
		else if (oldPos.y == newPos.y) {
			tempDownY = oldPos.x;
		}
		else {
			tempDownY = oldPos.y;
		}

		tempPoint.x = tempRightX;
		tempPoint.y = tempDownY;
		return tempPoint;
	}

	//1. pulling 액션
	void pullingZoomAction(double zoomLevel) {
		if (getPulled()) {
			int xdist = getZoomXdist(zoomLevel);
			int ydist = getZoomYdist(zoomLevel);
			int newOriginX = getOriginPos().x - xdist;
			int newOriginY = getOriginPos().y - ydist;
			setOriginPos(newOriginX, newOriginY); //원점 새로 지정 

			setOldPos(getNewPos()); //새로 갈아 끼우기

		}
	}

	void pullingAction() {
		if (getPulled()) {
			printf("pulling action \n");
			int xdist = getXdist();
			int ydist = getYdist();
			int newOriginX = getOriginPos().x - xdist;
			int newOriginY = getOriginPos().y - ydist;
			setOriginPos(newOriginX, newOriginY); //원점 새로 지정 

			setOldPos(getNewPos()); //새로 갈아 끼우기

		}
	}
	

};