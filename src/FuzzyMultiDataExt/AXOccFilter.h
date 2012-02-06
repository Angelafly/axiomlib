/****************************************************************************
*				AXOccFilter.h
*
*	Description: ��������� ������������� �� ������������� ��� ������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef AXOCCFILTER_H
#define AXOCCFILTER_H

#include "Heuristics.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class AXOccFilter : public Heuristics
{
public:
    AXOccFilter();
	
	// ��� ���������
	virtual std::string name() const;
	
	// ������, �� ������� ����������� ���������
	virtual int stage() const;
	
	// ����������� ����������� �����������
	virtual Heuristics* clone() const;
	
private:
	double m_threshold;
protected:
	virtual void initFromEnvImpl(const Environment &env);
	
	// ������ ��������� �� ���������
	virtual void run(FuzzyMultiDataExtAlgorithm& algorithm, int aType) const;	
};

}

}

#endif // AXOCCFILTER_H
