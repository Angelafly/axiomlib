/****************************************************************************
*				IntervalSet.h
*
*	Description: � ������ ����� ������ �����, ���������� ����� ����������
*				���������, ��������������� ���������� � ����������� ���������,
*				� ����� ���������, ��������������� ������ ���������� ���������
*				��� ��������� � ��� ����������� �������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef INTERVALSET_H
#define INTERVALSET_H

#include "ClippedFullFuzzyDataSet.h"
#include "IntInterval.h"

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

namespace AxiomLib {
	
// ����� ��� ������ ���������� ��������� ���������� � ����������� ���������
class IntervalSet {
private:
	// ����������, ����������� ��� ������������
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & archive, const unsigned int /*version*/) {
		archive & BOOST_SERIALIZATION_NVP(m_intervals);
	}

	// ����� ���������� ��������� ��� ���������� ���������� ���������
	std::vector<std::vector<IntInterval> > m_intervals;
	
	static void clipClassNo(int& classNo);
	
	IntervalSet() {}
public:
	
	IntervalSet(const ClippedFullFuzzyDataSet& dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division);
	
	// ��������� ��������� � ���������� �� � dataset'�
	static IntervalSet reset(ClippedFullFuzzyDataSet& dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division);
	
	// ��������� ������ ����� ����������� � ��������
	void apply(ClippedFullFuzzyDataSet& dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division) const;
	// ���������� �������� ��������� ��� ��������� ������ � ���������
	// (classNo < 0 �������� ���������� ���������)
	const IntInterval &getClippingInterval(int classNo, int multiTSNo) const;
	// ������������� �������� ��������� ��� ��������� ������ � ���������
	// (classNo < 0 �������� ���������� ���������)
	void setClippingInterval(const IntInterval &interval, int classNo, int tsNo);
	
	// ���������� ����� ���������� ��������� ������
	int getMultiTSCount(int classNo) const;
};

}

#endif // INTERVALSET_H
