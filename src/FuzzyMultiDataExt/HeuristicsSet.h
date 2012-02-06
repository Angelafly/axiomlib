/****************************************************************************
*				HeuristicsSet.h
*
*	Description: ����� �������� ���������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef HEURISTICSSET_H
#define HEURISTICSSET_H

#include <map>
#include <deque>

#include <boost/shared_ptr.hpp>

#include "../Environment.h"

#include "ForwardDeclarations.h"

#include "Heuristics.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

// �������� �� ���������� �������� �������� �� �������
// � ���������� �� � ������ ������� � ������ �������
class HeuristicsSet
{
public:
    HeuristicsSet();
	
	// ���������� -1, ���� �� ����� ��������� �� ������, ����� 0
	int initFromEnv(const Environment& env);
	
	void apply(FuzzyMultiDataExtAlgorithm* algorithm, int stage) const;
private:
	typedef boost::shared_ptr<Heuristics> PHeuristics;
	typedef std::deque<PHeuristics> HeuristicsVector;
	typedef std::map<int, HeuristicsVector> HeuristicsMap;
	
	HeuristicsMap m_heuristicsMap;

	static bool compareHeuristicsOrder(PHeuristics h1, PHeuristics h2);
};

};

};

#endif // HEURISTICSSET_H
