#include "Environment.h"
#include "AxiomSetPop.h"
#include "GoalStrategyFactory.h"
#include "GoalStrategy.h"
#include "RecognizerMetric.h"
#include "RecognizerExtend.h"
#include "Recognizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace AxiomLib;

#define comments true

int main (int argv, char** argc) {
	try {
		std::cout << "\nTest starts.\n";
		srand(time(0));
		if (comments)
		std::cout << "\n" << "AxiomLib 0.6 / Test_Execute.\nReading config-file." << "\n";
		 // ��������� ���� ������������ �� ��������� ���������
		Environment env;
		env.readConfigFile("./axiomlib_bg.conf");
		
		// ��������� ����� ������ ������
		AxiomSetPop axiomSetPop;
		// �������������� ����� ������ ������
		axiomSetPop.initFromEnv(env);

		// �������������� ��������� ���������� ������� �������
		string goalStrategyName;
		GoalStrategyFactory gsf;
		if (env.getStringParamValue(goalStrategyName, "goalClass") < 0)
			throw AxiomLibException("Top level: main : goal strategy is undefined.");
		GoalStrategy* goalStrategy;
		goalStrategy = gsf.create(goalStrategyName);
		goalStrategy->setParamsFromEnv(env);
		
		// ����� ��������������
		RecognizerExtend recognizer;
		//RecognizerMetric recognizer;
		// �������������� ��������������
		recognizer.setParamsFromEnv(env);
		
		// 1. ��������� �� ������������ ��������� �������� �������� � �������������, �������� ����� ������ ������� � ������� ����, ��������� ����������.
		recognizer.run(axiomSetPop);
		
		// 2. ��������� ���������� H(first, second)
		goalStrategy->compute(axiomSetPop);
		
		int tmpFirst, tmpSecond;
		double goal;
		std::cout << "\n 	Goals are \n	";
		for (int asNum = 0; asNum < axiomSetPop.size(); asNum++) {
			axiomSetPop.getStats (asNum, tmpFirst, tmpSecond, goal);
			std::cout << "	first " << tmpFirst;
			std::cout << "	second " << tmpSecond;
			std::cout << "	goal " << goal << "\n";
		}
		
		std::cout << "\nTest done.\n";
		return 0;
	}
	catch ( AxiomLibException testex ) {
	      std::cerr << "Fatal exception:\n\t";
	      std::cerr << testex.error() << "\n";
	 }
}
