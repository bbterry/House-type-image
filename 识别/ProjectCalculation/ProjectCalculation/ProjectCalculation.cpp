#include "ProjectCalculation.h"

ProjectCalculation::ProjectCalculation()
{

}

ProjectCalculation::~ProjectCalculation()
{

}

bool ProjectCalculation::readXML(const char * filename)
{
	//����һ��XML���ĵ�����
	TiXmlDocument *myDocument = new TiXmlDocument("lines.xml");
	myDocument->LoadFile();
	//��ø�Ԫ��
	TiXmlElement *RootElement = myDocument->RootElement();
	//�����Ԫ������
	cout << RootElement->Value() << endl;
	segment s;
	TiXmlElement *Line = RootElement->FirstChildElement();
	while(Line){
	TiXmlElement *point=Line->FirstChildElement();
	//
	TiXmlElement *x = point->FirstChildElement();
	TiXmlElement *y = x->NextSiblingElement();
	//���
	cout << x->GetText()<< endl;
	cout << y->GetText() << endl;
	cout<<"//"<<endl;

	s.p1.x=atoi(x->GetText());
	s.p1.y=atoi(y->GetText());
	point = point->NextSiblingElement();
	0
	x = point->FirstChildElement();
	y = x->NextSiblingElement();
	cout << x->GetText()<< endl;
	cout << y->GetText() << endl;
	cout<<"//"<<endl;

	s.p2.x=atoi(x->GetText());
	s.p2.y=atoi(y->GetText());
	segments.push_back(s);
	Line = Line->NextSiblingElement();
		}
	return true;
}

point * ProjectCalculation::findContour(vector<segment> sgm)
{
	point *p;
	return p;
}

void ProjectCalculation::calculate()
{

}
