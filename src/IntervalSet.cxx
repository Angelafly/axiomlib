#include "IntervalSet.h"

using namespace AxiomLib;

IntervalSet::IntervalSet(const ClippedFullFuzzyDataSet &dataSet, 
						 ClippedFullFuzzyDataSet::DataSetDivisionType division) {
	m_intervals.reserve(dataSet.getClassCount() + 1);
	std::vector<int> sizeVector;
	
	for(int classNo = -1; classNo < dataSet.getClassCount(); classNo++) {
		dataSet.getSizeForClass(division, classNo, sizeVector);
		m_intervals.push_back(std::vector<IntInterval>());
		m_intervals.back().reserve(sizeVector.size());
		for(int multiTSNo = 0; multiTSNo < (int) sizeVector.size(); multiTSNo++) {
			m_intervals.back().push_back(
					dataSet.getClippingInterval(division, classNo, multiTSNo)
					);
		}
	}
}

IntervalSet IntervalSet::reset(ClippedFullFuzzyDataSet &dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division) {
	IntervalSet result(dataSet, division);
	
	for(int classNo = -1; classNo < dataSet.getClassCount(); classNo++) {
		int multiTSCount = dataSet.getMutiTSCount(division, classNo);
		for(int multiTSNo = 0; multiTSNo < multiTSCount; multiTSNo++) {
			dataSet.setClippingInterval(division, IntInterval(0, dataSet.getMultiTSLength(division, classNo, multiTSNo) - 1), classNo, multiTSNo);
		}
	}
	
	return result;
}

void IntervalSet::apply(ClippedFullFuzzyDataSet &dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division) const {
	//TODO check sizes
	int classCount = dataSet.getClassCount();
	std::vector<int> sizeVector;
	
	for(int classNo = -1; classNo < classCount; classNo++) {
		dataSet.getSizeForClass(division, classNo, sizeVector);
		int multiTSCount = sizeVector.size();
		
		for(int multiTSNo = 0; multiTSNo < multiTSCount; multiTSNo++) {
			dataSet.setClippingInterval(division, getClippingInterval(classNo, multiTSNo), classNo, multiTSNo);
		}
	}
}

/****************************************************************************
*				 getClippingInterval
*
*	Description: ���������� �������� ��������� ��� �������� ����������
*	Parameters:	 classNo, multiTSNo - ������� ������ � ����������
*				(classNo < 0 �������� ���������� ���������)
*	Returns:	 const IntInterval & - �������� ���������
*	Throws:		 std::range_error, ���� ������� �����������
*	Author:		wictor
*	History:	-
*
****************************************************************************/
const IntInterval &IntervalSet::getClippingInterval(int classNo, int multiTSNo) const {
	clipClassNo(classNo);
	return m_intervals.at(classNo + 1).at(multiTSNo);
}

/****************************************************************************
*				 setClippingInterval
*
*	Description: ������������� �������� ��������� ��� �������� ����������
*	Parameters:	 interval - ����� �������� ���������
*				classNo, multiTSNo - ������� ������ � ����������
*				(classNo < 0 �������� ���������� ���������)
*	Returns:	 -
*	Throws:		 std::range_error, ���� ������� �����������
*	Author:		wictor
*	History:	-
*
****************************************************************************/
void IntervalSet::setClippingInterval(const IntInterval &interval, int classNo, int multiTSNo) {
	clipClassNo(classNo);
	m_intervals.at(classNo + 1).at(multiTSNo) = interval;
}

/****************************************************************************
*				 getMultiTSCount
*
*	Description: ���������� ����� ���������� ��������� ������
*	Parameters:	 classNo - ������ ������
*				(classNo < 0 �������� ���������� ���������)
*	Returns:	 const IntInterval & - �������� ���������
*	Throws:		 std::range_error, ���� ������ �����������
*	Author:		wictor
*	History:	-
*
****************************************************************************/
int IntervalSet::getMultiTSCount(int classNo) const {
	clipClassNo(classNo);
	return m_intervals.at(classNo + 1).size();
}

void IntervalSet::clipClassNo(int &classNo) {
	if(classNo < -1) {
		classNo = -1;
	}	
}
