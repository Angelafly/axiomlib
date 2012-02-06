/****************************************************************************
*				Treatment
*
*	Description:	������������ ����� ��� �������� �������������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_TREATMENT_HXX
#define __AXIOMS_LIB_TREATMENT_HXX

#include "Environment.h"
#include "AxiomLibException.h"
#include "DataSet.h"
#include <string>
#include <vector>

namespace AxiomLib {

class Treatment {

private:

protected:

public:

	// ����������� ����������� - ������
	Treatment (void) {}

	// ����������� ���������� - ������
	virtual ~Treatment () {}

	// ���������� ��� ������� ������ �������������
	virtual std::string name (void) const = 0;

	// ������� ������� �������� ���������� ������
	virtual int setParamsFromEnv (const Environment& env) = 0;

	// ������� ���������� � ������� �������� ������ �������� ������� ������� ����� ������� ����������
	virtual int addRes (double res) = 0;

	// ������� ��������� ���������� ������ �������������
	// ���������� �������� �� ���������, ����� ��������� ������� ������� ������ � ����� �������� ����� �� ������ ������������ ������ �������������
	virtual int change (void) = 0;

	// �������� �������� ������������� ��� ������� ������� ������
	virtual int process(DataSet &newDataSet, const DataSet &dataSet) const = 0;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_TREATMENT_HXX */
