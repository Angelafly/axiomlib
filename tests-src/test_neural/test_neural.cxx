#include "AxiomLibException.h"
#include "Environment.h"
#include "TemplateRecognizer.h"
#include "RecognizerFactory.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace AxiomLib;

void recognizeOnce(const std::string& confName,
					int &firstKindErrors,
					int &secondKindErrors) {
	Recognizer *pRecognizer = 0;
	try {
		Environment env;
		std::cout << "Reading config file " << confName << "...\n";
		// ��������� ���� ������������
		env.readConfigFile(confName.c_str());
		
		std::string dir, name;
		// ���������, �������� �� ������ ������ ���������
		if (env.getStringParamValue(dir, "BaseDataSetDir") < 0) {
			throw AxiomLib::AxiomLibException("main: Config parameter BaseDataSetDir not set");
		}
		if (env.getStringParamValue(name, "DataSet") < 0) {
			throw AxiomLib::AxiomLibException("main: Config parameter DataSet not set");
		}

		std::cout<<"Config file read"<<endl;
				
		EnvDataSet envDataSet;
		// �������������� envDataSet �� �������
		envDataSet.readConfigFile(env);

		DataSet dataSet;
		std::cout<<"Reading data set named '"<<name<<"' from dir '"<<dir<<"'..."<<std::endl;
		// ��������� dataSet, ��������� ���������, ���������� �� �������
		dataSet.readDataSet(dir, name);
		std::cout<<"Data set read"<<endl;

		// ������ �������� �������� ��� ����������� ���������
		dataSet.setNullStr(envDataSet);
				
		// ��������� ��� �������������� �� �������
		std::string recognizerTypeStr;
		if (env.getStringParamValue(recognizerTypeStr, "recognizerType") < 0) {
			throw AxiomLib::AxiomLibException("main: Config parameter recognizerType not set");
		}
		
		std::cout<<"Using recognizer: "<<recognizerTypeStr<<"\n";
		
		// ������� recognizer � �������� ������
		RecognizerFactory recognizerFactory;
		pRecognizer = recognizerFactory.create(recognizerTypeStr);
		
		// ������������� ���������
		pRecognizer->setParamsFromEnv(env);
		// ������������� �������
		pRecognizer->setDataSet(dataSet);
		
		// C��������� params
		std::vector<int> refParams;
		dataSet.getParamNums(refParams, env, envDataSet);
		std::vector<int>& testParams = refParams;
		
		// ��������� params
		pRecognizer->setParamsOfDataSet(refParams);
		
		TemplateRecognizer t;
		// ��������
		pRecognizer->learn(t);
		
		AxiomSet axiomSet;		
		
		firstKindErrors = 0;
		secondKindErrors = 0;
		// ������ � ��������� ����� ������
		pRecognizer->run(axiomSet, dataSet, testParams, firstKindErrors, secondKindErrors);
		
		std::cout<<"Overall statistics:\n";
		std::cout<<"\tfirst-kind errors: "<<firstKindErrors<<"\n\tsecond-kind errors: "<<secondKindErrors<<"\n";
		delete pRecognizer;
		pRecognizer = 0;
	} catch (AxiomLib::AxiomLibException testex) {
		delete pRecognizer;
		pRecognizer = 0;
		// ������������ ������
		throw;
	}	
}

int main (int argv, char** argc) {
	try {
		int f, s;
		recognizeOnce("axiomlib_neural.conf", f, s);
	} catch ( AxiomLib::AxiomLibException testex ) {
		std::cerr << "Fatal exception:\n\t";
		std::cerr << testex.error() << "\n";
	}
				
//		std::cout<<"Press enter to exit\n";
//		std::cin.get();
	return 0;
}
