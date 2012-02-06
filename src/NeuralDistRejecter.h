/****************************************************************************
*			NeuralDistRejecter.h
*
*	Description: �������� ������� ������ NeuralDistRejecter<NetWrapper>
*	Author:		wictor
*	History:	
*
****************************************************************************/

#include "NeuralCommon.h"

#ifndef NEURALREJECTER_H_
#define NEURALREJECTER_H_

namespace AxiomLib {

/****************************************************************************
*			NeuralDistRejecter
*
*	Description: ����� - ��������� ��� ��������� ����������� �������������
*				��������� � ������ ���������� minDist � useCentral
*				NetWrapper ������ ������������ �������
*				NetWrapperMinimalWithCentralStrings (��. NeuralConcepts.h)
*				���� ����� ���������� �� ��, ��� NetWrapper
*	Author:		wictor
*	History:	
*
****************************************************************************/
template<class NetWrapper>
class NeuralDistRejecter: public NetWrapper {
	// ����������� ���������� �� ����������� ���������
	double minDist;
	
	// ������ �������� ������� �����, � �������� ����� ���������� ����������
	std::vector<int> centralStrings;
	
	int lastCentral;
	
	bool useCentral;
	public:
	
	NeuralDistRejecter(): lastCentral(-1) {}
	
	// ������������� �� ��������
	template<class Settings>	
	void construct(const Settings &settings);
	
	// ���������� ������ ��� �������� - ���� ����� ����������� ������ ������� ���
	// ��������� ����������� �����
	void supplyTrainingData(const Qstring &str);
	
	// ���������� ����������� ������ - ����� ����� ������ ���
	// ��������� ������, ����������� ������� supplyTrainingData(),
	// ����� ����������
	// � ������ ����������� ������ (�� ���������� ������ ���� �������)
	void supplyCentralString(const Qstring &str);
	
	// ������ ������������� ��������� (� ���������� ����������� �� ����������)
	int run(const Qstring &str, double &d) const;
};

};

#include "NeuralDistRejecter.cxx"

#endif
