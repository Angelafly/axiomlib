/****************************************************************************
*				BaseGenetics
*
*	Description:	������� ����� ������������� ��������� �����������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_BASE_GENETICS_HXX
#define __AXIOMS_LIB_BASE_GENETICS_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include "SelectionStrategy.h"
#include "TransmuteStrategy.h"
#include "CrossoverStrategy.h"
#include "SelectionStrategyFactory.h"
#include "TransmuteStrategyFactory.h"
#include "CrossoverStrategyFactory.h"
#include "GoalStrategyFactory.h"
#include "AxiomLibException.h"
#include "GoalStrategy.h"
#include "DataSet.h"
#include "Recognizer.h"
#include "RecognizerFactory.h"

namespace AxiomLib {

class BaseGenetics {

private:
  // ������� �������� ���������
  signed int numOfIter; // ���������� �������� ������������� ��������� (���������� ��������)
  
  // ���������
  AxiomSetPop axiomSetPop;

  // ��������� ������ ���������, ������������������ �� ����������������� �����
  SelectionStrategy* selectionStrategy;
  TransmuteStrategy* transmuteStrategy;
  CrossoverStrategy* crossoverStrategy;
  GoalStrategy* goalStrategy;

  // ����� ��������������
  Recognizer* recognizer;
  
  // ���������� ��� �������� �������� ������� �������, �� �������� �������� ��������� ���������� ������������� ���������
  double exitCond;
  
  // ���� ��� ���������� ������ ������� ������ �� ����������� ������ �����
  std::string *axiomSetBaseDir; // ���������� ��� ����������
  std::string *whereToSave; //��� ����� ��� ����������
  
  // ����, ���������� �� ����� ����������� ���������� � ����������� ����� ������ (�������������� ��� �������� ������ ��� �� ��������)
  bool comments;
  
protected:
 
public:

  // ������ ����������� � �������� ���������� �� ���������
  BaseGenetics (void);

  // ���������� - ��������� ��� ��������� � ������ ������� ������ ������������ �������
  ~BaseGenetics(void);
  
  // ��������� ���������� ������������� ���������
  // �� ��� ���������� ������ ��� ���������
  int initFromEnv(const Environment& env);
  
  // ��������� ��������� ���������� ������������� ��������� - ��������������� �� ��� ��������� - ��� ������ ��� ������ ������ ������
  // �� ��� ���������� ������ ��� ���������
  int shortInitFromEnv(const Environment& env);
  
  // ��������� ������ ������ ������ ��� ������� ������ ������� ������� ������
  int setDataSet (const DataSet &newDataSet);
  
  // ��������� ������ �������� ����� �������� ��� ������� ����������� ������� ������
  int setNumOfIter (const int newNumOfIter);
  
  // ��������� ������� �������� ������������� ������� ���������� ������ ������ �� ������� ������� ������
  int setParamsOfDataSet (std::vector<int> &params);

  // ������ ������������� ���������
  int run(void);
  
  // ������ ������������� ��������� � ������� ������������ ��������� � ��������� ���������� �������� ������� �������
  double run (AxiomSetPop &asp);
  
  // ����� �����������
  int writeResultsToDisplay(void);

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_BASE_GENETICS_HXX */
