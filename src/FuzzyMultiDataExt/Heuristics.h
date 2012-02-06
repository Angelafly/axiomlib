/****************************************************************************
*				Heuristics.h
*
*	Description: ������� ����� ��� ��������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef FUZZYMULTIDATAEXT_HEURISTICS_H
#define FUZZYMULTIDATAEXT_HEURISTICS_H

#include <string>

#include "../Environment.h"

#include "ForwardDeclarations.h"

namespace AxiomLib {

namespace FuzzyMultiDataExt {

class Heuristics
{
public:
    Heuristics();
	
	// ������������� �� ���������
	void initFromEnv(const Environment& env);
	
	// ������� -- �� ���� ����������� ���������, � ����������� � ���� �������
	int order() const;
	
	// ��� ���������
	virtual std::string name() const = 0;
	
	// ������, �� ������� ����������� ���������
	virtual int stage() const = 0;
	
	// ����������� ����������� �����������
	virtual Heuristics* clone() const = 0;
	
	// ������ ��������� �� ���������
	virtual void run(FuzzyMultiDataExtAlgorithm& algorithm) const;
	
	static std::string getParamName(const std::string& heuristicsName, const std::string& paramName);
private:
	int m_order;

	// ����� �������, �� ������� ��������� ���������
	std::vector<std::string> m_classNames;
protected:
	// ����������� ���������� ���� order �� ��������� ��� ���������� ���������
	void setOrder(int value);
	
	virtual void initFromEnvImpl(const Environment& env) = 0;
	
	virtual void run(FuzzyMultiDataExtAlgorithm& algorithm, int classNo) const;
	
	std::string getParamName(const std::string& name) const;
};

};

};

#endif // FUZZYMULTIDATAEXT_HEURISTICS_H
