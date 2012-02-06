/****************************************************************************
*			RecognizerPercSingle.h
*
*	Description: �������� ���� RecognizerPercSingle
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef RECOGNIZERPERCSINGLE_H_
#define RECOGNIZERPERCSINGLE_H_

#include "RecognizerNeuralNet.h"
#include "PercSingle.h"

namespace AxiomLib {

/****************************************************************************
*			RecognizerPercSingle
*
*	Description: ��������������, ������������ ���� ������������ ����������
*				��� ���� ���� ����� (� ������ 0 �� �������������� �������
*				��� �������� � �������������)
*	Author:		wictor
*	History:	
*
****************************************************************************/
typedef RecognizerNeuralNet< PercSingle, PercSingleSettings > RecognizerPercSingle;

};

#endif
