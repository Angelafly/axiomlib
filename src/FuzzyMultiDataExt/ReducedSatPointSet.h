/****************************************************************************
*				ReducedSatPointSet.h
*
*	Description: ����� ����� ���������� ��� ���������� ����������� ��������� � ���������� ������
*				������ ���������� ���������. ������������ �� ������ ������� ������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef REDUCEDSATPOINTSET_H
#define REDUCEDSATPOINTSET_H

#include "../SatPointSet.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class ReducedSatPointSet
{
public:
	ReducedSatPointSet() {}
    ReducedSatPointSet(PSatPointSet normal, PSatPointSet abnormal);
	
	// ����������� ����������� �� ��������� ������������ shallow copy
	// ������ ����� ������������, ����� ����� deep copy
	ReducedSatPointSet deepCopy() const;
	
	// �������� ������������ �� ��������� ������������ shallow copy
	// ������ ����� ������������, ����� ����� deep copy
	void assign(const ReducedSatPointSet& other);
	
	ReducedSatPointSet& operator |=(const ReducedSatPointSet& other);
	ReducedSatPointSet& operator &=(const ReducedSatPointSet& other);
	
	bool operator==(const ReducedSatPointSet& other) const;
	
	const SatPointSet& normal() const { return *m_normal; }
	const SatPointSet& abnormal() const { return *m_abnormal; }
	
	bool isNull() const { return m_normal == PSatPointSet(); }
private:
	PSatPointSet m_normal;
	PSatPointSet m_abnormal;
};

}

}

#endif // REDUCEDSATPOINTSET_H
