/****************************************************************************
*				StartStrategy
*
*	Description:	����� ��������� ������ ��������������� � �����������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_START_STRATEGY_H
#define __AXIOMS_LIB_START_STRATEGY_H

#include "Environment.h"
#include "EnvDataSet.h"
#include "AxiomSetPop.h"
#include "AxiomLibException.h"
#include "BaseGenetics.h"
#include "Treatment.h"
#include "TreatmentFactory.h"
#include <string>
#include <vector>
#include <iostream>

namespace AxiomLib {

class StartStrategy {

private:

	// ������ ������������ ������� ������������� ������� ������ ������
	std::vector <BaseGenetics> gaVector;

	// ������ ������������ ��������� ������������� ���������
	std::vector <AxiomSetPop> popVector;

	// �������� ����� ������ - ��������� ��� ������� � ������ �� ������
	DataSet dataSet;

	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
  	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> params;

	// ������ ����� �� ������ ����� ���� � ���������������� - ������ ������ �� ��� � ��� 
	// � ��� ��������������� - ������ ������� ���� ������� � ����� ������� ����������� ������������ ���������� 
	// � ������ �������, �� ���� ���������� � ����������� (������� �������� ������� �������)
	std::vector <Treatment * > treatVector;

	// ������ � ������ ������� ��� ������� ������������� ���������
	std::string bgConfigFile;

	// ����� ������� � ������ �� ������� ������������� � ���������� ����������� (����������� �� ����������������� �����)
	int numOfStarts;

protected:

	// ��������������� ������� ���������� �������� � ������� mask �� ��������� � ������� resVector
	inline int sort (std::vector<int> &mask, std::vector<double> &resVector) const;

	// ��������������� ������� ���������� ����� ��������������� ��������������� �� �������� ���������� ������� �������
	inline int reduce(const int i, const std::vector<int> &mask);
	
	// ��������������� ������� �������� �������� ���������������� �� ������� �������
	inline int deleteProcs (std::vector<bool> &alive);

	// ��������������� ������� �������� ����� �������� �� �� ������ ����� ������ ������ �������������
	inline int countNumOfIters (const int i);

	// ��������������� ������� ���������� � ���� ���� ������ �� i-��� ���������������
	int saveToFile (const int i) const;

public:

	// ������ ����������� � �������� ���������� �� ���������
	StartStrategy (void);

	// ���������� - ��������� ��� ��������� � ������ ������� ������ ������������ �������
	~StartStrategy(void);

	// ������� ������������� ������� ������ - ������� ��������� ��������� ���������� ����������
	void init (void);

	// �������� ������� ������� ��������� ������ ���������������
	void start (void);

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_START_STRATEGY_H */
