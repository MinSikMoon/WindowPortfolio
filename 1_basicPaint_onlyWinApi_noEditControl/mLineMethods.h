
#pragma once
#include "mTextPixel.h"
#include "mScreenLineContainer.h"

//1. 일단 static으로 autoLineSwitch를 만들어 보자. 
//1. mString이 들어오면 개행해서 출력해준다. //기본 함수
 void autoLineSwitch(HDC hdc, mString& mStr, int screenWidth, int startPos, int wordHeight) {
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


//2. TCHAR 버젼 // loop cnt 뱉어주는 신버젼 실험
int autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
	mString mStr(_mStr); //임시 mStr 만든다. 

	if (mStr.getLength() == 0) {
		return 0;
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
	return loopCnt;

}

//autoLineSwitch에 screenLineContainer 넣어서 노드당 정보를 검출
//screenLineContainer와 nodeIdx를 넣어주면 node당 라인에 따른 정보들을 입력해준다. 
int autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight, mScreenLineContainer& screenLineContainer, size_m nodeIdx) {
	mString mStr(_mStr); //임시 mStr 만든다. 

	if (mStr.getLength() == 0) {
		return 0;
	}
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
	int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //화면에 몇 줄로 출력되나.
																 //printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	int tempLineWordCnt = 0;

	//이제 여기서 nodeIdx당 라인당 글자수, 첫 인덱스, 끝인덱스를 기록해줘야 한다. 
	while (1) {
		if (wordCnt == mStr.getLength())
			break;

		int tempLineWordCnt = 0;

		lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

		tempLineWordCnt = (lastIdx - startIdx + 1);
		wordCnt += tempLineWordCnt;
		screenLineContainer.setNodeLineData(nodeIdx, loopCnt, tempLineWordCnt, startIdx, lastIdx);

		textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
		startIdx = lastIdx + 1;
		//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}
	return loopCnt; //현재 들어온 문단하나가 몇개의 라인으로 출력되고 있는가
}

//테스트 : autoLineSwitch 를 0에서 시작된다고 가정하고 만들었는데. 생각해보니 x시작점도 생각해야 한다. 
int autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startXpos, int startYpos, int wordHeight, mScreenLineContainer& screenLineContainer, size_m nodeIdx) {
	mString mStr(_mStr); //임시 mStr 만든다. 

	if (mStr.getLength() == 0) {
		return 0;
	}
	
	int limitWidth = screenWidth;
	//int limitWidth = screenWidth - startXpos;
	int startIdx = 0;
	int lastIdx = 0;
	int loopCnt = 0;
	int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // 전체 문장의 픽셀너비
	int estCharWidth = tempStrPixelWidth / mStr.getLength(); // 대강 한글자당 픽셀 너비가 몇인가 예측
	int screenSentenceNum = tempStrPixelWidth / limitWidth + 1; //화면에 몇 줄로 출력되나.
																 //printf("mstr글자 갯수는 %d, 줄수는 %d \n", mStr.getLength(), screenSentenceNum);
	int wordCnt = 0;
	int tempLineWordCnt = 0;

	//이제 여기서 nodeIdx당 라인당 글자수, 첫 인덱스, 끝인덱스를 기록해줘야 한다. 
	while (1) {
		if (wordCnt == mStr.getLength())
			break;

		int tempLineWordCnt = 0;

		lastIdx = getScreenEndIdx(hdc, mStr, limitWidth, startIdx, estCharWidth, 1);

		tempLineWordCnt = (lastIdx - startIdx + 1);
		wordCnt += tempLineWordCnt;
		screenLineContainer.setNodeLineData(nodeIdx, loopCnt, tempLineWordCnt, startIdx, lastIdx);

		textOutCustom(hdc, mStr, startIdx, lastIdx, startXpos, loopCnt*wordHeight - startYpos);
		startIdx = lastIdx + 1;
		//printf("현재 loopcnt : %d, lastidx: %d, wordcnt: %d, 다음 startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
		loopCnt++;
	}
	return loopCnt; //현재 들어온 문단하나가 몇개의 라인으로 출력되고 있는가
}