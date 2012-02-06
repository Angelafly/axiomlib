/****************************************************************************
*					TransmuteTpl
*
*	Description:	������� ������ TransmuteTpl - ����������
*	Author:			dk
*	History:		
*
****************************************************************************/

#include "TransmuteTpl.h"

using namespace AxiomLib;

#define minProfit 0.5 // ������� �������� ��� ��������� profit ��������� �������
#define epsilon 0.001 // ����� �������� ������� ��� ��������� double'��
#define cutOffCrit 0.8 // �������� ��� �������� ����������� ������� �������
#define cutOffVal 0.5 // �� ������� �������� �������� ��������� ����������� ������ �� ������ ������

// ����������� ������
TransmuteTpl::TransmuteTpl () {
	/*
	axioms.clear();
	axiomsProfit.clear();
	axiomsMask.clear();
	axiomsMaskProb.clear();		
	*/
	
	canMutateOrder = true;
}

// ���������� ������ - ������
TransmuteTpl::~TransmuteTpl () {
		
}

/****************************************************************************
*					TransmuteTpl::name
*
*	Description:	���������� ��� ������� ������ �������
*	Parameters:		-
*	Returns:		string - ��� ������ �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
std::string TransmuteTpl::name (void) const {
	return (std::string) "TransmuteTpl";
}


/****************************************************************************
*					TransmuteTpl::setParamsFromEnv
*
*	Description:	��������� ���������� ������� �� �����
*	Parameters:		const Environment& env - ���������, ������ �������� ���������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ��������� ���������� ���
*	Author:			dk
*	History:
*
****************************************************************************/
int TransmuteTpl::setParamsFromEnv (const Environment& env) {
	
	if(env.getBoolParamValue(canMutateOrder, "canMutateOrder"))
	{
		canMutateOrder = true;
	}
	
	return 0;
}


/****************************************************************************
*					TransmuteTpl::sort
*
*	Description:	��������� �������� ������� vec � ������� ��������������� 
*					������������������ �������� � mask
*	Parameters:		vec - ����������� ������
*					mask - ������������������ ��������
*	Returns:		-
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/ 
inline void TransmuteTpl::sort (std::vector <double> &vec, std::vector <unsigned short> &mask) const {
	mask.resize(vec.size()); 
	for (unsigned short i = 0; i < vec.size(); i++) 
		mask[i] = i;
	unsigned short temp;
	for (unsigned short i = vec.size() - 1; i > 0; i-- ) {
		for (unsigned short j = 0; j < i; j++) {
			if (vec[mask[j]] < vec[mask[j+1]]) {
				temp = mask[j+1];
				mask[j+1] = mask[j];
				mask[j] = temp;
			}
		}
	}
}


/****************************************************************************
*					TransmuteTpl::transmute
*
*	Description:	������� ������� ������� ������, �������� � �����������
*	Parameters:		axiomSetPlus - ����� ��� �������
*	Returns:		0 - ���� ��� ��������� �������
*					-1 - ���� ������� �� ������
*					1 - ���� �������� ���������� ������� � �� ������ ���� �����������
*	Throws:			AxiomLibExeption - ���� ��������� ������� ������� �� ������� 
*					������� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
int TransmuteTpl::transmute (AxiomSetPlus &axiomSetPlus) const {
	// �������� �������� ���������� ������� �� ����������� ������� ��������
	if ((axiomSetPlus.ecTransProb < 0) || (axiomSetPlus.ecTransProb > 1) || 
		(axiomSetPlus.axTransProb < 0) || (axiomSetPlus.axTransProb > 1) || 
		(axiomSetPlus.asTransProb < 0) || (axiomSetPlus.asTransProb > 1) || 
		(axiomSetPlus.prTransProb < 0) || (axiomSetPlus.prTransProb > 1)) {
		throw AxiomLibException("Error in TransmuteTpl::transmute : one or more parameteres is out of range.");
		return -1;
	}

	// �������� �������� ���������� ������� - ���������� �� �������
	if ((axiomSetPlus.ecTransProb < epsilon) &&
		(axiomSetPlus.axTransProb < epsilon) &&
		(axiomSetPlus.asTransProb < epsilon) &&
		(axiomSetPlus.prTransProb < epsilon || !canMutateOrder)) {
			return 1;
	}

	// ���������� ���������� ���������� 
	std::vector <Axiom *> * axioms;
	axioms = &(axiomSetPlus.axiomSet->axioms);
	std::vector <unsigned short> axiomsMask;
	if (axioms->size() != axiomSetPlus.axiomProfit.size())
		throw AxiomLibException("TransmuteTpl::transmute : axioms profit is not set.");
	// ������������� �������� �����, ������� �������� ������ ������ � ������� � ������� ���������� ������� ����������
	sort (axiomSetPlus.axiomProfit, axiomsMask);
	// �������� ������ ����� ������� ���������� �������, ����� ������ - ��� 80% �� ������������ ���������� ������� (80 - ��������)
	double cutOff;
	cutOff = axiomSetPlus.axiomProfit[axiomsMask[0]] * cutOffCrit;
	double axTransProbMin;
	axTransProbMin = axiomSetPlus.axTransProb * cutOffVal;

	// ������� �� ������ ������ � ������������ �������
	if ((axiomSetPlus.ecTransProb > 0) || (axiomSetPlus.axTransProb > 0)) {
		for (unsigned int i = 0; i < axioms->size(); i++) {
			if (axiomSetPlus.axiomProfit[i] > cutOff)
				(*axioms)[i]->transmute (axiomSetPlus.ecTransProb, axTransProbMin);
			else
				(*axioms)[i]->transmute (axiomSetPlus.ecTransProb, axiomSetPlus.axTransProb);
		}
	}

	// ������� �� ������ ������� ������
	if (axiomSetPlus.asTransProb > 0) {
		double probOfTp = 0;
		int numOfEC = 0;
		for (unsigned int i = 0; i < axioms->size(); i++) {
			probOfTp = (double) rand() / (double) RAND_MAX;
			if (probOfTp < axiomSetPlus.asTransProb) {
				numOfEC = (*axioms)[i]->getNumOfEC();
				(*axioms)[i]->clear();
				(*axioms)[i]->initAxiomByRand(numOfEC);
			}
		}
	}

	// ������� �� ������ ��������� ���������� ������ � �������
	// TODO: ������� ��������� ���������, ����������� ��� ������� (���
	// ������ �������� �������������� ������� ������)
	if (axiomSetPlus.prTransProb > 0 && canMutateOrder) {
		int numOfSteps = 0;
		double probChange = 0.0;
		for (int i = 1; i < (int) axioms->size(); i++) {
			probChange = (double) rand() / (double) RAND_MAX;
			if (probChange < axiomSetPlus.prTransProb) {
				numOfSteps = round( ((double) rand() / (double) RAND_MAX) * i * axiomSetPlus.prTransProb );
				if (numOfSteps > 0) {
					axiomSetPlus.axiomSet->slide(i-numOfSteps, i);
				}
			}
		}
	}

	return 0;
}


/****************************************************************************
*					TransmuteTpl::transmute
*
*	Description:	������� ������� ��� ���������
*	Parameters:		population - ���������� ���������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int TransmuteTpl::transmute (std::vector <AxiomSetPlus> &population) const {
	// ����� �� ���������, �������� ������������ �� �����. ��� ���� ���������� ���������� ����������
	for (unsigned int i = 0; i < population.size(); i++) {
		transmute (population[i]);
		population[i].clearStat();
	}
	return 0;
}
