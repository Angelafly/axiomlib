/****************************************************************************
*			SelectionStrategy
*
*	Description:	���������� �������� ��������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_SELECTION_STRATEGY_HXX
#define __AXIOMS_LIB_SELECTION_STRATEGY_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include <string>
#include <vector>

namespace AxiomLib {

class SelectionStrategy {

 private:

 protected:
 
 public:

  // ����������� ����������� - ������
  SelectionStrategy (void) {}

  // ����������� ���������� - ������
  virtual ~SelectionStrategy () {}

  // ���������� ��� ������� ������ ��������
  virtual std::string name (void) const = 0;

  // ������������� �������� ���������� �������������� ����� � ���������� ����� ������������
  virtual void setComments (const bool newComments) = 0;
  
  // ������� ������� �������� ���������� ������
  virtual int setParamsFromEnv (const Environment& env) = 0;

  // �������� �������� �������� ��� ������ ����������
  virtual int select (AxiomSetPop& pop) const = 0;
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_SELECTION_STRATEGY_HXX */
