/****************************************************************************
*			RecognizerQnaryPercSingle.h
*
*	Description: �������� ���� RecognizerQnaryPercSingle
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef RECOGNIZERQNARYPERCSINGLE_H_
#define RECOGNIZERQNARYPERCSINGLE_H_

#include "RecognizerNeuralNet.h"
#include "QnaryPercSingle.h"

namespace AxiomLib {

/****************************************************************************
*			RecognizerQnaryPercSingle
*
*	Description: ��������������, ������������ ���� q-������ ����������
*				��� ���� ���� ����� (� ��������� �������������� ��������)
*	Author:		wictor
*	History:	
*
****************************************************************************/
typedef RecognizerNeuralNet< QnaryPercSingle, QnaryPercSingleSettings > RecognizerQnaryPercSingle;

};

#endif
