#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "locale.h"
#include <windows.h>
#include <tchar.h>
#include "resource3.h"
///////////////////////////////////////////////////////////////
#include "mOriginPoint.h"

#include "mMouse.h"

#include "mCircle.h"
#include "mRectangle.h"
#include "mLine.h"

#include "mShapeContainer.h"

#include "mOptions.h"
#include "mXmlMethods.h"
#import <msxml6.dll>
//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif

using namespace std;

//전역 변수들
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 600;

HINSTANCE g_Inst;
int g_focusedIdx = -1;
int g_orderFlag = -1;
int g_resizePoint;
//global 객체들
mMouse g_mouse;
mOriginPoint ORIGIN_POINT;
mShapeContainer g_msc;
mOptions g_options;


//////////////////////////////////////////////WIN PROC/////////////////////////////////////////////////////////////////////////////////////
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;

	OPENFILENAME OFN;
	TCHAR fileAddr[300];
	TCHAR lpstrFile[MAX_PATH] = TEXT("");
	HANDLE hFile;
	DWORD dwWritten;

	switch (Message) {
	case WM_CREATE: {
		ORIGIN_POINT.setStartPoint(g_mouse, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5 * 2);
		g_options.setFontName(TEXT("궁서"));
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case ID_ERASE_ALL: {
			if (g_msc.getShapeNum() != 0) {
				g_msc.deleteAll();
				g_focusedIdx = -1;
				g_orderFlag = -1;
				InvalidateRect(hwnd, NULL, TRUE);
			}
			break;
		}
		case ID_NEW_FILE: {
			printf("들어옴 ");
			if (g_msc.getShapeNum() != 0) {
				MessageBox(hwnd, TEXT("현재 작성중인 파일이 있습니다. 저장부터 하세요. "), TEXT("현재 파일 저장하기"), MB_OK);
				SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, NULL);
				g_msc.deleteAll();
				g_focusedIdx = -1;
				g_orderFlag = -1;
			}
			SendMessage(hwnd, WM_LBUTTONDOWN, NULL, NULL);
			SendMessage(hwnd, WM_LBUTTONUP, NULL, NULL);
			InvalidateRect(hwnd, NULL, TRUE);
			break;

		}
		case ID_40001: { //원
			g_orderFlag = Flag::CIRCLE;
			break;
		}
		case ID_40002: { //사각
			g_orderFlag = Flag::RECTANGLE;
			break;
		}
		case ID_40003: { //선분
			g_orderFlag = Flag::LINE;
			break;
		}
		case ID_40004: { //폰트
			g_options.setFontName(TEXT("궁서"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40005: { //폰트
			g_options.setFontName(TEXT("바탕"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40006: { //폰트
			g_options.setFontName(TEXT("굴림"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40008: { //폰트
			g_options.setFontName(TEXT("돋움"));
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_40007: { //원점
			ORIGIN_POINT.goToZero(g_mouse, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 5 * 2);
			g_options.setZoomLevel(1.0);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_FILE_OPEN: { //불러오기
			if (g_msc.getShapeNum() != 0) {
				MessageBox(hwnd, TEXT("현재 작성중인 파일이 있습니다. 저장부터 하세요. "), TEXT("현재 파일 저장하기"), MB_OK);
				SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, NULL);
				g_msc.deleteAll();
				g_focusedIdx = -1;
				g_orderFlag = -1;


			}
			memset(&OFN, 0, sizeof(OPENFILENAME)); //OFN 초기화
			OFN.lStructSize = sizeof(OPENFILENAME); //구조체 크기 저장
			OFN.hwndOwner = hwnd; //대화상자의 부모윈도우
			OFN.lpstrFilter = TEXT("텍스트 문서(.txt) \0*.txt\0 xml 문서(.xml) \0*.xml\0 모든 파일(*.*)\0*.*\0");
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = MAX_PATH;

			if (GetOpenFileName(&OFN) != 0) { //제대로 파일주소가 선택되었다면
				wsprintf(fileAddr, TEXT("%ls"), OFN.lpstrFile);
				//MessageBox(hwnd, TEXT("파일 열기 성공 "), TEXT("파일 열기 성공 "), MB_OK); //fileAddr에 주소가 담겨있다. 
			}
			else {
				break;
			}

			//1. 파일 주소 검사 
			//1. fileAddr을 분석한다. 
			mString addrMstr;
			addrMstr.replaceStr(cloneStr(fileAddr));
			TCHAR* fileType = addrMstr.subFromToEnd(addrMstr.getLength() - 4);
			wprintf(L"%ls \n", fileType);
			wprintf(L"%d", _tcscmp(TEXT(".xml"), fileType));

			if (_tcscmp(TEXT(".xml"), fileType) == 0) { //xml일때 

				mXmlMethods().openXml(fileAddr, g_msc, g_options, ORIGIN_POINT);


			}
			else {
				wprintf(L"%ls \n", fileAddr);
				hFile = CreateFile(fileAddr, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				TCHAR buffer[100000];
				if (hFile != INVALID_HANDLE_VALUE) {
					ReadFile(hFile, buffer, 100000, &dwWritten, NULL);
					CloseHandle(hFile); //파일은 다 썼으니 닫는다. 
					wprintf(L"%ls \n", buffer); //buffer에 텍스트가 담겨진다. 
				}

				mString tempMstr;
				TCHAR* oneLine;
				TCHAR* context;
				TCHAR* enterToken = TEXT("\r\n");
				TCHAR* tabToken = TEXT("\t");

				//1. 줌 읽기 : 첫째줄을 엔터기준으로 자르고, 첫번째 유니코드 표시를 없애준다. 
				oneLine = _tcstok_s(buffer, enterToken, &context);
				tempMstr.replaceStr(oneLine); //넣고 첫째 유니코드 마크를 지워준다. 
				tempMstr.eraseFirstChar();
				wprintf(L"%ls \n", tempMstr.getStr()); //tempMstr에 한줄이 담겨져 있다. 
				double zoomDouble = _wtof(tempMstr.getStr()); //줌레벨 저장됨.//tempmstr.getstr()에는 현재 tchar 문자열이 저장되어 있으므로 싱글 바이트로 바꿔준다 
				g_options.setZoomLevel(zoomDouble); //이 값으로 줌 레벨 저장 	//wprintf(L"%.2f \n", testDouble);

				int shapeType;
				int ulX;
				int ulY;
				int drX;
				int drY;
				TCHAR* content;



				while (1) {
					oneLine = _tcstok_s(context, enterToken, &context);// 줌레벨 떼고 남는게 context니까 이제는 context를 토크나이징 한다. 
																	   //wprintf(L"%ls \n", oneLine); //tempMstr에 한줄이 담겨져 있다. 

					if (oneLine == NULL)
						break;

					TCHAR* tabContext;
					//1. shapeTYPE 저장 		
					shapeType = _wtoi(_tcstok_s(oneLine, tabToken, &tabContext));
					//2. 좌상단, 우하단
					ulX = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
					ulY = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
					drX = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
					drY = _wtoi(_tcstok_s(tabContext, tabToken, &tabContext));
					content = _tcstok_s(tabContext, tabToken, &tabContext);


					if (shapeType == Flag::CIRCLE) {
						printf("원, %d %d %d %d \n", ulX, ulY, drX, drY);
						g_msc.add(new mCircle(ulX, ulY, drX, drY));
						wprintf(L"content담는중 %ls \n", content); //tempMstr에 한줄이 담겨져 있다. 
						if (content != NULL) {
							g_msc.replaceTextAt(g_msc.getShapeNum() - 1, content);
						}


					}
					else if (shapeType == Flag::RECTANGLE) {
						printf("사각형, %d %d %d %d \n", ulX, ulY, drX, drY);
						g_msc.add(new mRectangle(ulX, ulY, drX, drY));
						wprintf(L"content담는중 %ls \n", content); //tempMstr에 한줄이 담겨져 있다. 
						if (content != NULL) {
							g_msc.replaceTextAt(g_msc.getShapeNum() - 1, content);
						}
					}
					else if (shapeType == Flag::LINE)
					{
						printf("선, %d %d %d %d \n", ulX, ulY, drX, drY);
						g_msc.add(new mLine(ulX, ulY, drX, drY));

					}

				}
			}
			SendMessage(hwnd, WM_LBUTTONDOWN, NULL, NULL);
			SendMessage(hwnd, WM_LBUTTONUP, NULL, NULL);
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}
		case ID_FILE_SAVE: {
			//if(xml)
			mString saveContents;

			//txt 스타일 저장 
			memset(&OFN, 0, sizeof(OPENFILENAME)); //OFN 초기화
			OFN.lStructSize = sizeof(OPENFILENAME); //구조체 크기 저장
			OFN.hwndOwner = hwnd; //대화상자의 부모윈도우
			OFN.lpstrFilter = TEXT("텍스트 문서(.txt) \0*.txt\0 xml 문서(.xml) \0*.xml\0 모든 파일(*.*)\0*.*\0");
			OFN.lpstrFile = lpstrFile;
			OFN.nMaxFile = MAX_PATH;

			if (GetSaveFileName(&OFN) != 0) { //제대로 파일주소가 선택되었다면
				wsprintf(fileAddr, TEXT("%ls"), OFN.lpstrFile);
				MessageBox(hwnd, TEXT("저장되었습니다."), TEXT("저장되었습니다."), MB_OK); //fileAddr에 주소가 담겨있다. 
			}
			else {
				break;
			}

			//1. fileAddr을 분석한다. 
			mString addrMstr;
			addrMstr.replaceStr(cloneStr(fileAddr));
			TCHAR* fileType = addrMstr.subFromToEnd(addrMstr.getLength() - 4);
			wprintf(L"%ls \n", fileType);
			wprintf(L"%d", _tcscmp(TEXT(".xml"), fileType));

			if (_tcscmp(TEXT(".xml"), fileType) == 0) { //xml일때 
				if (g_msc.getShapeNum() != 0) {
					//xml Style
					saveContents.add(mXmlMethods().makeXml(fileAddr, g_msc, g_options, ORIGIN_POINT));
					//unsigned short unicodeMark = 0xFEFF;

					//hFile = CreateFile(fileAddr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					//WriteFile(hFile, &unicodeMark, sizeof(unicodeMark), &dwWritten, NULL); //이걸 표시해 주면 텍스트 파일이 유니코드로 인식이 되어 한글 인식이 된다. 
					//WriteFile(hFile, saveContents.getStr(), (saveContents.getLength() + 1) * sizeof(TCHAR), &dwWritten, NULL);

					//CloseHandle(hFile);
				}
			}
			else { // xml이 아닌 그 외일때 
				if (g_msc.getShapeNum() != 0) {
					//txt를 쓸 빈 mString을 만들자. 
					mString saveContents;
					TCHAR tempOneLine[100]; //이 한줄한줄을 saveContents에 add 시켜서 txt로 뽑자. 

											//줌 레벨 입력
					swprintf_s(tempOneLine, TEXT("%.2lf\r\n"), g_options.getZoomLevel());
					saveContents.add(tempOneLine); //한줄씩 추가

					for (int i = 0; i < g_msc.getShapeNum(); i++) {
						//printf("현재 좌상단: %d \n", g_msc.getUpLeftAt(i).x);
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getNameAt(i));
						saveContents.add(tempOneLine); //한줄씩 추가
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getUpLeftAt(i).x);
						saveContents.add(tempOneLine); //한줄씩 추가
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getUpLeftAt(i).y);
						saveContents.add(tempOneLine); //한줄씩 추가
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getDownRightAt(i).x);
						saveContents.add(tempOneLine); //한줄씩 추가
						wsprintf(tempOneLine, TEXT("%d\t"), g_msc.getDownRightAt(i).y);
						saveContents.add(tempOneLine); //한줄씩 추가
						saveContents.add(g_msc.getText(i)); //텍스트도 추가
						saveContents.add(TEXT("\r\n"));
					}
					wprintf(L"현재 mstr: %ls \n", saveContents.getStr());
					//txt로 만들 내용물이 생겼으니 저장만하자. 
					unsigned short unicodeMark = 0xFEFF;

					hFile = CreateFile(fileAddr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					WriteFile(hFile, &unicodeMark, sizeof(unicodeMark), &dwWritten, NULL); //이걸 표시해 주면 텍스트 파일이 유니코드로 인식이 되어 한글 인식이 된다. 
					WriteFile(hFile, saveContents.getStr(), (saveContents.getLength() + 1) * sizeof(TCHAR), &dwWritten, NULL);
					CloseHandle(hFile);

				}
			}
			break;
		}
		}


		break;
	}
					 //키보드 메시지
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_COMPOSITION:
	case WM_CHAR: {
		if (!g_msc.isEmpty())
			g_msc.procAt(hwnd, Message, wParam, lParam, g_focusedIdx);


		break;
	}



	case WM_MOUSEWHEEL: {
		if ((short)HIWORD(wParam) > 0) {
			g_options.zoomIn();
		}
		else if ((short)HIWORD(wParam) < 0) {
			g_options.zoomOut();
		}
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}



	case WM_MOUSEMOVE: {
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel()); //물리-> 논리//printf("마우스 현재 relative x,y = %d, %d \n", g_mouse.getRelativeNewPos().x, g_mouse.getRelativeNewPos().y);

		if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx) && g_focusedIdx != -1) {
			SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZEALL));
		}
		else {
			SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
		}

		if (g_mouse.getGrapped()) {
			//. 화면 풀링 로직
			if (g_mouse.getPulled()) {
				ORIGIN_POINT.move(g_mouse.getZoomXdist(g_options.getZoomLevel()), g_mouse.getZoomYdist(g_options.getZoomLevel())); //논리 -> 물리  
				g_mouse.pullingZoomAction(g_options.getZoomLevel());
				g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel()); //물리 -> 논리

			}
			if (g_orderFlag == Flag::RESIZE) { //논리 좌표들을 변경 
				g_msc.moveResizeAction(g_focusedIdx, g_resizePoint, g_mouse);
			}

			//. 도형이동 로직
			if (g_orderFlag == Flag::MOVE) {
				g_msc.moveAt(g_focusedIdx, g_mouse.getXdist(), g_mouse.getYdist());
				g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel()); //물리 -> 논리 
			}
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			if (g_focusedIdx != -1) { //리사이즈 조절 점 장착 
				g_resizePoint = g_msc.isClosedAt(g_focusedIdx, g_mouse.getRelativeNewPos());

				if (g_resizePoint == 1 || g_resizePoint == 3) { //좌상단만 변해준다.  
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZENWSE));
				}
				else if (g_resizePoint == 2 || g_resizePoint == 4) {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZENESW));
				}
				else if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_SIZEALL));
				}
				else {
					SetClassLongPtr(hwnd, GCLP_HCURSOR, (LONG_PTR)LoadCursor(NULL, IDC_ARROW));
				}


			}
		}

		break;
	}
	case WM_RBUTTONDOWN: {
		g_mouse.setPulled(true);
		g_mouse.setGrap(true);
		g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		break;
	}
	case WM_RBUTTONUP: {
		g_mouse.setPulled(false);
		g_mouse.setGrap(false);
		break;
	}


	case WM_LBUTTONDOWN: {
		//1. 1배일 때는 100,100이면 2배일 때는 50,50이 찍혀야 되니까 zoomLevel을 나눠서 넣어줘야 된다. 
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		g_mouse.setGrap(true);

		printf("relative new x,y : %d, %d, relative old = %d, %d \n", g_mouse.getRelativeNewX(), g_mouse.getRelativeNewY(), g_mouse.getRelativeOldX(), g_mouse.getRelativeOldY()); //잘 됨. 
		if (g_focusedIdx != -1) { //도형이 선택되어 있다면..
			if (g_resizePoint != -1) { //리사이즈 시작한다는 의미
				g_mouse.setTemPos1(g_msc.getUpLeftAt(g_focusedIdx));
				g_mouse.setTemPos2(g_msc.getDownRightAt(g_focusedIdx));
				g_orderFlag = Flag::RESIZE;
			}
			else {
				if (g_focusedIdx == g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx)) {
					g_orderFlag = Flag::MOVE; //움직이라는 명령 장착 
				}
			}


		}

		//. 도형 선택 로직
		if (g_orderFlag == Flag::NOTHING) {
			g_focusedIdx = g_msc.whoIsIn(g_mouse.getRelativeNewPos(), g_focusedIdx); //printf("돌고난후 focused: %d \n", g_focusedIdx); //어느 도형을 찍었는지 판별해준다 //선택 잘 됨.
																					 //printf("현재 마우스 안에 있는 도형: %d \n", g_focusedIdx);
		}

		g_mouse.setOldZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		break;
	}

	case WM_LBUTTONUP: {
		g_mouse.setGrap(false);
		g_mouse.setNewZoomPos(LOWORD(lParam), HIWORD(lParam), g_options.getZoomLevel());
		g_focusedIdx = g_msc.buMakeShapeAction(g_orderFlag, g_focusedIdx, g_mouse);

		g_orderFlag = Flag::NOTHING; //선택하기 위해서 
		InvalidateRect(hwnd, NULL, TRUE);
		break;
	}

	case WM_PAINT: {
		hdc = BeginPaint(hwnd, &ps);

		if (g_mouse.getGrapped()) {
			g_msc.paintShowZoomProgressAction(hdc, g_orderFlag, g_mouse, ORIGIN_POINT.getOriginPoint(), g_options.getZoomLevel());
		}
		g_options.setting(hdc);
		g_msc.showAllExcept_Zoom(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint(), g_options.getZoomLevel(), g_options.getFontSize());
		g_msc.showAt_zoom(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint(), g_options.getZoomLevel(), g_options.getFontSize());
		g_msc.showDotAt_relative(hdc, g_focusedIdx, ORIGIN_POINT.getOriginPoint());

		ORIGIN_POINT.show(hdc);
		g_options.settingOlds(hdc);
		EndPaint(hwnd, &ps);
		break;
	}


				   //======================마지막엔 콘솔 없애주기======================== 
	case WM_DESTROY: {

		PostQuitMessage(0);
		break;
	}

					 /* All other messages (a lot of them) are processed using default procedures */
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}




































//-------------------------------------------------WIN MAIN-------------------------------------------------------------------
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	_wsetlocale(LC_ALL, _T("korean"));
	g_Inst = hInstance;
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

			 /* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = _T("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, _T("WindowClass"), _T("Caption"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		WINDOW_WIDTH, /* width */
		WINDOW_HEIGHT, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/*
	This is the heart of our program where all input is processed and
	sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
	this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}