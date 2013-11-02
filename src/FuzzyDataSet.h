/****************************************************************************
*				ClippedFullFuzzyDataSet
*
*	Description: ����� ������ ��� ��������� FuzzyMultiData, ��������������
*				��������� ��������� �����
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef CLIPPEDFULLFUZZYDATASET_H
#define CLIPPEDFULLFUZZYDATASET_H

#include <vector>
#include <boost/lexical_cast.hpp>

#include "DataSetDivision.h"
#include "EnvDataSet.h"
#include "DataSetBase.h"
#include "IntInterval.h"

namespace AxiomLib {
	
class FuzzyDataSet : public DataSetBase
{
	
public:
	// ������� ��������
	enum DataSetDivisionType { First = 0, Reference = 0, Testing = 1, Validation = 2, Last = Validation };
	
	// ����� �������� �������� (�.�. ��������������� �����)
	// ����������� ��. � .cxx
	static const char *divisionNames[];
	
	// ����������� ���������
    FuzzyDataSet();
	
	// ������ ��������
	signed int readDataSet(std::string dataSetDir, std::string dataSetName);
	
	int readDataSet(const std::string &dataSetPath);
	
	std::string getMultiTSName(DataSetDivisionType division, int indexClass, int indexMultiTS) const;

	// ���������� ���������� ��� �� �������, ������ ������, ���������� � �����������
	bool getTSByIndex(DataSetDivisionType division, std::vector<double>& vec, int indexClass, int indexMultiTS, int indexTS) const;
	
	// ������� ���������� ��������� ��� �� ���������� ������ �� ������������ - ������ ������, ������ ���������� � ������, ������ ���� � ����������
	bool getTSByIndexFromClass (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS) const;	
	
	// ������� ���������� ��������� ��� �� ��������� ������� �� ������������ - ������ ������, ������ ���������� � ������, ������ ���� � ����������
	bool getTSByIndexFromTest (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS) const;
	
	// ������� ���������� ��������� ��� ����������� ��������� �� ��������� ������� �� ������������ - ������ ���������� � ������, ������ ���� � ����������
	bool getNormalTSFromClass (std::vector<double> &vec, int indexMultiTS, int indexTS) const;

	// ������� ���������� ��������� ��� ����������� ��������� �� ����������� ������� �� ������������ - ������ ���������� � ������, ������ ���� � ����������
	bool getNormalTSFromTest (std::vector<double> &vec, int indexMultiTS, int indexTS) const;
	
//	// ������� ������������� ����� ��� ��������� ���������
//	void setClippingIntervals(const IntervalSetPair &intervals);
	
//	// ���������� ����� ���������� ��������� ��� ��������� �������
//	const IntervalSetPair &getClippingIntervals() const;
	
	// ������������� �������� ��������� ��� ��������� �������, ������ � ����������
	void setClippingInterval(DataSetDivisionType division, const IntInterval &interval, int classNo, int multiTSNo);
	
	// ������� ������������� �������� ��������� ��� �������� ��������� ���������� 
	// (classNo = -1 - ���������� ���������)
	void setClassClippingInterval(const IntInterval &interval, int classNo, int multiTSNo);
	
	// ������� ������������� �������� ��������� ��� �������� �������� ����������
	// (classNo = -1 - ���������� ���������)
	void setTestClippingInterval(const IntInterval &interval, int classNo, int multiTSNo);
	
	IntInterval getClippingInterval(DataSetDivisionType division, int classNo, int multiTSNo) const;
	
	// ������� ���������� �������� ��������� ��� �������� ��������� ���������� 
	IntInterval getClassClippingInterval(int classNo, int multiTSNo) const;
		
	// ������� ���������� �������� ��������� ��� �������� �������� ���������� 
	IntInterval getTestClippingInterval(int classNo, int multiTSNo) const;
	
	// ���������� ����� ����������
	int getMultiTSLength(DataSetDivisionType division, int classNo, int multiTSNo) const;
	
	// ������� ���������� ����� ����������
	int getClassMultiTSLength(int classNo, int multiTSNo) const;
	
	// ������� ���������� ����� ����������
	int getTestMultiTSLength(int classNo, int multiTSNo) const;
	
	// ����� ������� ���������� ���������
	int getClassCount() const;

	// ����� ����� ������������ (������� �����)
	int getDimensionCount() const;
	
	// ����� ����������� � ������
	int getMutiTSCount(DataSetDivisionType division, int classNo) const;
	
	// ��������� �������� ������� ���������� ��������� ��� ��������� �������
	bool getSize(DataSetDivisionType division, int &v1, std::vector<int> &v2, std::vector<std::vector<int> > &v3) const;

	// ��������� �������� ������� ���������� ��������� reference
	bool getClassSize(int &v1, std::vector<int> &v2, std::vector<std::vector<int> > &v3) const;
	
	// ��������� �������� ������� ���������� ��������� testing
	bool getTestSize(int &v1, std::vector<int> &v2, std::vector<std::vector<int> > &v3) const;
	
	// ��������� ������� ������ ����������� ��������� ��������� �������
	bool getNormalSize(DataSetDivisionType division, int &v1, std::vector<int> &v2) const;
	
	// ��������� ������� ������ ����������� ��������� reference
	bool getNormalClassSize(int &v1, std::vector<int> &v2) const;
	
	// ��������� ������� ������ ����������� ��������� testing
	bool getNormalTestSize(int &v1, std::vector<int> &v2) const;
	
	// ��������� ������� ��������� ������ �� ��������� ������� (classNo = -1 -- ���������� ���������)
	bool getSizeForClass(DataSetDivisionType division, int classNo, std::vector<int> &v) const;
		
	void setNormalStr(EnvDataSet &env);
	
	void setNormalStr(const std::string& str);

	void bootstrapNormalBehavior(double p);
private:
	
	std::vector<DataSetDivision> divisions;
	
	void readNormalTS(const std::string &divisionPath, ClassTS &classTS);
	
	std::string normal;
	
};

};

#endif // CLIPPEDFULLFUZZYDATASET_H
