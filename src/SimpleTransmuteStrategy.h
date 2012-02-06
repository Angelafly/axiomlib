/****************************************************************************
*			SimpleTransmuteStrategy
*
*	Description:	������� ��������� �������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Transmute_STRATEGY_SIMPLE_HXX
#define __AXIOMS_LIB_Transmute_STRATEGY_SIMPLE_HXX

#include "TransmuteStrategy.h"
#include <string>
#include "AxiomSetPop.h"

namespace AxiomLib {

class SimpleTransmuteStrategy : public TransmuteStrategy{

 private:
 
  double ectp;	// ������� ������� �� ������ ���������� ������������ �������
  double atp;	// ������� ������� �� ������ ������
  double stp;	// ������� ������� �� ������ ������� ������
  double sptp;	// ������� ������� �� ������ ������� ������ � ������� ������

    // ������� ������� ��������� ������� ������ - ������� �������� � private ��� ����, ����� ��������� ����� �������� ���������� ����������
  int transmuteAxiomSet (unsigned int asNum);

 protected:
  
 public:

  // ����������� ����������� - ������
  SimpleTransmuteStrategy (void) {ectp = 0.0; atp = 0.0; stp = 0.0; sptp = 0.0;}

  // ����������� ���������� - ������
  ~SimpleTransmuteStrategy () {}

  // ���������� ��� ������� ������ ��������
  std::string name (void) const {return (std::string)"TransmuteSimple";}

  // ������� ������� �������� ���������� ������ �� ���������
  int setParamsFromEnv (const Environment& env);
  
  // �������� �������� �������� ��� ������ ����������
  int transmute (AxiomSetPop& pop);
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Transmute_STRATEGY_HXX */
