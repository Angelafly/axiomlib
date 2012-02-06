/****************************************************************************
*					ReducedRecognizer
*
*	Description:	����������� ����� ��� ��������� ���������� ������
*					������������� ��� �������� ����� ��������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ReducedRecognizer_HXX
#define __AXIOMS_LIB_ReducedRecognizer_HXX

#include "Environment.h"
#include <vector>

namespace AxiomLib {

class ReducedRecognizer {

 private:
 
 protected:
  
 public:
 
	// ����������� ����������� - ������
	// ReducedRecognizer (void) {}
	
	// ����������� ���������� - ������. ���������, ����� ���������� ����������� �������, ����������� ������ ���������
	virtual ~ReducedRecognizer (void) {}
	
	// ������� ������� �������� ���������� ������
	virtual int setParamsFromEnv (const Environment& env) = 0;
	
	// ������� ������ �������� ethalon � markUp � ������ ����������� ������ � result
	virtual int run (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <int> &result) = 0;
	
	// ������� ������ �������� ethalon � markUp -- � result ������� ������������ ����������
	virtual int run(const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) = 0;
	
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_ReducedRecognizer_STRATEGY_HXX */
