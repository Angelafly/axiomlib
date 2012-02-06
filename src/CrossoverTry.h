/****************************************************************************
*				CrossoverTry
*
*	Description:	����� ����������� ������ ������ - ������� ��������
*				������ ����������� �� ���� �������:
*					- ������� ������������ �������
*					- ������� ������
*					- ������� ������ ������
*				��������� ����������� ��� ������� �� ������� 
*				����������� �� ������������������ ����� ��� 
*				�������� ����������� ��������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_CROSSOVER_TRY_HXX
#define __AXIOMS_LIB_CROSSOVER_TRY_HXX

#include "CrossoverStrategy.h"
#include "Check.h"
#include <iostream>

namespace AxiomLib {

class CrossoverTry : public CrossoverStrategy {

   private:

	// ������� ����������� �� ������ �� ������� �����������
	// ������ �� ������� ����� ��������� �������� 
	// �� 0,0 (���������� ����������� �� ������ ������) �� 1,0 (����������� ��������� ������� �����������).
	double elemCondCrossLevel;
	double axiomCrossLevel;
	double axiomSetCrossLevel;

   protected:
   
	// ��������������� ������� ��� ����������� :
	// ���������� ������ ��� ������ ������ ��� ����������� 
	// � ���������������� ����������� �� ��������� �������
	
	// ������� ������ ��� ������ ������ ��� ����������� �� ������ ������ ������
	int choosePairs (std::vector <Pair> &asPairs);
	
	// ������� ����������� ���� ��������� ������ ������ �� ������ ������ ������
	int crossAxiomSets (std::vector<Axiom *>* axSet1, std::vector<Axiom *>* axSet2, AxiomSetWithStats &axiomSet);
	
	// ������� ������ ��� ������ ��� ����������� 
	int chooseAxQuads (std::vector <Quad> &axQuads);
	
	// ������� ����������� ���� ��������� ������ �� ������ ������
	int crossAxioms (Axiom* ax1, Axiom* ax2, AxiomSetWithStats &newAxiomSet);
	
	// ������� ������ ��� ������������ �������
	int chooseEcQuads (std::vector <QuadPlus> &ecQuads);
	
	// ����������� ���� ��������� ��� ������������ ������� �� ������ ���������� ������������� �������
	int crossEcs (Axiom* ax1, Axiom* ax2, std::string &ecName, AxiomSetWithStats &newAxiomSet);

   public:

	// ����������� - � �������� ���������� ������ �� ��������
	CrossoverTry (void);

	// ���������� - ������������ ��� ��������� � ������ ������� ���������� ������ ������������ �������
	~CrossoverTry () {};

	// ���������� ��� ������� ������ �������� ������� �������
	std::string name (void) const;

	// ������� ������� �������� ���������� ������� ������
	int setParamsFromEnv (const Environment& env);
	int setParams (const double newCrossLevel);
	int setParams (const double newElemCondCrossLevel, const double newAxiomCrossLevel, const double newAxiomSetCrossLevel);
	int setParams (std::map<std::string, std::string> &params);

	// ������� ����������� ��������� ������ ������ �� ������� ������
	int crossover (AxiomSetPop &axiomSetPop);

}; // of class

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_CROSSOVER_TRY_HXX */
