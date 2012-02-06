/****************************************************************************
*				AXStatClustering.h
*
*	Description: ��������� ������������� �� �������������� ��������������� ��� ������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef AXSTATCLUSTERING_H
#define AXSTATCLUSTERING_H

#include "StatClustering.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class AXStatClustering : public StatClustering
{
public:
    AXStatClustering();
	
	// ��� ���������
	virtual std::string name() const;
	
	// ������, �� ������� ����������� ���������
	virtual int stage() const;
	
	// ����������� ����������� �����������
	virtual AXStatClustering* clone() const;
	
protected:
	// ������ ��������� �� ���������
	virtual void run(FuzzyMultiDataExtAlgorithm& algorithm, int classNo) const;
	
};

}

}

#endif // AXSTATCLUSTERING_H
