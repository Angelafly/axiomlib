/****************************************************************************
*			SimpleTransmuteStrategy
*
*	Description:	������� ������ SimpleTransmuteStrategy - ����������
*	Author:		gevor
*	History:		
*
****************************************************************************/
#include "SimpleTransmuteStrategy.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			SimpleTransmuteStrategy::setParamsFromEnv
*
*	Description:	��������� ���������� ������� �� �����
*	Parameters:	const Environment& env - ������ �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ��������� ���
*	Author:		dk
*	History:
*
****************************************************************************/
int SimpleTransmuteStrategy::setParamsFromEnv (const Environment& env) {
	double tempDB1, tempDB2, tempDB3, tempDB4;
	if ( (env.getDoubleParamValue(tempDB1, "elCondTransLevel") >= 0) && (env.getDoubleParamValue(tempDB2, "axiomTransLevel") >= 0) && (env.getDoubleParamValue(tempDB3, "axSysTransLevel") >= 0) && (env.getDoubleParamValue(tempDB4, "priorityTransLevel") >= 0) ) {
		if ( (tempDB1 < 0) || (tempDB1 > 1) || (tempDB2 < 0) || (tempDB2 > 1) || (tempDB3 < 0) || (tempDB3 > 1) || (tempDB4 < 0) || (tempDB4 > 1) )
			throw AxiomLibException ("Error in SimpleTransmuteStrategy::setParamsFromEnv - transmute levels out of range.");
		ectp = tempDB1;
		atp = tempDB2;
		stp = tempDB3;
		sptp = tempDB4;
		//std::cout << "\n	paramsSet: " << ectp << " " << atp << " " << stp << " " << sptp;
		return 1;
	}
	if (env.getDoubleParamValue(tempDB1, "transLevel") >= 0) {
		if ((tempDB1 < 0) || (tempDB1 > 1))
			throw AxiomLibException ("Error in SimpleTransmuteStrategy::setParamsFromEnv - transLevel out of range.");
		ectp = tempDB1;
		atp = tempDB1;
		stp = tempDB1;
		sptp = tempDB1;
		return 2;
	}
	throw AxiomLibException ("Error in SimpleTransmuteStrategy::setParamsFromEnv - can not find all needed parameners.");
	return -1;
}


/****************************************************************************
*				SimpleTransmuteStrategy::transmuteAxiomSet
*
*	Description:	������� ������� ������� ������
*	Parameters:	asNum - ����� ������� ������ � ������� axiomSets ��� �������
*	Returns:		0 - ���� ��� ��������� �������
*				-1 - ���� ������� �� ������
*	Throws:		AxiomLibExeption - ���� ������� ��������� �� ������������� ������� �� ������� [0, 1]
*	Author:		dk
*	History:
*
****************************************************************************/
int SimpleTransmuteStrategy::transmuteAxiomSet (unsigned int asNum) {
	//�������� �������� ���������� ������� �� ����������� ������� ��������
	if ((ectp < 0) || (ectp > 1) || (atp < 0) || (atp > 1) || (stp < 0) || (stp > 1) || (sptp < 0) || (sptp > 1)) {
		throw AxiomLibException("Error in SimpleTransmuteStrategy::transmuteAxiomSet : one of parameteres is out of range.");
		return -1;
	}

	//������� �� ������ ������ � ������������ �������
	if ((ectp > 0) || (atp > 0)) {
		int resTransmuteAxiom = 0;
		for (unsigned int i = 0; i < (axiomSets[asNum])->size(); i++) {
			resTransmuteAxiom = ((*(axiomSets[asNum]))[i])->transmute (ectp, atp);
		}
	}

	//������� �� ������ ������� ������
	if (stp > 0) {
		double probOfTp = 0;
		int numOfEC = 0;
		for (unsigned int i = 0; i < (axiomSets[asNum])->size(); i++) {
			probOfTp = (double) rand() / (double) RAND_MAX;
			if (probOfTp < stp) {
				numOfEC = ((*(axiomSets[asNum]))[i])->getNumOfEC();
				((*(axiomSets[asNum]))[i])->clear();
				((*(axiomSets[asNum]))[i])->initAxiomByRand(numOfEC);
			}
		}
	}


	//������� �� ������ ��������� ���������� ������ � �������
	if (sptp > 0) {
		int numOfSteps = 0;
		double probChange = 0.0;
		for (int i = 1; i < (int) (axiomSets[asNum])->size(); i++) {
			probChange = (double) rand() / (double) RAND_MAX;
			if (probChange < sptp) {
				numOfSteps = round( ((double) rand() / (double) RAND_MAX) * i * sptp );
				if (numOfSteps > 0) {
					((*asPop)[asNum]).slide(i-numOfSteps, i);
				}
			}
		}
	}

	return 0;
}


/****************************************************************************
*			SimpleTransmuteStrategy::transmute
*
*	Description:	������� ������� ��� ������ ��������� - ������� ������ �� ������ ������������ ������� (����)
*	Parameters:	AxiomSetPop - ���������� ���������
*	Returns:		0
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
int SimpleTransmuteStrategy::transmute (AxiomSetPop& pop) {
	// �������� ������������ ���������� ��������
	// ����� �� ���������, �������� ��, ������� ��������� ����������
	initTransmute (pop);
	for (unsigned int i = 0; i < axiomSets.size(); i++) {
		transmuteAxiomSet (i);
		(*asPop)[i].voidStats();
	}
	return 0;
}
