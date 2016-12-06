#pragma once
#include "mHeaders.h"


typedef int size_m;


//1. 일반 전역 함수들
//1. TCHAR에 대한 sub
TCHAR* subFromTo(TCHAR* str, size_m startIdx, size_m endIdx) {
	
	int length = _tcslen(str);
	if (startIdx < 0 || endIdx >(length - 1) || startIdx > endIdx) { //필터 for 잘못된 입력
		printf("!!!!!!!!!********>>>>잘못된 범위 지정 in subFromTo() : sIdx: %d, eIdx: %d \n", startIdx, endIdx);
		system("pause");
		exit(-1); //에러
	}
	size_m tempLength = endIdx - startIdx + 1; //단순히 잘려진 문자의 갯수일 뿐, 인덱스와 헷갈리지 말자.  
	size_m bufferSize = (length + 1) * sizeof(TCHAR); //버퍼는 nul 공간까지 더해서 생성해주자. //주의 사항 str보다 무조건 같거나 커야함.
													  //printf("디버깅중: tempLength는 %d, buffersize는 %d \n", tempLength, bufferSize);
	TCHAR* tempStr = new TCHAR[bufferSize]; //임시 문자열 공간

	_tcscpy_s(tempStr, bufferSize, str + startIdx);
	tempStr[tempLength] = 0; //nul 값 넣어주기.

	return tempStr;
}

//2. MBCS 기반 문자열을 DBCS 기반 문자열로 변환
wchar_t* toWC(const char *str_param) {
	size_t convertedChars = 0;
	size_t str_param_len = strlen(str_param) + 1;

	wchar_t *wc = new wchar_t[str_param_len];
	setlocale(LC_ALL, "");
	mbstowcs_s(&convertedChars, wc, str_param_len, str_param, _TRUNCATE);

	return wc;
}

char* toAnsi(TCHAR* _inStr) {
	size_t convertedChars = 0;
	size_t strLen = _tcslen(_inStr)+1;

	char* ansi = new char[strLen];
	setlocale(LC_ALL, "Korean");
	wcstombs_s(&convertedChars,ansi, strLen, _inStr, strLen);
	ansi[strLen] = 0;
	return ansi;
}

//3. const tchar 문자열을 그냥 tchar로 복제
TCHAR* cloneStr(const TCHAR* _inStr) {
	int bufferSize = (_tcslen(_inStr) + 1) * sizeof(TCHAR);
	TCHAR* tempStr = new TCHAR[bufferSize];
	_tcscpy_s(tempStr, bufferSize, _inStr);
	return tempStr;
}

//////////////////////////////////////////// mString 클래스 ///////////////////////////////////////////////////////
class mString {
	//MEMBERS : private
	TCHAR* str;
	size_m length;

public:
	mString() { //빈 문자열을 기본 생성
		size_m tempLength = 1; //바로 null을 저장하기 위해 0의 위치에 
		size_m bufferSize = (tempLength + 1 ) * sizeof(TCHAR); // 유니코드를 대비해 2배로 잡아준다. //tempLength에 1더해줌
		str = new TCHAR[bufferSize];
		_tcscpy_s(str, bufferSize, TEXT(""));
		length = 0;
	}

	//1. CONSTRUCTOR : const TCHAR가 들어왔을 때// 복사해서 붙여준다. //즉 깊은 복사가 일어난다. 
	//1.1 문자열이 들어왔을 때
	mString(const TCHAR* _inStr) {
		size_m tempLength = _tcslen(_inStr); // 길이 저장
		size_m bufferSize = (tempLength + 1) * sizeof(TCHAR); // 유니코드를 대비해 2배로 잡아준다. 
		str = new TCHAR[bufferSize]; //str에 공간할당

		_tcscpy_s(str, bufferSize, _inStr); //str에 문자열을 복사한다. 
		length = tempLength; //길이도 저장한다. 
	}

	//1.2 문자가 들어왔을 때
	mString(TCHAR _inChar) {
		size_m bufferSize = 2 * sizeof(TCHAR); //2로 잡는다. 
		str = new TCHAR[bufferSize]; //str에 공간할당

		str[0] = _inChar;
		str[1] = 0; //nul을 넣어준다. 
		length = 1; //한글자 들어왔으니 길이는 1
	}

	//2. DESTRUCTOR
	~mString() {
		delete str;
	}


	//--------------METHODS---------------------
	//------------------------------ < 1. ADD : 오버로딩들 > ------------------------------------------
	//1.1. mString끼리 더하기
	void add(mString& _inStr) {
		
		size_m tempLength = length + _inStr.getLength(); //일단 길이를 임시 계산
		size_m bufferSize = (tempLength + 1) * sizeof(TCHAR); //두개를 합친 임시 공간 사이즈
		TCHAR* tempStr = new TCHAR[bufferSize]; //임시 공간을 만든다. 

		_tcscpy_s(tempStr, bufferSize, str); //가상 공간에 str을 복사
		_tcscat_s(tempStr, tempLength + 1, _inStr.getStr()); //뒤에다가 붙인다. 
		
		if (_tcslen(str) != 0) { //str이 null만 들어있으면 오류가 나더라. 
			delete str; //현재 str을 해제시키고 
		}
		//delete str;

		str = tempStr; //tempStr로 갱신시킨다.
		length = tempLength; //길이도 갱신 시킨다. 
	}

	//1.2 일반 TCHAR문자열 더하기
	void add(const TCHAR* _inStr) {
		mString tempStr(_inStr); //임시 mString 하나 만들고
		(*this).add(tempStr);// 붙인다. 
	}

	//1.3 문자 더하기
	void add(TCHAR _inChar) {
		mString tempStr(_inChar); //임시 mString을 만들고
		(*this).add(tempStr); //붙인다. 
	}

	//------------------------------------ <2. subMstr : 문자열 잘라서 새로운 mString 배출 > ------------------------------
	//2.1 from인덱스부터 to인덱스까지 잘라서(인덱스 포함) 새로운 문자열 리턴.
	TCHAR* subFromTo(size_m startIdx, size_m endIdx) {
		
		if (startIdx < 0 || endIdx >(length - 1) || startIdx > endIdx) { //필터 for 잘못된 입력
			printf("!!!!!!!!!********>>>>잘못된 범위 지정 in subFromTo() : sIdx: %d, eIdx: %d \n", startIdx, endIdx);
			system("pause");
			exit(-1); //에러
		}

		size_m tempLength = endIdx - startIdx + 1; //단순히 잘려진 문자의 갯수일 뿐, 인덱스와 헷갈리지 말자.  
		size_m bufferSize = (length + 1) * sizeof(TCHAR); //버퍼는 nul 공간까지 더해서 생성해주자. //주의 사항 str보다 무조건 같거나 커야함.
														  //printf("디버깅중: tempLength는 %d, buffersize는 %d \n", tempLength, bufferSize);
		TCHAR* tempStr = new TCHAR[bufferSize]; //임시 문자열 공간

		_tcscpy_s(tempStr, bufferSize, str + startIdx);
		tempStr[tempLength] = 0; //nul 값 넣어주기.

		return tempStr;
	}

	//2.2 from인덱스부터 끝까지 자르기
	TCHAR* subFromToEnd(size_m startIdx) {
		return subFromTo(startIdx, length - 1);
	} //startIdx가 1이고 length도 1이면 에러



	  //------------------------------------ <3. insertInto : 문자열의 특정 위치에 새로운 문자열을 삽입> ---------------------------------
	  //3.1 특정 인덱스에 새로운 문자열을 삽입해서 str을 새로 갱신한다. 
	void insertInto(size_m targetIdx, const TCHAR* _inStr) {
		if (targetIdx < 0 || targetIdx >(length - 1)) {
			printf("!!!!!!==============> 잘못된 범위 지정 in insertInto()/ targetIdx : %d \n", targetIdx);
			system("pause");
			exit(-1);
		}

		size_m tempLength = length + _tcslen(_inStr); //두 문장 길이를 더한 것이 새로운 길이겟지. 

													  //3.1.1 처음에 갖다 붙이려 할때
		if (targetIdx == 0) {

			mString tempStr(_inStr); //_inStr로 임시 mString을 만들고 여기에 역으로 나의 str을 붙여줄 것.
			tempStr.add(str); //붙여줌. 나의 str이 이제 tempStr의 str이 되어야 함. 
			delete str;
			str = tempStr.cloneStr(); //원래 문자열 해제하고 새 문자열로 교체
		}//3.1.2 마지막에 갖다 붙이려 할때// 이거는 그냥 add랑 같다. 
		else if (targetIdx == (length - 1)) {
			add(_inStr);
		}//3.1.3 사이 어딘가에 갖다 붙이는 케이스 
		else {
			TCHAR* temp1 = subFromTo(0, targetIdx - 1); //맨 앞단 문자열 분리
			TCHAR* temp3 = subFromToEnd(targetIdx); //맨 뒷단 문자열 분리
			mString temp2(temp1); //mString으로 만들고
			temp2.add(_inStr); //_inStr 갖다 붙이기
			temp2.add(temp3); //이제 temp2의 str은 우리가 원하는 결과를 가지게 되었다. 

			delete str;
			str = temp2.cloneStr();//str 갱신
			delete temp1;
			delete temp3; //자원 해제
		}
		length = tempLength;
	}

	//3.2 insertInto : mString이 인자로 들어올 경우
	void insertInto(size_m targetIdx, mString& _inMstr) {
		if (targetIdx < 0 || targetIdx >(length - 1)) {
			printf("!!!!!!==============> 잘못된 범위 지정 in insertInto()/ targetIdx : %d \n", targetIdx);
			system("pause");
			exit(-1);
		}
		TCHAR* _inStr = _inMstr.cloneStr(); //자원 해제 해주자.
		size_m tempLength = length + _tcslen(_inStr); //두 문장 길이를 더한 것이 새로운 길이겟지. 

													  //3.1.1 처음에 갖다 붙이려 할때
		if (targetIdx == 0) {

			mString tempStr(_inStr); //_inStr로 임시 mString을 만들고 여기에 역으로 나의 str을 붙여줄 것.
			tempStr.add(str); //붙여줌. 나의 str이 이제 tempStr의 str이 되어야 함. 
			delete str;
			str = tempStr.cloneStr(); //원래 문자열 해제하고 새 문자열로 교체
		}//3.1.2 마지막에 갖다 붙이려 할때// 이거는 그냥 add랑 같다. 
		else if (targetIdx == (length - 1)) {
			add(_inStr);
		}//3.1.3 사이 어딘가에 갖다 붙이는 케이스 
		else {
			TCHAR* temp1 = subFromTo(0, targetIdx - 1); //맨 앞단 문자열 분리
			TCHAR* temp3 = subFromToEnd(targetIdx); //맨 뒷단 문자열 분리
			mString temp2(temp1); //mString으로 만들고
			temp2.add(_inStr); //_inStr 갖다 붙이기
			temp2.add(temp3); //이제 temp2의 str은 우리가 원하는 결과를 가지게 되었다. 

			delete str;
			str = temp2.cloneStr();//str 갱신
			delete temp1;
			delete temp3; //자원 해제
		}
		delete _inStr;
		length = tempLength;
	}

	//-------------------------------------- <4. cloneStr : Str을 복제해서 문자열로 리턴해줌.> -----------------
	TCHAR* cloneStr() {
		int bufferSize = (length + 1) * sizeof(TCHAR);
		TCHAR* tempStr = new TCHAR[bufferSize]; //버퍼 사이즈만큼 힙공간 생성
		_tcscpy_s(tempStr, bufferSize, str);
		return tempStr;
	}


	//-------------------------------------- <5. replaceStr : str을 다른 문자열로 교체해준다. > -------------------
	void replaceStr(TCHAR* _inStr) {
		TCHAR* cleaner = str;

			//delete str; //str 해제
		
		str = _inStr; //str 교체
		//delete cleaner; //delete를 안해주면 어떻게 될까. delete때문에 문제가 계속 생긴다. 
		
		if(_inStr != NULL)
		length = _tcslen(_inStr); //길이도 교체
	}
	

	//------------------------------------- <6. eraseCharAt, eraseStrFromTo : 어디부터 어디까지의 인덱스 범위를 삭제한다. > ----------------
	void eraseCharAt(size_m targetIdx) {
		if (targetIdx < 0 || targetIdx >(length - 1) || length == 0) {
			printf("!!!!!!==============> 잘못된 범위 지정 in eraseCharAt()/ targetIdx : %d \n", targetIdx);
			system("pause");
			exit(-1);
		}

		if (targetIdx == 0) {

			TCHAR* tempStr = (*this).subFromToEnd(1);
			(*this).replaceStr(tempStr);
		}
		else if (targetIdx == length - 1) {
			TCHAR* tempStr = (*this).subFromTo(0, length - 2);
			(*this).replaceStr(tempStr);
		}
		else {
			TCHAR* tempStr1 = (*this).subFromTo(0, targetIdx - 1);
			TCHAR* tempStr2 = (*this).subFromTo(targetIdx + 1, length - 1);
			(*this).replaceStr(tempStr1);
			(*this).add(tempStr2);
		}
	}

	void eraseLastChar() {
		if (length == 0) {
			//그냥 빈문자열로 갱신
			/*mString tempStr;
			this->replaceStr(tempStr.cloneStr());
			length = 0;*/
			return; //여기서 안 빠져 나가서 오류 났었음. 
					/*printf("!!!!!!==============> 잘못된 범위 지정 in eraseLastChar()/ 현재길이가 0 \n");
					system("pause");
					exit(-1);*/
		}
		if (length == 1) { //한 글자 들어있으면 그냥 빈 문자열 만들어 준다. 
			mString tempStr;
			(*this).replaceStr(tempStr.cloneStr());
			length = 0;
			return; //여기서 안 빠져 나가서 오류 났었음. 
		}

		eraseCharAt(length - 1);
	}
	void eraseFirstChar() {
		if (length == 0) {
			printf("!!!!!!==============> 잘못된 범위 지정 in eraseFirstChar()/ 현재길이가 0 \n");
			system("pause");
			exit(-1);
		}
		eraseCharAt(0);
	}

	
	//-------------GETTER----------------------
	//1. str포인터 리턴
	TCHAR* getStr() {
		return str;
	}
	//2. 길이 리턴
	size_m getLength() {
		return length;
	}

	//debugging : show
	void show() {
		printf("\n ** < mString 정보 > ** \n");
		_tprintf(TEXT("%ls\n"), str);
		printf("문자열 길이는 length : %d \n", length);

	}

};