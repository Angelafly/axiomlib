/****************************************************************************
*			SimpleCrossoverStrategy
*
*	Description:	������� ��������� �����������: ������ �� ������ ����������
*			������������ �������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Crossover_STRATEGY_SIMPLE_HXX
#define __AXIOMS_LIB_Crossover_STRATEGY_SIMPLE_HXX

#include "CrossoverStrategy.h"
#include <string>
#include "AxiomSetPop.h"

namespace AxiomLib {

class SimpleCrossoverStrategy : public CrossoverStrategy{

 private:

 protected:

  double pCross; // ������� �����������

 public:

  // ����������� ����������� - ������
  SimpleCrossoverStrategy (void) {pCross = -1;};

  // ����������� ���������� - ������
  ~SimpleCrossoverStrategy () {};

  // ���������� ��� ������� ������ ��������
  std::string name (void) const {return (std::string)"CrossoverSimple";};

  // ������� ������� �������� ���������� ������
  int setParamsFromEnv (const Environment& env);
  int setParams (const double newCrossLevel);
  int setParams (const double newElemCondCrossLevel, const double newAxiomCrossLevel, const double newAxiomSetCrossLevel);

  // �������� �������� �������� ��� ������ ����������
  int crossover (AxiomSetPop& pop);
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Crossover_STRATEGY_HXX */
