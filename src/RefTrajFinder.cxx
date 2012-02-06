/****************************************************************************
*
*				������� ������ RefTrajFinder
*
****************************************************************************/

#include "RefTrajFinder.h"

using namespace AxiomLib;

// ���������� ������ - ������ �������� ��������� ������ �� ���������
RefTrajFinder::RefTrajFinder (void) {
	
} 


// ���������� ������ - �������� ��� ��������� � ������ ������ ������������ �������
RefTrajFinder::~RefTrajFinder (void) {
	
}


/****************************************************************************
*						RefTrajFinder::setDataSet
*
*       Description:	������� ��������� ������ ������ ������ � ������ �����
*       Parameters:		newDataSet - ����� ����� ������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int RefTrajFinder::setDataSet (const DataSet &newDataSet) {
	dataSet = newDataSet;
	return 0;
}


/****************************************************************************
*                       RefTrajFinder::setParamsOfDataSet
*
*       Description:	������� ��������� ������ ��������� ��������� ������ params
*       Parameters:		newParams - ����� �������� �������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int RefTrajFinder::setParamsOfDataSet (std::vector<int> &newParams) {
	params = newParams;
	return 0;
}


/****************************************************************************
*                       RefTrajFinder::copy
*
*       Description:	������� ���������� ����� ������� ������
*       Parameters:		-
*       Returns:		RefTrajFinder* - ������ �� ����������� ����� ������� ������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
RefTrajFinder* RefTrajFinder::copy (void) {
	RefTrajFinder* toReturn;
	toReturn = new RefTrajFinder;
	toReturn->setDataSet (dataSet);
	toReturn->setParamsOfDataSet (params);
	return (RefTrajFinder*) toReturn;
}


/****************************************************************************
*				RefTrajFinder::initFromEnv
*
*	Description:	������� ������������� ��������� ������� �� ������ �� 
*					���������, ������� �������� ��� �������� �������
*	Parameters:	env - ������ ������ ���������, ������ ������� ��������� ������� ������
*	Returns:	0
*	Throws:		AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
int RefTrajFinder::initFromEnv (const Environment& env) {
	
	return 0;
}


/****************************************************************************
*					RefTrajFinder::run
*
*	Description:	�������� ������� � ������ - ��������� �������� ������
*					������ ��������� ����������.
*	Parameters:	void
*	Returns:	0
*	Throws:		AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
int RefTrajFinder::run (void) {

	return 0;
}
