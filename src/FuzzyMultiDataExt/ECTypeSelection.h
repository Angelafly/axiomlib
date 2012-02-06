/****************************************************************************
*				ECTypeSelection.h
*
*	Description: �������� ������� ������������ ����� ��, �������� ��� ���������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef ECTYPESELECTION_H
#define ECTYPESELECTION_H

#include <set>
#include <string>

#include "Heuristics.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class ECTypeSelection : public Heuristics
{
public:
    ECTypeSelection();
	
	// ��� ���������
	virtual std::string name() const;
	
	// ������, �� ������� ����������� ���������
	virtual int stage() const;
	
	// ����������� ����������� �����������
	virtual ECTypeSelection* clone() const;
	
	// ������ ��������� �� ���������
	virtual void run(FuzzyMultiDataExtAlgorithm& algorithm) const;
	
protected:	

	void initFromEnvImpl(const Environment &env);

private:
	enum SelectionPolicy { Exclusive = 0, /* ���������� ��� ����� ��������� � ������� */ 
		                   Inclusive = 1 /* ���������� ������ ��������� � ������� */ };
	
	struct compare {
		bool operator()(const std::pair<bool, std::string>& p1, const std::pair<bool, std::string>& p2) const;
	};
	
	SelectionPolicy m_selectionPolicy;
	std::set<std::pair<bool, std::string>, compare> m_ecTypes;
	
	void selectExclusive(FuzzyMultiDataExtAlgorithm& algorithm) const;
	void selectInclusive(FuzzyMultiDataExtAlgorithm& algorithm) const;
};

}

}

#endif // ECTYPESELECTION_H
