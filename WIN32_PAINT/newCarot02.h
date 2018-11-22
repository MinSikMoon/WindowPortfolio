#pragma once
#include "mString.h"

class newCarot {
private://=====================private 멤버
	size_m frontWordNum;
	size_m cNodeIdx;
	size_m cLineIdx;
	size_m cIdx; //cIdx는 방향키이동때 중요, 입력할 때는 cIdx를 믿으면 안된다. 

	int width;
	int height;
	size_m upperLineNum;
	//==========================private 함수들

	//-. cIdx가 몇 번째 라인에 있는지 알아내기 
	size_m getLineIdxByCidx(size_m cIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(nodeIdx); //현재 노드가 몇줄로 출력하는가 구한다. 
															 // cIdx가 몇 번째 라인에 존재하는지 알아낸다. 
		for (int i = 0; i < curNodeLineNum; i++) {
			int startCidx = lc.getFirstCaretIdx(nodeIdx, i);
			int endCidx = lc.getLastCaretIdx(nodeIdx, i);
			if (startCidx <= cIdx && cIdx <= endCidx) {
				return i;
			}
		}

		return 0; //한줄도 없으면 0 캐럿 인덱스 
	}
	//-. 기존 cIdx앞에는 몇 글자가 존재하는지 알아내주는 함수//=================> cIdx로 frontWordNum알아낸다. 
	//cIdx로 frontWordCnt가 문제가 있는게, 개행을 하면서 같은 cIdx라도 글자 갯수가 달라진다. 따라서 frontWordNum이 더 중요하다. 
	size_m getFrontWordNumByCidx(size_m cIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		if (cIdx == 0) {
			return 0;
		}
		int curLineIdx = getLineIdxByCidx(cIdx, nodeIdx, lc);
		return cIdx - curLineIdx;
	} 
	//lc에 들어있는 실제 물리적 마지막 캐럿인덱스는 몇인가?
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


	//1. 실제 idx로 실제 라인인덱스 알아내기
	size_m getLineIdxInNode(size_m realIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curNodeLineNum = lc.getNodeLineNumInfo(nodeIdx); //현재 노드가 몇줄로 출력하는가 구한다. 

		for (int i = 0; i < curNodeLineNum; i++) {
			int startIdx = lc.getFirstIdx(nodeIdx, i); //현재 i번째 줄의 실제 첫번째 인덱스
			int endIdx = lc.getLastIdx(nodeIdx, i); //'' 마지막 인덱스

			if (startIdx <= realIdx && realIdx <= endIdx) //둘 사이에 들어가 있으면 i번째 줄이다. 
				return i;
		}

		return 0; //0일 경우는 0번째 줄이니까.
	}
	
	//2. 글자의 실제 idx로 뒤에 있는 캐럿인덱스 알아내기
	size_m getCidxByRealIdx(size_m realIdx, size_m nodeIdx, mScreenLineContainer& lc) {
		int curLineIdx = getLineIdxInNode(realIdx, nodeIdx, lc); //현재 realIdx가 소속된 라인인덱스 
		return realIdx + curLineIdx + 1; //realIdx뒤에 있는 캐럿 인덱스 
	}

	//3. 위 두개를 사용해서 이제는 frontWordNum으로 뒤에있는 캐럿인덱스 조사해주기 
	size_m getCidxByFrontWordNum(size_m FrontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		if (FrontWordNum == 0)
			return 0;

		int realIdx = FrontWordNum - 1; //실제 인덱스는 갯수에서 하나 빼줘야지. 
		return getCidxByRealIdx(realIdx, nodeIdx, lc);
	}

	//4. frontWordNum으로 라인인덱스 구하기
	size_m getLineIdxByFrontWordNum(size_m frontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		int realIdx = frontWordNum - 1;
		return getLineIdxInNode(realIdx, nodeIdx, lc);
	}
	//5. frontWordNum으로 위로 몇줄 있나 구하기
	size_m getUpperLineNumByFrontWordNum(size_m frontWordNum, size_m nodeIdx, mScreenLineContainer& lc) {
		return getLineIdxByFrontWordNum(frontWordNum, nodeIdx, lc);
	}


	//6. 현재 내 캐럿위로 몇줄있나? 노드가 아니라 textSource안에서 -> yPixel구할 때 쓰임
	size_m getTotalUpperLineNum(size_m nodeIdx, mScreenLineContainer& lc) {
		int sum = 0;
		printf("gettotalupperlinenum: 들어온 nodeIdx는 %d개 \n", nodeIdx);
		//이전 노드까지의 모든 라인총갯수 더하기
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


	//1. line 세팅
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

	//1. bool 종류들
	bool isLineFirst(mScreenLineContainer& lc) {
		size_m firstCidx = lc.getFirstCaretIdx(cNodeIdx, cLineIdx);
		if (cIdx == firstCidx) {
			return true;
		}
		return false;
	}










	//============================================================PUBLIC
public:
	//1. 생성자
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
	//2. 이동과 입력 따로 만들어줘야 한다. 
	//-1. 입력시  
	void input(mScreenLineContainer& lc) {
		
		int tempFrontWordNum = frontWordNum + 1; //가상으로 하나 증가시켜줘본값.
		int physicalFrontWordNum = getPhysicalTotalFrontWordNum(cNodeIdx, lc);
		//1. 물리적 글자수보다 더 커지는 걸 방지
		if (tempFrontWordNum > physicalFrontWordNum) {
			frontWordNum = physicalFrontWordNum;
		}
		else {
			frontWordNum = tempFrontWordNum; //lc에는 글자가 들어와있고 캐럿은 가상의 글자 숫자다. lc에 아무런 영향을 못끼치니까 괜찮다. //nodeIdx도 나중에 먼저 갱신해줘야한다. 
							//1. 기존 frontWordNum에 한글자를 추가해준다. 
		}
				
		//2. 새로운 frontWordNum기준으로 cIdx와 lineIdx, upperLineNum을 정해준다. 
		setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setUpperLineNum(getUpperLineNumByFrontWordNum(frontWordNum, cNodeIdx, lc));
	}

	

	//int backSpace(mScreenLineContainer& lc) {
	//	printf("캐럿 backspace 안에서의 lc상황: 현재 frontwordnum: %d, cNodeIdx: %d \n ", frontWordNum, cNodeIdx);
	//	int isNodeReduced = 0;

	//	int tempFrontWordNum = frontWordNum - 1; //가상으로 한글자 감소 시켜줘본다. 
	//	if (tempFrontWordNum < 0) { // temp 0보다 작아진다면 0번째 노드면 0으로 고정, 앞에 노드가 더 있다면 앞노드의 마지막 글자로 가자. 
	//								//frontWordNum = 0;
	//		if (cNodeIdx == 0) {
	//			frontWordNum = 0;
	//		}
	//		else { //앞에 노드가 더 있다면  //현재 캐럿이 속한 라인의 wordCnt가 0이면 해당 노드도 없애줘야 한다. 
	//			cNodeIdx--; //전 노드로 이동 // 1로 알려준다. 
	//			
	//			int tempLastLineIdx = lc.getNodeLineNumInfo(cNodeIdx) - 1; //갯수니까 하나빼서 인덱스
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
	//		frontWordNum = tempFrontWordNum; //한글자 감소한 값으로 새로 장착.
	//	}

	//	//2. 새로운 frontWordNum기준으로 cIdx와 lineIdx, upperLineNum을 정해준다. 
	//	setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
	//	setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));
	//	setUpperLineNum(getUpperLineNumByFrontWordNum(frontWordNum, cNodeIdx, lc));
	//	return isNodeReduced;

	//}
	int backSpace(mScreenLineContainer& lc) {
		printf("캐럿 backspace 안에서의 lc상황: 현재 frontwordnum: %d, cNodeIdx: %d \n ", frontWordNum, cNodeIdx);
		int isNodeReduced = 0;

		int tempFrontWordNum = frontWordNum - 1; //가상으로 한글자 감소 시켜줘본다. 
		if (tempFrontWordNum < 0) { // temp 0보다 작아진다면 0번째 노드면 0으로 고정, 앞에 노드가 더 있다면 앞노드의 마지막 글자로 가자. 
									//frontWordNum = 0;
			if (cNodeIdx == 0) {
				frontWordNum = 0;
			}
			else { //앞에 노드가 더 있다면  //현재 캐럿이 속한 라인의 wordCnt가 0이면 해당 노드도 없애줘야 한다. 
				cNodeIdx--; //전 노드로 이동 // 1로 알려준다. 

				int tempLastLineIdx = lc.getNodeLineNumInfo(cNodeIdx) - 1; //갯수니까 하나빼서 인덱스 //전노드의 마지막 라인 인덱스검출
				printf("templastlineidx: %d \n", tempLastLineIdx);

				if (tempLastLineIdx < 0) { //전노드가 한줄밖에 없는 거라면 
					tempLastLineIdx = 0; //라인인덱스는 0
					frontWordNum = lc.getWordCnt(cNodeIdx, 0);//frontWordNum = 0;? // 전노드의 마지막 워드 카운트가 되어야 겠지. 
				}
				else {
					frontWordNum = lc.getLastIdx(cNodeIdx, tempLastLineIdx) + 1;
				}

				isNodeReduced = 1;
			}

		}
		else {
			frontWordNum = tempFrontWordNum; //한글자 감소한 값으로 새로 장착.
		}

		//2. 새로운 frontWordNum기준으로 cIdx와 lineIdx, upperLineNum을 정해준다. 
		setClineIdx(getLineIdxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setCidx(getCidxByFrontWordNum(frontWordNum, cNodeIdx, lc));
		setUpperLineNum(getUpperLineNumByFrontWordNum(frontWordNum, cNodeIdx, lc));
		return isNodeReduced;

	}
	//-3 엔터처리 ===========테스트중
	void enter() {
		frontWordNum = 0; //
		cNodeIdx++; //다음 노드로 넘어간다. 
		cIdx = 0;
		cLineIdx = 0;
		upperLineNum =0; //노드안에서 위로 0줄이라는 의미 
	}

	






	//-홈
	//-엔드
	//-상하좌우

	//-출력 //여기서 문제가 생긴다.
	size_m getCnodeIdx() {
		return cNodeIdx;
	}


size_m getXpixel(HDC hdc, mTextSource& textSource,mScreenLineContainer& lc) {
		
		int xPixel;
		if (isLineFirst(lc)) {
			xPixel = 0;
		}
		else {
			int tempStartIdx = lc.getFirstIdx(cNodeIdx, cLineIdx); //첫번째 인덱스를 알아낸다.

			
			xPixel = getMstrPixelWidth(hdc, mString(textSource.getTextAt(cNodeIdx)), tempStartIdx, frontWordNum-1); //길이를 알아낸다. 
		}
		return xPixel;
	}
	size_m getYpixel() {
		return height*upperLineNum;
	}

	size_m getYpixel(mScreenLineContainer& lc) {
		return height*getTotalUpperLineNum(cNodeIdx, lc);
	}




	//디버깅
	void show(mScreenLineContainer& lc) {
		printf(" ====> 현재 캐롯의 cIdx: %d, nodeIdx: %d, cLine: %d, FrontWORDnUM: %d, upperlineNum: %d\n", cIdx, cNodeIdx, cLineIdx, frontWordNum, upperLineNum);
		//printf(" ====> 실제 물리적 이 문단의 마지막 캐럿 인덱스는 %d \n", getLastCaretIdxInNode(cNodeIdx, lc));
	}

};