/***************************************************************************
*					RefTrajFinder
*
*	Description:	�����, ����������� �������� ��������� ��������� ���������� 
*					� ������ ������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Ref_Traj_Finder_HXX
#define _Ref_Traj_Finder_HXX

#include "AxiomLibException.h"
#include "Round.h"
#include "Environment.h"
#include "DataSet.h"

#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class RefTrajFinder {

 private:

	// ������ �������� ������: 
	// ����� ��������� �����
	DataSet dataSet;

	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> params;
	
 protected:

 public:

	// ����������� ������ � �������� ���� ���������� �� ���������
	RefTrajFinder (void);  
  
	// �����������, ������������ ��� ���������� �������� ������ �������
	~RefTrajFinder (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� ��������� ������ ������ ������ � ������ �����
	int setDataSet (const DataSet &newDataSet);

	// ������� ��������� ������ ��������� ��������� ������, ������� ��������� ����� ���� �� ������ ������ ������������
	int setParamsOfDataSet (std::vector<int> &newParams);

	// ������� ���������� ����� ������� ������
	RefTrajFinder* copy (void);

	// �������� ������� ������ - ������ ������ ��������� ��������� ����������
	int run (void);

}; // end of class

}; // end of namespace

#endif /* _Ref_Traj_Finder_HXX */
