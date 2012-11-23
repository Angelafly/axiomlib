/****************************************************************************
*					FuzzyMultiDataExtAlgorithm
*
*	Description:	�����, ����������� �������������� �������� � �����������
*					� ����������� �������/������ ���������� � �����������
*					������� ����� (������������ � GUI)
*	Author:			wictor
*	History:	
*
****************************************************************************/
#ifndef FUZZYMULTIDATAEXT_H
#define FUZZYMULTIDATAEXT_H

#include <boost/tuple/tuple.hpp>

#include "Environment.h"

#include "ElemCondPlusStat.h"
#include "AxiomExprPlus.h"
#include "AxiomExprSetPlus.h"
#include "FuzzyDataSet.h"

#include "ReducedRecognizer.h"
#include "GoalStrategy.h"

#include "FuzzyMultiDataExt/ForwardDeclarations.h"

#include "Logger.h"

#include "StringPlus.h"

namespace AxiomLib {
	
class FuzzyMultiDataExtAlgorithm
{
public:	
    FuzzyMultiDataExtAlgorithm();
	~FuzzyMultiDataExtAlgorithm();
	
	// ������������� ����� �������� ���������� ������ comments
	int setComments (const bool newVal);
	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� ������������� ��������� ������ ������ ��� ����������� ������������� � �������� ������� ������� ��� ������� ������
	//int initFromEnvRecognitionOnly (const Environment& env);
	
	// ������ � ��������
	FuzzyDataSet &getDataSet();
	const FuzzyDataSet& getDataSet() const;
	
	// �������� ������� ������ - ������ ������� ������� ������
	void run (void);

	// ������� ������� � ������ ����� ��
	void setECTypes(const std::vector<ElemCondPlus>& ecTypes);
	const ElemCondPlus& getECType(int n) const;
	bool isECTypeSelected(int n) const;
	void setECTypeSelected(int n, bool value);
	int getECTypeSize() const;
	
	// ������ �������� ������ ����� ��
	void runZeroLevelHeuristics();
	
	// ������ ������ ������ �� 3-� ������ ��������
	void runFirstLevel (void);
	
	// ��������� ������ ��
	void runFirstLevelHeuristics();
	
	// ������� ������� � ������ ��
	void setECs(const std::vector<std::vector<std::vector<std::vector<ElemCondPlusStat> > > >& ecs);
	const ElemCondPlusStat & getEC(int aType, int dimension, int type, int n) const;
	bool isECSelected(int aType, int dimension, int type, int n) const;
	void setECSelected(int aType, int dimension, int type, int n, bool value);
	void getECSize(std::vector<std::vector<std::vector<int> > > &result) const;
	
	int getECSize() const ;
	int getECSize(int aType) const ;
	int getECSize(int aType, int dimension) const;
	int getECSize(int aType, int dimension, int ecType) const;

	struct ECIndex {
		int abType;
		int dimension;
		int ecType;
		int n;
		ECIndex(int abType, int dimension, int ecType, int n);
	};

	const ElemCondPlusStat & getEC(const ECIndex& index) const;
	bool isECSelected(const ECIndex& index);
	void setECSelected(const ECIndex& index, bool value);

	static ECIndex makeECIndex(int abType, int dimension, int ecType, int n);
	
	void saveECs(const std::string& dir, const std::string& listFileName, bool selectedOnly = false) const;
	void loadECs(const std::string& dir, const std::string& listFileName);
	
	// ������ ������ ������ �� 3-� ������ ��������
	void runSecondLevel();
	
	// ��������� ������ ������
	void runSecondLevelHeuristics();
	
	// ������� ������� � ������ ������
	void setAxioms(const std::vector<std::vector<AxiomExprPlus> >& axioms);
	const AxiomExprPlus &getAX(int aType, int n) const;
	bool isAXSelected(int aType, int n) const;
	void setAXSelected(int aType, int n, bool value);
	void getAXSize(std::vector<int> &result) const;
	
	int getAXSize() const;
	int getAXSize(int aType) const;
	
	void saveAXs(const std::string& dir, const std::string& listFileName, bool selectedOnly = false) const;
	void loadAXs(const std::string& dir, const std::string& listFileName);
	
	// ������ ������ ������� �� 3-� ������ ��������
	void runThirdLevel();
	
	// ������� ������� � ������ ������ ������
	void setAxiomSets(const std::vector<AxiomExprSetPlus>& axiomSets);
	AxiomExprSetPlus &getAS(int n);
	const AxiomExprSetPlus& getAS(int n) const;
	int getASSize() const;
	
	void saveASs(const std::string& dir) const;
	void saveASs() const;
	void loadASs(const std::string& dir, const std::vector<std::string>& asNames);
	
	void recalculateMatterECFunc(ElemCondPlusStat& ec, int abType);
	void recalculateMatterAxiomFunc(AxiomExprPlus& ax, int abType);
	void recalculateMatterAxiomSetFunc(AxiomExprSetPlus& as);

	bool oneVsAllMode() const { return mOneVsAllMode; }
	
private:
	
	// �������� ����� ������, �� ������� ������������ ��������
	FuzzyDataSet fuzzyDataSet;

	// ����� "���� ������ ����". ���� true, ��
	// ��� ������� �� � ������ ��� ���������� ������ ���������� ���������
	// ���������� ������ ������� ���������� ��������� ����������� � ������� �������� 1-�� � 2-�� ������
	// ��� ���������� ����������� ���������
	bool mOneVsAllMode;

	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> dataSetParams;

	// ��������� ��� ���������� ������ ������
	std::string axiomBaseDir;
	
	// ��� �����, � ������� ��������� ������ ���� ����������� ������
	std::string fileNameECList;
	// ��� �����, � ������� ��������� ������ ���� ����������� ������
	std::string fileNameAxiomList;
	// ��������� ��� ���������� ������ �������
	std::string axiomSetBaseDir;
	std::string axiomSetName;

 protected:
	Logger* logger;
	
	FuzzyMultiDataExt::HeuristicsSet* heuristicsSet;
	
	FuzzyMultiDataExt::ECTypeStage* stage0;
	FuzzyMultiDataExt::ECStage* stage1;
	FuzzyMultiDataExt::AXStage* stage2;
    FuzzyMultiDataExt::ASStage* stage3;

	// ������ ������������ �������, ����������� � ���������� �� ��������
	//std::vector <std::vector <ElemCondPlus> > bestECs;
	//std::vector <std::vector <double> > bestECres; // ������� ������� ��� ��������� �������
	// ������ � ������� ������, ���������� �������� ������������ �������, ������� ����� �������������� �� ������ ���� ���������
	std::vector <std::vector <std::vector <std::vector <std::string> > > > bestECForAllAbnormalTypes;
	// ��������� ���� ������, ���������� �������� ������������ �������, ������� ���������� �� ������ ����� ������, ��� ��� ������ � ���������������� �����
	std::set <std::string> predefinedECFileName;

	// ������ �������� ������ � ���������� ������ ������ - ��� ������� ���� ���������� ���������
	std::vector <std::vector <std::string> > bestAxiomsFileNames;
	// ��������� ���� ������, ���������� �������� ������, ������� ���������� � ������ ���� ������, ��� ��� ������ � ���������������� �����
	std::set <std::string> predefinedAxiomsFileName;

	// ������ � ������� ����������� ������ � ���������� ������ ������ ������ � ���������� �� ���
	std::vector <StringPlus> bestAxiomSetsNamesPlus;

};

};

#endif // FUZZYMULTIDATAEXT_H
