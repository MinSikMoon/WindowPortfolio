
#pragma once

#include "mLineMethods.h"


typedef list<TCHAR*> text;

/*==========================inner method==================================*/
class mTextSource { //�ȿ� ����Ʈ�� �־ ���� ��ü�� �ƴ´�. 
	text* textSource; //���� ��ü�� �ƴ� ����Ʈ�� ����Ű�� ������ 
	size_m textNum; //1������ � �ִ���, �� ��尡 � �ִ��� ������. �ε����� �ƴ϶�.

public:
	//GETTERS
	size_m getTextNum() { //���� ������ �� �� ���� ����
		return textNum;
	}

	text* getTextSource() { //���� ���ڿ��� ����� ����Ʈ�� �ּҰ��� ����
		return textSource;
	}

	//1. CONSTRUCTOR : �⺻ : �ؽ�Ʈ ����(���)�� 0�� �ǰ� �ϰ�. ����Ʈ�� ������ 0�ΰɷ� �ʱ�ȭ 
	mTextSource() : textNum(0) {
		textSource = new text(0); //����� 0�� ����Ʈ�� �����. 
	}

	//2. DESTRUCTOR
	~mTextSource() {
		for (text::iterator it = (*textSource).begin(); it != (*textSource).end(); it++) {
			delete (*it);
		}
		delete textSource;
	}

	//3. ���� �ϳ� �߰�
	void addText(TCHAR* _inStr) {
		(*textSource).push_back(_inStr);
		textNum++;
	}
	void addText(const TCHAR* _inStr) {
		TCHAR* tempStr = cloneStr(_inStr);
		(*textSource).push_back(tempStr);
		textNum++;
	}
	void addText(mString& mStr) {
		TCHAR* tempStr = mStr.cloneStr();
		(*textSource).push_back(tempStr);
		textNum++;
	}
	//4. insertTextAt : ���ϴ� ��� �ε����� ������ �߰��Ѵ�. //�������� �ڷ� �з���. // 
	void insertTextAt(size_m nodeIdx, TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. ����Ʈ�� �ϳ� ����� ��� ���ڿ��� �ִ´�. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(_inStr);

		//2. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr�� ����Ű�� ���� splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum ����
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, const TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. �׳� tchar ���ڿ��� ����
		TCHAR* tempStr = cloneStr(_inStr);

		//1. ����Ʈ�� �ϳ� ����� ��� ���ڿ��� �ִ´�. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr�� ����Ű�� ���� splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum ����
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, mString& mStr) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>�߸��� ���� in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. �׳� tchar ���ڿ��� ����
		TCHAR* tempStr = mStr.cloneStr();

		//1. ����Ʈ�� �ϳ� ����� ��� ���ڿ��� �ִ´�. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr�� ����Ű�� ���� splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum ����
		textNum++;
	}
	
	//5. eraseText : �ε����� ����Ű�� ���� �����͸� �����Ѵ�. 
	void eraseTextAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>�߸��� ���� in eraseTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. iterator�� ���Ѵ�. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}

		//2. erase()
		(*textSource).erase(itr);
		
		//3. ���� ���� ���̱�
		textNum--;
	}

	
	//5.1 itr ��°� �ʹ� �����Ƽ� �ε��� ����Ű�� itr �ϳ� ������. 
	text::iterator getItrAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in replaceTextAt() : getItrAt => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			itr++;
		}
		return itr;
	}

	//6. getTextAt() : �ε����� ����Ű�� ���� ��� ������, �� ���ڿ��� ������ �´�. 
	TCHAR* getTextAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in replaceTextAt() :  getnodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		text::iterator itr = getItrAt(nodeIdx);
		return (*itr);
		//_tprintf(TEXT("%ls \n"), (*itr));
	}

	//6.5 cloneTextAt
	TCHAR* cloneTextAt(size_m nodeIdx) {
		mString* tempStr = new mString(getTextAt(nodeIdx));
		TCHAR* temp = tempStr->cloneStr();
		delete tempStr;
		return temp;
	}
	//7. replaceTextAt() : �ε����� ����Ű�� ����� ���ڿ��� ���ο� ���ڿ��� ���Ƴ����. 
	void replaceTextAt(size_m nodeIdx, TCHAR* _newStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in replaceTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		insertTextAt(nodeIdx, _newStr); //�о�ְ� 
		eraseTextAt(nodeIdx + 1); //�ڷ� �з��� ���� ������ ����
	}
	void replaceTextAt(size_m nodeIdx, const TCHAR* _newStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in replaceTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		TCHAR* tempStr = cloneStr(_newStr);
		replaceTextAt(nodeIdx, tempStr);
	}
	void replaceTextAt(size_m nodeIdx, mString& _newStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>�߸��� ���� in replaceTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		TCHAR* tempStr = _newStr.cloneStr();
		replaceTextAt(nodeIdx, tempStr);
		
	}

	//8. showAllText : ���� autoLineSwitch�� �̿��ؼ� ȭ�鿡 ��� ������ ��µǴ� �Լ��� ������. 
	void showAllText(HDC hdc, size_m screenWidth, int firstLinePos, size_m wordHeight) {
		if (textNum == 0) //������� �׳� ����
			return;

		int totalLoopCnt = 0;
		
		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			totalLoopCnt += autoLineSwitch(hdc, (*itr), screenWidth, (firstLinePos + totalLoopCnt*wordHeight)*(-1), wordHeight);
		
		}

	}

	//showAllText : ���� ���� ù�ε���, ���ε���, ���ڼ� �� �˾Ƴ�.
	void showAllText(HDC hdc, size_m screenWidth, int firstLinePos, size_m wordHeight, mScreenLineContainer& screenLineContainer) {
		screenLineContainer.setTextSourceNodeNum(textNum); //��尡 ����� �������ش�. 

														   //����
		if (textNum == 0) //������� �׳� ����
			return;

		//����
		int totalLoopCnt = 0;
		int tempNodeIdx = 0; //��� �ε��� ���� ����
		int tempLineNumInNode; //��� ���� �ε��� ���� ���� ����, ������ �ʱ�ȭ �Ǿ����. ���� �ϳ��� ��� �ϳ��̱� ����

		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			tempLineNumInNode = 0;
			tempLineNumInNode += autoLineSwitch(hdc, (*itr), screenWidth, (firstLinePos + totalLoopCnt*wordHeight)*(-1), wordHeight, screenLineContainer, tempNodeIdx);
			totalLoopCnt += tempLineNumInNode;
			//printf("tempLineNum = %d \n", tempLineNumInNode);
			screenLineContainer.setNodeLineNumInfo(tempNodeIdx++, tempLineNumInNode); //���� �� ������ ������ �ִ��� ����.
		}
		screenLineContainer.setTotalLineNum(totalLoopCnt); //�ؽ�Ʈ ��ü ������ ����� ����. 
	}

	//showAllText2 : x��ǥ �������� �־��ִ� ����:���� ���� ù�ε���, ���ε���, ���ڼ� �� �˾Ƴ�.
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, size_m wordHeight, mScreenLineContainer& screenLineContainer) {
		screenLineContainer.setTextSourceNodeNum(textNum); //��尡 ����� �������ش�. 

														   //����
		if (textNum == 0) //������� �׳� ����
			return;

		//����
		int totalLoopCnt = 0;
		int tempNodeIdx = 0; //��� �ε��� ���� ����
		int tempLineNumInNode; //��� ���� �ε��� ���� ���� ����, ������ �ʱ�ȭ �Ǿ����. ���� �ϳ��� ��� �ϳ��̱� ����
		
		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			tempLineNumInNode = 0;
			//�׽�Ʈ autoLineSwitch���� ���� �ִµ�, ������ 0���̸� lineNum�� �߰��� �ȵȴ�. 
			tempLineNumInNode += autoLineSwitch(hdc, (*itr), screenWidth, firstLineXpos, (firstLineYpos + totalLoopCnt*wordHeight)*(-1), wordHeight, screenLineContainer, tempNodeIdx);
			totalLoopCnt += tempLineNumInNode;
			
			screenLineContainer.setNodeLineNumInfo(tempNodeIdx++, tempLineNumInNode); //���� �� ������ ������ �ִ��� ����.
		}
		screenLineContainer.setTotalLineNum(totalLoopCnt); //�ؽ�Ʈ ��ü ������ ����� ����. 
	}

	//�׽�Ʈ : showAllText�� �ܼ��� ���������̳� ������ �������� �ϰ�, ���ڸ� �����ִ� ���� ���⼭ ����. 
	void textShowByLineContainer(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, size_m wordHeight, mScreenLineContainer& screenLineContainer) {
		text::iterator itr = (*textSource).begin();
		int totalLoopCnt = 0;

		int i = 0;
		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) { //���� ��� �ϳ��� ����. 
			int lineNum = screenLineContainer.getNodeLineNumInfo(i); 
			if (lineNum == 0) { //������ �����Ѵ�. 
				TextOut(hdc, firstLineXpos, totalLoopCnt*wordHeight - firstLineYpos, TEXT(""), 0);
				totalLoopCnt++;
			}
			else {
				mString mStr((*itr));
				for (int j = 0; j < lineNum; j++) {
					textOutCustom(hdc, mStr, screenLineContainer.getFirstIdx(i, j),screenLineContainer.getLastIdx(i,j), firstLineXpos, totalLoopCnt*wordHeight - firstLineYpos);
					totalLoopCnt++;
				}
			}
			i++;
		}
	}

	//showAlltext���� ���������̳� ������ �����ϰ� textout���� �ϴ� ���� ����� �ʹ� ����.
	//���� �����̳� ������ ���� �����ϴ� �� ������.  // �� ������ ����. �� �ٷ� ���������� ī��Ʈ �̾��ش�. 
	int setOneNodeLineContainerInfo(HDC hdc, TCHAR* _mStr, int screenWidth, int startXpos, int startYpos, int wordHeight, size_m nodeIdx, mScreenLineContainer& screenLineContainer) {
		mString mStr(_mStr); //�ӽ� mStr �����. // mStr�� �� �����̴�. ��ü �ؽ�Ʈ�� �ƴ϶�.

		if (mStr.getLength() == 0) {
			return 1; //�ϴ� ������ �ϳ� ���Դٸ� �⺻������ 0���� �ƴ϶� 1���̴�. 
		}

		int limitWidth = screenWidth;
		int startIdx = 0;
		int lastIdx = 0;
		int loopCnt = 0; //���ڰ� �� ���ڶ� �� ������ 1���� �ȴ�.  
		int tempStrPixelWidth = getMstrPixelWidthFull(hdc, mStr); // ��ü ������ �ȼ��ʺ�
		int estCharWidth = tempStrPixelWidth / mStr.getLength(); // �밭 �ѱ��ڴ� �ȼ� �ʺ� ���ΰ� ����
		int wordCnt = 0;
		int tempLineWordCnt = 0;

		//���� ���⼭ nodeIdx�� ���δ� ���ڼ�, ù �ε���, ���ε����� �������� �Ѵ�. 
		while (1) {

			if (wordCnt == mStr.getLength())
				break;

			int tempLineWordCnt = 0;

			lastIdx = getScreenEndIdx(hdc, mStr, limitWidth, startIdx, estCharWidth, 1);
					
			if (lastIdx < startIdx) //�������� �ƴ϶� �̰� ��������. �̷� ��� ���͸� �������. 
				return 0;

			tempLineWordCnt = (lastIdx - startIdx + 1);
			wordCnt += tempLineWordCnt;
			screenLineContainer.setNodeLineData(nodeIdx, loopCnt, tempLineWordCnt, startIdx, lastIdx);
			
			startIdx = lastIdx + 1;
			loopCnt++;
		}
		return loopCnt; //���� ���� �����ϳ��� ��� �������� ��µǰ� �ִ°�
	}
	void setAllLineContainerInfo(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, size_m wordHeight, mScreenLineContainer& screenLineContainer) {
		screenLineContainer.setTextSourceNodeNum(textNum); //1. ��尡 ����� �������ش�. 
		
		if (textNum == 0) //������ �ϳ��� ������  �׳� ����
			return;

		int totalLoopCnt = 0;
		int tempNodeIdx = 0; //��� �ε��� ���� ����
		int tempLineNumInNode; //��� ���� �ε��� ���� ���� ����, ������ �ʱ�ȭ �Ǿ����. ���� �ϳ��� ��� �ϳ��̱� ����

		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			tempLineNumInNode = 0;
			tempLineNumInNode += setOneNodeLineContainerInfo(hdc, (*itr), screenWidth, firstLineXpos, (firstLineYpos + totalLoopCnt*wordHeight)*(-1), wordHeight, tempNodeIdx, screenLineContainer);
			totalLoopCnt += tempLineNumInNode;

			screenLineContainer.setNodeLineNumInfo(tempNodeIdx++, tempLineNumInNode); //���� �� ������ ������ �ִ��� ����.
		}
		screenLineContainer.setTotalLineNum(totalLoopCnt); //�ؽ�Ʈ ��ü ������ ����� ����. 

	}
	
	
	
	
	//debugging : show
	void show() {
		printf("cur list size : %d \n", (*textSource).size());
		printf("cur textNum : %d \n", textNum);
		for (text::iterator it = (*textSource).begin(); it != (*textSource).end(); it++) {
			_tprintf(TEXT("%ls \n"), (*it));
		}

	}
};