/****************************************************************************
*			SimpleSelectionStrategy
*
*	Description:	������� ��������� ��������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_SELECTION_STRATEGY_SIMPLE_HXX
#define __AXIOMS_LIB_SELECTION_STRATEGY_SIMPLE_HXX

#include "SelectionStrategy.h"
#include "AxiomSetPop.h"
#include <string>
#include <stdlib.h>

namespace AxiomLib {

class SimpleSelectionStrategy : public SelectionStrategy{

 private:
  
  // ��������������� ������� ���������� �������� mask �� ��������� goal
  inline int sort (std::vector<int> &mask, std::vector<double> &goals) const;
  
  // ����������� ��������������� ������� ���������� �������� mask �� ��������� goal
  inline int quickSort (std::vector<int> &mask, std::vector<double> &goals) const;

 protected:

  double pBest; // ������� ������
  
  bool comments; //  ����� ������������ � ����������� ����� ������

 public:

  // ����������� ����������� - ������
  SimpleSelectionStrategy (void) {pBest = -1;}

  // ����������� ���������� - ������
  ~SimpleSelectionStrategy () {}
  
  // ��������� �������� ���������� ���������� comments - ��� �������� ������ ���������� � std::cout
  void setComments (const bool newComments) {comments = newComments;}

  // ���������� ��� ������� ������ ��������
  std::string name (void) const {return (std::string)"SelectionSimple";}

  // ������� ������� �������� ���������� ������
  int setParamsFromEnv (const Environment& env);

  // �������� �������� �������� ��� ������ ����������
  int select (AxiomSetPop& pop) const;
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_SELECTION_STRATEGY_HXX */
