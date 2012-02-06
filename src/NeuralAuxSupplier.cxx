/****************************************************************************
*			NeuralAuxSupplier.cxx
*
*	Description: ���������� ����, ��� ������� � NeuralAuxSupplier.h
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef NEURALAUXSUPPLIER
#define NEURALAUXSUPPLIER

#include "NeuralAuxSupplier.h"
#include "NeuralCoin.h"

namespace AxiomLib {

/****************************************************************************
*					NeuralAuxSupplier<NetWrapper>::construct
*
*	Description:	������������� �� ����������
*					��� Settings ������ ����� ����� int numAux, double auxDiff
*					� ������������� NetWrapper::construct
*	Parameters:	const Settings &settings - ���������
*	Returns:	-
*	Throws:		AxiomLibException, ���� numAux < 0 || auxDiff <= 0
*	Author:		wictor
*	History:
*
****************************************************************************/
template<class NetWrapper>
template<class Settings>
void NeuralAuxSupplier<NetWrapper>::construct(const Settings &settings) {
	NetWrapper::construct(settings);
	if(settings.numAux < 0) {
		throw AxiomLibException("NeuralAuxSupplier<NetWrapper>::construct() : invalid numAux");
	}
	if(settings.auxDiff <= 0) {
		throw AxiomLibException("NeuralAuxSupplier<NetWrapper>::construct() : invalid auxDiff");
	}
	const int &numAux = settings.numAux;
	const double &diff = settings.auxDiff;
	const int &numSymbols = settings.commonSettings.srcQ;
	for(int j = 0; j < NetWrapper::getNumTrain(); j++) {
		const Qstring &str = NetWrapper::getTrainString(j);
		for(int i = 0; i < numAux; i++) {
			Qstring aux(str);
			Coiner coiner(str.size(), diff);
			for(unsigned int k = 0; k < str.size(); k++) {
				if(coiner.coin(k)) aux[k] = (str[k] + 1) % numSymbols;
			}
			NetWrapper::supplyAuxTrainingData(aux, j);
		}
	}
}

};

#endif
