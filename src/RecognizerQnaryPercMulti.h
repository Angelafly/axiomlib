/****************************************************************************
*			RecognizerQnaryPercMulti.h
*
*	Description: �������� ���� RecognizerQnaryPercMulti
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef RECOGNIZERQNARYPERCMULTI_H_
#define RECOGNIZERQNARYPERCMULTI_H_

#include "RecognizerNeuralNet.h"
#include "QnaryPercMulti.h"

namespace AxiomLib {

/****************************************************************************
*			RecognizerQnaryPercMulti
*
*	Description: ��������������, ������������ ���� q-������ ����������
*				��� ������� �������� ����� �����
*	Author:		wictor
*	History:	
*
****************************************************************************/
typedef RecognizerNeuralNet< QnaryPercMulti, QnaryPercMultiSettings > RecognizerQnaryPercMulti;

};

#endif
