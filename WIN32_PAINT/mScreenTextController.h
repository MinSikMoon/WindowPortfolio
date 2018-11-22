
#include "mTextSource.h"
#include "mScreenLineContainer.h"
#pragma once

class mScreenTextController {
	//0. ���� ��ü�� ������ �ִ� Ŭ����
	mTextSource textSource;
	map<int, int> nodeLineNum; //���� ������� ������ �ִ��� ������ ������ �ִ� ��
	mScreenLineContainer lc; //���� ���� ���� �ε����� ������ ���� textSource�� ������ ���ڿ� �ε����� ������ �ε���, ���ڼ��� �˼�����.

							 //1. ��ü ������ ���� ������ 
	size_m wordPixelHeight; // ������ ���̴� �ȼ��� ���ΰ�?
	size_m totalPixelHeight; // ��ü �ؽ�Ʈ�� ���̴� �ȼ��� ���ΰ�? 
	size_m totalLineNum; // ��ü �ؽ�Ʈ�� �� ���ΰ�? 
	size_m upperLineNum; // ȭ�� ���� �߸� ���� �� ���ΰ�?
	size_m restLineNum; // ���� ȭ�鿡 ���̴� �� + ���� ���� ���� totalLineNum - upperLineNum

						//2. ȭ�鿡 ��µǴ� ù��° ����� ȭ�� �ǳ��� ������ ���� ���� �ش� ��� �ȿ����� ���� �ε���
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;

public:
	//1. �ϴ� static���� autoLineSwitch�� ����� ����. 
	//1. mString�� ������ �����ؼ� ������ش�. //�⺻ �Լ�
	static void autoLineSwitch(HDC hdc, mString& mStr, int screenWidth, int startPos, int wordHeight) {
		if (mStr.getLength() == 0) {
			return;
		}
		int startIdx = 0;
		int lastIdx = 0; 
		int loopCnt = 0;
		int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
		int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
		int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //ȭ�鿡 �� �ٷ� ��µǳ�.
		//printf("mstr���� ������ %d, �ټ��� %d \n", mStr.getLength(), screenSentenceNum);
		int wordCnt = 0;
		while (1) {
			//printf("���� wordcnt : %d, ��ǥ�� %d \n", wordCnt, mStr.getLength());
			if (wordCnt == mStr.getLength())
				break;
			
			lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);
			
			wordCnt += (lastIdx - startIdx + 1);
			textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
			startIdx = lastIdx+1;
			//printf("���� loopcnt : %d, lastidx: %d, wordcnt: %d, ���� startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
			loopCnt++;
		}
		
	}
	//2. TCHAR ����
	static void autoLineSwitch(HDC hdc, TCHAR* _mStr, int screenWidth, int startPos, int wordHeight) {
		mString mStr(_mStr); //�ӽ� mStr �����. 
		
		if (mStr.getLength() == 0) {
			return;
		}
		int startIdx = 0;
		int lastIdx = 0;
		int loopCnt = 0;
		int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
		int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
		int screenSentenceNum = tempStrPixelWidth / screenWidth + 1; //ȭ�鿡 �� �ٷ� ��µǳ�.
																	 //printf("mstr���� ������ %d, �ټ��� %d \n", mStr.getLength(), screenSentenceNum);
		int wordCnt = 0;
		while (1) {
			//printf("���� wordcnt : %d, ��ǥ�� %d \n", wordCnt, mStr.getLength());
			if (wordCnt == mStr.getLength())
				break;

			lastIdx = getScreenEndIdx(hdc, mStr, screenWidth, startIdx, estCharWidth, 1);

			wordCnt += (lastIdx - startIdx + 1);
			textOutCustom(hdc, mStr, startIdx, lastIdx, 0, loopCnt*wordHeight - startPos);
			startIdx = lastIdx + 1;
			//printf("���� loopcnt : %d, lastidx: %d, wordcnt: %d, ���� startIdx : %d \n", loopCnt, lastIdx, wordCnt, startIdx);
			loopCnt++;
		}

	}

};