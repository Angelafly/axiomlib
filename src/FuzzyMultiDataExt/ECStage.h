/****************************************************************************
*				ECStage
*
*	Description: �����, ��������������� ������ ���������� ��
*				��������� FuzzyMultiDataExt
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef FUZZYMULTIDATAEXT_EC_H
#define FUZZYMULTIDATAEXT_EC_H

#include <boost/tuple/tuple.hpp>

#include "../Environment.h"
#include "../ClippedFullFuzzyDataSet.h"
#include "Common.h"
#include "ForwardDeclarations.h"


namespace AxiomLib {

namespace FuzzyMultiDataExt {

typedef ElementSelection<AxiomLib::ElemCondPlusStat> ECSelection;
	
class ECStage
{
public:	
    ECStage(ClippedFullFuzzyDataSet* fuzzyDataSet, 
	                     ECTypeStage* stage0);
	
	void setECs(const std::vector<std::vector<std::vector<std::vector<ElemCondPlusStat> > > > & value);
	
	int initFromEnv(const Environment& env);
	
	void run();
	
	// ������� ������� � ������ ��
	const ElemCondPlusStat & getEC(int aType, int dimension, int type, int n) const;
	
	bool isECSelected(int aType, int dimension, int type, int n) const;
	
	void setECSelected(int aType, int dimension, int type, int n, bool value);
	
	void getECSize(std::vector<std::vector<std::vector<int> > > &result) const;
	
	int getECSize() const ;
	int getECSize(int aType) const ;
	int getECSize(int aType, int dimension) const;
	int getECSize(int aType, int dimension, int ecType) const;
	
	int getECTotalCount() const;
	
	void recalculateMatterECFunc(ElemCondPlusStat& ec, int abType) const;
	
private:
	
	typedef std::vector<std::vector<std::vector<std::vector<
			ECSelection
			> > > > ECMultiVector;

	typedef boost::tuples::tuple<double, double, double, double> StatVector;
	
	const ClippedFullFuzzyDataSet* fuzzyDataSet;
	const ECTypeStage* stage0;
	
	ECMultiVector elemConditions;	
	
	
	std::vector<int> dataSetParams;
	std::vector<std::string> dataSetParamNames;
	
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
	
	void checkIndices(int i, int j, int k, int l) const;
	
	std::string getECName(int abType, int dimension, int ecType, int n) const;
	
	void prune();
	
	void setNames();
};

};

};

#endif // FUZZYMULTIDATAEXT_EC_H
