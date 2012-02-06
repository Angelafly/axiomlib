/****************************************************************************
*			NeuralAuxSupplier.h
*
*	Description: �������� ������ NeuralAuxSupplier
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef NEURALAUXSUPPLIER_H_
#define NEURALAUXSUPPLIER_H_

#include "NeuralCommon.h"

namespace AxiomLib {

/****************************************************************************
*			NeuralAuxSupplier<NetWrapper>
*
*	Description: ����� - ���������, �������������� �������������� ����������
*				����� ��� ��������������� �������� (�.� ���������� supplyAuxTrainingSata()
*				� ������������� ���������������� ��������)
*				NetWrapper - ������ ������������ NetWrapper ��� Holder (��. NeuralConcepts.h)
*				������ ����� ���������� �� ��, ��� � NetWrapper
*	Author:		wictor
*	History:	
*
****************************************************************************/
template<class NetWrapper>
class NeuralAuxSupplier: public NetWrapper {
	public:
	
	// ��������� �������������� �����
	template<class Settings>
	void construct(const Settings &settings);
};

};

#include "NeuralAuxSupplier.cxx"

#endif