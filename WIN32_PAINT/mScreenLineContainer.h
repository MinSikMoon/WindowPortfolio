#pragma once
#include "mString.h"


class mScreenLineContainer {
private:
	//1. ��� �����̳�/ ���� ȭ�� ���δ� ���ڰ���, ù �ε���, ���ε��� ��� ���� 
	map<int, map<int, map<int, int> > > container; //���**
	//2. ���� ���� ȭ�鿡�� ���ٷ� ǥ�õǴ��� ���
	map<int, int> nodeLineNumInfo; //���� �� ������ ������ �ִ���. ** 
	//3. ��ü �ؽ�Ʈ�� �� ���ΰ� **
	size_m totalLineNum;
	//4. ȭ�� ���� �߸� ���� �� ���ΰ�------------------------------
	size_m upperLineNum;
	//5. ���� ȭ�鿡 ���̴� �� + ���� �� ����---------------------------------
	size_m restLineNum; //total-upper
	//6. ȭ��� ���̴� ù ������ � ��� �Ҽ��ΰ�---------------------------------
	size_m screenFirstNodeIdx;
	size_m screenFirstLineIdxInNode;
	//7. ȭ�� ������ ����----------------------------------------------
	size_m screenLastNodeIdx;
	size_m screenLastLineIdxInNode;
	//8. �ؽ�Ʈ �ҽ��� �� ��尡���� �ִ��� ���������̳ʵ� ������. **
	size_m textSourceNodeNum;

public:
	//1. �⺻ ������  ����
	mScreenLineContainer() {}

	//SETTERS & GETTERS
	//1. ��ü �ؽ�Ʈ�� ���� �� 
	void setTotalLineNum(size_m n) {
		totalLineNum = n;
	}
	size_m getTotalLineNum() {
		return totalLineNum;
	}

	//2. ���� �� ������ ������ �ִ°�
	void setNodeLineNumInfo(size_m nodeIdx, size_m lineNum) {
		nodeLineNumInfo[nodeIdx] = lineNum;
	}
	size_m getNodeLineNumInfo(size_m nodeIdx) {
		return nodeLineNumInfo[nodeIdx];
	}

	//3. �ؽ�Ʈ �ҽ��� ��� ������ �츮�� ����. 
	void setTextSourceNodeNum(size_m n) {
		textSourceNodeNum = n;
	}
	size_m getTextSourceNodeNum() {
		return textSourceNodeNum;
	}

	//4. ���������̳��� ���Ϳ� ���� // 0,1,7,8 //0����� 1�� ���� �ε����� 7�� ���ڸ� ������, ù ������ ���ڿ��� �ε����� 8�̰� ������ �ε����� ~~��. 
	void setNodeLineData(int nodeIdx, int lineIdx, int lineWordCnt, int lineFirstIdx, int lineLastIdx) {
		container[nodeIdx][lineIdx][0] = lineWordCnt;
		container[nodeIdx][lineIdx][1] = lineFirstIdx;
		container[nodeIdx][lineIdx][2] = lineLastIdx;
	};
	
	//5.  �� ����� �� �ε��� ������ ���ڼ��� ���Ѵ�. 
	int getWordCnt(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][0];
	};

	//6. ù �ε����� ���Ѵ�. 
	int getFirstIdx(int nodeIdx, int lineIdx) {
		if (lineIdx == 0) {
			return 0;
		}
		return container[nodeIdx][lineIdx][1];
	};

	//7. �� �ε����� ���Ѵ�. 
	int getLastIdx(int nodeIdx, int lineIdx) {
		return container[nodeIdx][lineIdx][2];
	};

	//8. �ش� ���� ������ ù 'ĳ���ε���'�� ���Ѵ�.
	size_m getFirstCaretIdx(int nodeIdx, int lineIdx) {
		return getFirstIdx(nodeIdx, lineIdx) + lineIdx;
	}

	//9. �ش� ���� ������ ������ 'ĳ���ε���'�� ���Ѵ�. 
	size_m getLastCaretIdx(int nodeIdx, int lineIdx) {
		int tempFirstIdx = getFirstCaretIdx(nodeIdx, lineIdx); //������ ù ĳ���ε���
		int tempWordCnt = getWordCnt(nodeIdx, lineIdx);
		return tempFirstIdx + tempWordCnt;
	}

	


	//debugging
	void show() {
		printf("============<lineContainer> �����============= \n");
		printf("cur totalLineNum = %d \n", totalLineNum);
		for (int i = 0; i < textSourceNodeNum; i++) {
			printf("[%d]����� ���ΰ����� %d \n", i, nodeLineNumInfo[i]);
		}

		for (int i = 0; i < textSourceNodeNum; i++) {
			for (int j = 0; j < nodeLineNumInfo[i]; j++) {
				printf("[%d]����� [%d]������ ���ڰ���, ù�ε���, ���ε��� = %d,%d,%d \n", i, j, getWordCnt(i, j), getFirstIdx(i, j), getLastIdx(i, j));
			}
			
		}

		printf("===========================================\n \n");

	}

	
};