
#pragma once
#include "mTextPixel.h"


class mKeyboard {
private:
	mString mStr; //���� ȭ�鿡 ������� ���ڿ�
	TCHAR tempStr[2]; //�������� �ѱ��� ���� ��
	TCHAR tempCompleteStr[2]; //�ϼ��� ���� ���� ��
	bool isCompleted; //�ϼ��� ���ڰ� �ִ���. //completeStr�� ���ڰ� ����ִ���. 
	bool curComposingStat;
	bool prevComposingStat; //���� ���� ime��� ����, 

	HIMC hImc;

	bool isProced;
	bool isBacked;

public:
	//1. ������
	mKeyboard() {
		printf("mk1 �ʱ�ȭ \n");
		printf("mk1 mstr ���ڼ� %d \n", mStr.getLength());
		isCompleted = FALSE;
		curComposingStat = FALSE; //���� �ܰ��� IME ������� 
		prevComposingStat = FALSE; //���� �ܰ��� IME ������� 
		isBacked = FALSE;
	}
	//1.2 �ı���
	

	//�޼ҵ� 1. ime endcomposition:
	void endComposition() {
		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		memset(tempStr, 0, 2); //tempStr�� �ƹ��͵� ���� �ʴ´�. 
	}

	//�޼ҵ� 2. ime composition:
	void composition(HIMC hImc, HWND hwnd, LPARAM lParam) {
		hImc = ImmGetContext(hwnd);
		memset(tempCompleteStr, 0, 2);

		curComposingStat = TRUE; //���� IME�� ��������� �˸�.

		if (lParam & GCS_COMPSTR) { //�������� ���ڿ� �޾ƿ���.
			ImmGetCompositionString(hImc, GCS_COMPSTR, tempStr, 2);
			tempStr[1] = 0;
		}

		if (lParam & GCS_RESULTSTR) { //�ϼ��� ���ڿ��� �ִٸ� �޾ƿ��� 
			ImmGetCompositionString(hImc, GCS_RESULTSTR, tempCompleteStr, 2);
			tempStr[1] = 0;
			isCompleted = TRUE; //���� �ϼ��� ���ڰ� ����Ǿ������� �˷���.
		}

		ImmReleaseContext(hwnd, hImc);
	}

	//�޼ҵ� 3. char
	void makeChar(HWND hwnd, WPARAM wParam) {
		if (!prevComposingStat) { //���� ���� ime�� ������� �ʾҴµ� //�׳� �ڿ� ���ٿ��ִ� ��.

			if (curComposingStat) { //����� ime�� ������� ��, tempStr�� �ٿ��ش�. 

				mStr.add(tempStr); //tempStr�� �����ְ� invalidate
			}
			else { //���絵 ime�� ������� �ʾҴٸ� wParam�� �ٿ��ش�. 

				mStr.add((TCHAR)wParam);
			}
		}
		else { //���� ���� �ѱ� �Է� �̾��ٸ� 

			if (curComposingStat) { //���� ���� �ѱ��Է�, ���ݵ� �ѱ��Է� => �ϼ��� ���ڰ� �ִ��� �Ǵ� 
				if (isCompleted) { //�ϼ� ���ڰ� �ִٸ� 

								   //�ѿ� ��ȯŰ�� ��ŵ�Ѵ�. 
					_tprintf(L"3���� �ѿ���ȯŰ : %ls \n", tempCompleteStr);
					mStr.eraseLastChar(); //���� ���� ����� 
					mStr.add(tempCompleteStr); //�ϼ��� ���� �ְ�
					isCompleted = FALSE; //�ϼ��Ȱ� ������.
					mStr.add(tempStr); //�������� ���ڵ� �ְ� 

				}
				else { //���� ���� �ѱ� �Է�, ���ݵ� �ѱ��Է� => �ϼ� ���ڴ� ���ٸ� 

					mStr.eraseLastChar();
					mStr.add(tempStr); //�������� ���ڸ� �ִ´�. 
				}
			}
			else { //���� ���� �ѱ� �Է��ε�, ������ �ƴҶ� 
				if (isCompleted) { //�ϼ� ���ڰ� �ִٸ� 

					mStr.eraseLastChar(); //���� ���� ����� 
					mStr.add(tempCompleteStr); //�ϼ��� ���� �ְ�
					isCompleted = FALSE; //�ϼ��Ȱ� ������.
					mStr.add((TCHAR)wParam);
				}
				else { //�ϼ� ���� ������ �������� �� �־������.

					mStr.eraseLastChar(); //�ѿ�Ű�� ������ �̰͵� ���� ������ 
										  //mStr.add(tempStr); //�������� ���� �ְ�
					mStr.add((TCHAR)wParam);
				}
			}
		}

		prevComposingStat = curComposingStat;
		curComposingStat = FALSE;
		//printf("char ��\n");
		InvalidateRect(hwnd, NULL, TRUE);
	}
	//testing
	void mProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
		switch (Message) {
	
		case WM_IME_ENDCOMPOSITION: { //�ѱ� �Է��ϴٰ� �ѿ�Ű �������� tempStr�� ����ش�. 
			if (isBacked) {
				SendMessage(hwnd, WM_CHAR, VK_BACK, VK_BACK);
				isBacked = FALSE;
				break;
			}
			//printf("is backed ==> %d \n", isBacked == TRUE ? 1 : 0);
			printf("endcomposition \n");
			endComposition();
			break;
		}
		case WM_IME_COMPOSITION: { //IME ��� 
			printf("compsitioning \n");
			hImc = ImmGetContext(hwnd);
			composition(hImc, hwnd, lParam);
			ImmReleaseContext(hwnd, hImc);
		}

		case WM_CHAR: { //mStr�� ���ڸ� �����ִ� ����
			switch (wParam) {
			case VK_BACK: {
				printf("back �����̽� ���� isback true���� \n");
				isBacked = TRUE;
				mStr.eraseLastChar();
				curComposingStat = FALSE;
				prevComposingStat = curComposingStat;
				InvalidateRect(hwnd, NULL, TRUE);
				return;
			}
			}
			printf("char \n");
			makeChar(hwnd, wParam);
			break;
		}
		}
	}

	//3. GETTER
	int getMstrLength() {
		return mStr.getLength();
	}

	mString& getMstr() {
		return mStr;
	}


};