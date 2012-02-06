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

#include "ClippedFullFuzzyDataSet.h"
#include "AxiomExpr.h"
#include "ElemCondPlus.h"

namespace AxiomLib {

//����� ����� ������������ ��� ���������� ������
class SatPointSet
{
public:
	// �������� ������ ����� ������������ �� �� � ��������
    SatPointSet(const ElemCondPlus &ecPlus, const ClippedFullFuzzyDataSet &dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division, 
				int classNo);
	
	// �������� ������ ����� ������������ �� ������� � ��������
	SatPointSet(const AxiomExpr &axiom, const ClippedFullFuzzyDataSet &dataSet, ClippedFullFuzzyDataSet::DataSetDivisionType division, int classNo);
		
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
	
	ClassSatVector m_satPoints;
};

typedef boost::shared_ptr<SatPointSet> PSatPointSet;

};

#endif // SATPOINTSET_H
