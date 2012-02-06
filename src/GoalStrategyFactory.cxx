/****************************************************************************
*
*			������� ������ GoalStrategyFactory
*
****************************************************************************/

#include "GoalStrategyFactory.h"
#include "PrimeGoalStrategy.h"
#include "AxiomLibException.h"

using namespace AxiomLib;
using namespace std;

/****************************************************************************
*			GoalStrategyFactory::create
*
*	Description:	������� ����� ���������� ����
*	Parameters:	name - ��� ������, ��������� �������� �������
*	Returns:	Goal* - ��������� �� ��������� �����
*	Throws:		AxiomLibException - ���� ������ ���
*	Author:		dk
*	History:
*
****************************************************************************/
GoalStrategy* GoalStrategyFactory::create (const std::string name) {
  if (name == "GoalFunctionPrime") {
    PrimeGoalStrategy* toReturn;
    toReturn = new PrimeGoalStrategy;
    return toReturn;
  }
  throw AxiomLibException ("GoalStrategyFactory::create : can not find the requested Goal strategy.");
}
