
#include "mTextSource.h"
#include "mScreenLineContainer.h"
#pragma once

class mScreenTextController {
	//0. 본문 자체를 가지고 있는 클래스
	mTextSource textSource;
	map<int, int> nodeLineNum; //노드당 몇라인을 가지고 있는지 갯수를 가지고 있는 맵
	mScreenLineContainer lc; //노드와 노드안 라인 인덱스를 넣으면 실제 textSource의 노드상의 문자열 인덱스와 마지막 인덱스, 글자수를 알수있음.

							 //1. 전체 사이즈 관련 변수들 
	size_m wordPixelHeight; // 문자의 높이는 픽셀로 몇인가?
	size_m totalPixelHeight; // 전체 텍스트의 높이는 픽셀로 몇인가? 
	size_m totalLineNum; // 전체 텍스트는 몇 줄인가? 
	size_m upperLineNum; // 화면 위로 잘린 줄은 몇 줄인가?
	size_m restLineNum; // 현재 화면에 보이는 줄 + 남은 줄의 갯수 totalLineNum - upperLineNum

						//2. 화면에 출력되는 첫번째 문장과 화면 맨끝의 문장이 속한 노드와 해당 노드 안에서의 라인 인덱스
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;

public:
	//1. 일단 static으로 autoLineSwitch를 만들어 보자. 
	//1. mString이 들어오면 개행해서 출력해준다. //기본 함수
	static void autoLineSwitch(HDC hdc, mString& mStr, int screenWidth, int startPos, int wordHeight) {
		if (mStr.getLength() == 0) {
			return;
		}
		int startIdx = 0;
		int lastIdx = 0; 
		int loopCnt = 0;
		int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
		int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
		int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //화면에 몇 줄로 출력되나.
		//printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
		int wordCnt = 0;
		while (1) {
			//printf("현재 wordcnt : %d, 목표는 %d \n", wordCnt, mStr.getLength());
			if (wordCnt == mStr.getLength())
				break;
			
			lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);
			
			wordCnt += (lastIdx - startIdx + 1);
			textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
			startIdx = lastIdx+1;
			//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
			loopCnt++;
		}
		
	}
	//2. TCHAR 버젼
	static void autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
		mString mStr(_mStr); //임시 mStr 만든다. 
		
		if (mStr.getLength() == 0) {
			return;
		}
		int startIdx = 0;
		int lastIdx = 0;
		int loopCnt = 0;
		int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
		int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
		int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //화면에 몇 줄로 출력되나.
																	 //printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
		int wordCnt = 0;
		while (1) {
			//printf("현재 wordcnt : %d, 목표는 %d \n", wordCnt, mStr.getLength());
			if (wordCnt == mStr.getLength())
				break;

			lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

			wordCnt += (lastIdx - startIdx + 1);
			textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
			startIdx = lastIdx + 1;
			//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
			loopCnt++;
		}

	}

};