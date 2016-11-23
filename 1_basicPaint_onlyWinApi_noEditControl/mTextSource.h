
#pragma once

#include "mLineMethods.h"


typedef list<TCHAR*> text;

/*==========================inner method==================================*/
class mTextSource { //안에 리스트가 있어서 본문 자체를 싣는다. 
	text* textSource; //본문 자체를 싣는 리스트를 가리키는 포인터 
	size_m textNum; //1문단이 몇개 있는지, 즉 노드가 몇개 있는지 갯수다. 인덱스가 아니라.

public:
	//GETTERS
	size_m getTextNum() { //현재 문단이 몇 개 인지 리턴
		return textNum;
	}

	text* getTextSource() { //현재 문자열이 저장된 리스트의 주소값을 리턴
		return textSource;
	}

	//1. CONSTRUCTOR : 기본 : 텍스트 숫자(노드)가 0이 되게 하고. 리스트도 사이즈 0인걸로 초기화 
	mTextSource() : textNum(0) {
		textSource = new text(0); //사이즈가 0인 리스트를 만든다. 
	}

	//2. DESTRUCTOR
	~mTextSource() {
		for (text::iterator it = (*textSource).begin(); it != (*textSource).end(); it++) {
			delete (*it);
		}
		delete textSource;
	}

	//3. 문단 하나 추가
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
	//4. insertTextAt : 원하는 노드 인덱스에 문단을 추가한다. //나머지는 뒤로 밀려남. // 
	void insertTextAt(size_m nodeIdx, TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. 리스트를 하나 만들고 대상 문자열을 넣는다. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(_inStr);

		//2. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr이 가리키는 곳에 splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum 갱신
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, const TCHAR* _inStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. 그냥 tchar 문자열로 복사
		TCHAR* tempStr = cloneStr(_inStr);

		//1. 리스트를 하나 만들고 대상 문자열을 넣는다. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr이 가리키는 곳에 splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum 갱신
		textNum++;
	}
	void insertTextAt(size_m nodeIdx, mString& mStr) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>잘못된 범위 in insertTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		//0. 그냥 tchar 문자열로 복사
		TCHAR* tempStr = mStr.cloneStr();

		//1. 리스트를 하나 만들고 대상 문자열을 넣는다. 
		text* tempTextSource = new text(0);
		(*tempTextSource).push_back(tempStr);

		//2. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}
		//3. itr이 가리키는 곳에 splice
		(*textSource).splice(itr, (*tempTextSource));

		//4. textNum 갱신
		textNum++;
	}
	
	//5. eraseText : 인덱스가 가리키는 곳의 데이터를 삭제한다. 
	void eraseTextAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx > (textNum-1)) {
			printf("============================>잘못된 범위 in eraseTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}

		//1. iterator를 구한다. 
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			++itr;
		}

		//2. erase()
		(*textSource).erase(itr);
		
		//3. 문단 갯수 줄이기
		textNum--;
	}

	
	//5.1 itr 얻는게 너무 귀찮아서 인덱스 가리키는 itr 하나 만들자. 
	text::iterator getItrAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in replaceTextAt() : getItrAt => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		text::iterator itr = (*textSource).begin();
		for (int i = 0; i < nodeIdx; i++) {
			itr++;
		}
		return itr;
	}

	//6. getTextAt() : 인덱스가 가리키는 곳의 노드 데이터, 즉 문자열을 가지고 온다. 
	TCHAR* getTextAt(size_m nodeIdx) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in replaceTextAt() :  getnodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		text::iterator itr = getItrAt(nodeIdx);
		return (*itr);
		//_tprintf(TEXT("%ls \n"), (*itr));
	}

	//7. replaceTextAt() : 인덱스가 가리키는 노드의 문자열을 새로운 문자열로 갈아끼운다. 
	void replaceTextAt(size_m nodeIdx, TCHAR* _newStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in replaceTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		insertTextAt(nodeIdx, _newStr); //밀어넣고 
		eraseTextAt(nodeIdx + 1); //뒤로 밀려난 기존 문장을 삭제
	}
	void replaceTextAt(size_m nodeIdx, const TCHAR* _newStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in replaceTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		TCHAR* tempStr = cloneStr(_newStr);
		replaceTextAt(nodeIdx, tempStr);
	}
	void replaceTextAt(size_m nodeIdx, mString& _newStr) {
		if (nodeIdx < 0 || nodeIdx >(textNum - 1)) {
			printf("============================>잘못된 범위 in replaceTextAt() : nodeIdx => %d \n", nodeIdx);
			system("pause");
			exit(-1);
		}
		TCHAR* tempStr = _newStr.cloneStr();
		replaceTextAt(nodeIdx, tempStr);
		
	}

	//8. showAllText : 이제 autoLineSwitch를 이용해서 화면에 모든 노드들이 출력되는 함수를 만들어보자. 
	void showAllText(HDC hdc, size_m screenWidth, int firstLinePos, size_m wordHeight) {
		if (textNum == 0) //비었으면 그냥 리턴
			return;

		int totalLoopCnt = 0;
		
		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			totalLoopCnt += autoLineSwitch(hdc, (*itr), screenWidth, (firstLinePos + totalLoopCnt*wordHeight)*(-1), wordHeight);
		
		}

	}

	//showAllText : 이제 노드당 첫인덱스, 끝인덱스, 글자수 다 알아냄.
	void showAllText(HDC hdc, size_m screenWidth, int firstLinePos, size_m wordHeight, mScreenLineContainer& screenLineContainer) {
		screenLineContainer.setTextSourceNodeNum(textNum); //노드가 몇개인지 전달해준다. 

														   //필터
		if (textNum == 0) //비었으면 그냥 리턴
			return;

		//본론
		int totalLoopCnt = 0;
		int tempNodeIdx = 0; //노드 인덱스 검출 변수
		int tempLineNumInNode; //노드 라인 인덱스 갯수 검출 변수, 루프당 초기화 되어야함. 루프 하나가 노드 하나이기 때문

		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			tempLineNumInNode = 0;
			tempLineNumInNode += autoLineSwitch(hdc, (*itr), screenWidth, (firstLinePos + totalLoopCnt*wordHeight)*(-1), wordHeight, screenLineContainer, tempNodeIdx);
			totalLoopCnt += tempLineNumInNode;
			//printf("tempLineNum = %d \n", tempLineNumInNode);
			screenLineContainer.setNodeLineNumInfo(tempNodeIdx++, tempLineNumInNode); //노드당 몇 라인을 가지고 있는지 저장.
		}
		screenLineContainer.setTotalLineNum(totalLoopCnt); //텍스트 전체 라인이 몇개인지 저장. 
	}

	//showAllText2 : x좌표 시작점도 넣어주는 버젼:이제 노드당 첫인덱스, 끝인덱스, 글자수 다 알아냄.
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, size_m wordHeight, mScreenLineContainer& screenLineContainer) {
		screenLineContainer.setTextSourceNodeNum(textNum); //노드가 몇개인지 전달해준다. 

														   //필터
		if (textNum == 0) //비었으면 그냥 리턴
			return;

		//본론
		int totalLoopCnt = 0;
		int tempNodeIdx = 0; //노드 인덱스 검출 변수
		int tempLineNumInNode; //노드 라인 인덱스 갯수 검출 변수, 루프당 초기화 되어야함. 루프 하나가 노드 하나이기 때문

		for (text::iterator itr = (*textSource).begin(); itr != (*textSource).end(); itr++) {
			tempLineNumInNode = 0;
			tempLineNumInNode += autoLineSwitch(hdc, (*itr), screenWidth, firstLineXpos, (firstLineYpos + totalLoopCnt*wordHeight)*(-1), wordHeight, screenLineContainer, tempNodeIdx);
			totalLoopCnt += tempLineNumInNode;
			//printf("tempLineNum = %d \n", tempLineNumInNode);
			screenLineContainer.setNodeLineNumInfo(tempNodeIdx++, tempLineNumInNode); //노드당 몇 라인을 가지고 있는지 저장.
		}
		screenLineContainer.setTotalLineNum(totalLoopCnt); //텍스트 전체 라인이 몇개인지 저장. 
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