#include "ProjectCalculation.h"
#include <iostream>

void main(int agc, char ** agv)
{
	char * filename = (agc == 2) ? agv[1] : "elements.xml";
	ProjectCalculation calc;
	if(!calc.readXML(filename)) return ;
	system("pause");
}