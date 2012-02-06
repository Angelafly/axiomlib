/****************************************************************************
*				NeuralCommonSettings.h
*
*	Description:	����, ���������� ����������� �������� NeuralCommonSettings
*					� NeuralCoderSettings
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef NEURALCOMMONSETTINGS_H_
#define NEURALCOMMONSETTINGS_H_

#include "Environment.h"
#include <string>

namespace AxiomLib {

/****************************************************************************
*				NeuralCommonSettings
*
*	Description:	���������, ���������� ����� ��������� ������������� ���������:
*					srcQ, minDist, comments, � ����� ������� ������������� �� ���������
*	Author:		wictor
*	History:	
*
****************************************************************************/
struct NeuralCommonSettings {
	// ����� �������� ��������
	int srcQ;
	// ����������� ���������� �� ����������� ���������
	double minDist;
	// ���� ����, ����� �� �������� ���������� � cout
	bool comments;
	// ���� ����, ��� ��� ��������� �� ���������� ����� ������������ primary-������
	// ����� ������������ ������, ����� ������� ��������� ���������������
	bool useCentral;
	// ������������� �� ���������
	void initFromEnv(const Environment &env);
};

/****************************************************************************
*				NeuralCoderSettings
*
*	Description:	���������, ���������� ��������� �����������:
*					dstQ � numVar, � ����� ������� ������������� �� ���������
*	Author:		wictor
*	History:	
*
****************************************************************************/
struct NeuralCoderSettings {
	// ����� �������� ��������� �������� �����������
	int dstQ;
	// �������� num_var
	int numVar;
	// �������� window_size
	int windowSize;
	// ������������� �� ���������
	void initFromEnv(const Environment &env);
};

};

#endif