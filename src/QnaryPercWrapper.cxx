/****************************************************************************
*						QnaryPercWrapper.cxx
*
*       Description:	���������� ����, ��� ��������� � QnaryPercWrapper.h
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef QNARYPERCWRAPPER_CXX_
#define QNARYPERCWRAPPER_CXX_

#include "QnaryPercWrapper.h"

namespace AxiomLib {

/****************************************************************************
*						QnaryPercWrapper<Holder>::construct
*
*       Description:	��������� ���������� � �������� ����
*       Parameters:		const Settings &settings - ���������
*						��� Settings ������ ����� ���� int qO,
*						���� CommonSettings commonSettings
*						� ������������� ����������� Holder::construct()
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
template<class Settings>
void QnaryPercWrapper<Holder>::construct(const Settings &settings) {
	// ������������� �������� ������ - ��������� ������
	Holder::construct(settings);
	// ����� ������ ����, ������ ���� ����� ��������� ����� ������ 1
	// ����� � run ������ ���������� 0
	if(Holder::getNumTrain() >= 2) {
		int actualQo;
		if (settings.qO < 1) {
			// ���� qO < 1, �� �������, ��� ��� ����� ����� ��������� �����
			// �.�. � ��� ���� � ����� �������� �������� � ������ ���������,
			// ������ ����� ��������� �����
			actualQo = Holder::getNumTrain();
		} else if (settings.qO == 1) {
			// ���� qO = 1, �� �������, ��� ��� ����� ����� ��������� ������� �������� 
			// �.�. � ���� ������ ����� ��������� ���. �������� ����� ����� ��������� ��. ��������
			actualQo = Holder::getCoderDstQ();
		} else {
			actualQo = settings.qO;
		}
		// ������������� q-������� �����������
		qp.construct(
			// ����� ��������� ������� �������� - ������������ ������������
			Holder::getCoderDstQ(),
			// ����� ��������� �������� ��������
			actualQo,
			// ����� ������� �������� - ������������ ����� 
			// �������������� ��������� ������
			getCoderDstLen(Holder::getMaxTrainDim()),
			// ����� �������� �������� - ����������� ��� ����, ����� �����������
			// ����� ������� ����� ��������� ������ � qO-����� ������� ���������
			numDigits(actualQo, (Holder::getNumTrain() - 1)
			)
		);
	}
}

/****************************************************************************
*						QnaryPercWrapper<Holder>::clear
*
*       Description:	������� ������� ������ (� ��� ����� ������������ ��������)
*       Parameters:		-
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
void QnaryPercWrapper<Holder>::clear() {
	Holder::clear();
}

/****************************************************************************
*						QnaryPercWrapper<Holder>::train
*
*       Description:	������� ������� ���� �� ����� ��������������� �� ��������� �������
*       Parameters:		-
*       Returns:		-
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
void QnaryPercWrapper<Holder>::train()
{
	// ���� ����� ��������� ����� ������ 2 - �������
	if(Holder::getNumTrain() < 2) {
		return;
	}
	
	// ����� ��������� ������ (�� �������� ������ �������� ������, �� ��������������)
	int outputIndex = 0;
	// ������, ��������������� ������ ����
	Qstring output(numDigits(qp.getQo(), Holder::getNumTrain() - 1));
	
	// ���� �� ���� ��������� �������
	for (int i = 0; i < Holder::getNumTrain(); i++)
	{
		// �������������� i-� ��������� ������
		const Qstring inpTranslated = Holder::getTranslated(i);
		
		makeZero(output);
		// ���������� � output ����� ��������� ������ � ����
		// ����� � qO-����� ��
		numToQnaryVector(qp.getQo(), outputIndex, output);
		
		// ������� ���� �� i-� ����� � �����. ������
		qp.train(inpTranslated, output);
		
		// ���� �� ���. ��������� ������� (��������������� i-� ��������� ������)
		for (int j = 0; j < Holder::getNumAuxTrain(i); j++)
		{
			// �������������� j-� ���. ��������� ������, ��������������� 
			// i-� ��������� ������
			const Qstring inpAuxTranslated = Holder::getAuxTranslated(i, j);
			
			// ������� �� ��� ����, ����� ������ ���� ��� ��, ��� � 
			// � i-� ��������� ������
			qp.train(inpAuxTranslated, output);
		}
		
		outputIndex++;
	}
}

/****************************************************************************
*						QnaryPercWrapper<Holder>::run
*
*       Description:	������� ��������� ������������ ��������
*						
*       Parameters:		const Qstring &inp - ���� ���� ()
*						double &p - �����, ������� ����� ���������� ���� ��� ������ (���� �� ������������)
*       Returns:		����� ��������� ������ (� ��� �������, � ������� ��� ���� ������ �������� supplyTrainingData()), 
*						���� -1, ���� ���� ������ �����, ��������� �� ����� ��������� ��������� �����
*       Throws:			-
*       Author:			wictor
*       History:
*
****************************************************************************/
template<class Holder>
int QnaryPercWrapper<Holder>::run(const Qstring &inp, double &p) const {
	// ���� ��������� ������ ������ ����, �� ���������� 0
	// (�.�. �� �� ��� �� ������ ����������)
	if (Holder::getNumTrain() == 1) {
		return 0;
	// ���� �� 0, �� ���������� -1 (�.�. �� ������ �� ����������)
	} else if (Holder::getNumTrain() == 0) {
		return -1;
	}
	
	// �������� ����
	const Qstring inpCoded = Holder::coderTranslate(inp);
	// ��������� ����� ����
	int index = qnaryVectorToNum(qp.getQo(), qp.run(inpCoded, p));
	// ���� ����� �����������, ���������� -1
	if(index >= Holder::getNumTrain() || index < 0) return -1;
	return index;
}

};

#endif
