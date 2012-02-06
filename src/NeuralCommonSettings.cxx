/****************************************************************************
*				NeuralCommonSettings.cxx
*
*	Description:	����, ���������� ���������� ����, ��� ������� � NeuralCommonSettings.h
*					(��������� NeuralCommonSettings - ����� ��������� ��� ������ RecognizerNeuralNet)
*	Author:		wictor
*	History:	
*
****************************************************************************/
#include "NeuralCommonSettings.h"
#include "NeuralCommon.h"
#include "AxiomLibException.h"

namespace AxiomLib {

/****************************************************************************
*					NeuralCommonSettings::initFromEnv()
*
*	Description:	������������� �� ���������. ��� ��������� ���������
*					��� srcQ - "numSymbols", ��� ��������� ����������
*					����� ��������� � ������� ����� ���������.
*					��� comments ��������� - false
*	Parameters:	Environment& env - ���������
*	Returns:	-
*	Throws:		AxiomLibException, ���� ��������, ��� �������� ��� ���������, �� ����������
*	Author:		wictor
*	History:
*
****************************************************************************/
void NeuralCommonSettings::initFromEnv(const Environment &env) {
	// ��������� ����� ��������
	if(env.getIntParamValue(srcQ ,"numSymbols") < 0) {
		throw AxiomLibException("PercSingleSettings::initFromEnv(): numSymbols not set");
	};
		
	// ��������� normalDist
	if(env.getDoubleParamValue(minDist ,"normalDist") < 0) {
		throw AxiomLibException("PercSingleSettings::initFromEnv(): normalDist not set");
	};
	
	// ��������� �������� comments, �� ��������� false
	if(env.getBoolParamValue(comments, "comments") < 0) {
		comments = false;
	}

	// ��������� �������� useCentral, �� ��������� false
	if(getBoolParamValueFromEnv(env, useCentral, "useCentral") < 0) {
		useCentral = false;
	}
}

/****************************************************************************
*					NeuralCoderSettings::initFromEnv()
*
*	Description:	������������� �� ���������. ��� ���������� ���������
*					 ��������� � ������� ����� ���������.
*					��� dstQ ��������� 2,
*					��� numVar 0
*	Parameters:	Environment& env - ���������
*	Returns:	-
*	Throws:		AxiomLibException, ���� ��������, ��� �������� ��� ���������, �� ����������
*	Author:		wictor
*	History:
*
****************************************************************************/
void NeuralCoderSettings::initFromEnv(const Environment &env) {
	// ��������� dstQ
	if(env.getIntParamValue(dstQ ,"dstQ") < 0) {
		// �� ��������� - 2
		dstQ = 2;
	}
	
	// ��������� numVar, �� ��������� - 0
	if(env.getIntParamValue(numVar ,"numVar") < 0) {
		numVar = 0;
	}
	
	// ��������� windowSize, �� ��������� - 0
	if(env.getIntParamValue(windowSize ,"windowSize") < 0) {
		windowSize = 1;
	}
}

};
