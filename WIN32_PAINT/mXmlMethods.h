#pragma once
#import <msxml6.dll>
#include "mShapeContainer.h"
#include "mOptions.h"
#include "mOriginPoint.h"
#include "mCircle.h"
#include "mRectangle.h"
#include "mLine.h"

class mXmlMethods {
	BSTR tempStr, tempStr2;
	BSTR bstrInst1, bstrInst2;
	//TCHAR test[100] = L"version=\"1.0\" encoding = \"UTF-8\" ";
	BSTR tempStr3;
	IXMLDOMDocumentPtr xmlDocument;
	IXMLDOMProcessingInstructionPtr xmlProcessingInstruction;
	IXMLDOMElementPtr elem_file;
	IXMLDOMElementPtr	elem_zoomLevel, elem_nowScreenXY, elem_shapeList;
	IXMLDOMElementPtr	elem_shape;
	IXMLDOMElementPtr elem_class, elem_p1, elem_p2, elem_txt;
	_variant_t variant;

public:
	//1. constructor
	mXmlMethods() {
		bstrInst1 = ::SysAllocString(TEXT("xml"));
		bstrInst2 = ::SysAllocString(TEXT("version=\"1.0\" encoding = \"UTF-8\" "));

	}

	//2. method : makeXml
	BSTR makeXml(TCHAR* fileAddr, mShapeContainer& msc, mOptions& mOption, mOriginPoint& mOgPoint) {
		if (msc.getShapeNum() == 0)
			return NULL;

		if (SUCCEEDED(::CoInitialize(NULL))) {
			{
				//. 에러 필터
				if (!SUCCEEDED(xmlDocument.CreateInstance(__uuidof(DOMDocument)))) {
					::wprintf(L"xmlDocument가 생성되지 않았습니다.\n");
					::CoUninitialize();
					return NULL;
				}


				//1. xml Instruction을 만든다. 
				xmlDocument->createProcessingInstruction(bstrInst1, bstrInst2, &xmlProcessingInstruction); //요소를 만들고
				xmlDocument->appendChild(xmlProcessingInstruction, NULL); //붙인다. 

																		  //2. file element 만들고 붙이기
				tempStr = SysAllocString(TEXT("file"));
				xmlDocument->createElement(tempStr, &elem_file); //bstr1을 elem_file로 만들고   
				xmlDocument->appendChild(elem_file, NULL); // xmlElement1을 xmlDocument에 붙인다.
				SysFreeString(tempStr);

				//3. zoomLevel 붙이기 : file밑에 있다. 
				tempStr = SysAllocString(TEXT("zoomLevel"));
				xmlDocument->createElement(tempStr, &elem_zoomLevel); //bstr1을 elem_file로 만들고   
				_bstr_t zoomLevelStr = mOption.getZoomLevel(); //zoomLevel을 string으로 저장 
				elem_zoomLevel->put_text(zoomLevelStr); //줌 레벨 사이에 넣는다. 
				elem_file->appendChild(elem_zoomLevel, NULL); // xmlElement1을 xmlDocument에 붙인다.
				SysFreeString(tempStr);

				//4. nowScreenXy 붙이기 
				tempStr = SysAllocString(TEXT("nowScreenXY"));
				xmlDocument->createElement(tempStr, &elem_nowScreenXY); //element만들기
				_bstr_t nowScreenX = mOgPoint.getOriginPoint().x;
				_bstr_t nowScreenY = mOgPoint.getOriginPoint().y;

				tempStr3 = SysAllocString(TEXT(""));
				wsprintf(tempStr3, TEXT("%d %d"), mOgPoint.getOriginPoint().x, mOgPoint.getOriginPoint().y);
				elem_nowScreenXY->put_text((BSTR)tempStr3);

				elem_file->appendChild(elem_nowScreenXY, NULL); // 붙이기
				SysFreeString(tempStr);
				SysFreeString(tempStr3);

				//SysFreeString(tempStr2);

				//5. shapeList 붙이기 //이제 shapeList밑으로 shape들을 반복적으로 만들어 붙인다. 
				tempStr = SysAllocString(TEXT("shapeList"));
				xmlDocument->createElement(tempStr, &elem_shapeList); //bstr1을 elem_file로 만들고   
				elem_file->appendChild(elem_shapeList, NULL); // xmlElement1을 xmlDocument에 붙인다.
				SysFreeString(tempStr);

				//6. shape들 붙이기 
				for (int i = 0; i < msc.getShapeNum(); i++) {
					//1. shape만들기
					tempStr = SysAllocString(TEXT("shape"));
					xmlDocument->createElement(tempStr, &elem_shape); //bstr1을 elem_file로 만들고 //shapeList밑에 붙인다. 
					elem_shapeList->appendChild(elem_shape, NULL); // xmlElement1을 xmlDocument에 붙인다.
					SysFreeString(tempStr);

					//6.1 class 만들기 
					tempStr = SysAllocString(TEXT("class"));
					xmlDocument->createElement(tempStr, &elem_class);

					if (msc.getNameAt(i) == Flag::CIRCLE) {
						tempStr2 = SysAllocString(TEXT("Circle"));
					}
					else if (msc.getNameAt(i) == Flag::RECTANGLE) {
						tempStr2 = SysAllocString(TEXT("Rect"));
					}
					else {
						tempStr2 = SysAllocString(TEXT("Line"));
					}
					elem_class->put_text(tempStr2); //도형 모양 넣고
					elem_shape->appendChild(elem_class, NULL); //shape밑에 class 붙이기 
					SysFreeString(tempStr);
					SysFreeString(tempStr2);

					//6.2 p1만들기
					tempStr = SysAllocString(TEXT("p1"));
					xmlDocument->createElement(tempStr, &elem_p1);

					tempStr3 = SysAllocString(TEXT(""));
					wsprintf(tempStr3, TEXT("%d %d"), msc.getUpLeftAt(i).x, msc.getUpLeftAt(i).y);
					elem_nowScreenXY->put_text((BSTR)tempStr3);


					elem_p1->put_text(tempStr3);


					elem_shape->appendChild(elem_p1, NULL); //shape밑에 class 붙이기 
					SysFreeString(tempStr);
					SysFreeString(tempStr3);

					//6.3 p2만들기
					tempStr = SysAllocString(TEXT("p2"));
					xmlDocument->createElement(tempStr, &elem_p2);

					tempStr3 = SysAllocString(TEXT(""));
					wsprintf(tempStr3, TEXT("%d %d"), msc.getDownRightAt(i).x, msc.getDownRightAt(i).y);
					elem_nowScreenXY->put_text((BSTR)tempStr3);


					elem_p2->put_text(tempStr3);

					elem_shape->appendChild(elem_p2, NULL); //shape밑에 class 붙이기 
					SysFreeString(tempStr);
					SysFreeString(tempStr3);

					//6.4 text만들기
					tempStr = SysAllocString(TEXT("txt"));
					xmlDocument->createElement(tempStr, &elem_txt);

					tempStr2 = SysAllocString(msc.getText(i));
					elem_txt->put_text(tempStr2);

					elem_shape->appendChild(elem_txt, NULL); //shape밑에 class 붙이기 
					SysFreeString(tempStr);
					SysFreeString(tempStr2);


				}

				//디버깅
				xmlDocument->get_xml(&tempStr);
				variant = fileAddr;
				xmlDocument->save(variant);
			}

			::CoUninitialize();

		}

		return tempStr;
	}

	void openXml(TCHAR* fileAddr, mShapeContainer& msc, mOptions& mOption, mOriginPoint& mOgPoint) {
		if (SUCCEEDED(::CoInitialize(NULL))) {
			{
				_variant_t variant;
				BSTR tempBstr1;
				IXMLDOMDocumentPtr xmlDocument;
				IXMLDOMNodeListPtr xmlNodeList1 = NULL;
				IXMLDOMNodePtr pNode = NULL;
				IXMLDOMNodePtr parentNode = NULL;
				IXMLDOMNodePtr childNode = NULL;
				long listLength;
				TCHAR* spaceToken = TEXT(" ");

				VARIANT_BOOL variantBool;

				if (!SUCCEEDED(xmlDocument.CreateInstance(__uuidof(DOMDocument)))) {
					::wprintf(L"xmlDocument가 생성되지 않았습니다.\n");
					::CoUninitialize();
					return;
				}

				//1. 파일열기 
				variant = fileAddr;
				xmlDocument->load(variant, &variantBool); //xmlDocument를 load한다. 

														  //1. zoomLevel을 얻는다. //1 얻어옴 
				xmlDocument->selectSingleNode(_bstr_t("/file/zoomLevel"), &pNode);
				pNode->get_text(&tempBstr1); //tempBstr1을 숫자로 바꿔야 한다. 
				double zoomDouble = _wtof(tempBstr1);
				mOption.setZoomLevel(zoomDouble); //줌레벨 저장 
				wprintf(L"%.2lf \n", zoomDouble);

				//2. now screen xy
				xmlDocument->selectSingleNode(_bstr_t("/file/nowScreenXY"), &pNode);
				pNode->get_text(&tempBstr1); //tempBstr1을 두 개로 분리시켜야 한다. 
				TCHAR* oneData;
				TCHAR* context;
				int screenX, screenY;
				int tempInt;

				//1. x원점 
				oneData = _tcstok_s(tempBstr1, spaceToken, &context);
				tempInt = _wtoi(oneData);
				printf("%d ,", tempInt);
				//2. y원점
				oneData = _tcstok_s(context, spaceToken, &context);
				tempInt = _wtoi(oneData);
				printf("%d ,", tempInt);

				//3. shapeList 읽어오기 
				xmlDocument->selectNodes(_bstr_t("/file/shapeList/shape"), &xmlNodeList1);
				xmlNodeList1->get_length(&listLength);

				//4. 이제 shape들을 하나씩 읽으면서 shape들을 만들어 준다. 
				for (int i = 0; i < (int)listLength; i++) {
					int shapeType;
					int ulX;
					int ulY;
					int drX;
					int drY;
					TCHAR* content;

					xmlNodeList1->get_item(i, &parentNode); //shape하나 읽어온다. 



															//2. 좌상단 읽기
					parentNode->selectSingleNode(_bstr_t("p1"), &childNode);
					childNode->get_text(&tempBstr1);

					// 좌상단 x 
					oneData = _tcstok_s(tempBstr1, spaceToken, &context);
					ulX = _wtoi(oneData);
					// 좌상단 y
					oneData = _tcstok_s(context, spaceToken, &context);
					ulY = _wtoi(oneData);

					//3. 우하단 읽기
					parentNode->selectSingleNode(_bstr_t("p2"), &childNode);
					childNode->get_text(&tempBstr1);

					//우하단 x 
					oneData = _tcstok_s(tempBstr1, spaceToken, &context);
					drX = _wtoi(oneData);

					// 우하단 y
					oneData = _tcstok_s(context, spaceToken, &context);
					drY = _wtoi(oneData);


					//1. class읽기
					parentNode->selectSingleNode(_bstr_t("class"), &childNode);
					childNode->get_text(&tempBstr1);

					if (_tcscmp(TEXT("Circle"), tempBstr1) == 0) {
						msc.add(new mCircle(ulX, ulY, drX, drY));
						//4. 컨텐츠를 얻어와 본다. 
						parentNode->selectSingleNode(_bstr_t("txt"), &childNode);
						childNode->get_text(&tempBstr1);
						if (tempBstr1 != NULL)
							msc.replaceTextAt(msc.getShapeNum() - 1, tempBstr1);

					}
					else if (_tcscmp(TEXT("Rect"), tempBstr1) == 0) {
						msc.add(new mRectangle(ulX, ulY, drX, drY));
						//4. 컨텐츠를 얻어와 본다. 
						parentNode->selectSingleNode(_bstr_t("txt"), &childNode);
						childNode->get_text(&tempBstr1);
						if (tempBstr1 != NULL)
							msc.replaceTextAt(msc.getShapeNum() - 1, tempBstr1);
					}
					else {
						msc.add(new mLine(ulX, ulY, drX, drY));
					}
				}
			}


			::CoUninitialize();
		}

	}
};