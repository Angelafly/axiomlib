/****************************************************************************
*			Recognizer
*
*	Description:	����������� ����� ��� ��������� ���������� ������
*				������������� ��� ��������� ������ ������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Recognizer_HXX
#define __AXIOMS_LIB_Recognizer_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include "DataSet.h"
#include "RefMarkupSet.h"
#include "CompareStatistic.h"
#include "CompareStatisticSimple.h"
#include "CompareClassFactory.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>

//#include "undefs.h"

//#include <limits.h>

//#include "Defines.h"

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/fstream.hpp"

#include "omp.h"

namespace AxiomLib {

class TemplateRecognizer;

class Recognizer {

 private:
 
 protected:
  
 public:
 
  // ����������� ����������� - ������
  // Recognizer (void) {}

  // ����������� ���������� - ������
  virtual ~Recognizer (void) {}
  
  // ��������� ������ �������� ���������� ������ comments
  virtual void setComments(const bool newValue) = 0;

  // ��������� ������ �������� ���������� ������ outputToFile
  virtual void setOutputToFile(const bool newValue) = 0;

  // ������� ������� �������� ���������� ������, � ��� ����� - ������� �������� � �������� ������
  virtual int setParamsFromEnv (const Environment& env) = 0;
  
  // ������� ������� �������� ������ ������������ ����� ���������� ������, � ��� ����� - ������� �������� � �������� ������
  virtual int setNominalParamsFromEnv (const Environment& env) = 0;
  
  // ������� ��������� ������ ������ ������ � ������� ����� ��������������
  virtual int setDataSet (const DataSet &newDataSet) = 0;
  
  // ������� ��������� ������ ��������� ��������� ������ params
  virtual int setParamsOfDataSet (std::vector<int> &newParams) = 0;

  // ������� ���������� ����� ��������������
  virtual Recognizer* copy (void) = 0;

  // ������� �������� ��������� �������������
  virtual double learn (TemplateRecognizer &templateRecognizer) = 0;
	
	virtual double learn(TemplateRecognizer &templateRecognizer, int& resFirst, int& resSecond, std::vector<double>& stat, int asIndex = -1);
  
  // �������� �������� ����� � ������� ������,
  // �������� ��������������, ��������� ����������
  // ������ 1 � ������� ����
  virtual signed int run (AxiomSetPop& pop) = 0;
  
  virtual signed int run (AxiomSetPop& pop, std::vector <double> &precs) = 0;

  virtual signed int run (AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, int& resFirst, int& resSecond, const int asIndex = -1) = 0;

  virtual signed int run (AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, int& resFirst, int& resSecond, std::vector<double> &stat, const int asIndex = -1) = 0;
	
};

// end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_RecognizerExtend_STRATEGY_HXX */
