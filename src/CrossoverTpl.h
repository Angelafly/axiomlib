/****************************************************************************
*					CrossoverTpl
*
*	Description:	����� ����������� ������ ������ ��� �������� �������� ��� ��������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_CROSSOVER_TPL_HXX
#define __AXIOMS_LIB_CROSSOVER_TPL_HXX

#include "Environment.h"
#include "Axiom.h"
#include "AxiomSet.h"
#include "AxiomSetPlus.h"
#include "AxiomLibException.h"
#include "CrossoverStrategy.h"
#include "Check.h"
#include "Round.h"
#include <string>
#include <vector>

namespace AxiomLib {

/*struct Pair {
	unsigned short first;
	unsigned short second;
};

struct Quad {
	unsigned short first;
	unsigned short fax;
	unsigned short second;
	unsigned short sax;
};

struct QuadPlus {
	unsigned short first;
	unsigned short fax;
	unsigned short second;
	unsigned short sax;
	std::string ecName;
};*/

class CrossoverTpl {

 private:

	std::vector < std::vector <Axiom *>* > axioms;

	std::vector < std::vector <double> > axiomsProfit;

	std::vector < std::vector <unsigned short> > axiomsMask;

	std::vector < std::vector <double> > axiomsMaskProb;

 protected:

	// ��������������� ������� ��� ����������� :
	// ���������� ������ ��� ������ ������ ��� ����������� 
	// � ���������������� ����������� �� ��������� �������

	// ������� ������ ��� ������ ������ ��� ����������� �� ������ ������ ������
	int choosePairs (std::vector <Pair> &asPairs, const std::vector <AxiomSetPlus> &population);

	// ������� ����������� ���� ��������� ������ ������ �� ������ ������ ������
	int crossAxiomSets (unsigned short as1, unsigned short as2, AxiomSetPlus &axiomSetPlus);

	// ������� ������ ��� ������ ��� ����������� 
	int chooseAxQuads (std::vector <Quad> &axQuads, const std::vector <AxiomSetPlus> &population);

	// ������� ����������� ���� ��������� ������ �� ������ ������
	int crossAxioms (unsigned short as1, unsigned short ax1, unsigned short as2, unsigned short ax2, AxiomSetPlus &axiomSetPlus);

	// ������� ������ ��� ������������ �������
	int chooseECQuads (std::vector <QuadPlus> &ecQuads, const std::vector <AxiomSetPlus> &population);

	// ����������� ���� ��������� ��� ������������ ������� �� ������ ���������� ������������� �������
	int crossECs (unsigned short as1, unsigned short ax1, unsigned short as2, unsigned short ax2, std::string &ecName, AxiomSetPlus &axiomSetPlus);

	// ������� ���������� � ��������� ��������� ������ ������
	inline void addSets (std::vector <AxiomSetPlus> &newPop, const std::vector <AxiomSetPlus> &population, std::vector < std::vector <AxiomSetPlus> > &newSets, std::vector < std::vector <int> > &setsMask) const;

	// ��������� �������� ������� vec � ������� ��������������� ������������������ �������� � mask
	inline void sort (std::vector <double> &vec, std::vector <unsigned short> &mask);

 public:

	// ����������� - � �������� ���������� ������ �� ��������
	CrossoverTpl ();

	// ���������� - ������������ ��� ��������� � ������ ������� ���������� ������ ������������ �������
	~CrossoverTpl ();

	// ���������� ��� ������� ������ �������� ������� �������
	std::string name (void) const;

	// ������� ������� �������� ���������� ������� ������
	int setParamsFromEnv (const Environment& env);

	// ������� ������������� ������ ��������� ����������� - ��������� ������� ������ �� ������ ������
	void initCrossover (const std::vector <AxiomSetPlus> &population);
	
	// ������� ����������� ��������� ������ ������ �� ������� ������
	int crossover (const std::vector <AxiomSetPlus> &population, std::vector <AxiomSetPlus> &toRet);

}; // of class

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_CROSSOVER_TPL_HXX */
