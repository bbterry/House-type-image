#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <atlstr.h>
using namespace std;

//CString GetAppPath()
//{//��ȡӦ�ó����Ŀ¼
//    TCHAR modulePath[MAX_PATH];
//    GetModuleFileName(NULL, modulePath, MAX_PATH);
//    CString strModulePath(modulePath);
//    strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
//    return strModulePath;
//}

//bool CreateXmlFile(CString& szFileName)
//{//����xml�ļ�,szFilePathΪ�ļ������·��,�������ɹ�����true,����false
//    try
//    {
//        //����һ��XML���ĵ�����
//        TiXmlDocument *myDocument = new TiXmlDocument();
//        //����һ����Ԫ�ز����ӡ�
//        TiXmlElement *RootElement = new TiXmlElement("Persons");
//        myDocument->LinkEndChild(RootElement);
//        //����һ��PersonԪ�ز����ӡ�
//        TiXmlElement *PersonElement = new TiXmlElement("Person");
//        RootElement->LinkEndChild(PersonElement);
//        //����PersonԪ�ص����ԡ�
//        PersonElement->SetAttribute("ID", "1");
//        //����nameԪ�ء�ageԪ�ز����ӡ�
//        TiXmlElement *NameElement = new TiXmlElement("name");
//        TiXmlElement *AgeElement = new TiXmlElement("age");
//        PersonElement->LinkEndChild(NameElement);
//        PersonElement->LinkEndChild(AgeElement);
//        //����nameԪ�غ�ageԪ�ص����ݲ����ӡ�
//        TiXmlText *NameContent = new TiXmlText("������");
//        TiXmlText *AgeContent = new TiXmlText("22");
//        NameElement->LinkEndChild(NameContent);
//        AgeElement->LinkEndChild(AgeContent);
//        CString appPath = GetAppPath();
//        CString seperator = "\\";
//        CString fullPath = appPath.GetBuffer(0)+seperator+szFileName;
//        myDocument->SaveFile(fullPath.c_str());//���浽�ļ�
//    }
//    catch (string& e)
//    {
//        return false;
//    }
//    return true;
//}

void ReadXmlFile(string& szFileName)
{//��ȡXml�ļ���������
   /* try
    {
        CString appPath = GetAppPath();
        string seperator = "\\";
        string fullPath = appPath.GetBuffer(0)+seperator+szFileName;*/
        //����һ��XML���ĵ�����
        TiXmlDocument *myDocument = new TiXmlDocument("lines.xml"/*fullPath.c_str()*/);
        myDocument->LoadFile();
        //��ø�Ԫ�أ���Persons��
        TiXmlElement *RootElement = myDocument->RootElement();
        //�����Ԫ�����ƣ������Persons��
        cout << RootElement->Value() << endl;
        ////��õ�һ��Person�ڵ㡣
        //TiXmlElement *FirstPerson = RootElement->FirstChildElement();
        ////��õ�һ��Person��name�ڵ��age�ڵ��ID���ԡ�
        //TiXmlElement *NameElement = FirstPerson->FirstChildElement();
        //TiXmlElement *AgeElement = NameElement->NextSiblingElement();
        //TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
        ////�����һ��Person��name���ݣ��������ǣ�age���ݣ�����ID���ԣ�����
        //cout << NameElement->FirstChild()->Value() << endl;
        //cout << AgeElement->FirstChild()->Value() << endl;
        //cout << IDAttribute->Value()<< endl;
		    //��õ�һ��Line�ڵ㡣
		TiXmlElement *Line = RootElement->FirstChildElement();
		while(Line){
    TiXmlElement *point=Line->FirstChildElement();
    ////
    TiXmlElement *x = point->FirstChildElement();
    TiXmlElement *y = x->NextSiblingElement();
    ////���
    cout << x->GetText()<< endl;
    cout << y->GetText() << endl;
	cout<<atoi(x->GetText())<<endl;
	cout<<"//"<<endl;

	point = point->NextSiblingElement();

	x = point->FirstChildElement();
    y = x->NextSiblingElement();
	cout << x->GetText()<< endl;
    cout << y->GetText() << endl;

	cout<<"//"<<endl;

	Line = Line->NextSiblingElement();
		}
    //}
    //catch (string& e)
    //{
    //    return false;
    //}
    //return true;
}
//int main()
//{
//    string fileName = "lines.xml";
//  //  CreateXmlFile(fileName);
//    ReadXmlFile(fileName);
//	system("pause"); 
//}
