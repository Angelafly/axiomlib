/***************************************************************************
*					Check
*
*	Description:	����� ��� ��������
*					(� ��������� ��� �������� ������������ ���� ������������ �������
*					�� �� ��������� � ������� ������)
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef __CHECK_LIB_AXIOM_HXX
#define __CHECK_LIB_AXIOM_HXX

#include "./elem_conditions/ElemCondition.h"
#include "Axiom.h"
#include "AxiomLibException.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class Check {

 private:
 
 protected:
 
 public:
 
  // ������ �����������
    Check(void);
  
  // ������ �����������
    ~Check(void);
    
  // ������� �������� �� ������������� �� "�" ���� ������������ ������� �� ���� ������
    signed int checkAndCompatibility (Axiom* ax1, int ecNum1, Axiom* ax2, int ecNum2);
    
  // ������� ������ ������� ���� ����������� ������� �� ���� ������
    signed int change (Axiom* ax1, int ecNum1, Axiom* ax2, int ecNum2);
    
};// end of class

}; //  end of namespace

#endif /* __CHECK_LIB_AXIOM_HXX */
