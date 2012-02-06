/****************************************************************************
*			PrimeGoalStrategy
*
*	Description:	������� ��������� �������� ������� �������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Goal_STRATEGY_Prime_HXX
#define __AXIOMS_LIB_Goal_STRATEGY_Prime_HXX

#include "GoalStrategy.h"
#include <string>
#include "AxiomSetPop.h"

namespace AxiomLib {

class PrimeGoalStrategy : public GoalStrategy{

 private:

 protected:

  // �-�� ��� ������� I � II ����
  double gcFirst;
  double gcSecond;

 public:

  // ����������� ����������� - ������
  PrimeGoalStrategy (void) {gcFirst = -1; gcSecond=-1;};

  // ����������� ���������� - ������
  ~PrimeGoalStrategy () {};

  // ���������� ��� ������� ������ ��������
  std::string name (void) const {return (std::string)"GoalFunctionPrime";};

  // ������� ������� �������� ���������� ������
  int setParamsFromEnv (const Environment& env);

  // �������� ���������� ������� ������� ��� ���� ��������� ���������
  int compute (AxiomSetPop& pop) const;

  // �������� ���������� ������� ������� �� ����� ������ ������� � ������� ����
  double compute (int nFirst, int nSecond) const;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Goal_STRATEGY_HXX */
