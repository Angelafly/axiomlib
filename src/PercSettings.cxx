/****************************************************************************
*						PercSettings.cxx
*
*       Description:	���������� ����, ��� ��������� � PercSettings.h
*       Author:			wictor
*       History:
*
****************************************************************************/
#include "PercSettings.h"
#include "NeuralActFuncStr.h"
#include "NeuralCommon.h"

namespace AxiomLib {

/****************************************************************************
*					PercSettings::initFromEnv()
*
*	Description:	������������� �� ���������.
*					����� ���������� ��������� ��������� � ������� ����� ���������.
*	Parameters:	Environment& env - ���������
*	Returns:	-
*	Throws:		AxiomLibException, ���� �������� �� ����������
*	Author:		wictor
*	History:
*
****************************************************************************/
void PercSettings::initFromEnv(const Environment &env) {
	// ��������� ��������� �����������
	// ��������� ����� �����
	if(env.getIntParamValue(numLay ,"numLay") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): numLay not set");
	}
	
	// ��������� ������ � ����������
	std::string algorithmStr;
	if(env.getStringParamValue(algorithmStr ,"trainingAlgorithm") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): trainingAlgorithm not set");
	}
	
	// ��������� ������ � ���������� � FANN::training_algorithm_enum
	int algorithmNum = trainFromStr(algorithmStr);
	if (algorithmNum < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): incorrect trainingAlgorithm");
	}
	trainingAlgorithm = (FANN::training_algorithm_enum) algorithmNum;
	
	// ��������� ������ �� ������� ������������� �������
	std::string actFuncsStr;
	if(env.getStringParamValue(actFuncsStr ,"actFuncs") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): actFuncs not set");
	}
	
	// ��������� ������ �� ������� ������������� � ������ ����� ������������� �������
	std::vector<std::string> actFuncsStrs;
	getVectorFromString(actFuncsStrs, actFuncsStr);
	// ��������� ������ ����� ������������� ������� � ������ �� FANN::activation_function_enum
	actFuncs.clear();
	actFuncs.reserve(actFuncsStrs.size());
	for(unsigned int i = 0; i < actFuncsStrs.size(); i++) {
		int actFuncNum = actFuncFromStr(actFuncsStrs[i]);
		if(actFuncNum < 0) {
			throw AxiomLibException("PercSettings::initFromEnv(): incorrect actFuncs");
		}
		actFuncs.push_back((FANN::activation_function_enum) actFuncNum);
	}
	
	// ��������� ������ �������� (����������) ����� �������� �����
	std::string numNeuronsFStr;
	if(env.getStringParamValue(numNeuronsFStr ,"numNeuronsF") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): numNeuronsF not set");
	}
	// ��������� � ������ �� double
	getVectorFromString(numNeuronsF, numNeuronsFStr);
	
	// ��������� ������ ���������� ������������� �������
	std::string actFuncsSStr;
	if(env.getStringParamValue(actFuncsSStr ,"actFuncsS") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): actFuncsS not set");
	}
	// ��������� � ������ �� double
	getVectorFromString(actFuncsS, actFuncsSStr);
	
	// ��������� ����������� ��������
	if(env.getDoubleParamValue(learningRate ,"learningRate") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): learningRate not set");
	};

	// ��������� ������ ��������
	if(env.getDoubleParamValue(learningMomentum ,"learningMomentum") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): learningMomentum not set");
	};
	
	// ��������� �������� ������
	if(env.getDoubleParamValue(desiredError ,"desiredError") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): desiredError not set");
	};

	// ��������� ������������ ����� ����
	if(env.getIntParamValue(maxIters ,"maxIters") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): maxIters not set");
	}

	// ��������� ������������ ����� ����
	if(env.getIntParamValue(itersBetweenReports ,"itersBetweenReports") < 0) {
		throw AxiomLibException("PercSettings::initFromEnv(): itersBetweenReports not set");
	}
	
	// ��������� ��������� �� ���������������
	if(!valid()) {
		throw AxiomLibException("PercSettings::initFromEnv(): invalid perceptron settings");
	}	
}

};