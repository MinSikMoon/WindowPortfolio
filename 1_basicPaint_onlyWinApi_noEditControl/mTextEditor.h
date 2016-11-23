#pragma once
#include "mTextSource.h"
#include "mKeyboard.h"
#include "mCaretMaster.h"


class mTextEditor {
private:
	//�����
	size_m WORD_HEIGHT; //���� ����
	mKeyboard keyboard; //Ű����
	mTextSource textSource; //����
	mScreenLineContainer lineContainer; //�������� �����̳�
	mCaretMaster caret; //ĳ��
	size_m CARET_WIDTH; //ĳ���� �ʺ�


public:
	//1. ������
	mTextEditor() {}

	mTextEditor(size_m caretWidth, size_m wordHeight) :caret(caretWidth, wordHeight) {
		addText(keyboard.getMstr());
	}

	void make(size_m caretWidth, size_m wordHeight) {
		caret.make(caretWidth, wordHeight);
		addText(keyboard.getMstr());
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
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		keyboard.mProc(hwnd, Message, wParam, lParam);
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

	//4. �����ֱ� 
	void showAllText(HDC hdc, size_m screenWidth, int firstLineXpos, int firstLineYpos) {
		textSource.showAllText(hdc, screenWidth, firstLineXpos, firstLineYpos, 16, lineContainer);
		lineContainer.show();
	}
};