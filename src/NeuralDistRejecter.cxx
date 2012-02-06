/****************************************************************************
*						NeuralDistRejecter.cxx
*
*       Description:	���������� �������� �������-������ ������� ������ NeuralDistRejecter
*       Author:			wictor
*       History:
*
****************************************************************************/

#ifndef NEURALDISTREJECTER_CXX_
#define NEURALDISTREJECTER_CXX_

#include "NeuralDistRejecter.h"
#include <boost/foreach.hpp>

namespace AxiomLib {

/****************************************************************************
*						NeuralDistRejecter<NetWrapper>::construct
*
*       Description:	������������� �� ��������
*       Parameters:		const Settings &settings - ���������
*						��� Settings ������ ����� ���� double commonSettings.minDist
*						� ������������� ����������� NetWrapper::construct()
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
template<class Settings>
void NeuralDistRejecter<NetWrapper>::construct(const Settings &settings) {
	NetWrapper::construct(settings);
	minDist = settings.commonSettings.minDist;
	useCentral = settings.commonSettings.useCentral;
}
/****************************************************************************
*						NeuralDistRejecter<NetWrapper>::run
*
*       Description:	������� ��������� ������������ ��������, ����� ��������� 
*						��������� �� �������� � ������� ������
*       Parameters:		const Qstring &inp - ���� ����
*						double &p - �����, ������� ����� ���������� ���� ��� ������ (���� �� ������������)
*       Returns:		����� ��������� ������ (� ��� �������, � ������� ��� ���� ������ �������� supplyTrainingData()), 
*						���� -1, ���� ���� ������ �����, ��������� �� ����� ��������� ��������� �����,
*						���� ���� � ���������� ���������� ������, ������������� �� ���������� �� str �������,
*						��� minDist (��� ����� �������� � ����������)
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class NetWrapper>
int NeuralDistRejecter<NetWrapper>::run(const Qstring &str, double &p) const {
	// ������ ������������� ���������
	int res = NetWrapper::run(str, p);
		
	// ���� �� ������ -1, �� ���������� -1
	if(res<0) {
		return res;
	}
	
	int index;
	if(useCentral) {
		index = centralStrings[res] > -1 ? centralStrings[res] : res;
	} else {
		index = res;
	}
	// ��������� ���������� �� str �� ������, �� ������� �� ������ ����������
	double d = difference(str, NetWrapper::getTrainString(index));
	// ���� ��� ���������� ������ minDist, �� ���������� -1
	if(d > minDist) { 
		return -1;
	}
	// ����� ���������� ��������� ������������� ���������
	return res;
}

template<class NetWrapper>
void NeuralDistRejecter<NetWrapper>::supplyCentralString(const Qstring &str) {
	lastCentral = NetWrapper::getNumTrain();
	this->supplyTrainingData(str);
}

template<class NetWrapper>
void NeuralDistRejecter<NetWrapper>::supplyTrainingData(const Qstring &str) {
	centralStrings.push_back(lastCentral);
	NetWrapper::supplyTrainingData(str);
}

};

#endif
