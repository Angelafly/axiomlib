/****************************************************************************
*			RecognizerExtend
*
*	Description:	����� ��� ��������� ���������� ������
*				������������� ��� ��������� ������ ������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_RecognizerExtend_HXX
#define __AXIOMS_LIB_RecognizerExtend_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include "Recognizer.h"
#include "DataSet.h"
#include "RefMarkupSet.h"
#include "CompareStatistic.h"
#include "CompareStatisticSimple.h"
#include "CompareClassFactory.h"
#include "GoalStrategyFactory.h"
#include "GoalStrategy.h"
#include <string>
#include <vector>

namespace AxiomLib {

class TemplateRecognizer;

class RecognizerExtend : public Recognizer {

 private:
 
  // ��������������� ������� : ������� ���������� ����������� ��������� �������� � ������� ��������
  inline signed int initRefs (AxiomSetPop& pop, const int asNum, const int param, std::vector <RefMarkupSet> &refer) const;

  inline signed int initRefs (AxiomSet& axiomSet, const int param, std::vector <RefMarkupSet> &refer, std::vector <bool> &refStat) const;

  // ��������������� ��������� ���������� �������
  inline void updateVec (std::vector <bool> &vec, std::vector <bool> &newVec) const;

  // ��������������� ��������� ��� �������� ���������� �� ����������� �������������
  inline void countStat (std::vector <double> &stat, int resFirst, int resSecond, int resShouldBe, std::vector <bool> &refStat, std::vector <bool> &rowStat) const;
  
  // ��������������� ������� : ������� ������������� ��������� �������� �� ������ �������� ���� � ��������� ���������, ����� ���� ���� ������� ����� ������ ������� � ������� ����
  inline signed int recognition (int &first, int &second,  std::vector <std::vector <int> > &markUps, std::vector <int> &corrMarkUp, std::vector < std::vector <RefMarkupSet> > &refs, int numOfTest) const;
  
  // ��������������� ������� : ������� �����������
  inline signed int joinResults (std::vector <int> &result, std::vector < std::vector <int> > &prelimResult, const int numOfClasses) const;

  // ������ ������ ����� - ���������� ����� ����� ���������� ���������� ��������� � ������ �����
  inline signed int analizeTest (std::vector <int> &corrMarkUp, int &res);

  // ��������������� ������� �������� - ���������� �������� �������� precision
  double sublearn (TemplateRecognizer &templateRecognizer, std::vector <double> &curRes, std::vector <double> &curPrecision, std::vector < std::vector <signed short> > &comparisonTable);

  // ��������������� ������� ��� ��������� �������� - ��������� �������� �������� ����������� ��������� ��������� precision
  inline void manageSets (unsigned int direction, std::vector <double> &curRes, std::vector <double> &curPrecision) const;

  // ��������������� ������� ��� ��������� �������� - ����� ������� ����������� ��� ��������� �� ������������� ������� �����������
  inline unsigned int chooseDirection (std::vector < std::vector <signed short> > &compTable) const;

  // ������� ������� ���������� �����
  int clearPath (std::string asName, int asIndex = -1);

  // ���� ������������ �������� ������� ��� �������� ���������� ��� �� �������
  int searchFreeIndex (const std::string &curPath) const;

  // ������� ������� ��� ����� ��� ������� ������ � ��������� ��� �� ���������� ���������� ������
  int genFileName (int testNum);

  // ������� ���������� ���� � ��� �������� � ����
  int saveRows (std::vector <double> &row, std::vector<int> &markUp, std::string paramName) const;

  // ������� ���������� ���� � ��� �������� � ����
  int updateRows (std::vector <int> &corrMarkUp, std::vector<int> &result) const;

  // ������� ���������� ���� � ��� �������� � ����
  int saveRefs (std::vector <RefMarkupSet> &ref, std::string paramName) const;

 protected:
 
  DataSet dataSet; // ������� dataSet, ���� ������������
  
  std::vector<int> params; // ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
  					  // ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������

  double precision; // ���������� ������ ����������� � ���������� �������� ��������� �����
  
  bool comments; // ���������� ������ ��� �������� ���������� � ���, �������� �� ���������� ���������� ������������ ����� ������

  bool outputToFile; // ���������� ������ ��� �������� ���������� � ���, �������� ���������� ���� � ����
  std::string whereToSave; // ���������� ������ ��� �������� ����, �� �������� ��������� �����
  std::string saveDirName; // ��������� ��� ����� ����������, � ������� ��������� �����
  // ���������� ���������� ������ ��� �������� ������� ���� ����������� ������ � ����������
  std::string curDir; 
  std::string curFile;
 
  CompareStatistic* compareStatistic; // ����� �������� ����� ������ I � II ���� �� �������� �������
  
  GoalStrategy* goalStrategy; // ��������� �������� ������� ������� �� ������� I � II ����

 public:
 
  // ����������� ������ � �������� �������� �� ���������
  RecognizerExtend (void);

  // ���������� ������ � ������������ ���� ��������� ������������ ��������
  ~RecognizerExtend (void);
  
  // ��������� ������ �������� ���������� ������ comments
  void setComments(const bool newValue) { comments = newValue;}

  // ��������� ������ �������� ���������� ������ outputToFile
  void setOutputToFile(const bool newValue) { outputToFile = newValue;}

  // ��������� ������ �������� ���������� ������ outputToFile, saveDirName � whereToSave
  void setOutputToFile(const bool newValue, const std::string newWhereToSave, const std::string newSaveDirName) { outputToFile = newValue; whereToSave = newWhereToSave; saveDirName = newSaveDirName; }

  // ������� ������� �������� ���������� ������, � ��� ����� - ������� �������� � �������� ������
  int setParamsFromEnv (const Environment& env);
  
  // ������� ������� �������� ������ ������������ ����� ���������� ������, � ��� ����� - ������� �������� � �������� ������
  int setNominalParamsFromEnv (const Environment& env);
  
  // ������� ��������� ������ ������ ������ � ������� ����� ��������������
  int setDataSet (const DataSet &newDataSet);
  
  // ������� ��������� ������ �������� ��������� precision
  int setPrecision (const double newPrecision);
  
  // ������� ��������� ������ ��������� ��������� ������ params
  int setParamsOfDataSet (std::vector<int> &newParams);

  // ������� ��������� ������ ������ �������� ����������
  int setCompareStatistic (CompareStatistic *newCompareStatistic);

  // ������� ���������� ����� ������� ��������������
  Recognizer* copy (void);

  // ������� �������� ������� ��������� �������������
  double learn (TemplateRecognizer &templateRecognizer);
  
  // �������� �������� ����� � ������� ������,
  // �������� ��������������, ��������� ����������
  // ������ 1 � ������� ����
  signed int run (AxiomSetPop& pop);
  
  // �������� �������� ����� � ������� ������,
  // �������� ��������������, ��������� ����������
  // ������ 1 � ������� ����
  signed int run (AxiomSetPop& pop, std::vector <double> &precs);

  // ������ ������������� �� ��������� ������ ������ � ������� ������ 
  signed int run (AxiomSet& axiomSet, DataSet& extDataSet, std::vector<int>& extParams, int& resFirst, int& resSecond, const int asIndex = -1);

  // ������ ������������� �� ��������� ������ ������ � ������� ������ � ��������� ���������� �� ��������������� ��������
  signed int run (AxiomSet& axiomSet, DataSet& extDataSet, std::vector<int>& extParams, int& resFirst, int& resSecond, std::vector<double> &stat, const int asIndex = -1);
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_RecognizerExtend_STRATEGY_HXX */
