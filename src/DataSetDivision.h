/****************************************************************************
*				DataSetDivision
*
*	Description: �����, ��������������� ������� ������ ������
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef DATASETDIVISION_H
#define DATASETDIVISION_H

#include <vector>
#include <boost/tuple/tuple.hpp>

#include "MultiTS.h"
#include "IntInterval.h"

namespace AxiomLib {

class DataSetDivision
{
public:
    DataSetDivision(const ReferenceClassesTS &abnormalTS, const ClassTS &normalTS);
	
	// ���������� ����� ������� ���������� ���������
	int getClassCount() const; 
	
	// ����� ����������� � ������
	int getMultiTSCount(int classNo) const;
	
	// ���������� ���������� ��� �� ��������
	bool getTSByIndex(std::vector<double> &v, int behaviourType, int multiTSNo, int tsNo, bool clip = true) const;
	
	// ���������� ����� ����������
	int getMultiTSLength(int behaviourType, int multiTSNo) const;
	
	// ������ � ���������� �����������
	IntInterval getClippingInterval(int behaviourType, int multiTSNo) const;
	void setClippingInterval(const IntInterval &interval,int behaviourType, int multiTSNo);
	
	// ���������� ����������� ���������� ���������� ��������� ��� ������� ������ ���������� ���������
	bool getAbnormalSize(std::vector<std::vector<int> > &v) const;
	
	// ���������� ����������� ���������� ��������� ������
	bool getClassSize(int classNo, std::vector<int> &v) const;
	
	// ���������� ����������� ���������� ����������� ���������
	bool getNormalSize(std::vector<int> &v) const;
private:
	typedef boost::tuples::tuple<MultiTS, IntInterval> ClippedMultiTS;
	typedef std::vector<ClippedMultiTS> ClippedClassTS;
	typedef std::vector<ClippedClassTS> ClippedClassTSSet;
	
	ClippedClassTSSet m_tsSet;
	
	const ClippedClassTS &getClippedClassTS(int behaviourType) const;
	const ClippedMultiTS &getClippedMultiTS(int behaviourType, int multiTSNo) const;
	
	ClippedClassTS &getClippedClassTS(int behaviourType);
	ClippedMultiTS &getClippedMultiTS(int behaviourType, int multiTSNo);
	
};

}

#endif // DATASETDIVISION_H
