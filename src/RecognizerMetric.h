/****************************************************************************
*			RecognizerMetric
*
*	Description:	����� ��� ��������� ���������� ������
*				������������� ��� ��������� ������ ������ 
*				��� ������������� ������� � �������� ���� ����������
*				����� ����������� �������� ��������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_RecognizerMetric_HXX
#define __AXIOMS_LIB_RecognizerMetric_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include "Recognizer.h"
#include "DataSet.h"
#include "RefMarkupSetSimple.h"
#include "CompareStatistic.h"
#include "CompareStatisticSimple.h"
#include "CompareClassFactory.h"
#include <string>
#include <vector>

namespace AxiomLib {

class RecognizerMetric : public Recognizer{

 private:
 
  // ��������������� ������� : ������� ���������� �������� � ������� ���� ��������� ��������
  inline signed int initRefs (AxiomSetPop& pop, const int asNum, const int param, std::vector <RefMarkupSetSimple> &refer);
  
  // ��������������� ������� : ������� ������������� ��������� �������� �� ������ �������� ���� � ��������� ���������, ����� ���� ���� ������� ����� ������ ������� � ������� ����
  inline signed int recognition (int &first, int &second,  std::vector <std::vector <int> > &markUps, std::vector <int> &corrMarkUp, std::vector < std::vector <RefMarkupSetSimple> > &refs, int numOfTest) const;
  
  // ��������������� ������� : ������� �����������. ���������� ����� ���� ���������. 
  // � ������� : ������� �����������-������������ ������ ��� ���������� ���� �������
  inline signed int joinResults (std::vector <int> &result, std::vector < std::vector <int> > &prelimResult, const int numOfClasses) const;

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
  int saveRefs (std::vector <RefMarkupSetSimple> &ref, std::string paramName) const;

 protected:
 
  RefMarkupSetSimple rms; // ������� ������, � ������� �������� ������� ����� ��� �������� ��������, �� ������ �������� ��������� ��� �������� ��������� ���������� (���� � ��� ����������, ������� �������� ���� ���, � �� ��� ������ �������� �������� �������� ����������)
 
  DataSet dataSet; // ������� dataSet, ���� ������������
  
  std::vector<int> params; // ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
  					  // ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������

  double precision; // ���������� ��� �������� ����������� ����������� ���������� ����� ���������� ��� ������� ������� �� �������
  
  bool comments; // ���������� ������ ��� �������� ���������� � ���, �������� �� ���������� ���������� ������������ ����� ������

  bool outputToFile; // ���������� ������ ��� �������� ���������� � ���, �������� ���������� ���� � ����
  std::string whereToSave; // ���������� ������ ��� �������� ����, �� �������� ��������� �����
  std::string saveDirName; // ��������� ��� ����� ����������, � ������� ��������� �����
  // ���������� ���������� ������ ��� �������� ������� ���� ����������� ������ � ����������
  std::string curDir;
  std::string curFile;
 
  CompareStatistic* compareStatistic;
  
 public:
 
  // ����������� �����������
  RecognizerMetric (void);

  // ����������� ���������� - �������� ���� ������������ �������� ������
  ~RecognizerMetric ();
  
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

  // ������� ��������� ������ �������� ��������� precision
  int setPrecision (const double newPrecision);
  
  // ������� ��������� ������ ������ ������ � ������� ����� ��������������
  int setDataSet (const DataSet &newDataSet);
  
  // ������� ��������� ������ ��������� ��������� ������ params
  int setParamsOfDataSet (std::vector<int> &newParams);

  // ������� ��������� ������ ������ �������� ����������
  int setCompareStatistic (CompareStatistic *newCompareStatistic);

  // ������� ��������� ������ ������ ��������
  int setRMS (RefMarkupSetSimple &newRMS);

  // ������� ���������� ����� ������� ��������������
  Recognizer* copy (void);

  // ������� �������� ������� ��������� �������������
  double learn (TemplateRecognizer &templateRecognizer);
  
  // �������� �������� ����� � ������� ������, �������� ��������������, ��������� ���������� ������ ������� � ������� ����
  signed int run (AxiomSetPop& pop);
  
  // �������� �������� ����� � ������� ������,
  // �������� ��������������, ��������� ����������
  // ������ 1 � ������� ����
  signed int run (AxiomSetPop& pop, std::vector <double> &precs);

  // ! ! ! �������� ! ! !
  // ������ ������������� �� ��������� ������ ������ � ������� ������ 
  signed int run (AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, int& resFirst, int& resSecond, const int asIndex = -1) {return 0;}

  // ! ! ! �������� ! ! !
  // ������ ������������� �� ��������� ������ ������ � ������� ������ 
  signed int run (AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, int& resFirst, int& resSecond, std::vector<double> &stat, const int asIndex = -1) {return 0;}
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_RecognizerMetric_STRATEGY_HXX */
