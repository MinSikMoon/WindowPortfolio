#pragma once

#include <windef.h>

class mZoomOrigin{
private:
	POINT zoomUpleft;
	POINT zoomDownRight;


public:
	void setZoomUpLeft(POINT zoomUpLeft) {
		this->zoomUpleft = zoomUpLeft;
	}
	void setZoomDownRight(POINT zoomDownRight) {
		this->zoomDownRight = zoomDownRight;
	}

	POINT getZoomUpLeft() {
		return zoomUpleft;
	}
	POINT getZoomDownRight() {
		return zoomDownRight;
	}
};