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
				//. ���� ����
				if (!SUCCEEDED(xmlDocument.CreateInstance(__uuidof(DOMDocument)))) {
					::wprintf(L"xmlDocument�� �������� �ʾҽ��ϴ�.\n");
					::CoUninitialize();
					return NULL;
				}


				//1. xml Instruction�� �����. 
				xmlDocument->createProcessingInstruction(bstrInst1, bstrInst2, &xmlProcessingInstruction); //��Ҹ� �����
				xmlDocument->appendChild(xmlProcessingInstruction, NULL); //���δ�. 

																		  //2. file element ����� ���̱�
				tempStr = SysAllocString(TEXT("file"));
				xmlDocument->createElement(tempStr, &elem_file); //bstr1�� elem_file�� �����   
				xmlDocument->appendChild(elem_file, NULL); // xmlElement1�� xmlDocument�� ���δ�.
				SysFreeString(tempStr);

				//3. zoomLevel ���̱� : file�ؿ� �ִ�. 
				tempStr = SysAllocString(TEXT("zoomLevel"));
				xmlDocument->createElement(tempStr, &elem_zoomLevel); //bstr1�� elem_file�� �����   
				_bstr_t zoomLevelStr = mOption.getZoomLevel(); //zoomLevel�� string���� ���� 
				elem_zoomLevel->put_text(zoomLevelStr); //�� ���� ���̿� �ִ´�. 
				elem_file->appendChild(elem_zoomLevel, NULL); // xmlElement1�� xmlDocument�� ���δ�.
				SysFreeString(tempStr);

				//4. nowScreenXy ���̱� 
				tempStr = SysAllocString(TEXT("nowScreenXY"));
				xmlDocument->createElement(tempStr, &elem_nowScreenXY); //element�����
				_bstr_t nowScreenX = mOgPoint.getOriginPoint().x;
				_bstr_t nowScreenY = mOgPoint.getOriginPoint().y;

				tempStr3 = SysAllocString(TEXT(""));
				wsprintf(tempStr3, TEXT("%d %d"), mOgPoint.getOriginPoint().x, mOgPoint.getOriginPoint().y);
				elem_nowScreenXY->put_text((BSTR)tempStr3);

				elem_file->appendChild(elem_nowScreenXY, NULL); // ���̱�
				SysFreeString(tempStr);
				SysFreeString(tempStr3);

				//SysFreeString(tempStr2);

				//5. shapeList ���̱� //���� shapeList������ shape���� �ݺ������� ����� ���δ�. 
				tempStr = SysAllocString(TEXT("shapeList"));
				xmlDocument->createElement(tempStr, &elem_shapeList); //bstr1�� elem_file�� �����   
				elem_file->appendChild(elem_shapeList, NULL); // xmlElement1�� xmlDocument�� ���δ�.
				SysFreeString(tempStr);

				//6. shape�� ���̱� 
				for (int i = 0; i < msc.getShapeNum(); i++) {
					//1. shape�����
					tempStr = SysAllocString(TEXT("shape"));
					xmlDocument->createElement(tempStr, &elem_shape); //bstr1�� elem_file�� ����� //shapeList�ؿ� ���δ�. 
					elem_shapeList->appendChild(elem_shape, NULL); // xmlElement1�� xmlDocument�� ���δ�.
					SysFreeString(tempStr);

					//6.1 class ����� 
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
					elem_class->put_text(tempStr2); //���� ��� �ְ�
					elem_shape->appendChild(elem_class, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr2);

					//6.2 p1�����
					tempStr = SysAllocString(TEXT("p1"));
					xmlDocument->createElement(tempStr, &elem_p1);

					tempStr3 = SysAllocString(TEXT(""));
					wsprintf(tempStr3, TEXT("%d %d"), msc.getUpLeftAt(i).x, msc.getUpLeftAt(i).y);
					elem_nowScreenXY->put_text((BSTR)tempStr3);


					elem_p1->put_text(tempStr3);


					elem_shape->appendChild(elem_p1, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr3);

					//6.3 p2�����
					tempStr = SysAllocString(TEXT("p2"));
					xmlDocument->createElement(tempStr, &elem_p2);

					tempStr3 = SysAllocString(TEXT(""));
					wsprintf(tempStr3, TEXT("%d %d"), msc.getDownRightAt(i).x, msc.getDownRightAt(i).y);
					elem_nowScreenXY->put_text((BSTR)tempStr3);


					elem_p2->put_text(tempStr3);

					elem_shape->appendChild(elem_p2, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr3);

					//6.4 text�����
					tempStr = SysAllocString(TEXT("txt"));
					xmlDocument->createElement(tempStr, &elem_txt);

					tempStr2 = SysAllocString(msc.getText(i));
					elem_txt->put_text(tempStr2);

					elem_shape->appendChild(elem_txt, NULL); //shape�ؿ� class ���̱� 
					SysFreeString(tempStr);
					SysFreeString(tempStr2);


				}

				//�����
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
					::wprintf(L"xmlDocument�� �������� �ʾҽ��ϴ�.\n");
					::CoUninitialize();
					return;
				}

				//1. ���Ͽ��� 
				variant = fileAddr;
				xmlDocument->load(variant, &variantBool); //xmlDocument�� load�Ѵ�. 

														  //1. zoomLevel�� ��´�. //1 ���� 
				xmlDocument->selectSingleNode(_bstr_t("/file/zoomLevel"), &pNode);
				pNode->get_text(&tempBstr1); //tempBstr1�� ���ڷ� �ٲ�� �Ѵ�. 
				double zoomDouble = _wtof(tempBstr1);
				mOption.setZoomLevel(zoomDouble); //�ܷ��� ���� 
				wprintf(L"%.2lf \n", zoomDouble);

				//2. now screen xy
				xmlDocument->selectSingleNode(_bstr_t("/file/nowScreenXY"), &pNode);
				pNode->get_text(&tempBstr1); //tempBstr1�� �� ���� �и����Ѿ� �Ѵ�. 
				TCHAR* oneData;
				TCHAR* context;
				int screenX, screenY;
				int tempInt;

				//1. x���� 
				oneData = _tcstok_s(tempBstr1, spaceToken, &context);
				tempInt = _wtoi(oneData);
				printf("%d ,", tempInt);
				//2. y����
				oneData = _tcstok_s(context, spaceToken, &context);
				tempInt = _wtoi(oneData);
				printf("%d ,", tempInt);

				//3. shapeList �о���� 
				xmlDocument->selectNodes(_bstr_t("/file/shapeList/shape"), &xmlNodeList1);
				xmlNodeList1->get_length(&listLength);

				//4. ���� shape���� �ϳ��� �����鼭 shape���� ����� �ش�. 
				for (int i = 0; i < (int)listLength; i++) {
					int shapeType;
					int ulX;
					int ulY;
					int drX;
					int drY;
					TCHAR* content;

					xmlNodeList1->get_item(i, &parentNode); //shape�ϳ� �о�´�. 



															//2. �»�� �б�
					parentNode->selectSingleNode(_bstr_t("p1"), &childNode);
					childNode->get_text(&tempBstr1);

					// �»�� x 
					oneData = _tcstok_s(tempBstr1, spaceToken, &context);
					ulX = _wtoi(oneData);
					// �»�� y
					oneData = _tcstok_s(context, spaceToken, &context);
					ulY = _wtoi(oneData);

					//3. ���ϴ� �б�
					parentNode->selectSingleNode(_bstr_t("p2"), &childNode);
					childNode->get_text(&tempBstr1);

					//���ϴ� x 
					oneData = _tcstok_s(tempBstr1, spaceToken, &context);
					drX = _wtoi(oneData);

					// ���ϴ� y
					oneData = _tcstok_s(context, spaceToken, &context);
					drY = _wtoi(oneData);


					//1. class�б�
					parentNode->selectSingleNode(_bstr_t("class"), &childNode);
					childNode->get_text(&tempBstr1);

					if (_tcscmp(TEXT("Circle"), tempBstr1) == 0) {
						msc.add(new mCircle(ulX, ulY, drX, drY));
						//4. �������� ���� ����. 
						parentNode->selectSingleNode(_bstr_t("txt"), &childNode);
						childNode->get_text(&tempBstr1);
						if (tempBstr1 != NULL)
							msc.replaceTextAt(msc.getShapeNum() - 1, tempBstr1);

					}
					else if (_tcscmp(TEXT("Rect"), tempBstr1) == 0) {
						msc.add(new mRectangle(ulX, ulY, drX, drY));
						//4. �������� ���� ����. 
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