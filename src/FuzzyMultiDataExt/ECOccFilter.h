/****************************************************************************
*				ECOccFilter.h
*
*	Description: ��������� ������������� �� ������������� ��� ��
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef ECOCCFILTER_H
#define ECOCCFILTER_H

#include <utility>

#include "Heuristics.h"

namespace AxiomLib {

class ElemCondPlusStat;

namespace FuzzyMultiDataExt {

class ECOccFilter : public Heuristics
{
public:
    ECOccFilter();
	
	virtual std::string name() const;
	
	virtual int stage() const;
	virtual ECOccFilter* clone() const;
	
protected:
	
	virtual void initFromEnvImpl(const Environment &env);
	
	virtual void run(FuzzyMultiDataExtAlgorithm &algorithm, int classNo) const;
private:	
	
	//static bool ecCompare(const AxiomLib::ElemCondPlusStat& ec1, const AxiomLib::ElemCondPlusStat& ec2);
		
	// ������ �� ������������ �������������
	double m_offset;
	// ����������� ���������
	int m_requiredSaturation;
	// ������������ ���������
	int m_maxSaturation;
	// ������������� ����� �� ��� ������� ���� ���������� ���������
	int m_maxECs;
};

}

}

#endif // ECOCCFILTER_H
