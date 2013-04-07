/****************************************************************************
*			SatPointSet.h
*
*	Description: �������� ������, ���������������� ��� �������� ������ ����� 
*				���������� �� ��� �������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef SATPOINTSET_H
#define SATPOINTSET_H

#include <vector>

#include <boost/shared_ptr.hpp>

#include "FuzzyDataSet.h"
#include "AxiomExpr.h"
#include "ElemCondPlus.h"

namespace AxiomLib {

// ����� ����� ������������ ��� ���������� ������ (������ ����������)
class SatPointSet
{
public:

	enum SatPointSetType {
		ClassSatPoints,
		ComplementarySatPoints
	};

	template<class Axiom>
	SatPointSet(const Axiom &axiom, const FuzzyDataSet &dataSet, FuzzyDataSet::DataSetDivisionType division,
				int classNo, SatPointSetType satPointSetType = ClassSatPoints);
		
	// ����� ����������
	int size() const { return m_satPoints.size(); }
	
	// ����� ����� (�����) ����������
	int size(int multiTSNo) const { return m_satPoints[multiTSNo].size(); }

	// ��������� �� ������� � ����� ����������
	bool isSat(int multiTSNo, int i) const { return (bool)m_satPoints[multiTSNo][i]; }
	
	// �������� ������� �� &
	SatPointSet &operator&=(const SatPointSet& other);
	
	// �������� ������� �� |
	SatPointSet &operator|=(const SatPointSet& other);
	
	bool operator == (const SatPointSet& other) const;
	
	// ����� �����, �� ������� ������� �����������
	int numSat(int multiTSNo) const;
	
private:
	typedef char SatValue;
	typedef std::vector<SatValue> SatVector;
	typedef std::vector<SatVector> ClassSatVector;

	SatPointSet();
	
	ClassSatVector m_satPoints;
};

typedef boost::shared_ptr<SatPointSet> PSatPointSet;

template<class Axiom>
SatPointSet::SatPointSet(const Axiom &axiom, const FuzzyDataSet &dataSet, FuzzyDataSet::DataSetDivisionType division,
			int classNo, SatPointSet::SatPointSetType satPointSetType) {

	if(classNo < -1 || classNo >= dataSet.getClassCount()) {
		classNo = -1;
	}

	std::vector<int> classes;
	classes.reserve(dataSet.getClassCount());

	switch(satPointSetType) {
	case SatPointSet::ClassSatPoints:
		classes.push_back(classNo);
		break;
	case SatPointSet::ComplementarySatPoints:
		for(int i = -1; i < dataSet.getClassCount(); ++i) {
			if(i != classNo) {
				classes.push_back(i);
			}
		}
		break;
	default: throw AxiomLibException("SatPointSet::SatPointSet() : unexpected satPointSetType : "
									 + boost::lexical_cast<std::string>(satPointSetType));
	}

	if(classes.empty()) {
		return;
	}

	int multiTSCount = 0;

	for(size_t i = 0; i < classes.size(); ++i) {
		multiTSCount += dataSet.getMutiTSCount(division, classes[i]);
	}

	m_satPoints.reserve(multiTSCount);

	std::vector<std::vector<double> > multiTS;

	int dimensionCount = dataSet.getDimensionCount();

	std::vector<bool> dimensions(dimensionCount);

	axiom.getDimensions(dimensions);

	// �������� ������ ����������
	size_t currentMultiTS = 0;

	for(size_t i = 0; i < classes.size(); ++i) {
		int currentClass = classes[i];

		for(int classMultiTSNo = 0; classMultiTSNo < dataSet.getMutiTSCount(division, currentClass); classMultiTSNo++) {
			multiTS.resize(dimensionCount);
			for(int tsNo = 0; tsNo < dimensionCount; tsNo++) {
				if(dimensions[tsNo]) {
					dataSet.getTSByIndex(division, multiTS[tsNo], currentClass, classMultiTSNo, tsNo);
				}
			}

			int multiTSLen = (int) dataSet.getMultiTSLength(division, currentClass, classMultiTSNo);

			m_satPoints.push_back(SatVector(multiTSLen));

			for(int i = 0; i < multiTSLen; i++) {
				m_satPoints.back()[i] = (SatValue) (axiom.check(i, multiTS) == 1);
			}

			++currentMultiTS;
		}
	}
}

};

#endif // SATPOINTSET_H
