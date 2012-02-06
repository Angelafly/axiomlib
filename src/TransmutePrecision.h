/****************************************************************************
*			TransmutePrecision
*
*	Description:	������� ��������� ��������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Transmute_STRATEGY_Precision_HXX
#define __AXIOMS_LIB_Transmute_STRATEGY_Precision_HXX

#include "TransmuteStrategy.h"
#include <string>
#include "AxiomSetPop.h"

namespace AxiomLib {

class TransmutePrecision : public TransmuteStrategy{

 private:

 protected:

  double ecTransLevel; // ������� ������� �� ������ ���������� Ec

 public:

  // ����������� ����������� - ������
  TransmutePrecision (void) {ecTransLevel = -1;}

  // ����������� ���������� - ������
  ~TransmutePrecision () {}

  // ���������� ��� ������� ������ ��������
  std::string name (void) const {return (std::string)"TransmuteSimple";}

  // ������� ������� �������� ���������� ������
  int setParamsFromEnv (const Environment& env);

  // �������� �������� �������� ��� ������ ����������
  int transmute (AxiomSetPop& pop);
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Transmute_Precision_HXX */
