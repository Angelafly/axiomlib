/****************************************************************************
*				ECStage
*
*	Description: �����, ��������������� ������ ���������� ��
*				��������� FuzzyMultiDataExt
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef FUZZYMULTIDATAEXT_EC_SIMPLE_H
#define FUZZYMULTIDATAEXT_EC_SIMPLE_H

#include <boost/tuple/tuple.hpp>

#include "../Environment.h"
#include "../FuzzyDataSet.h"
#include "Common.h"
#include "ForwardDeclarations.h"

#include "ECStage.h"


namespace AxiomLib {

namespace FuzzyMultiDataExt {
	
class ECStageSimple : public ECStage
{
public:	
	ECStageSimple(FuzzyDataSet* fuzzyDataSet,
	                     ECTypeStage* stage0);	
	
	int initFromEnv(const Environment& env);
	
	void run();
	
	bool isECSelected(int aType, int dimension, int type, int n) const;	

	void setECSelected(int aType, int dimension, int type, int n, bool value);
	
	void recalculateMatterECFunc(ElemCondPlusStat& ec, int abType) const;	
private:
	typedef boost::tuples::tuple<double, double, double, double> StatVector;
	
	std::vector<int> dataSetParams;
	std::vector<std::string> dataSetParamNames;
	
	bool entropyObjective;
	bool ecReversing;

	// ��������� ��������� ��������� ������������ �������
	int leftLimit;
	int rightLimit;
	// ��������, �� ������� ��������� ��������� �������� ��� ������
	double reserve;
	// ����� ��������, �� ������� ��������� �������� ��������� ���������� ��� ������ (������� �����)
	int numOfLevels;
	// ����� ������ ����������� ������������ ������� ������� ����, ������� ����������� ����� ������� ����� ������ ���������
	int numBestECs;
	// ������������ �����, �� �������� ����� �� ���������� ��������� numBestECs
	int numBestECsMax;	
	
	double goalOccurenceWeight;
	
	// ������ ��� �������� ������, ��� �������� ��������� ������������ �������, ����� ������� ����� ���������
	std::string ecNameTemplate;	
	
	Logger* logger;
	
	// ����� ����� ��������� ��������� ��������� ���� ��� ������ ����
	mutable std::vector<std::vector<double> > m_storage;
	
	void setTaskCount(int n) const;
	
	void selectElemCond(int classNo, int dimensionNo, int dimensionIndex, int taskNo);
	
	double matterECFunc (ElemCondPlusStat &ec, const int param, const int abnormalBehaviourType, int taskNo) const;
	
	int storeBestECs (std::vector <ECSelection> &bestECs, ElemCondPlusStat &ec, double &goal, double &statNorm, double &statAbnorm) const;
	
	int numOfCarriedOutItems (ElemCondPlus &ec, std::vector <double> &row) const;
	
	std::string getECName(int abType, int dimension, int ecType, int n) const;
	
	void prune();
	
	void setNames();
};

};

};

#endif // FUZZYMULTIDATAEXT_EC_SIMPLE_H
