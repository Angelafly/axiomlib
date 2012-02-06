/****************************************************************************
*						PercSettings.h
*
*       Description:	���������� ��������� PercSettings
*       Author:			wictor
*       History:
*
****************************************************************************/
#ifndef PERCSETTINGS_H_
#define PERCSETTINGS_H_

#include <vector>

#include "NeuralFANNHeader.h"

#include "Environment.h"
#include <string>

namespace AxiomLib {

/****************************************************************************
*						PercSettings
*
*       Description:	���������, ���������� ��������� ������������� �����������
*						� ������� ������������� �� ���������
*       Author:			wictor
*       History:
*
****************************************************************************/
struct PercSettings {
	// ����� ������� ����� (��������� ��� ���� ����� ��������)
	int numLay;
	// ������� ����� �������� � ����
	std::vector<double> numNeuronsF;
	// ������������� ������� �����
	std::vector<FANN::activation_function_enum> actFuncs;
	// ��������� (s) ������������� ������� �� �����
	std::vector<double> actFuncsS;
	// �������� ��������
	FANN::training_algorithm_enum trainingAlgorithm;
	// ����������� ��������
	double learningRate;
	// ������ (�������) ��������
	double learningMomentum;
	// ������������ ����� ���� ��������
	int maxIters;
	// ����� �������� ����� ��������� �� ��������
	int itersBetweenReports;
	// �������� ������ (�� ������� ������������� ��������)
	double desiredError;
	//FANN::callback_type callback;
	
	// �������� ������ �������� �� ������������
	// TODO: ��������
	bool valid() const { 
		return (numLay - 1) == numNeuronsF.size() &&
			numLay == actFuncs.size() &&
			numLay == actFuncsS.size() &&
			numLay > 0 &&
			learningRate > 0.0 &&
			maxIters > 0;
	}
	
	// ������������� �� ���������
	void initFromEnv(const Environment &env);
	
};

};

#endif