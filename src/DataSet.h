/****************************************************************************
*			DataSet
*
*	Description:	����� --- ����� ������ (��. ���� ../formats.x)
*				������ ��� ������ ������ �� �������� � ���������,
*				�������� ������, �� ������ �� �������
*	Author:		eav
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_DataSet_HXX
#define __AXIOMS_LIB_DataSet_HXX

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <boost/tokenizer.hpp>

#include "AxiomLibException.h"
#include "Environment.h"
#include "EnvDataSet.h"
#include "TestTS.h"
#include "DataSetBase.h"

namespace AxiomLib {

class DataSet : public DataSetBase {

  private:
  
  protected:
	// ��������� ��������� ���� ��� ������� ��������������
	ReferenceClassesTS referenceClassesTS;
  
	// ��������� �������� ����
	TestSets testSets;

	// ������� ������� �� ������ � ������ ��������� ���� ��� ������� �� �������
	signed int readClassesTS(const std::string &path);
  
	// ������� ������� �� ������  � ������ �������� �������
	signed int readTestesTS (const std::string &path);
    
  public:
    
	// ��������� �����������
	DataSet(void);
	
	// ������ ����� ������ �� �������� path (������ ������ � �������� ������ � ����� ../formats.x
	signed int readDataSet(const std::string &dataSetDir, const std::string &dataSetName);	
	
	signed int readDataSet(const std::string &baseDataSetDir);
 
	// ������� ���������� ��������� ��� �� ���������� ������ �� ������������ - ������ ������, ������ ���������� � ������, ������ ���� � ����������
	bool getTSByIndexFromClass (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS) const;

	// ������� ������������� ����� �������� ���� �� ������������ - ������ ������, ������ ���������� � ������, ������ ���� � ����������
	bool setTSByIndexFromClass (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS);
	
	// ������� ���������� ����� ������� ���������� ���������
	int getNumberOfClasses() const;
	
	// ������� ���������� ����� ��������� � �������� referenceClassesTS
	bool getClassSize (int &numOfClasses, std::vector<int> &numOfMultiTS, std::vector<std::vector<int> > &numOfTS) const;
	
	// ������� ���������� ����� ��������� � �������� testSets
	bool getTestSize (int &numOfTests, std::vector<int> &numOfTS) const;

	// ������� ���������� ��������� ��� �� ������ �����, �� ������ ���� � ���������� � �����
	bool getTSByIndexFromTests (std::vector<double> &vec, int indexTest, int indexTS) const;

	// ������� ������������� ����� ��� �� ������ �����, �� ������ ���� � ���������� � �����
	bool setTSByIndexFromTests (std::vector<double> &vec, int indexTest, int indexTS);
	
	// ������� ���������� ��������� ��� �� ������ �����, �� ������ ���� � ���������� � �����
	bool getTSByIndexFromTests (std::vector<int> &vec, int indexTest, int indexTS) const;
	
	// ������� ������������� ����� ��� �� ������ �����, �� ������ ���� � ���������� � �����
	bool setTSByIndexFromTests (std::vector<int> &vec, int indexTest, int indexTS);
	
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_DataSet_HXX */
