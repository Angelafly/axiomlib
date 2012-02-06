/****************************************************************************
*			TreatmentSample
*
*	Description:	������ ������ ������������� �����
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_TREATMENT_SAMPLE_HXX
#define __AXIOMS_LIB_TREATMENT_SAMPLE_HXX

#include "Treatment.h"
#include "AxiomSetPop.h"
#include <string>
#include <stdlib.h>

namespace AxiomLib {

class TreatmentSample : public Treatment{

private:

protected:

public:
  
	// ����������� �����������
	TreatmentSample (void) {}

	// ����������� ����������
	~TreatmentSample () {}

	// ���������� ��� ������� ������ �������������
	std::string name (void) const {return (std::string) "TreatmentSample";}

	// ������� ������� �������� ���������� ������
	int setParamsFromEnv (const Environment& envStart);

	// ������� ���������� � ������� �������� ������ �������� ������� ������� ����� ������� ����������
	int addRes (double res);

	// ������� ��������� ���������� ������ �������������
	// ���������� �������� �� ���������, ����� ��������� ������� ������� ������ � ����� �������� ����� �� ������ ������������ ������ �������������
	int change (void);

	// �������� �������� ������������� ��� ������� ������� ������
	int process (DataSet &newDataSet, const DataSet &dataSet) const;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_TREATMENT_SAMPLE_HXX */
