/****************************************************************************
*			RecognizerNeuralNet.h
*
*	Description: �������� ������� ������ RecognizerNeuralNet - ��������������,
*				������������� ��������� ����
*	Author:		wictor
*	History:	
*
****************************************************************************/

//#define RECOGNIZERNEURALNET_H_

#ifndef RECOGNIZERNEURALNET_H_
#define RECOGNIZERNEURALNET_H_

#include "Environment.h"
#include "AxiomSetPop.h"
#include "Recognizer.h"
#include "DataSet.h"
#include "NeuralCommon.h"
#include <vector>
#include <string>

namespace AxiomLib {

/****************************************************************************
*			RecognizerNeuralNet.h
*
*	Description: ����� ��� ������������� ��������� �������� � ��������������
*				������������� ���������.
*				NetObject ������ ������������ ������� NetWrapperMinimal (��. NeuralConcepts.h)
*				Settings ������ ������������� ����������� NetObject::construct(),
*				����� ���� bool commonSettings.comments
*				� ������������ ������� Settings
*	Author:		wictor
*	History:	
*
****************************************************************************/
template<class NetObject, class Settings>
class RecognizerNeuralNet: public Recognizer {
private:
	// ���� ����, ����� �� �������� ���������� � cout
	bool comments;
	
	// ���� ����, ����� �� ������������ ����������� ������
	bool useCentral;
	
	// ��������� ��� ������� NetObject
	Settings settings;
		
	// �������� ������
	int delta;
	
	// ������������ ������
	NetObject netObject;
	
	// �������
	DataSet dataSet;
	
	// ������ ��� ����� ��������, ������� ����� ������������
	std::vector<int> dataSetParams;
	
	// ����������� (����� ������ � ��������� ������� ���� -> ����� ������ ��������� ��������)
	std::vector<int> classNumbers;
	
	// ������� ����� ������ ������� � ������� ����
	void countErrors(const std::vector<int> &res, const std::vector<int> &ref, 
		int &first, int &second);
	
	//  ������������� ��������� ��������, ����� ��������� � 
	//	"���������� �������" � ������� ����� ������ 
	//	������� � ������� ����
	int runInternal(DataSet& dataSet, std::vector<int>& params, 
		int& resFirst, int& resSecond);
	
protected:
	
public:

	// ����������� ���������	
	RecognizerNeuralNet():comments(false) {}
	
	virtual void setOutputToFile(const bool newValue) {}
	
	// ��������� ����� �����������
	void setComments(const bool newValue) { comments = newValue; }
	
	//    ���������������� ��������� ������� �� env
	int setParamsFromEnv(const Environment& env);

	// ��������� ������� ���������� �� ��������� (� ������ ������ �� �� �����, ��� setParamsFromEnv(env))
	int setNominalParamsFromEnv(const Environment& env) { return setParamsFromEnv(env); }
	
	// ��������� ��������
	int setDataSet(const DataSet &newDataSet) {
		dataSet = newDataSet;
		return 0;
	}
	
	// ��������� ���������� ��������
	int setParamsOfDataSet(std::vector<int> &newParams) { 
		dataSetParams = newParams; 
		return 0; 
	}
	
	// �������� ����� �� ����
	Recognizer* copy(void) {
		// ������������ ����������� �� ��������� ����� ���������� 
		return new RecognizerNeuralNet<NetObject, Settings>(*this);
	}
	
	// �������� ���� �� ��������, ����� ������������ setDataSet()
	double learn(TemplateRecognizer &templateRecognizer);
	
	// �������������, ���� ������� ������� - ��������
	signed int run(AxiomSetPop& pop) { return 0; }

	// �������������, ���� ������� ������� - ��������
	signed int run(AxiomSetPop& pop, std::vector <double> &precs) { return 0; }

	//       ������������� ��������� ��������, ����� ��������� � "���������� �������" � ������� ����� ������ ������� � ������� ����
	//				��� ������� �������� runInternal
	signed int run(AxiomSet& axiomSet, DataSet& dataSet, 
		std::vector<int>& params, int& resFirst, int& resSecond, const int asIndex = -1);

	//   ������������� ��������� ��������, ����� ��������� � "���������� �������" � ������� ����� ������ ������� � ������� ����
	//						��� ������� �������� runInternal
	signed int run(AxiomSet& axiomSet, DataSet& dataSet, 
		std::vector<int>& params, int& resFirst, int& resSecond, std::vector<double> &stat, const int asIndex = -1);
};

};

#include "RecognizerNeuralNet.cxx"

#endif
