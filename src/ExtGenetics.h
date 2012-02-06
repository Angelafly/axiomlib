/****************************************************************************
*				ExtGenetics
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
#include "PopElemExt.h"
#include "AxiomSetWithStats.h"
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

class ExtGenetics {

private:
  // ������� �������� ���������
  signed int numOfIter; // ���������� �������� ������������� ��������� (���������� ��������)
  
  // ���������
  std::vector <PopElemExt> population;
  // Initial population size (number from environment)
  int popSize;
  
  // ������ ����� ���������
  std::vector <double> paramsPop;

  // ��������� ������ ���������, ������������������ �� ����������������� �����
  //SelectionStrategy* selectionStrategy;
  //TransmuteStrategy* transmuteStrategy;
  CrossoverStrategy* crossoverStrategy;
  GoalStrategy* goalStrategy;
  
  // Degree of crossover at level of elementary conditions, at the level of axioms and at the level of axiom set
  double eccl;
  double acl;
  double ascl; 

  // Degree of tranmsute at level of elementary conditions and at the level of axioms
  double ectp;
  double atp;
  // Tranmsute degree of precision parameter
  double prtp;

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
  ExtGenetics (void);

  // ���������� - ��������� ��� ��������� � ������ ������� ������ ������������ �������
  ~ExtGenetics(void);
  
  // ��������� ���������� ������������� ���������
  // �� ��� ���������� ������ ��� ���������
  int initFromEnv(const Environment& env);
  
  // ��������� ������ �������� ����� �������� ��� ������� ����������� ������� ������
  int setNumOfIter (const int newNumOfIter);

  // ������ ������������� ���������
  int run(void);
  
  int select ();

  int transmute (); 
  
  int crossover ();

  int storeBest (PopElemExt &bestElem);

  void writeResultsToDisplay (void);

  int packAxiomSetPop (AxiomSetPop &aspTemp);

  int packAxiomSetPop (AxiomSetPop &aspTemp, std::vector<double> &prec);

  int unpackAxiomSetPop (AxiomSetPop &aspTemp);

  int unpackAxiomSetPop (AxiomSetPop &aspTemp, std::vector<double> &prec);

  inline int sort (std::vector<int> &mask, std::vector<double> &goals) const;

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_BASE_GENETICS_HXX */
