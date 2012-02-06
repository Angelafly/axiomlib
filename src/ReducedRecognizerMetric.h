/****************************************************************************
*					ReducedRecognizerMetric
*
*	Description:	����� ��� ��������� ���������� ������
*					������������� ��� ��������� ������ ������ 
*					��� ������������� ������� � �������� ���� ����������
*					����� ����������� �������� ��������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ReducedRecognizerMetric_HXX
#define __AXIOMS_LIB_ReducedRecognizerMetric_HXX

#include "ReducedRecognizerDistance.h"
#include "AxiomLibException.h"
#include "Round.h"
//#include "Defines.h"
// ���������� � cxx

namespace AxiomLib {

class ReducedRecognizerMetric : public ReducedRecognizerDistance {

 private:
	
 protected:
 
	// ���������� ��� �������� ����������� ����������� ���������� ����� ���������� ��� ������� ������� �� �������
	// double precision; � ������������ ������
  
 public:

	// ����������� �����������
	ReducedRecognizerMetric (void);

	// ����������� ���������� - �������� ���� ������������ �������� ������
	~ReducedRecognizerMetric (void);

	// ������� ������� �������� ���������� ������, � ��� ����� - ������� �������� � �������� ������
	int setParamsFromEnv (const Environment& env);

	// ������� �������� ������ �������� ethalon � markUp � ������ ����������� ������ � result
	int run (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result);

	// ������� ��������� ����� �������� ethalon � markUp � ������ ����������� ������ � result
	int countMetricDist (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) const;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_ReducedRecognizerMetric_STRATEGY_HXX */
