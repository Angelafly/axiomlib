/****************************************************************************
*				CompareStatisticSimple
*
*	Description:	����� �������� ����� ������ ������� � �������� ����
*	Author:		dk,gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_COMPARE_STATISTIC_SIMPLE_HXX
#define __AXIOMS_LIB_COMPARE_STATISTIC_SIMPLE_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "CompareStatistic.h"
#include "all_axioms.h"
#include "Axiom.h"
#include <math.h>
#include <iostream>      
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class CompareStatisticSimple : public CompareStatistic {
	private:
	
	// ��������� ������ �������� �������, � ������� ������ ����������� ��� �������� 
	// �� ���� - ���� ���������� ����� ���� ����� - �� �������������� ��� ���� � ���������� �������� � �������� �����������
	// ����� ����� ����������� �����
	int left;
	
	// ����� ����� ����������� ������
	int right;

	int ImH; // Very important param - ������������ ���������� ����� ��������� ��������� ��� ������� �������������� 
		     // 				  - ����� ���� ������� ��������� ��� ���������� � first

	protected:

	public:

	// ����������� ����������� � �������� ���������� �� ���������
	CompareStatisticSimple (void);

	// ���������� ��� ������� ������ �������� ���������� ��������
	std::string name (void);
	
	// ������� ������� �������� ���������� ������
	int setParams (const int newLeft, const int newRight, const int newImH);
	int setParams (std::map<std::string, std::string> &params);
	int setParamsFromEnv (const Environment& env);

	// ������� ���������� ����� ������� ������
	CompareStatistic* copy (void);
	
	// �������� ����� ������ ������� � ������� ���� �� ���� � ���������� ��������
	int getStatistic (const std::vector<int> &row, const std::vector<int> &correct, double &first, double &second, const bool comments);

};

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_COMPARE_STATISTIC_SIMPLE_HXX */
