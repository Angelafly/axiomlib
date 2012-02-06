/****************************************************************************
*			GoalStrategy
*
*	Description:	���������� ������� ������� (� ����������� �� ����� ������ I � II ����)
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Goal_STRATEGY_HXX
#define __AXIOMS_LIB_Goal_STRATEGY_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include <string>
#include <vector>

namespace AxiomLib {

class GoalStrategy {

 private:

 protected:
 
 public:

  // ����������� ����������� - ������
  GoalStrategy (void) {}

  // ����������� ���������� - ������
  virtual ~GoalStrategy () {}

  // ���������� ��� ������� ������ ��������
  virtual std::string name (void) const = 0;

  // ������� ������� �������� ���������� ������
  virtual int setParamsFromEnv (const Environment& env) = 0;

  // �������� ���������� ������� ������� ��� ���� ��������� ���������
  virtual int compute (AxiomSetPop& pop) const = 0;

  // �������� ���������� ������� ������� �� ����� ������ ������� � ������� ����
  virtual double compute (int nFirst, int nSecond) const = 0;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Goal_STRATEGY_HXX */
