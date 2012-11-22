/***************************************************************************
*					FuzzyDataLearnAlgorithm
*
*	Description:	����� ����������� �������� ������ ������� ������ ��� 
*					�� �������� ���������� ����������� ���������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Fuzzy_Data_Learn_Algorithm_HXX
#define _Fuzzy_Data_Learn_Algorithm_HXX

#include "all_axioms.h"
#include "AxiomLibException.h"
#include "AxiomBase.h"
#include "AxiomExpr.h"
#include "AxiomExprSet.h"
#include "ElemCondPlus.h"
#include "ElemConditionsFactory.h"
#include "Round.h"
#include "Environment.h"
#include "CompareStatistic.h"
#include "CompareClassFactory.h"
#include "GoalStrategyFactory.h"
#include "GoalStrategy.h"
#include "PreprocessFactory.h"
#include "FuzzyDataSet_old.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>

namespace AxiomLib {

class FuzzyDataLearnAlgorithm {

 private:
	// ������, �� ������� ��������� �������� ���������� ������� ������ � ������ ��������
	unsigned short stage;
	
	// �������� ����� ������, �� ������� ������������ ��������
	FuzzyDataSet_old fuzzyDataSet;
	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> dataSetParams;

	// ��������� ��� ���������� ������ �������
	std::string axiomSetBaseDir;
	std::string axiomName;

	// ��������� ��������� ��������� ������������ �������
	int leftLimit;
	int rightLimit;
	// ��������, �� ������� ��������� ��������� �������� ��� ������
	double reserve;
	// ����� ��������, �� ������� ��������� �������� ��������� ���������� ��� ������ (������� �����)
	int numOfLevels;
	// ��������� ��������� ���������, ����������� ��� �������� ������� ������� ���������
	double costEthIn;
	double costEthOut;
	double costNormalIn;
	double costNormalOut;

	// ��������� ��������� ��������� ������
	// ������������ ����� ������ � ��������� 
	int maxAxiomPopSize;
	// ������������ ����� �������� ��������� ������ ������
	int maxIterNum;
	// ������������ �������� ������� ������� ��� �������, ���� ��� �����������, �� ����� ����� ������ ���������������
	double maxAxiomRes;
	// ������� ������ ������ ���������, ������� ����������� ��� ���������� ���������
	double percentBestAxioms;

	// ��������� ��� ���������� ����������� ������������� ������� �����
	double costStrongPoint;
	// ��������� �������� ��������� ����������� ����� � ������������ ��� �������� ��� ������� ����������� �� �������� �������
	double costMaxMinLenRatio;
	// ��������� �������� � 0 ��������� ����� �����, ��� ������� ����������� � ������ ����� �� �������� �������.
	double costLenRatio;
	// ��������� ���������� �������� ������������ ������� �� ����������� ����������� ���������
	double costHiAreasNum;
	// ��������, � ������� ��������� costHiAreasNum
	double hiAreasNumEffect;

	// ��������� ������, ���� ��� ������ �������� �������� ������� �������� ��� ������������ � ������ ��������
	double shiftErrIn;

	// ������� ���������� �������� (���������� ��� ������ �������� �� ����� �� �������)
	double extendMarkUpLensLevel;

 protected:

	// ������� ���������� ������, �� ������� ������������ �������� �� ����������� � �������� �����
	bool comments;

	// ����� �������� ����� ������ I � II ���� �� �������� �������
	CompareStatistic* compareStatistic;

	// ��������� �������� ������� ������� �� ������� I � II ����
	GoalStrategy* goalStrategy;

	// ������ ������������ �������, ����������� � ���������� �� ��������
	std::vector <std::vector <ElemCondPlus> > bestECs;
	std::vector <std::vector <double> > bestECres; // ������� ������� ��� ��������� �������

	// ������ ������, ����������� �� ���� ������������ ������ �� ������������ �������
	std::vector <AxiomExpr> bestAxioms;
	std::vector <double> bestAxiomsRes;

	// ������ ������ ������, ����������� �� ��������� ������
	std::vector <AxiomExprSet> bestAxiomSets;
	std::vector <double> bestAxiomSetsRes;


	// ������� ������

	// ������� ������� � ��������� ������������ �������
	int selectElemCond (const int param);

	// ������� ������������ ������ �� ������������ �������
	int formAxioms (const int param);

	// ������� ������������ ������� ������ �� ������
	int createAxiomSet (const int param);

	// ������� ���������� ������ ������������ ������� � ��������������� �������� ������� �������
	inline int storeBestECs (ElemCondPlus &ec, double &curFun, std::vector <ElemCondPlus> &bestECs, std::vector <double> &bestECres);

	// ������� ����� ����� � ����, �� ������� ������������ ������� �����������
	inline int numOfCarriedOutItems (ElemCondPlus &ec, std::vector <double> &row) const;

	// ������� ������� ������� ��� ������������� �������
	double matterECFunc (ElemCondPlus &ec, const int param);
	
	// ������� ������� ������� ��� �������
	double matterAxiomFunc (AxiomExpr &ax, const int param);

	// ������� ��������� ��������� ������������ ������ �� ������������� � ���������� ��������
	int cutDownBestAxioms (void);

	// ��������������� ������� ���������� ������� �� �������� � ��������� �������� ���������� � ��������� �������
	inline int getPriority (std::vector <double> &vec, std::vector <unsigned int> &pos) const;

	// ����������� ���� ������ � ���������� ���������� ��� ���������� ����� ������
	int combineAxioms (const unsigned int i, const unsigned int j, AxiomExpr &axiomExpr, double &axiomExprRes, const int param);

	// ������� �������� �������� axiomExpr ���� curTS � ���� ���������� (hiAreasNum - ����� �������� ���������, hiAreasLen - ����� ����� ����� ��������)
	inline int axiomMarkUpStat (AxiomExpr &axiomExpr, std::vector <double> &row, int &hiAreasNum, int &hiAreasMinLen, int &hiAreasMaxLen) const;

	// �������� ���� curTS �������� axiom � ������� ���������� � resMarkUp
	inline int createMarkUp (std::vector <int> &resMarkUp, std::vector <double> &curTS, AxiomExpr &axiom, const int axNum) const;

	// C���� �������� �������� � ������������ ���������� ��������
	inline int createBaseMarkUp (std::vector <int> &sens, std::vector < std::vector <int> > &markUps) const;

	// ���� �������� markUp ��� �� �������� � ����� � ���������� ������, ������� ��� (��������) ����
	inline double shiftErrors (std::vector <int> &markUp, const int from, const int upTo) const;

	// ���� ������������ �������� row1 � row2 - ������� ����� ������������ �������� �������� � ��������� �� ��������������� �����������
	inline double shiftErrors (std::vector <int> &row1, const int from1, std::vector <int> &row2, const int from2, const int len) const;

	// ��������������� ������� ���������� �������� newMarkUp � newMarkUpSens �� ������� markUp � ��������� ������ �������� ����������.
	inline int moveVal (std::vector <int> &newMarkUp, std::vector <int> &newMarkUpSens, const int newFrom, std::vector <int> &markUp, const int from, const int upTo) const;

	// ��������������� ������� ���������� �������� newMarkUp � newMarkUpSens �� �������� base, sens � ��������� ������ �������� ����������.
	inline int moveVal (std::vector <int> &newMarkUp, std::vector <int> &newMarkUpSens, const int newFrom, std::vector <int> &base, std::vector <int> &sens, const int from, const int upTo) const;

	// ��������������� ������� ���������� �������� newMarkUp � newMarkUpSens �� ������� markUp � ������� base � ��������� ������ �������� ����������.
	inline int moveVal (std::vector <int> &newMarkUp, std::vector <int> &newMarkUpSens, const int newFrom, std::vector <int> &base, std::vector <int> &sens, const int fromBase, std::vector <int> &markUp, const int fromMarkUp, const int len) const;

	// ������������ ���������� �������� ��� ��������� ���� ����������� ��������� � �������� �������
	inline int formGeneralizedMarkUps (std::vector <int> &baseMarkUp, std::vector < std::vector <int> > &resMarkUps, int axNum, int i, int numOfMultiTS, const int param);

	// �������� ���������� ����������� ������� ����� ���������� ���������
	int getExtraMarkUps (std::vector < std::vector <int> > &markUps, std::vector < std::vector <int> > &correctMarkUps, int axNum, int i, const int param) const;

	// ��������� �������� � ���������� �� � ����������� ��������
	int simplifyMarkUp (std::vector <int> &simp, std::vector <int> &len, std::vector <int> &row, const int axNum) const;

	// ������� �������� ���������� ����� �� �������� � ������
	int subcheck (std::vector<int> &rowAxiomTypes, std::vector<int> &rowAxiomLen, int upTo, std::vector<int> &axiomTypes, std::vector<int> &axiomMinLen, std::vector<int> &axiomMaxLen, int curEnd) const;

	// ����������� ����������� ��������
	int splitMarkUps (std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <int> &set, std::vector < std::vector <int> > &markUps, std::vector < std::vector <int> > &markUpsMinLen, std::vector < std::vector <int> > &markUpsMaxLen) const;

	// ������� ������������� �������� �� ����������� ��������� �������
	int countMarkUpEfficiency (std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <std::vector <int> > &localMarkUps, std::vector <bool> &localMarkUpsEngaged) const;

	// ���������� ����������� �� �������� �������� ���������� ��������� �������
	bool setEngagedMarkUps (std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <std::vector <int> > &localMarkUps, std::vector <bool> &localMarkUpsEngaged) const;

	// �������������� ����������� �������� � ������� ��������
	int expandMarkUp (std::vector <int> &simpleMarkUp, std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen) const;

	// ��������� �������� ������� �� �������� �����������
	inline int extendMarkUpLen (std::vector <int> &stdLen, std::vector <int> &minLen, std::vector <int> &maxLen) const;

	// ������� ��������� ��� ����������� ������������� ��������� �������� � �������� �������������� ����
	int addResults (std::vector <int> &result, std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <int> &wholeRow, int indicator) const;

	// ������� �������� ���������� ��� ������� �������� �� ����������� ������� � �� ��������� ���������� ����������� ���������
	inline int countExtraStat (std::vector <int> &curSet, std::vector < std::vector <int> > &simpMarkUps, std::vector < std::vector <int> > &simpMarkUpsMinLens, std::vector < std::vector <int> > &simpMarkUpsMaxLens, std::vector < std::vector <std::vector <int> > > &extraMarkUps, std::vector <std::vector <int> > &correctExtraMarkUps, const int indicator) const;

	// ������������ ���������� � �������� ��� ��������� ���� ����������� ���������  
	inline int createCandidates (std::vector < std::vector <int> > &candMarkUps, std::vector < std::vector <int> > &candMarkUpsMinLens, std::vector < std::vector <int> > &candMarkUpsMaxLens, std::vector < std::vector <int> > &baseMarkUps, std::vector < std::vector <std::vector <int> > > &localMarkUps, std::vector < std::vector <std::vector <int> > > &extraMarkUps, std::vector <std::vector <int> > &correctExtraMarkUps, const int indicator) const; 

	// ����� �������� ��� ��������� ���� ����������� ��������� �� �������������� ����������
	int sortCandidateMarkUps (std::vector < std::vector <int> > &markUps, std::vector < std::vector <int> > &candMarkUps, const int  param) const;

	// �������� ������ ������ �� ������ �������� � ����� ������ ������
	int formAxiomSets (std::vector < std::vector <std::vector <int> > > &markUps, std::vector < std::vector <std::vector <int> > > &markUpsMinLens, std::vector < std::vector <std::vector <int> > > &markUpsMaxLens);

	// ���������� �������������� ������ ������ � �������� ���������� ����������� ���������
	int saveResults (void);

 public:

	// ������ �����������: 
	FuzzyDataLearnAlgorithm (void);  
  
	// �����������, ������������ ��� ���������� �������� ������ �������
	~FuzzyDataLearnAlgorithm (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);
	
	// �������� ������� ������ - ������ ������� ������� ������
	int run (void);

	// ������������� ����� �������� ���������� ������ comments
	int setComments (const bool newVal);

}; // end of class

}; // end of namespace

#endif /* _Fuzzy_Data_Learn_Algorithm_HXX */
