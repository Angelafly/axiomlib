/****************************************************************************
*					FullFuzzyDataSet
*
*	Description:	����� - ���������� ������ �������� ������ �� ���� ���������� 
*					����������� ��������� � ��������� �������. ����� ����, 
*					� ������ ������� ������� ������� ��� ����� ����������� ����������. 
*					���� ������ ����������, ���������� ��������� ���������, �
*					���������� ����������� ���������.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_FullFuzzyDataSet_HXX
#define __AXIOMS_LIB_FullFuzzyDataSet_HXX

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

class FullFuzzyDataSet : public DataSet {

  private:
 
	// ������ - ������ ������������ ����� ���������������, �������� ������������ ���������� ��������� �������
	std::string normal;
	  
  protected:
	
	// ���������� ���������� ��������� ������� - ������������ ����������� ���������
	// ��������� ��������� ���� ���������� ����������� ��������� �� Reference
	ClassTS referenceNormalTS;

	// ���������� ����������� ������� �������� ��� �� ��� � ���������� �� ��������� �������
	// ��������� ��������� ���� ��� ������� ��������������
	ReferenceClassesTS testingClassesTS;

	// ��������� ��������� ���� ���������� ����������� ��������� �� Testing
	ClassTS testingNormalTS;	

	// ������ ���������� ����������� � ������ ����������� ��������� �� ���������� ����
	signed int readNormalClassTS (std::string &path, ClassTS &normalTS);
 
  public:
    
	// ��������� �����������
	FullFuzzyDataSet(void);
 
	// ������ ����� ������ �� ���������� ��������
	signed int readDataSet(std::string dataSetDir, std::string dataSetName);

	// ������� ���������� ��������� ��� ����������� ��������� �� ��������� ������� �� ������������ - ������ ���������� � ������, ������ ���� � ����������
	bool getNormalTSFromClass (std::vector<double> &vec, int indexMultiTS, int indexTS) const;

	// ������� ���������� ��������� ��� ����������� ��������� �� ����������� ������� �� ������������ - ������ ���������� � ������, ������ ���� � ����������
	bool getNormalTSFromTest (std::vector<double> &vec, int indexMultiTS, int indexTS) const;

	// ������� ������ ���������� � ����� ��������� ����� �������������� ����������� ��������� � ��������� �������
	bool getNormalClassSize (int &numOfMultiTS, std::vector<int> &numOfTS) const;

	// ������� ������ ���������� � ����� ��������� ����� �������������� ����������� ��������� � ����������� �������
	bool getNormalTestSize (int &numOfMultiTS, std::vector<int> &numOfTS) const;

	// ������� ���������� ��������� ��� �� ��������� ������� �� ������������ - ������ ������, ������ ���������� � ������, ������ ���� � ����������
	bool getTSByIndexFromTest (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS) const;

	// ������� ���������� ����� ��������� � �������� testingClassesTS
	bool getTestSize (int &numOfClasses, std::vector<int> &numOfMultiTS, std::vector<std::vector<int> > &numOfTS) const;

	// ������� ������������� ����� ����������� ��� ������ ��������� � ��������� ������� ��������� ���������� ����������� ���������
	signed int setNormalStr (EnvDataSet &envDataSet);
	
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_FullFuzzyDataSet_HXX */
