/****************************************************************************
*				RecognizerNeuralNet.cxx
*
*	Description:	������� ������� ������ 
*					RecognizerNeuralNet<NetObject, Settings> - ����������
*	Author:		wictor
*	History:		
*
****************************************************************************/

#ifndef RECOGNIZERNEURALNET_CXX_
#define RECOGNIZERNEURALNET_CXX_

#include "RecognizerNeuralNet.h"
#include <iostream>
#include <cstdlib>

namespace AxiomLib {

/****************************************************************************
*                       RecognizerNeuralNet<NetObject, Settings>::countErrors
*
*       Description:    ������� ����� ������ ������� � ������� ����
*       Parameters: const std::vector<int> &res - ������ ���������� �����������
*					const std::vector<int> &ref - ������ "���������� �������"
*					int &first
*					int &second
*       Returns:        -
*       Throws:         -
*       Author:         wictor
*       History:
*
****************************************************************************/
template<class NetObject, class Settings>
void RecognizerNeuralNet<NetObject, Settings>::countErrors(const std::vector<int> &res, const std::vector<int> &ref, 
	int &first, int &second) {
	// �������� �� ���������� �������� res � ref
	if(res.size() != ref.size()) {
		throw AxiomLibException("countErrors() : res and ref sizes don't match");
	}
	
	first = 0;
	second = 0;
	
	// ������ ������� ����
	for(unsigned int i = 0; i < ref.size(); i++) {
		if(res[i] != 0 && ref[i] == 0) {
			if(!scanDelta(ref, i, delta, res[i])) {
				first++;
			}
		}
	}
	
	// ������ ������� ����
	int curr;
	int prev = 0;
	for (unsigned int i = 0; i < ref.size(); i++) {
		curr = ref[i];
		bool isCurrNormal = (curr == 0);
		if (!isCurrNormal && (curr!=prev)) {
			if(!scanDelta(res, i, delta, curr)) {
				second++;
			}
		}
		prev = curr;
	}
}

/****************************************************************************
*                     RecognizerNeuralNet<NetObject, Settings>::run
*
*       Description:    ������������� ��������� ��������, ����� ��������� � "���������� �������" � ������� ����� ������ ������� � ������� ����
*						��� ������� �������� runInternal
*       Parameters: AxiomSet& axiomSet - ������������   
*					DataSet& dataSet - ������� 
*					std::vector<int>& params - ������ ������������ ����� ��������
*					int& resFirst - ����� ������ ������� ����
*					int& resSecond - ����� ������ ������� ����
*       Returns:        0
*       Throws:         -
*       Author:         wictor
*       History:
*
****************************************************************************/
template<class NetObject, class Settings>
signed int RecognizerNeuralNet<NetObject, Settings>::run(AxiomSet& axiomSet, DataSet& dataSet, 
	std::vector<int>& params, int& resFirst, int& resSecond, const int asIndex) {
	return runInternal(dataSet, params, resFirst, resSecond);
}

/****************************************************************************
*                       RecognizerNeuralNet<NetObject, Settings>::run
*
*       Description:    ������������� ��������� ��������, ����� ��������� � "���������� �������" � ������� ����� ������ ������� � ������� ����
*						��� ������� �������� runInternal
*       Parameters: AxiomSet& axiomSet - ������������   
*					DataSet& dataSet - ������� 
*					std::vector<int>& params - ������ ������������ ����� ��������
*					int& resFirst - ����� ������ ������� ����
*					int& resSecond - ����� ������ ������� ����
*					std::vector<double> &stat - ������������
*       Returns:        0
*       Throws:         -
*       Author:         wictor
*       History:
*
****************************************************************************/
template<class NetObject, class Settings>
signed int RecognizerNeuralNet<NetObject, Settings>::run(AxiomSet& axiomSet, DataSet& dataSet, 
	std::vector<int>& params, int& resFirst, int& resSecond, std::vector<double> &stat, const int asIndex) {
	return runInternal(dataSet, params, resFirst, resSecond);
}

/****************************************************************************
*                       RecognizerNeuralNet<NetObject, Settings>::learn
*
*       Description:    �������� ���� �� ��������, ����� ������������ setDataSet()
*       Parameters:     templateRecognizer - ������������
*       Returns:        0
*       Throws:         AxiomLibException - ���� ������ dataSetParams ����� 0
*       Author:         wictor
*       History:
*
****************************************************************************/
template<class NetObject, class Settings>
double RecognizerNeuralNet<NetObject, Settings>::learn(TemplateRecognizer &templateRecognizer) {
	if(dataSetParams.size() == 0) {
		throw AxiomLibException("learn() : no parameters to analyze (dataSetParams is empty)");
	}

	classNumbers.clear();
	
	// ��������� ����������� ��������� �����
	int numOfClasses;
	std::vector<int> numOfMultiTS;
	std::vector<std::vector<int> > numOfTS;
	dataSet.getClassSize(numOfClasses, numOfMultiTS, numOfTS);	
	
	Qstring str;
	// TODO: reserve some memory for classNumbers
	// �������� �� ������� ����������
	for(unsigned int paramNumber = 0; paramNumber < dataSetParams.size(); paramNumber++) {		
		// �������� �� �������
		for(int classNo = 0; classNo < numOfClasses; classNo++) {
			// �������� �� �����������	
			for(int multiTSNo = 0; multiTSNo < numOfMultiTS[classNo]; multiTSNo++) {							
				// �������� �������� ��������� ����������
				std::vector<double> currTS;
				dataSet.getTSByIndexFromClass(currTS, classNo, multiTSNo, dataSetParams[paramNumber]);
				
				// ��������� �� std::vector � Qstring
				copyVector(currTS, str);
				// �������� ��������� ������
				if(multiTSNo == 0 && useCentral) {
					netObject.supplyCentralString(str);					
				} else {
					netObject.supplyTrainingData(str);
				}
				
				// ���������� � classNumbers ����� �������� ������ ���� ���� (���, ����� �� �������� �� 1, �.�. 0 - ���������� ���������)
				classNumbers.push_back(classNo+1);
			}
		}
	}	
	
	// ����� construct - ������ ���� (��� ����� �� (� ���������) ����� 
	// ����� ������ �����, ��� ��� �� ���������� ��������� ������ �� ������ ���� �������)
	netObject.construct(settings);

	// ����� train - ������� ���� (�� ������, ������� ��� ��������)
	netObject.train();
	
	return 0;
}

/****************************************************************************
*		RecognizerNeuralNet<NetObject, Settings>::setParamsFromEnv
*
*       Description:    ���������������� ��������� ������� �� env
*       Parameters:     const Environment& env - �����
*       Returns:        0
*       Throws:         AxiomLibException - ���� ��� ������-�� ���������
*       Author:         wictor
*       History:
*
****************************************************************************/
template<class NetObject, class Settings>
int RecognizerNeuralNet<NetObject, Settings>::setParamsFromEnv(const Environment& env) {
	// ������������� �������� ������������� �������
	settings.initFromEnv(env);
	
	// ������������� ����� �����������
	comments = settings.commonSettings.comments;
	
	// ������������� ����� ������������� primary-�����
	useCentral = settings.commonSettings.useCentral;
	
	// ������������� delta
	if(env.getIntParamValue(delta ,"delta") < 0) {
		throw AxiomLibException("setParamsFromEnv(): delta not set");
	};

	return 0;
}

/****************************************************************************
*                       RecognizerNeuralNet<NetObject, Settings>::runInternal
*
*       Description:    ������������� ��������� ��������, ����� ��������� � 
*						"���������� �������" � ������� ����� ������ 
*						������� � ������� ����
*       Parameters:     DataSet& dataSet - ������� 
*					std::vector<int>& params - ������ ������������ ����� ��������
*					int& resFirst - ����� ������ ������� ����
*					int& resSecond - ����� ������ ������� ����
*       Returns:        0
*       Throws:         AxiomLibException - ���� ������ params ����� 0
*       Author:         wictor
*       History:
*
****************************************************************************/
template<class NetObject, class Settings>
int RecognizerNeuralNet<NetObject, Settings>::runInternal(DataSet& dataSet, std::vector<int>& params, 
	int& resFirst, int& resSecond) {
	
	if(params.size() == 0) {
		throw AxiomLibException("RecognizerNeuralNet<NetObject, Settings>::runInternal() : no parameters to analyze (params is empty)");
	}

	//����� ������, ���������� ��� ������ �������� ������ ���� �� params (���������� �� ������ �������� �� params)
	int first=0, second=0;
	
	// �������� ����������� ������
	int numOfTests;
	std::vector<int> numOfTS;
	dataSet.getTestSize(numOfTests, numOfTS);
	
	// �������� �� ������
	for(int testNo = 0; testNo < numOfTests; testNo++) {			
		std::vector<int> currTS, currRefTS;


		// �������� "���������� �����"
		dataSet.getTSByIndexFromTests(currRefTS, testNo, -1);
			//�������� �� ��������� �� params
		for(unsigned int paramNo=0; paramNo < params.size(); paramNo++) {
			// �������� �������� ������� ����������
			dataSet.getTSByIndexFromTests(currTS, testNo, params[paramNo]);

			// ��������, ������� �������� ��� ��������
			std::vector<int> resTS(currTS.size(), 0);
						
			// �������� ������� � ������� ������, �� ������� ���� ����� ��������
			int maxI = currTS.size() - netObject.getMinTrainDim();
			
			// �������� �� ����������� ������� ����������
			for (int i = 0; i < maxI; i++) {
				Qstring str;
				// ����� ���� �����, ������ ������������ ����� ������ � �������� ��� � str
				copyVector(currTS, str, i, netObject.getMaxTrainDim());
				double p;
				// ��������� ������������ ��������
				int result = netObject.run(str, p);
				
				// ����������� ���������
				if(result>=0) {
					// ���� ��������� ��������, �� ������ ����� �� ������ �� ������� i
					resTS[i] = classNumbers[result];
					// � ���������� ������ ��������� ������ � �������
					i+=netObject.getTrainString(result).size();
				} // ����� - 0 ��� �����
			}
			// ����������� ���������� ����� � ���������� �����, ������ ������ � ���������� ������
			countErrors(resTS, currRefTS, first, second);
			if(comments) {
				std::cout<<"Test number: "<<testNo<<"\tTS number: "<<
				dataSetParams[paramNo]<<"\nfirst kind errors: "<<
				first<<"\nsecond kind errors: "<<second<<"\n";
			}
			
			// ����������� ����� ����� ������
			resFirst += first;
			resSecond += second;
		}
	}
	return 0;
}

};

#endif
