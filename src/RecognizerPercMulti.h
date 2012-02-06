/****************************************************************************
*			RecognizerPercMulti.h
*
*	Description: �������� ���� RecognizerPercMulti
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef RECOGNIZERPERCMULTI_H_
#define RECOGNIZERPERCMULTI_H_

#include "RecognizerNeuralNet.h"
#include "PercMulti.h"

namespace AxiomLib {

/****************************************************************************
*			RecognizerPercMulti
*
*	Description: ��������������, ������������ ���� ������������ ����������
*				��� ������� �������� ����� �����
*	Author:		wictor
*	History:	
*
****************************************************************************/
typedef RecognizerNeuralNet< PercMulti, PercMultiSettings > RecognizerPercMulti;

};

#endif
