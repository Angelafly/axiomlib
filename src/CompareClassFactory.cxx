/****************************************************************************
*
*			������� ������ CompareClassFactory
*
****************************************************************************/

#include <stdlib.h>
#include <math.h>
#include "AxiomLibException.h"
#include "Round.h"
#include "AllCompareStatistic.h"

#include "CompareClassFactory.h"

using namespace AxiomLib;

// �����������, �������������� ������� ��� ���������� �������
CompareClassFactory::CompareClassFactory(void) {

}
  
// �����������
CompareClassFactory::~CompareClassFactory(void) {

}
  

/****************************************************************************
*			CompareClassFactory::create
*
*	Description:	������� ����� �������� ����� ������ ������� � ������� ����
*	Parameters:	name - ��� ������, ��������� �������� �������
*	Returns:		CompareStatistic* - ���������� �� ��������� �����
*	Throws:		AxiomLibException - ���� �� ������ � ������ ������� ����� � ������ � ���������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
CompareStatistic* CompareClassFactory::create (const std::string &name) {
	if (name == "CompareStatisticSimple") {
		CompareStatisticSimple* toReturn;
		toReturn = new CompareStatisticSimple;
		return toReturn;
	}
	throw AxiomLibException ("Error in CompareClassFactory::create: given name of CompareClass is wrong");
}
