/****************************************************************************
*
*			������� ������ CrossoverStrategyFactory
*
****************************************************************************/

#include "CrossoverStrategyFactory.h"

using namespace AxiomLib;
using namespace std;

/****************************************************************************
*			CrossoverStrategyFactory::create
*
*	Description:	������� ����� ��������
*	Parameters:	name - ��� ������, ��������� �������� �������
*	Returns:	Crossover* - ��������� �� ��������� �����
*	Throws:		AxiomLibException - ���� ������ ���
*	Author:		dk
*	History:
*
****************************************************************************/
CrossoverStrategy* CrossoverStrategyFactory::create (const std::string name) {
	if (name == "CrossoverSimple") {
		SimpleCrossoverStrategy* toReturn;
		toReturn = new SimpleCrossoverStrategy;
		return toReturn;
	}
	if (name == "CrossoverTry") {
		CrossoverTry* toReturn;
		toReturn = new CrossoverTry;
		return toReturn;
	}
	throw AxiomLibException ("CrossoverStrategyFactory::create : can not find the requested crossover strategy.");
}
