/****************************************************************************
*			SimpleCrossoverStrategy
*
*	Description:	������� ������ SimpleCrossoverStrategy - ����������
*	Author:		gevor
*	History:		
*
****************************************************************************/
#include "SimpleCrossoverStrategy.h"
#include "AxiomLibException.h"
#include "GaOps.h"

using namespace AxiomLib;
using namespace std;

/****************************************************************************
*			SimpleCrossoverStrategy::setParamsFromEnv
*
*	Description:	��������� ���������� ������� �� �����
*	Parameters:	const Environment& env - ������ �������
*	Returns:	0
*	Throws:		AxiomLibException - ���� ��������� ���
*	Author:		gevor
*	History:
*
****************************************************************************/
int SimpleCrossoverStrategy::setParamsFromEnv (const Environment& env)
{
  if (env.getDoubleParamValue(pCross, "crossLevel") < 0) {
    throw AxiomLibException ("Error in CrossoverSimple::setParamsFromEnv - can not find all needed parameners.");
  }
  if (pCross < 0 || pCross > 1)
    throw AxiomLibException ("Error in CrossoverSimple::setParamsFromEnv - pCross out of range.");
  return 0;
}


/****************************************************************************
*				SimpleCrossoverStrategy::setParams
*
*	Description:	������� ������� �������� ���������� 
*	Parameters:	����� ��������� �����������:
*				newElemCondCrossLevel - �������� ����������� �� ������ ������������ ������� 
*				newAxiomCrossLevel - �������� ����������� �� ������ ������
*				newAxiomSetCrossLevel - �������� ����������� �� ������ ������ ������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� �������� ���������� ������������
*	Author:		dk
*	History:
*
****************************************************************************/
int SimpleCrossoverStrategy::setParams (const double newElemCondCrossLevel, const double newAxiomCrossLevel, const double newAxiomSetCrossLevel) {
	// �������� �� ������������ ��������� ��������� ����������
	if (	(newElemCondCrossLevel < 0.0) || (newElemCondCrossLevel > 1.0) ||
				(newAxiomCrossLevel < 0.0) || (newAxiomCrossLevel > 1.0) ||
				(newAxiomSetCrossLevel < 0.0) || (newAxiomSetCrossLevel > 1.0) 	) 
		throw AxiomLibException ("Error in SimpleCrossoverStrategy::setParams: some of crossLevel Parameters are out of range.");
	// ��������� ����� �������� ����������
	pCross = (newElemCondCrossLevel + newAxiomCrossLevel + newAxiomSetCrossLevel) / 3.0;
	return 0;
}


/****************************************************************************
*				SimpleCrossoverStrategy::setParams
*
*	Description:	������� ������� �������� ���������� 
*	Parameters:	����� ��������� �����������:
*				newCrossLevel - �������� ����������� �� ���� �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� �������� ���������� ������������
*	Author:		dk
*	History:
*
****************************************************************************/
int SimpleCrossoverStrategy::setParams (const double newCrossLevel) {
	// �������� �� ������������ ��������� ��������� ����������
	if ( (newCrossLevel < 0.0) || (newCrossLevel > 1.0) ) 
		throw AxiomLibException ("Error in SimpleCrossoverStrategy::setParams: given crossLevel is out of range.");
	// ��������� ����� �������� ����������
	pCross = newCrossLevel;
	return 0;
}


/****************************************************************************
*			SimpleCrossoverStrategy::crossover
*
*	Description:	������� ����������� ��� ������ ��������� - ���� ������ �� ������
*			���������� ������������ �������
*	Parameters:	AxiomSetPop - ���������� ���������
*	Returns:	0
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int SimpleCrossoverStrategy::crossover (AxiomSetPop& pop) 
{
  // �������, ��������� ��������� ����� - ���������� pCross ��������� ���������
  if (pCross < 0)
    throw AxiomLibException ("Error in CrossoverSimple::crossover - pCross unititialized.");

  // �������� ������ ������ ������
  initCrossover (pop);
  
  int popSize = asPop->size();
  int nCross = (int)(pCross * ((double)popSize)); // ������� ��������� � �����������
  int newSize = nCross;// + popSize; // ����� ������ ���������
  
  // ������� ����� ������
  vector<AxiomSetWithStats>* newAsPop = new vector<AxiomSetWithStats>;
  newAsPop->resize(newSize);
  
  // ���� ���� ������������ ������� - ��������� ������� �������� ���� � ����������, ��������� � ����� ������
  for (int i = 0; i < newSize; i++) {
    // �������� ��������� ������� 2 ������� ������
    int firstASNum = (int) (( (double) rand() / (double) RAND_MAX) * ((double)popSize  - 1.0));
    int secondASNum = (int) (( (double) rand() / (double) RAND_MAX) * ((double)popSize  - 1.0));

    // �������� ��������� ����, ������, ���� ��� ����� �������� ���������
    if (firstASNum >= popSize || secondASNum >= popSize || firstASNum < 0 || secondASNum < 0)
      throw AxiomLibException ("Error in CrossoverSimple::crossover - FATAL INTERNAL ERROR 1.");
    
    // ������ ����� �� ���� �������� ������, ������� ��������� �������, � � ��� - ������������
    // �������; ���� ����������� ������������ �� ����������� - ���-�� ����� ��������
    CrossoverOp_Params((*newAsPop)[i], (*asPop)[firstASNum], (*asPop)[secondASNum]);

//    (*newAsPop)[i] = (*asPop)[0];

    // �������� ���������� ��� ���������� �����������
    (*newAsPop)[i].voidStats();
  }
  
  // ��������� ����� �������� � ��������� ������ ������
  pop.addSets(*newAsPop);
  return nCross;
}
