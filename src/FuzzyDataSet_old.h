/****************************************************************************
*					FuzzyDataSet
*
*	Description:	����� - ���������� ������ �������� ������ �� ���� ���������� 
*					����������� ��������� � ��������� �������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_FuzzyDataSet_HXX
#define __AXIOMS_LIB_FuzzyDataSet_HXX

#include "AxiomLibException.h"
#include "Environment.h"
#include "EnvDataSet.h"
#include "DataSet.h"
#include "MultiTS.h"
#include "TestTS.h"
#include <iostream>
#include <string>
#include <vector>

namespace AxiomLib {

class FuzzyDataSet_old : public DataSet {

  private:
 
	// ������ - ������ ������������ ����� ���������������, �������� ������������ ���������� ��������� �������
	std::string normal;
	
	// ��������� ��������� ���� ��� ������� ��������������
	ClassTS normalTS;
  
  protected:
 
  public:
    
	// ��������� �����������
	FuzzyDataSet_old(void);
 
	// ������ ����� ������ �� ���������� ��������
	signed int readDataSet(std::string dataSetDir, std::string dataSetName);
	
	// ������ ���������� ����������� � ������ ����������� ��������� �� ���������� ����
	signed int readNormalClassTS (std::string &path);
	
	// ������� ���������� ��������� ���, ��������������� ����������� ���������, �� ���������� ������ �� ������������ - ������ ����������, ������ ���� � ����������
	bool getNormalTS (std::vector<double> &vec, int indexMultiTS, int indexTS) const;

	// ������� ������������� ����� �������� ���� ��������� �������, ��������������� ����������� ���������
	bool setNormalTS (std::vector<double> &vec, int indexMultiTS, int indexTS);

	// ������� ������ ���������� � ����� ��������� ����� �������������� ����������� ���������
	bool getNormalClassSize (int &numOfMultiTS, std::vector<int> &numOfTS) const;

	// ������� ������������� ����� ����������� ��� ������ ��������� � ��������� ������� ��������� ���������� ����������� ���������
	signed int setNormalStr (EnvDataSet &envDataSet);
	
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_FuzzyDataSet_HXX */
