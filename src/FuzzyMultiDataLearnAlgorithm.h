/***************************************************************************
*					FuzzyMultiDataLearnAlgorithm
*
*	Description:	����� ����������� �������� ������ ������� ������ ���
*					�� �������� ���������� ����������� ��������� �� ���������
*					�������, ���������� ����������� ����������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Fuzzy_Multi_Data_Learn_Algorithm_HXX
#define _Fuzzy_Multi_Data_Learn_Algorithm_HXX

#include "all_axioms.h"
#include "AxiomLibException.h"
#include "AxiomBase.h"
#include "AxiomExpr.h"
#include "AxiomExprPlus.h"
#include "AxiomExprSet.h"
#include "AxiomExprSetPlus.h"
#include "ElemCondPlus.h"
#include "ElemCondPlusStat.h"
#include "ElemConditionsFactory.h"
#include "Round.h"
#include "Environment.h"
#include "ReducedRecognizer.h"
#include "ReducedRecognizerFactory.h"
//#include "CompareStatistic.h"
#include "CompareClassFactory.h"
#include "GoalStrategyFactory.h"
#include "GoalStrategy.h"
#include "PreprocessFactory.h"
//#include "FullFuzzyDataSet.h"
#include "ClippedFullFuzzyDataSet.h"
#include "StringPlus.h"
//#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <omp.h>

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"

namespace AxiomLib {

class FuzzyMultiDataLearnAlgorithm {
	friend class FuzzyMultiDataAccessor;
 private:
	// ������, �� ������� ��������� �������� ���������� ������� ������ � ������ ��������
	unsigned short stage;
	
	// �������� ����� ������, �� ������� ������������ ��������
	ClippedFullFuzzyDataSet fuzzyDataSet;
	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> dataSetParams;

	// ��������� ��� ���������� ������ ������
	std::string axiomBaseDir;
	std::string axiomNameTemplate;
	// ������ ��� �������� ������, ��� �������� ��������� ������������ �������, ����� ������� ����� ���������
	std::string ecNameTemplate;
	// ��� �����, � ������� ��������� ������ ���� ����������� ������
	std::string fileNameECList;
	// ��� �����, � ������� ��������� ������ ���� ����������� ������
	std::string fileNameAxiomList;
	// ��������� ��� ���������� ������ �������
	std::string axiomSetBaseDir;
	std::string axiomSetName;

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

	// ��������� ��������� ��������� ������
	// ������������ ����� ������ � ���������
	int maxAxiomPopSize;
	// ������������ ����� �������� ��������� ������ ������
	int maxIterNum;
	// ������������ �������� ������� ������� ��� �������, ���� ��� �����������, �� ����� ����� ������ ���������������
	double maxAxiomRes;
	// ������� ������ ������ ���������, ������� ����������� ��� ���������� ���������
	double percentBestAxioms;

	// ��������� ��������� ���������� ������ ������
	// ��������� ������, ���� ��� ������ �������� �������� ������� �������� ��� ������������ � ������ ��������
	double shiftErrIn;
	// ������� ���������� �������� (���������� ��� ������ �������� �� ����� �� �������)
	double extendMarkUpLensLevel;
	// ������������ ����� ������ � ���������
	int maxAxiomSetPopSize;
	// ������������ ����� ����� � ��������� ���������� ������ ������
	int maxNumberOfSteps;
	// �������� ��������� ��� ��������� ���������� ������� ������ - ��������� �������� ������� �������
	double bestAxiomSetGoal;
	// ���� ������ ����������� ������ ������ ��� ������� ��������
	int numberOfBestAxiomSets;
	// ������� ������ ������ ������ � ���������, ������� ����������� ��� �������� �� ��������� ��� - (1 - percentBestAxiomSets) - ���������� ��������
	double percentBestAxiomSets;
	// ��������� ��������� ������������ �������� ��������� ����������
	// ����� ���������� ��� ������� �� ������� ���������� ��������� �� ������ Reference, ������� ����� �������������� ��� ������������ �������� ��������� ����������
	int numberOfUsedClassTraj;

 protected:

	// ������� ���������� ������, �� ������� ������������ �������� �� ����������� � �������� �����
	bool comments;

	bool managed;

	// ����� ��� ������������� ��������� ���������� � ������������� ����
	ReducedRecognizer* recognizer;

	// ����� �������� ����� ������ I � II ���� �� �������� �������
	//CompareStatistic* compareStatistic;
	int ccNumPoints; // ����� ����� � �������, ����� �������� ��������� ������ �� ������ ������ ����� � ���� ������� (������������ ��� �������� ����������)

	// ��������� �������� ������� ������� �� ������� I � II ����
	GoalStrategy* goalStrategy;

	//�������� ���� ������������ �������
	std::vector<ElemCondPlusStat> initialECTypes;

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

	// ������ ������ ������, ����������� �� ��������� ������
	std::vector <AxiomExprSetPlus> bestAxiomSets;
	// ������ � ������� ����������� ������ � ���������� ������ ������ ������ � ���������� �� ���
	std::vector <StringPlus> bestAxiomSetsNamesPlus;

		// ������� ������

	// ������� ������� � ��������� ������������ ������� - ��������
	int selectElemCond (void);

	// ������� ������� � ��������� ������������ ������� ��� ��������� ���� ���������� ���������
	int selectElemCond (const int abnormalBehaviourType, const int rank, const int groupRankFrom, const int groupRankUpTo, std::vector <std::vector <std::vector <std::string> > > &bestECFileNames);

	// ������� ������� � ��������� ������������ ������� ��� ��������� ���� ���������� ��������� � ��� �������� ����������� �������� ������
	int selectElemCond (const int abnormalBehaviourType, const int dimension, std::vector <std::vector <ElemCondPlusStat> > &bestECs) const;
	int selectElemCond (const int abnormalBehaviourType, const int dimension, std::vector <std::vector <ElemCondPlus> > &bestECs, std::vector <std::vector <double> > &bestECres);

	// ������� ������������ ������ �� ������������ �������
	int formAxioms (void);

	// ������������ ������ ��� ��������� ���� ���������� ���������
	int formAxioms (const int abType, std::vector <std::vector <std::vector <std::string> > > &ecFileNames, std::vector <AxiomExprPlus> &axioms, std::vector <std::string> &axiomFileNames) const;
	int formAxioms (const int abType, std::vector <std::vector <std::vector <std::string> > > &ecFileNames, std::vector <AxiomExpr> &axioms, std::vector <double> &axiomRes, std::vector <std::string> &axiomFileNames) const;

	// ������� ������������ ������� ������ �� ������
	int createAxiomSet (void);

	// ���������� ���� ���� ������ � ���������� ������������ ������� � ����
	std::string saveBestECFileNames (const int rank) const;

	// ���������� ���� ���� ������ � ���������� ������ ������ � ����
	std::string saveBestAxiomFileNames (void) const;

	// ���������� ���� ������ � ���������� ������������ ������� �� �����
	int restoreBestECFileNames (void);

	// ���������� ���� ������ � ���������� ������������ ������� �� ����� c ��������� � ���������� �����/������
	int restoreBestECFileNames (const std::string filePath);

	// ���������� ���� ������ � ���������� ������ �� �����
	int restoreBestAxiomFileNames (void);

	// ���������� ���� ������ � ���������� ������ �� ����� c ��������� � ���������� �����/������
	int restoreBestAxiomFileNames (const std::string filePath);

	// ������� �������� ����� ������ � ���������� ������ ������ ������ � ������ ���������, �� ������� ��� ������������� �������� ��������
	int gatherBestAxiomsSets (const int rank, const int size);

	// ������� ���������� �� ������ �������� ������� ������� ��� ������ ������ ������
	int displayBestResult(void) const;

	// ������� ��������� ������� ������ � ������� ������ ������
	int addToBestAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets);

	// ������� ��������� ������� ������ �� ������ ������� � ������ � ������������ � ������������� ���������� ��������
	int addAxiomSets(std::vector <AxiomExprSetPlus> &nextStepAxiomSets, std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets) const;

	// ������� ��������� �������� ������� ������ ������ ������ � ��������� ����� ��������� ������� ������� �� ��������� ��������
	int sortBestAxiomSets (void);

	// ������� ��������� �� ���������� ������� ������� �������� �������� ������� axiomSets
	int sortAxiomSets (const std::vector <AxiomExprSetPlus> &axiomSets, std::vector <int> &indecies) const;

	// ������� �������� �������� ����� ������ ������ ������, ��� ������� �������� ������� ������� ������ ��������� ���������
	int chooseIndicesOfBestAxiomSets (std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets, double &goal) const;

	// ������� ���������� ������ ������� ��������� �������� � ������ ��� -1, ���� ������ �������� ���
	inline int isIn (const unsigned int elem, const std::vector <unsigned int> &vec) const;

	// ������� ��������� � ����� ������ �������������� �� ������ ����� �����
	int saveBestAxiomsInFiles (const int abnormalBehaviourType, std::vector <AxiomExprPlus> &axioms, std::vector <std::string> &axiomFileNames) const;

	// ������� �������� ����� ������ � ���������� ������ ������ � ������ ���������, �� ������� ��� ������������� �������� ��������
	int gatherBestAxioms (void);

	// ������� ��������� ������ ���� ������ � ���������� ������ ������ - ������� �� ����������������� �����
	int updateBestAxiomsListFromEnv(void);

	// ������� ����������� �� ����� ����� ��� ��������� �������� ���������
	int disintegrateAxiomFileName(const std::string fileName, int &at, int &index) const;

	// ������� ������������ �������� �������� [rangeMin, rangeMax] �� size ������� (���������) � ������ ��������� ��� �������� ������ (��������)
	int splitRange (const int rank, const int size, const int rangeMin, const int rangeMax, int &from, int &upTo, int &rankFrom, int &rankUpTo) const;

	// ������� ��������� ��������� ����� ���������� ��������� �� ������������ ��� ����������� ��������� � ������ ������ ���������
	int splitAbnormalTypeRange (int rank, int size, int &from, int &upTo, int &rankFrom, int &rankUpTo) const;

	// ������� ����� ���������� ���������� �� ���� ��������� ����� ������� ����� ������ ���������
	//int gatherBestECfromProcesses (int rank, int size);
	int gatherBestECNames (const int rankFrom, const int rankUpTo, std::vector <std::vector <std::vector <std::string> > > &ecNames, std::vector < std::vector < std::vector <std::string> > > &allNames) const;

	// �������  ��������� ������ ���� ������ � ��������� ���������� ������������ ������� ������� ������ �� ����������������� �����
	int updateBestECListFromEnv(void);

	// ������� ������ ��������� ����� � ������ str ������� � ���������� �������
	int getNearestNumber (const std::string str, std::string::size_type &index) const;

	// ������� ��������� �������� ����� �� ����� - � ����������� ������������ ������� �������
	int disintegrateECFileName(const std::string fileName, int &at, int &dim, int &vecI, int &vecJ);

	// ������� ��������� �������� ������������ ������� �� �������� ������ ������ ������� � �����
	int saveBestECinFile (const int abnormalBehaviourType, const int  dimen, std::vector <std::vector <ElemCondPlusStat> > &bestECs, std::vector <std::vector <std::string> > &ecNames) const;

	// ������� ���������� ������ ������������ ������� � ��������������� �������� ������� �������
	inline int storeBestECs (std::vector <ElemCondPlusStat> &bestECs, ElemCondPlusStat &ec, double &goal, double &statNorm, double &statAbnorm) const;

	// ������� ����� ����� � ����, �� ������� ������������ ������� �����������
	inline int numOfCarriedOutItems (ElemCondPlus &ec, std::vector <double> &row) const;

	// ������� ����� ����� � ����, �� ������� ������� �����������
	inline int numOfCarriedOutItems (AxiomExpr &ax, std::vector < std::vector <double> > &row, int &rowLen) const;

	// ������� ������� ������� ��� ������� ������
	double matterAxiomSetFunc (AxiomExprSetPlus &as) const;

	// ������� ������� ������� ��� ������� ������ ��� ��������� ���� ���������� ��������� � �������� �������� ��������� ���������� ����� ����
	double matterAxiomSetFunc (AxiomExprSetPlus &as, int abType, std::vector <int> &genMarkUp, double &goalVal, int &errFirstVal, int &errSecondVal) const;

	// ������� ������� ����� ��������� �������� � ����
	inline int getStatistic (std::vector <int> &row) const;

	// ������� ��������� ����� ������ ������ �� ������� ������� �� ��������� � ���������� ������ ��������
	int cutDownAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) const;

	// ������� ��������� ��������� ������������ ������ �� ������������� � ���������� ��������
	int cutDownBestAxioms (std::vector <AxiomExprPlus> &axioms) const;

	// ��������������� ������� ���������� ������� �� �������� � ��������� �������� ���������� � ��������� �������
	inline int getPriority (std::vector <AxiomExprPlus> &vec, std::vector <unsigned int> &pos) const;

	// ����������� ���� ������ � ���������� ���������� ��� ���������� ����� ������
	int combineAxioms (AxiomExprPlus &axiomFirst, AxiomExprPlus &axiomSec, AxiomExprPlus &axiomExprPlus, const int abType) const;

	// �������� ������������ ���� �������� ������ as
	inline int createRefMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, const int abType, const int multiTSNumber, std::vector < std::vector <int> > &numOfTS) const;

	// �������� ������������ ���� �������� ������ as 
	inline int createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &abType, int &indexTest, int &numOfTestTS) const;

	// �������� ������������ ���� �������� ������ as 
	inline int createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &indexTest, int &numOfTestTS) const;

	// ������� ������� ������� �������� ���������� ���������� ��������� �� ��������� ����������, ���������� ������� ���������� ���������
	inline int createMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, std::vector < std::vector <int> > &resMarkUps) const;

	// ������� ������� �������� ��������, ��������� ������ �� ����� �������
	inline int createSimpleMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, const int numOfAxioms) const;

	// ������� �������� �� ������ ���������� � �������� ��������� ���������� ��������� ���� ������
	int chooseBestMarkUp (AxiomExprSetPlus &as, int abType, std::vector <int> &markUp, std::vector <std::vector <int> > &genMarkUps, int &errFirstVal, int &errSecondVal) const;

	// ��������� �������� � ���������� �� � ����������� ��������
	inline int simplifyMarkUps (std::vector <std::vector <int> > &markUps) const;
	//  ������ �������
	//int simplifyMarkUp (std::vector <int> &markUp, std::vector <int> &markUpLen, std::vector <int> &genMarkUp, std::vector <int> &genMarkUpSens) const;
	//  ������ ������ ������� ����������
	//int simplifyMarkUp2 (std::vector <int> &markUp, std::vector <int> &markUpLen, std::vector <int> &genMarkUp, std::vector <int> &genMarkUpSens) const;
	
	// ������� ������ ���������� ����� ������� ����������
	//inline double countCutCriteria (int &len, int &val) const;

	// ������� ��������� ������� ������������ ��������� ���� �����
	int getSquare (const std::vector <int> &str1, const std::vector <int> &str2, std::vector < std::vector <int> > &square) const;

	// ������� ���� ��� ������������ ����� ��������������������� ���� �����
	int getSubsequences (const std::vector <int> &str1, const std::vector <int> &str2, int i, int j, const std::vector < std::vector <int> > &square, std::vector < std::vector <int> > &res) const;

	// ��������� �������� ������� �� �������� �����������
	//inline int extendMarkUpLen (std::vector <int> &row, std::vector <int> &minLen, std::vector <int> &maxLen, std::vector <int> &stdLen) const;

	// ���������� �������������� ������ ������
	int saveBestAxiomSets (const int rank, std::vector <std::string> &dataSetParams);

 public:

	// ������ �����������:
	FuzzyMultiDataLearnAlgorithm (void);  
  
	// �����������, ������������ ��� ���������� �������� ������ �������
	~FuzzyMultiDataLearnAlgorithm (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� ������������� ��������� ������ ������ ��� ����������� ������������� � �������� ������� ������� ��� ������� ������
	int initFromEnvRecognitionOnly (const Environment& env);
	
	// �������� ������� ������ - ������ ������� ������� ������
	int run (void);

	// ������ ������ ������ �� 3-� ������ ��������
	std::string runFirstLevel (void);

	// ������ ������ ������ �� 3-� ������ �������� (��� ���������� ���������)
	std::string runSecondLevel (std::vector <std::string> &ecListFileVecs);
	// ������ ������ ������ �� 3-� ������ ��������
	std::string runSecondLevel (const std::string ecListFilePath);

	// ������ ������ ������� �� 3-� ������ ��������
	std::string runThirdLevel (const std::string axiomListFilePath);

	// ������� ������� ������� ��� ������� ������
	double matterAxiomSetFunc (const std::string baseDir, const std::string fileName, int &errFirst, int &errSecond) const;
	
	// ������� ������� ������� ��� ������������� �������
	double matterECFunc (ElemCondPlusStat &ec, const int param, const int abnormalBehaviourType) const;
	
	// ������� ������� ������� ��� �������
	double matterAxiomFunc (AxiomExprPlus &ax, const int abType) const;

	// ������� ������� ������� ��� ������� ������ � �������� ������� �������� ��������� ����������
	double matterAxiomSetFunc (AxiomExprSetPlus &as, std::vector <std::vector <int> > &markUps) const;	

	// ������������� ����� �������� ���������� ������ comments
	int setComments (const bool newVal);

}; // end of class

}; // end of namespace

#endif /* _Fuzzy_Multi_Data_Learn_Algorithm_HXX */
