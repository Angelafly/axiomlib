/****************************************************************************
*				CompareClassFactory
*
*	Description:	
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_COMPARE_CLASS_FACTORY_HXX
#define __AXIOMS_LIB_COMPARE_CLASS_FACTORY_HXX

#include <iostream>
#include <string>
#include <vector>

#include "CompareStatistic.h"

namespace AxiomLib {

class CompareClassFactory {

 private:

 protected:
 
 public:

  // �����������, �������������� ������� ��� ���������� �������
  CompareClassFactory(void);
  
  // �����������
  ~CompareClassFactory(void);
  
  // ������� ����� �������� ������� ������� 
  CompareStatistic* create (const std::string &name);
  
};

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_COMPARE_CLASS_FACTORY_HXX */
