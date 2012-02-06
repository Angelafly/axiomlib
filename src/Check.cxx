/****************************************************************************
*			������� ������ Check.h
*
*	Description:	����� ��� �������� - ����������
*	Author:		dk
*	History:	
*
****************************************************************************/
#include "Check.h"

using namespace AxiomLib;

// ������ �����������
Check::Check() {}

// ������ ����������
Check::~Check() {}

/****************************************************************************
*				Check::checkAndCompatibility
*
*	Description:	������� �������� �� ������������� �� "�" ���� ������������ ������� �� ���� ������
*	Parameters:	ax1 - ��������� �� ������ �������
*				ecNum1 - ����� ������� ������������ ������������� ������� � ������ ������
*				ax2 - ��������� �� ������ �������
*				ecNum2 - ����� ������� ������������� ������� �� ������ �������
*	Returns:		1  - ���� ����������
*				0  ���� �� ����������
*				-1  ���� ������������
*	Throws:		AxiomLibException - ���� ������� ��������� �� ������������� ��������� ��������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Check::checkAndCompatibility (Axiom* ax1, int ecNum1, Axiom* ax2, int ecNum2) {
	if ( (ecNum1 < 0) || (ecNum2 < 0) || ( ecNum1 >= ax1->getNumOfEC() ) || ( ecNum2 >= ax2->getNumOfEC() ) )
		throw AxiomLibException ("Error in Check::checkAndCompatibility :given positions of ecs' are out of range.");
	return (ax1->elemConditions[ecNum1])->checkAndCompatibility( *(ax2->elemConditions[ecNum2]) );
}


/****************************************************************************
*				Check::change
*
*	Description:	������� ������ ������� ���� ����������� ������� �� ���� ������
*	Parameters:	ax1 - ��������� �� ������ �������
*				ecNum1 - ����� ������� ������������� ������� � ������ �������
*				ax2 - ��������� �� ������ �������
*				ecNum2 - ����� ������� ������������� ������� �� ������ �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� ��������� �� ������������� ��������� ��������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Check::change (Axiom* ax1, int ecNum1, Axiom* ax2, int ecNum2) {
	if ( (ecNum1 < 0) || (ecNum2 < 0) || ( ecNum1 >= ax1->getNumOfEC() ) || ( ecNum2 >= ax2->getNumOfEC() ) )
		throw AxiomLibException ("Error in Check::change :given positions of ecs' are out of range.");
	ElemCondition* tmp;
	tmp = ax2->elemConditions[ecNum2];
	ax2->elemConditions[ecNum2] = ax1->elemConditions[ecNum1];
	ax1->elemConditions[ecNum1] = tmp;
	return 0;
}
