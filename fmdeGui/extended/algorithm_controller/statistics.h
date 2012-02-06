#ifndef STATISTICS_H
#define STATISTICS_H

//#include "AxiomExprPlus.h"
//#include "ElemCondPlusStat.h"
#include "FullFuzzyDataSet.h"

//��������� ���������� � �������� ������� ������� ������� ��� ��
template<class Axiom>
void updateStatistics(Axiom &axiomPlus, 
					  const AxiomLib::FullFuzzyDataSet &fullFuzzyDataSet);


#endif // STATISTICS_H
