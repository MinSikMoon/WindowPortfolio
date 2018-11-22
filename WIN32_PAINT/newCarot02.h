#pragma once
#include "mString.h"

class newCarot {
private://=====================private ���
	size_m frontWordNum;
	size_m cNodeIdx;
	size_m cLineIdx;
	size_m cIdx; //cIdx�� ����Ű�̵��� �߿�, �Է��� ���� cIdx�� ������ �ȵȴ�. 

	int width;
	int height;
	size_m upperLineNum;
	//==========================private �Լ���

	//-. cIdx�� �� ��° ���ο� �ִ��� �˾Ƴ��� 
	size_m getLineIdxByCidx(size_m cIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(nodeIdx); //���� ��尡 ���ٷ� ����ϴ°� ���Ѵ�. 
															 // cIdx�� �� ��° ���ο� �����ϴ��� �˾Ƴ���. 
		for (int i = 0; i < curNodeLineNum; i++) {
			int startCidx = lc.getFirstCaretIdx(nodeIdx, i);
			int endCidx = lc.getLastCaretIdx(nodeIdx, i);
			if (startCidx <= cIdx && cIdx <= endCidx) {
				return i;
			}
		}

		return 0; //���ٵ� ������ 0 ĳ�� �ε��� 
	}
	//-. ���� cIdx�տ��� �� ���ڰ� �����ϴ��� �˾Ƴ��ִ� �Լ�//=================> cIdx�� frontWordNum�˾Ƴ���. 
	//cIdx�� frontWordCnt�� ������ �ִ°�, ������ �ϸ鼭 ���� cIdx�� ���� ������ �޶�����. ���� frontWordNum�� �� �߿��ϴ�. 
	size_m getFrontWordNumByCidx(size_m cIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		if (cIdx == 0) {
			return 0;
		}
		int curLineIdx = getLineIdxByCidx(cIdx, nodeIdx, lc);
		return cIdx - curLineIdx;
	} 
	//lc�� ����ִ� ���� ������ ������ ĳ���ε����� ���ΰ�?
	size_m getPhysicalLastCidx(size_m nodeIdx, mScreenLineContainer& lc) {
		int lastLineIdx = lc.getNodeLineNumInfo(nodeIdx) - 1;
		int lastRealIdx = lc.getLastIdx(nodeIdx,lastLineIdx);
		return lastRealIdx + lastLineIdx + 1;
	}
	size_m getPhysicalTotalFrontWordNum(size_m nodeIdx, mScreenLineContainer& lc) {
		int lastLineIdx = lc.getNodeLineNumInfo(nodeIdx) - 1;
		if (lc.getWordCnt(nodeIdx, lastLineIdx) == 0)
			return 0;

		return lc.getLastIdx(nodeIdx, lastLineIdx) + 1;
	}


	//1. ���� idx�� ���� �����ε��� �˾Ƴ���
	size_m getLineIdxInNode(size_m realIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(nodeIdx); //���� ��尡 ���ٷ� ����ϴ°� ���Ѵ�. 

		for (int i = 0; i < curNodeLineNum; i++) {
			int startIdx = lc.getFirstIdx(nodeIdx, i); //���� i��° ���� ���� ù��° �ε���
			int endIdx = lc.getLastIdx(nodeIdx, i); //'' ������ �ε���

			if (startIdx <= realIdx && realIdx <= endIdx) //�� ���̿� �� ������ i��° ���̴�. 
				return i;
		}

		return 0; //0�� ���� 0��° ���̴ϱ�.
	}
	
	//2. ������ ���� idx�� �ڿ� �ִ� ĳ���ε��� �˾Ƴ���
	size_m getCidxByRealIdx(size_m realIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curLineIdx = getLineIdxInNode(realIdx, nodeIdx, lc); //���� realIdx�� �Ҽӵ� �����ε��� 
		return realIdx + curLineIdx + 1; //realIdx�ڿ� �ִ� ĳ�� �ε��� 
	}

	//3. �� �ΰ��� ����ؼ� ������ frontWordNum���� �ڿ��ִ� ĳ���ε��� �������ֱ� 
	size_m getCidxByFrontWordNum(size_m FrontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		if (FrontWordNum == 0)
			return 0;

		int realIdx = FrontWordNum - 1; //���� �ε����� �������� �ϳ� �������. 
		return getCidxByRealIdx(realIdx, nodeIdx, lc);
	}

	//4. frontWordNum���� �����ε��� ���ϱ�
	size_m getLineIdxByFrontWordNum(size_m frontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		int realIdx = frontWordNum - 1;
		return getLineIdxInNode(realIdx, nodeIdx, lc);
	}
	//5. frontWordNum���� ���� ���� �ֳ� ���ϱ�
	size_m getUpperLineNumByFrontWordNum(size_m frontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		return getLineIdxByFrontWordNum(frontWordNum, nodeIdx, lc);
	}


	//6. ���� �� ĳ������ �����ֳ�? ��尡 �ƴ϶� textSource�ȿ��� -> yPixel���� �� ����
	size_m getTotalUpperLineNum(size_m nodeIdx, mScreenLineContainer& lc) {
		int sum = 0;
		printf("gettotalupperlinenum: ���� nodeIdx�� %d�� \n", nodeIdx);
		//���� �������� ��� �����Ѱ��� ���ϱ�
		for (int i = 0; i < nodeIdx; i++) {
			int tempLineNum = lc.getNodeLineNumInfo(i);
			if (tempLineNum == 0) {
				tempLineNum = 1;
			}
			//sum += lc.getNodeLineNumInfo(i);
			sum += tempLineNum;
		}

		return sum + cLineIdx; 
	}


	//1. line ����
	void setClineIdx(size_m newLineIdx) {
		cLineIdx = newLineIdx;
	}
	void setFrontWordNum(size_m newNum) {
		frontWordNum = newNum;
	}
	void setCidx(size_m newIdx) {
		cIdx = newIdx;
	}
	void setCnodeIdx(size_m newIdx) {
		cNodeIdx = newIdx;
	}

	void setUpperLineNum(size_m newNum) {
		upperLineNum = newNum;
	}

	//1. bool ������
	bool isLineFirst(mScreenLineContainer& lc) {
		size_m firstCidx = lc.getFirstCaretIdx(cNodeIdx, cLineIdx);
		if (cIdx == firstCidx) {
			return true;
		}
		return false;
	}










	//============================================================PUBLIC
public:
	//1. ������
	newCarot() {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
		upperLineNum = 0;

		int width = 2;
		int height = 16;
	}
	newCarot(int width, int height) {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
		upperLineNum = 0;

		this->width = width;
		this->height = height;
	}
	void make(int width, int height) {
		cIdx = 0;
		cNodeIdx = 0;
		cLineIdx = 0;
		frontWordNum = 0;
		upperLineNum = 0;

		this->width = width;
		this->height = height;
	}

	size_m getHeight() {
		return height;
	}
	//2. �̵��� �Է� ���� �������� �Ѵ�. 
	//-1. �Է½�  
	void input(mScreenLineContainer& lc) {
		
		int tempFrontWordNum = frontWordNum + 1; //�������� �ϳ� ���������ົ��.
		int physicalFrontWordNum = getPhysicalTotalFrontWordNum(cNodeIdx, lc);
		//1. ������ ���ڼ����� �� Ŀ���� �� ����
		if (tempFrontWordNum > physicalFrontWordNum) {
			frontWordNum = physicalFrontWordNum;
		}
		else {
			frontWordNum = tempFrontWordNum; //lc���� ���ڰ� �����ְ� ĳ���� ������ ���� ���ڴ�. lc�� �ƹ��� ������ ����ġ�ϱ� ������. //nodeIdx�� ���߿� ���� ����������Ѵ�. 
							//1. ���� frontWordNum�� �ѱ��ڸ� �߰����ش�. 
		}
				
		//2. ���ο� frontWordNum�������� cIdx�� lineIdx, upperLineNum�� �����ش�. 
		setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setUpperLineNum(getUpperLineNumByFrontWordNum(frontWordNum, cNodeIdx, lc));
	}

	

	//int backSpace(mScreenLineContainer& lc) {
	//	printf("ĳ�� backspace �ȿ����� lc��Ȳ: ���� frontwordnum: %d, cNodeIdx: %d \n ", frontWordNum, cNodeIdx);
	//	int isNodeReduced = 0;

	//	int tempFrontWordNum = frontWordNum - 1; //�������� �ѱ��� ���� �����ົ��. 
	//	if (tempFrontWordNum < 0) { // temp 0���� �۾����ٸ� 0��° ���� 0���� ����, �տ� ��尡 �� �ִٸ� �ճ���� ������ ���ڷ� ����. 
	//								//frontWordNum = 0;
	//		if (cNodeIdx == 0) {
	//			frontWordNum = 0;
	//		}
	//		else { //�տ� ��尡 �� �ִٸ�  //���� ĳ���� ���� ������ wordCnt�� 0�̸� �ش� ��嵵 ������� �Ѵ�. 
	//			cNodeIdx--; //�� ���� �̵� // 1�� �˷��ش�. 
	//			
	//			int tempLastLineIdx = lc.getNodeLineNumInfo(cNodeIdx) - 1; //�����ϱ� �ϳ����� �ε���
	//			printf("templastlineidx: %d \n", tempLastLineIdx);

	//			if (tempLastLineIdx < 0) { //
	//				tempLastLineIdx = 0;
	//				frontWordNum = 0;
	//			}
	//			else { 
	//				//int tempLastIdx = lc.getLastIdx(cNodeIdx, tempLastLineIdx);
	//				//printf("templastidx = %d \n");
	//				frontWordNum = lc.getLastIdx(cNodeIdx, tempLastLineIdx) + 1;
	//			}
	//			
	//			isNodeReduced = 1;
	//		}

	//	}
	//	else {
	//		frontWordNum = tempFrontWordNum; //�ѱ��� ������ ������ ���� ����.
	//	}

	//	//2. ���ο� frontWordNum�������� cIdx�� lineIdx, upperLineNum�� �����ش�. 
	//	setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
	//	setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));
	//	setUpperLineNum(getUpperLineNumByFrontWordNum(frontWordNum, cNodeIdx, lc));
	//	return isNodeReduced;

	//}
	int backSpace(mScreenLineContainer& lc) {
		printf("ĳ�� backspace �ȿ����� lc��Ȳ: ���� frontwordnum: %d, cNodeIdx: %d \n ", frontWordNum, cNodeIdx);
		int isNodeReduced = 0;

		int tempFrontWordNum = frontWordNum - 1; //�������� �ѱ��� ���� �����ົ��. 
		if (tempFrontWordNum < 0) { // temp 0���� �۾����ٸ� 0��° ���� 0���� ����, �տ� ��尡 �� �ִٸ� �ճ���� ������ ���ڷ� ����. 
									//frontWordNum = 0;
			if (cNodeIdx == 0) {
				frontWordNum = 0;
			}
			else { //�տ� ��尡 �� �ִٸ�  //���� ĳ���� ���� ������ wordCnt�� 0�̸� �ش� ��嵵 ������� �Ѵ�. 
				cNodeIdx--; //�� ���� �̵� // 1�� �˷��ش�. 

				int tempLastLineIdx = lc.getNodeLineNumInfo(cNodeIdx) - 1; //�����ϱ� �ϳ����� �ε��� //������� ������ ���� �ε�������
				printf("templastlineidx: %d \n", tempLastLineIdx);

				if (tempLastLineIdx < 0) { //����尡 ���ٹۿ� ���� �Ŷ�� 
					tempLastLineIdx = 0; //�����ε����� 0
					frontWordNum = lc.getWordCnt(cNodeIdx, 0);//frontWordNum = 0;? // ������� ������ ���� ī��Ʈ�� �Ǿ�� ����. 
				}
				else {
					frontWordNum = lc.getLastIdx(cNodeIdx, tempLastLineIdx) + 1;
				}

				isNodeReduced = 1;
			}

		}
		else {
			frontWordNum = tempFrontWordNum; //�ѱ��� ������ ������ ���� ����.
		}

		//2. ���ο� frontWordNum�������� cIdx�� lineIdx, upperLineNum�� �����ش�. 
		setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setUpperLineNum(getUpperLineNumByFrontWordNum(frontWordNum, cNodeIdx, lc));
		return isNodeReduced;

	}
	//-3 ����ó�� ===========�׽�Ʈ��
	void enter() {
		frontWordNum = 0; //
		cNodeIdx++; //���� ���� �Ѿ��. 
		cIdx = 0;
		cLineIdx = 0;
		upperLineNum =0; //���ȿ��� ���� 0���̶�� �ǹ� 
	}

	






	//-Ȩ
	//-����
	//-�����¿�

	//-��� //���⼭ ������ �����.
	size_m getCnodeIdx() {
		return cNodeIdx;
	}


size_m getXpixel(HDC hdc, mTextSource& textSource,mScreenLineContainer& lc) {
		
		int xPixel;
		if (isLineFirst(lc)) {
			xPixel = 0;
		}
		else {
			int tempStartIdx = lc.getFirstIdx(cNodeIdx, cLineIdx); //ù��° �ε����� �˾Ƴ���.

			
			xPixel = getMstrPixelWidth(hdc, mString(textSource.getTextAt(cNodeIdx)), tempStartIdx, frontWordNum-1); //���̸� �˾Ƴ���. 
		}
		return xPixel;
	}
	size_m getYpixel() {
		return height*upperLineNum;
	}

	size_m getYpixel(mScreenLineContainer& lc) {
		return height*getTotalUpperLineNum(cNodeIdx, lc);
	}




	//�����
	void show(mScreenLineContainer& lc) {
		printf(" ====> ���� ĳ���� cIdx: %d, nodeIdx: %d, cLine: %d, FrontWORDnUM: %d, upperlineNum: %d\n", cIdx, cNodeIdx, cLineIdx, frontWordNum, upperLineNum);
		//printf(" ====> ���� ������ �� ������ ������ ĳ�� �ε����� %d \n", getLastCaretIdxInNode(cNodeIdx, lc));
	}

};