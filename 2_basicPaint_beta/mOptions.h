#pragma once
#include <Windows.h>
class mOptions {
private: //�ܷ���, �ܰ��� xform, ��Ʈũ��, 
		 //0. �ʱ� ������ ũ��

		 //1. ��
	double zoomLevel;
	double zoomInc;
	double zoomMin;
	double zoomMax;
	XFORM zoomXform;


	//2. ��Ʈ
	HFONT hFont;
	map<TCHAR*,HFONT> hFontMap;
	double fontSize;
	TCHAR* fontName;

	//3. ��
	HPEN hPen;
	double penWidth;
	
	//4. old 
	HFONT hOldFont;
	HPEN hOldPen;

public:
	//1. constructor
	mOptions() {
		zoomLevel = 1.0;
		zoomInc = 0.1;
		zoomXform.eM11 = zoomLevel;
		zoomXform.eM22 = zoomLevel;
		zoomMin = 1.0;
		zoomMax = 20.0;

		fontSize = 16;
		
		saveNewFont(TEXT("�ü�"));
		saveNewFont(TEXT("����"));
		saveNewFont(TEXT("����"));
		saveNewFont(TEXT("����"));

		penWidth = 1;
		hPen = CreatePen(PS_SOLID, penWidth, RGB(0, 0, 0));
	}
	

	//2. zoom ����
	void setZoomLevel(double newZoomLevel) {
		zoomLevel = newZoomLevel;
	}
	void setZoomInc(double newZoomInc) {
		zoomInc = newZoomInc;
	}
	void setZoomMin(double newZoomMin) {
		zoomMin = newZoomMin;
	}
	void setZoomMax(double newZoomMax) {
		zoomMax = newZoomMax;
	}
	void zoomIn() {
		zoomLevel += zoomInc;

		if (zoomLevel > zoomMax) {
			zoomLevel = zoomMax;
		}
	}
	void zoomOut() {
		zoomLevel -= zoomInc;

		if (zoomLevel < zoomMin) {
			zoomLevel = zoomMin;
		}
	}
	double getZoomLevel() {
		return zoomLevel;
	}
	
	
	void setFontSize(double size) {
		fontSize = size;
	}
	double getFontSize() {
		return fontSize;
	}
	void setFontName(TCHAR* newFontName) {
		fontName = newFontName; //ö���� �ּҰ����� �Ѵ�. 
	}

	void saveNewFont(TCHAR* newFontName) {
		hFontMap[newFontName] = (HFONT)CreateFont(fontSize, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, newFontName);
	}

	HFONT getFont(TCHAR* fontName) {
		/*wprintf(L"���� �۲��� %ls \n", this->fontName);
		wprintf(L"%ls �۲��� �޶� \n", fontName);
		wprintf(L"��ȯ�Ǵ� ���� %lf \n", hFontMap[fontName]);*/
		return hFontMap[fontName];
	}
	//4. pen ����
	HPEN getPen() {
		return hPen;
	}
	void setPenWidth(double newPenWidth) {
		penWidth = newPenWidth;
	}

	//5. paint
	void setting(HDC hdc) {
		
		hFont = getFont(fontName);
		hPen = getPen();
		hOldFont =(HFONT)SelectObject(hdc, hFont);
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		zoomXform.eM11 = zoomLevel;
		zoomXform.eM22 = zoomLevel;
		SetGraphicsMode(hdc, GM_ADVANCED);
		SetWorldTransform(hdc, &zoomXform);
	}
	void settingOlds(HDC hdc) {
		SelectObject(hdc, hOldFont);
		SelectObject(hdc, hOldPen);
	}
};
