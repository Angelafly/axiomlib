/****************************************************************************
*				ECStatClustering.h
*
*	Description: ��������� ������������� �� �������������� ��������������� ��� ��
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef ECSTATCLUSTERING_H
#define ECSTATCLUSTERING_H

#include "StatClustering.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class ECStatClustering : public StatClustering
{
public:
    ECStatClustering();
	
	// ��� ���������
	virtual std::string name() const;
	
	// ������, �� ������� ����������� ���������
	virtual int stage() const;
	
	// ����������� ����������� �����������
	virtual ECStatClustering* clone() const;
	
protected:	
	// ������ ��������� �� ���������
	virtual void run(FuzzyMultiDataExtAlgorithm& algorithm, int abType) const;
};

}

}

#endif // ECSTATCLUSTERING_H
