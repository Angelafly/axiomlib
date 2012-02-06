/****************************************************************************
*				TreatmentSample
*
*	Description:	������� ������ TreatmentSample - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "TreatmentSample.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			TreatmentSample::setParamsFromEnv
*
*	Description:	��������� ���������� ������
*	Parameters:	const Environment& envStart - ������ �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ��������� ���
*	Author:		dk
*	History:
*
****************************************************************************/
int TreatmentSample::setParamsFromEnv (const Environment& env) {
	
	return 0;
}


/****************************************************************************
*				TreatmentSample::addRes
*
*	Description:	������� ���������� � ������� �������� ������ �������� ������� ������� ����� ������� ����������
*	Parameters:	res - ����������� �������� ������� �������� ���������� �� ��������� ������� ������ ��� ������ ������ ������������� �����
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int TreatmentSample::addRes (double res) {
	
	return 0;
}


/****************************************************************************
*			TreatmentSample::change
*
*	Description:	������� ��������� ���������� ������ �������������
*	Parameters:	-
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int TreatmentSample::change (void) {
	
	return 0;
}


/****************************************************************************
*			TreatmentSample::process
*
*	Description:	�������� �������� ������������� ��� ������� ������� ������
*	Parameters:	dataSet - ������� ����� ������ ��� ��������������
*				newDataSet - ��������������� ����� ������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int TreatmentSample::process (DataSet &newDataSet, const DataSet &dataSet) const {
	newDataSet = dataSet;
	return 0;
}
