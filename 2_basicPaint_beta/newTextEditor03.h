#pragma once

#include "mTextSource.h"
#include "mKeyboard.h"
#include "newCarot02.h"


class mTextEditor {
private:
	//�����
	//size_m WORD_HEIGHT; //���� ����
	mKeyboard keyboard; //Ű����
	mTextSource textSource; //����
	mScreenLineContainer lineContainer; //�������� �����̳�
	newCarot caret; //ĳ��
	
	
	//������ : �⺻�� 0,0 �̰���. 
	int startX;
	int startY;

public:
	//1. ������
	mTextEditor(): startX(0), startY(0) {}

	mTextEditor(size_m caretWidth, size_m wordHeight) :caret(caretWidth, wordHeight), startX(0), startY(0) {
		addText(keyboard.getMstr());
	}

	void make(size_m caretWidth, size_m wordHeight) {
		startX = 0;
		startY = 0;
		caret.make(caretWidth, wordHeight);
		addText(keyboard.getMstr());
	}

	size_m getWordHeight() {
		return caret.getHeight();
	}
	//�޼ҵ�
	//1. �ؽ�Ʈ �ϳ� �߰�(�����ϳ� �߰�) => ��� �ϳ��� �߰��Ǵ� ��
	void addText(TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(const TCHAR* _inStr) {
		textSource.addText(_inStr);
	}
	void addText(mString& mStr) {
		textSource.addText(mStr);
	}
	//2. Ű���� proc
	/*void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		keyboard.mProc(hwnd, Message, wParam, lParam);
	}*/
	int mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		return keyboard.mProc(hwnd, Message, wParam, lParam);
	}
	void replaceKeyboard(TCHAR* _inStr) {
		keyboard.replace(_inStr);
	}

	//3. ���� ���� ��ü
	void replaceTextAt(size_m nodeIdx, TCHAR* _newStr) {
		textSource.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceTextAt(size_m nodeIdx, const TCHAR* _newStr) {
		textSource.replaceTextAt(nodeIdx, _newStr);
	}
	void replaceTextAt(size_m nodeIdx, mString& _newStr) {
		textSource.replaceTextAt(nodeIdx, _newStr);
	}

	void replaceCurText(size_m nodeIdx) {
		textSource.replaceTextAt(nodeIdx, keyboard.getMstr());
	}

	void replaceCurText() {
		//wprintf(L"%d��带 %ls�� replace \n", caret.getCnodeIdx(), keyboard.getMstr());
		textSource.replaceTextAt(caret.getCnodeIdx(), keyboard.getMstr());
	}
	void eraseCurText() {

		textSource.eraseTextAt(caret.getCnodeIdx());
	}
	

	//4. �����ֱ� //�����ָ鼭 lc�� ��� �������� �������. 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		int wordWidth = 16;

		if (screenWidth < wordWidth)
			return; //���۵� ���� 

		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, wordWidth, lineContainer);
		//textSource.textShowByLineContainer(hdc, screenWidth, firstLineXpos, firstLineYpos, wordWidth, lineContainer);
		//lineContainer.show();
	}
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, double wordWidth) {
		if (screenWidth < wordWidth)
			return; //���۵� ���� 

		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, wordWidth, lineContainer);
		//lineContainer.show();
	}

	void showAllZoomText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, double zoomLevel) {
		int wordWidth = 16;

		if (screenWidth < 20)
			return; //���۵� ���� 

		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, wordWidth, lineContainer);
		//textsource.showAllZoomText()
	}
	void showAllZoomText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos, double zoomLevel, int wordWidth) {
		if (screenWidth < wordWidth)
			return; //���۵� ���� 

		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, wordWidth, lineContainer);
		//textsource.showAllZoomText()
	}
	//4.5 lineContainer�� ����ִ� ��� ������ ���� ����. wordHeight�� ���ؼ� ���� => ���� �ؽ�Ʈ�� ��ü ���̴� ���ΰ��� ���Ѵ�. 
	size_m getLcTotalLineNum() {
		return lineContainer.getTotalLineNum();
	}
	size_m getTextHeight() {
		printf("���� textEditor03, ���μ��� %d, wordheights: %d \n", lineContainer.getTotalLineNum(), getWordHeight());
		return getLcTotalLineNum()*getWordHeight();
	}

	//5. ���� �»�� ��ǥ �ٲٱ� 
	void changeStartPoint(int x, int y) {
		startX = x;
		startY = y;
	}
	int getStartX() {
		return startX;
	}
	int getStartY() {
		return startY;
	}
	//�Է°���
	//Ű���� ���ڿ��� �������ֱ� 
	void keyboardClean() {
		keyboard.Clean();
	}

	//ĳ������
	void caretInput() {
		caret.input(lineContainer);
	}

	void caretBackSpace() {
		int isNodeReduced = caret.backSpace(lineContainer); 
		//1. ĳ����尡 �ϳ� �پ����ٸ�
		//Ű������ mstr�� �� ����� ������ �������� ��ü�Ǿ�� �Ѵ�. 
		if (isNodeReduced) { //��尡 �پ��鼭 ���ÿ� ���ο� ���ڼ��� 0�̶�� 
			//_tprintf(TEXT("��尡 �ϳ� �پ���. %ls \n"), textSource.getTextAt(caret.getCnodeIdx()));
			keyboard.setMstr(textSource.cloneTextAt(caret.getCnodeIdx()));
		}
		
		
	}

	void caretEnter() {
		caret.enter();
	}

	size_m getCaretXpixel(HDC hdc) {
		return caret.getXpixel(hdc, textSource, lineContainer) + startX;
	}
	size_m getCaretYpixel() {
		return caret.getYpixel(lineContainer) + startY;
	}
	size_m getCaretNode() {
		return caret.getCnodeIdx();
	}

	void getCarotInfo() {
		caret.show(lineContainer);
	}

	mString& getKeyboardStr(){
		return keyboard.getMstr();
	}

	//textsource�� ���� text������
	TCHAR* getTextAt(int idx) {
		return textSource.getTextAt(idx);
	}

	//��尡 ������ add���ִ°�
	
};