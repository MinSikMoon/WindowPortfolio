#pragma once
#include "mString.h"


class mScreenLineContainer {
private:
	//1. 노드 컨테이너/ 노드당 화면 라인당 글자갯수, 첫 인덱스, 끝인덱스 기록 가능 
	map<int, map<int, map<int, int> > > container; //노드**
	//2. 노드당 현재 화면에서 몇줄로 표시되는지 기록
	map<int, int> nodeLineNumInfo; //노드당 몇 라인을 가지고 있는지. ** 
	//3. 전체 텍스트는 몇 줄인가 **
	size_m totalLineNum;
	//4. 화면 위로 잘린 줄은 몇 줄인가------------------------------
	size_m upperLineNum;
	//5. 현재 화면에 보이는 줄 + 남은 줄 갯수---------------------------------
	size_m restLineNum; //total-upper
	//6. 화면상 보이는 첫 문장은 어떤 노드 소속인가---------------------------------
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	//7. 화면 마지막 문장----------------------------------------------
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;
	//8. 텍스트 소스가 몇 노드가지고 있는지 라인컨테이너도 가지자. **
	size_m textSourceNodeNum;

public:
	//1. 기본 생성자  생략
	mScreenLineContainer() {}

	//SETTERS & GETTERS
	//1. 전체 텍스트의 라인 수 
	void setTotalLineNum(size_m n) {
		totalLineNum = n;
	}
	size_m getTotalLineNum() {
		return totalLineNum;
	}

	//2. 노드당 몇 라인을 가지고 있는가
	void setNodeLineNumInfo(size_m nodeIdx, size_m lineNum) {
		nodeLineNumInfo[nodeIdx] = lineNum;
	}
	size_m getNodeLineNumInfo(size_m nodeIdx) {
		return nodeLineNumInfo[nodeIdx];
	}

	//3. 텍스트 소스의 노드 갯수를 우리도 알자. 
	void setTextSourceNodeNum(size_m n) {
		textSourceNodeNum = n;
	}
	size_m getTextSourceNodeNum() {
		return textSourceNodeNum;
	}

	//4. 라인컨테이너의 세터와 게터 // 0,1,7,8 //0노드의 1번 라인 인덱스는 7개 숫자를 가지고, 첫 문장의 문자열의 인덱스는 8이고 마지막 인덱스는 ~~다. 
	void setNodeLineData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
		container[nodeIdx][lineIdx][0] = lineWordCnt;
		container[nodeIdx][lineIdx][1] = lineFirstIdx;
		container[nodeIdx][lineIdx][2] = lineLastIdx;
	};
	
	//5.  몇 노드의 몇 인덱스 라인의 글자수를 구한다. 
	int getWordCnt(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][0];
	};

	//6. 첫 인덱스를 구한다. 
	int getFirstIdx(int nodeIdx, int lineIdx) {
		if (lineIdx == 0) {
			return 0;
		}
		return container[nodeIdx][lineIdx][1];
	};

	//7. 끝 인덱스를 구한다. 
	int getLastIdx(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][2];
	};

	//8. 해당 노드와 라인의 첫 '캐럿인덱스'를 구한다.
	size_m getFirstCaretIdx(int nodeIdx, int lineIdx) {
		return getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}

	//9. 해당 노드와 라인의 마지막 '캐럿인덱스'를 구한다. 
	size_m getLastCaretIdx(int nodeIdx, int lineIdx) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx); //문장의 첫 캐럿인덱스
		int tempWordCnt = getWordCnt(nodeIdx, lineIdx);
		return tempFirstIdx + tempWordCnt;
	}

	


	//debugging
	void show() {
		printf("============<lineContainer> 디버깅============= \n");
		printf("cur totalLineNum = %d \n", totalLineNum);
		for (int i = 0; i < textSourceNodeNum; i++) {
			printf("[%d]노드의 라인갯수는 %d \n", i, nodeLineNumInfo[i]);
		}

		for (int i = 0; i < textSourceNodeNum; i++) {
			for (int j = 0; j < nodeLineNumInfo[i]; j++) {
				printf("[%d]노드의 [%d]라인의 글자갯수, 첫인덱스, 끝인덱스 = %d,%d,%d \n", i, j, getWordCnt(i, j), getFirstIdx(i, j), getLastIdx(i, j));
			}
			
		}

		printf("===========================================\n \n");

	}

	
};