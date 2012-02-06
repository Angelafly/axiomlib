/****************************************************************************
*			TransmuteStrategy
*
*	Description:	���������� �������� ��������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Transmute_STRATEGY_HXX
#define __AXIOMS_LIB_Transmute_STRATEGY_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
#include <string>
#include <vector>

namespace AxiomLib {

class TransmuteStrategy {

 private:

 protected:
 
  std::vector<AxiomSetWithStats>*  asPop;

  std::vector<std::vector<Axiom *>* > axiomSets;
 
 public:

  // ����������� ����������� - ������
  TransmuteStrategy (void) {
	asPop = NULL;
	axiomSets.resize(0);
  }

  // ����������� ���������� - ������
  virtual ~TransmuteStrategy () {}

  // ���������� ��� ������� ������ ��������
  virtual std::string name (void) const = 0;

  // ������� ������� �������� ���������� ������
  virtual int setParamsFromEnv (const Environment& env) = 0;

  // �������� �������� �������� ��� ������ ����������
  virtual int transmute (AxiomSetPop& pop) = 0;
  
  // ������� ������������� ������ ��������� �������
  // ��������� ������� ������ �� ������ ������ � ������ �� asPop
  int initTransmute (AxiomSetPop &axiomSetPop) {
	asPop = &(axiomSetPop.asPop);
	axiomSets.resize(asPop->size());
	for (unsigned int i = 0; i < asPop->size(); i++) {
		axiomSets[i] = &((*asPop)[i].axioms);
	}
	return 0;
  }
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Transmute_STRATEGY_HXX */
