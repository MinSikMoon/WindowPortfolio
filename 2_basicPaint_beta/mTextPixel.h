#pragma once
#include "mString.h"


//1. mString�� �޾Ƽ� �ȼ� ���α��̸� �������ִ� �Լ�
size_m getMstrPixelWidth(HDC hdc, mString& mStr, size_m startIdx, size_m endIdx) {
	
	size_m length = mStr.getLength(); //mStr�� str ����
	if (startIdx < 0 || endIdx >(length-1) || startIdx > endIdx) { //���� for �߸��� �Է�
		printf("!!!!!!!!!********>>>>�߸��� ���� ���� in getStrPixelWidth() : length: %d ,sIdx: %d, eIdx: %d \n",length, startIdx, endIdx);
		system("pause");
		//exit(-1); //����
	}

	SIZE size;
	if (startIdx > endIdx) { //�߸��� ���̽� ���Ϳ뵵 
		startIdx = endIdx;
	}

	TCHAR* tempStr = mStr.subFromTo(startIdx, endIdx); //�ӽ� ���ڿ� �̰�.
	GetTextExtentPoint(hdc, tempStr, _tcslen(tempStr), &size); //���� �纸��
	delete tempStr; //�ӽ� ���ڿ� ����

	return size.cx;
}

//2. mString�� str �ȼ� ���� ���� �״�� ����
size_m getMstrPixelWidthFull(HDC hdc, mString& mStr) {
	SIZE size;
	GetTextExtentPoint(hdc, mStr.getStr(), _tcslen(mStr.getStr()), &size); //���� �纸��
	return size.cx;
}

//3. textOut Ŀ����
void textOutCustom(HDC hdc, mString& mStr, size_m startIdx, size_m endIdx, int xPos, int yPos) {
	
	TCHAR* tempStr = mStr.subFromTo(startIdx, endIdx);
	int tempLength = _tcslen(tempStr);

	TextOut(hdc, xPos, yPos, tempStr, tempLength);
	SetBkMode(hdc, TRANSPARENT);
	delete tempStr;
}

//4. ���� ȭ�鿡�� ���ٿ� ������ �� �ε������� ����� �����Ѱ�?
int getScreenEndIdx(HDC hdc, mString& mStr, int limitWidth, int startIdx, int avgCharWidth, int detailCustom) {
	if (startIdx < 0) {
		printf("====================================> �߸��� ���� int getScreenEndIdx() startIdx�� 0���� �۾��� : %d \n", startIdx);
		system("pause");
		exit(-1);
	}
	if (getMstrPixelWidth(hdc, mStr, startIdx, mStr.getLength() - 1) <= limitWidth) {
		return (mStr.getLength() - 1);
	}



	//1. ��� ���� �ʺ� �̿��ؼ� limitWidth �ȿ� �� ����� ���� ���� �����غ���.
	int estWordNum = limitWidth / avgCharWidth + 1;
	if (estWordNum > mStr.getLength()) {
		estWordNum = mStr.getLength();
	}

	//2. �� ���� ���ڷ� ���� ���ο� ���ڿ� �����
	int tempEndIdx = startIdx + estWordNum - 1;
	//�׽�Ʈ�� ����
	if (tempEndIdx > (mStr.getLength() - 1))
		tempEndIdx = mStr.getLength() - 1;

	mString tempStr(mStr.subFromTo(startIdx, tempEndIdx)); //�ӽ� mString �ϳ� �����
														   //3. ���� limitWidth�� ���ϸ鼭 �����غ���.
	if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth) { //��� ������� �ѱ��ھ� 
		while (1) {
			tempEndIdx--;
			tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //���ɷ� ��ü 
			if (getMstrPixelWidthFull(hdc, tempStr) <= limitWidth)
				break;
		}
	}
	else {
		while (1) { //ª���� �Ѱ��� �����ش�. 
			tempEndIdx++;
			tempStr.replaceStr(mStr.subFromTo(startIdx, tempEndIdx)); //���ɷ� ��ü 
			if (getMstrPixelWidthFull(hdc, tempStr) > limitWidth)
				break;
		}
	}

	return tempEndIdx - detailCustom; //�ϳ� ����� ȭ�� �ٱ����� �� ��������.
}

