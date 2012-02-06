/****************************************************************************
*					ReducedRecognizerDTW
*
*	Description:	����� ��� ������������� ��������� ������� � ���� ��������
*					� �������������� ��������� �������� ���������� DTW.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ReducedRecognizerDTW_HXX
#define __AXIOMS_LIB_ReducedRecognizerDTW_HXX

#include "ReducedRecognizerDistance.h"
#include "AxiomLibException.h"
#include "Round.h"

//#include "Defines.h"
// ���������� � cxx

namespace AxiomLib {

class ReducedRecognizerDTW : public ReducedRecognizerDistance {

 private:
 
 protected:
 
	// ���������� ��� �������� ����������� ����������� ���������� ����� ���������� ��� ������� ������� �� �������
	// double precision; � ������������ ������

	// ���������� ��� �������� ����������� ���������� ��������� ����������
	double stretch;
  
 public:

	// ����������� �����������
	ReducedRecognizerDTW (void);

	// ����������� ���������� - �������� ���� ������������ �������� ������
	~ReducedRecognizerDTW (void);

	// ������� ������� �������� ���������� ������, � ��� ����� - ������� �������� � �������� ������
	int setParamsFromEnv (const Environment& env);

	// ������� ������� ������ �������� ethalon � markUp � ������ ����������� ������ � result
	int run (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result);

	// ���������� ������� ���������� ���������� ����� ��������
	int countDTWDist (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) const;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_ReducedRecognizerDTW_STRATEGY_HXX */
