/****************************************************************************
*				CompareStatistic
*
*	Description:	����� ������ ������� ������������� ��������� ��������
*	Author:		dk,gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_COMPARE_STATISTIC_HXX
#define __AXIOMS_LIB_COMPARE_STATISTIC_HXX

#include "AxiomLibException.h"
#include "Environment.h"
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class CompareStatistic {
	private:

	protected:

	public:

	// ����������� �����������
	CompareStatistic (void) {}
	
	// ����������� ����������
	virtual ~CompareStatistic() {}

	// ���������� ��� ������� ������ �������� ���������� ��������
	virtual std::string name (void) = 0;

	// ���������� ��������� ������� ������ �������� ����������
	//virtual int getParams (std::map<std::string, std::string> &params) = 0;
	
	// ������� ������� �������� ���������� ������
	virtual int setParams (std::map<std::string, std::string> &params) = 0;

	// ������� ������� �������� ���������� ������ �� ���������
	virtual int setParamsFromEnv (const Environment& env) = 0;

	// ���������� ����� ������ �������� ����������
	virtual CompareStatistic* copy (void) = 0;
	
	// �������� ������ - ������� � ������� ����
	virtual int getStatistic (const std::vector<int> &row, const std::vector<int> &correct, double &first, double &second, const bool comments) = 0;

};

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_COMPARE_STATISTIC_HXX */
