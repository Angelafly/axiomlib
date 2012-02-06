/****************************************************************************
*			ElemConditionsFactory
*
*	Description:	��������� ������������ ������� �� ������ ��� �� �����
*				����� ������������ ������������� � �������� ������,
*				����� �������� � ���� ��� ������ - ������������ �������
*				�� ���������� axioms.
*	Author:		Eugene A. Vasin
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_ElemConditionsFactory_HXX
#define __AXIOMS_LIB_ElemConditionsFactory_HXX

#include "Round.h"

#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>

#include "elem_conditions/ElemCondition.h"
#include "ElemCondPlus.h"

namespace AxiomLib {

class ElemConditionsFactory {

 private:

	 std::vector < std::vector<bool> > compatibilityMatrix;

 protected:
 
  // ��������������� ������� ������ ������������� ������� � ������� ������������� ��������
  int findMaxNum (std::vector <int> &vec);

  // ��������������� ������� ������������ ���������� �������� � ��������� ��������� �� ����
  int andVector (std::vector <bool>& whereVec, const std::vector <bool>& whatFirst, const std::vector <bool>& whatSecond);

  // ��������������� ����������� ��������� ������������ ������� ����������� ����������� ������� �������� �����
  int subproc (std::vector < std::vector<int> >& prev, int &prevLast, std::vector <int>& cur, std::vector <bool> &curComp, const int numOfEC);

  // ��������������� ������� �������� �������� ����������� ������������ ������ �������� �����
  int compatTable (std::vector < std::vector<int> >& prev, const int numOfEC);

  // ������, �������� ��������� �� ������� ���� �����, � ���������� �����������
  std::vector<ElemCondition*> all_axioms;
 
 public:

  // �����������, �������������� ������� ��� ���������� ������, ��������
  // ������ ���� ������
  ElemConditionsFactory(void);
  
  // �����������, ������������ ��� ���������� ��������
  ~ElemConditionsFactory(void);

  // ���������� ������ �� ��������� ������������ ������� � ������� num
  // ������� ��������� ��������� ������������� ��� ����������
  ElemCondition* createAxiomByNum(const signed int num);

  // ���������� ������ ���������� �� ������������ ������� �������� �������� ��� ���������� �������
  int  createAxiomByRand(std::vector<ElemCondition* >& toReturn, const signed int numOfEC);

  // ���������� ������ ���������� �� ������������ ������� �������� �������� ��� ���������� �������
  //int  createAxiomByRand(std::vector< std::vector <ElemCondPlus> >& toReturn, const signed int numOfEC);
  int  createAxiomByRand(std::vector< std::vector <int> > &toReturn, const signed int numOfEC);

  // ���������� ������ �� ��������� ������������ ������� � ������ name
  // ������� ��������� ��������� ������������� ��� ����������
  // ���� �� ������� ��� ������� � ����������� ��������
  ElemCondition* createAxiomByName(const char* name);
  
  // ���������� ����� ���� ������ � �������� ������ BackInserter
  template<class BackInserter>
  int getECNames(BackInserter inserter);

};

template<class BackInserter>
int ElemConditionsFactory::getECNames(BackInserter inserter)
{
	for(int i = 0; i <(int) all_axioms.size(); i++)
	{
		*inserter = all_axioms[i]->name();
		++inserter;
	}
	return 0;
}

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_ElemConditionsFactory_HXX */
