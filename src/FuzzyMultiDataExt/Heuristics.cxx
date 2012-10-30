#include <algorithm>
#include <string>

#include "../Logger.h"
#include "../FuzzyMultiDataExtAlgorithm.h"

#include "Common.h"

#include "Heuristics.h"

using namespace AxiomLib;
using namespace AxiomLib::FuzzyMultiDataExt;

Heuristics::Heuristics() : m_order(1)
{
}

std::string Heuristics::getParamName(const std::string &heuristicsName, const std::string &paramName) {
	return heuristicsName + "_" + paramName;
}

std::string Heuristics::getParamName(const std::string &paramName) const {
	return getParamName(this->name(), paramName);
}

int Heuristics::order() const {
	return m_order;
}

void Heuristics::setOrder(int value) {
	m_order = value;
}

void Heuristics::run(FuzzyMultiDataExtAlgorithm &algorithm, int classNo) const {
	// ��� ������������ ������ Heuristics ���������� �������������� ���� �� ������� run
	throw AxiomLibException("Heuristics::run() : method not implemented");
}

void Heuristics::initFromEnv(const Environment &env) {
	//Logger::getInstance()->writeDebug("Entering initFromEnv() for "+name());
	env.getParamValue(m_order, getParamName("order"));
	
	m_classNames.clear();
	
	std::set<std::string> classNames;
	if(env.getStringSetParamValue(classNames, "AllHeuristics_className")) {
		env.getStringSetParamValue(classNames, getParamName("className"));
	}
	
	m_classNames.reserve(classNames.size());
	m_classNames.assign(classNames.begin(), classNames.end());
	
	initFromEnvImpl(env);
	//Logger::getInstance()->writeDebug("Leaving initFromEnv() for "+name());
}

void Heuristics::run(FuzzyMultiDataExtAlgorithm &algorithm) const {
	
	// 1. ��������� ����� ������� �������, ��������� ��� ���������� ���������
	std::vector<std::string> allClassNames = algorithm.getDataSet().getClassNames();
	const std::vector<std::string>& classNames = m_classNames.empty() ? allClassNames : m_classNames;
	
	std::vector<int> classNums;
	classNums.reserve(classNames.size());
	
	for(auto it = classNames.begin(); it != classNames.end(); ++it) {
		int index = isIn(*it, allClassNames);
		
		if(index < 0) {
			// �����, ��� ��� ������ ������� ������ �����, � �� � initFromEnv
			// �� ������ �� ����� �������
			throw AxiomLibException("Heuristics::run(): invalid class name : "+*it);
		}
		
		classNums.push_back(index);
	}
	
	// 2. ���������, ����� ��� ������������ �������
	std::sort(classNums.begin(), classNums.end());
	
	// 3. ��������� ��������� � ��������� �������
	for(auto it = classNums.begin(); it != classNums.end(); ++it) {
		int classNo = *it;
		
		Logger::getInstance()->writeComment("Running heuristics "+name()+
		                                    " for class "+allClassNames[classNo]);
		
		run(algorithm, classNo);
		
		Logger::getInstance()->writeComment("Finished heuristics "+name()+
		                                    " for class "+allClassNames[classNo]);
	}
}
