/****************************************************************************
*			PrimeGoalStrategy
*
*	Description:	������� ������ PrimeGoalStrategy - ����������
*	Author:		gevor
*	History:		
*
****************************************************************************/
#include "PrimeGoalStrategy.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			PrimeGoalStrategy::setParamsFromEnv
*
*	Description:	��������� ���������� ������� ������� �� �����
*	Parameters:	const Environment& env - ������ �������
*	Returns:	0
*	Throws:		AxiomLibException - ���� ��������� ���
*	Author:		gevor
*	History:
*
****************************************************************************/
int PrimeGoalStrategy::setParamsFromEnv (const Environment& env) {
	if ( (env.getDoubleParamValue(gcFirst, "gcFirst") < 0) ||
		(env.getDoubleParamValue(gcSecond, "gcSecond") < 0 ) ) {
			throw AxiomLibException ("Error in GoalFunctionPrime::setParamsFromEnv - can not find all needed parameners.");
	}
	if (gcFirst < 0)
		throw AxiomLibException ("Error in GoalPrime::setParamsFromEnv - gcFirst is negative.");
	if (gcSecond < 0)
		throw AxiomLibException ("Error in GoalPrime::setParamsFromEnv - gcSecond is negative.");
	return 0;
}


/****************************************************************************
*			PrimeGoalStrategy::compute
*
*	Description:	������� �� ���������� ������ I � II ���� � ���������, goal function ������ ������
*	Parameters:	AxiomSetPop - ���������� ���������
*	Returns:		0
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int PrimeGoalStrategy::compute (AxiomSetPop& pop) const {
	// �������� ���������� �� ������������ ��������
	if ( (gcFirst < 0) || (gcSecond < 0) )
		throw AxiomLibException ("Error in GoalPrime::compute - params uninitialized.");

	int nFirst, nSecond;
	double goal;
	// ����� �� ���, ��������� ����
	for (int i = 0; i < pop.size(); i++) {
		pop.getStats(i, nFirst, nSecond, goal);
		if (nFirst < 0 || nSecond < 0) {
			std::cout << "\n nFirst is " << nFirst;
			std::cout << "\n nSecond is " << nSecond << "\n";
			throw AxiomLibException ("Error in GoalPrime::compute : negative number of errors in the population. Probably, it is uninitialized.");
		}
		goal = compute (nFirst, nSecond);
		pop.setStats(i, nFirst, nSecond, goal);
	}

	return 0;
}


/****************************************************************************
*			PrimeGoalStrategy::compute
*
*	Description:	C������ �������� ������� ������� �� ���������� ������ I � II ����
*	Parameters:		nFirst - ����� ������ I ����
*					nSecond - ����� ������ II ����
*	Returns:		double - ���������� �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
double PrimeGoalStrategy::compute (int nFirst, int nSecond) const {
	double toRet;
	toRet = ((double) nFirst)*gcFirst + ((double) nSecond)*gcSecond;
	return toRet;
}
