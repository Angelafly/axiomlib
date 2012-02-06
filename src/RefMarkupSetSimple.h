/****************************************************************************
*			RefMarkupSetSimple
*
*	Description:	����� ��������� ��������, ��������������� ������
*				������ ��������������
*				�������� ������������ � ������� ���� ��� ������������ 
*				��������������� �� �������.
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_REF_MARKUP_SET_Simple_HXX
#define __AXIOMS_LIB_REF_MARKUP_SET_Simple_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "all_axioms.h"
#include "Axiom.h"
#include "AxiomSet.h"
#include "RefMarkupSimple.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

namespace AxiomLib {

class RefMarkupSetSimple {

 private:

 protected:
 
    // ������� ����� �� ������ �������� (����� �����������) ��������� ������ �������� ��������� ����������
    RefMarkupSimple rmu;

 public:
 
    // ���������  ��������  // ��������� � ������� � PRIVATE
    std::vector<RefMarkupSimple> refMarkups;
  
    // ������ �����������
    RefMarkupSetSimple(void);  

    // ���������� ��������� ��� ��������� � ������ ������� ������ ������������ �������
    ~RefMarkupSetSimple(void);
    
    // ��������� ���������� �� ����������������� �����
    int setParamsFromEnv (const Environment& env);
    
    // ���������� ����� ��������� �������� ������ �������� ������
    void addRefMarkup(std::vector<double>& refPattern, AxiomSet& as, const bool comments);
    
    // ���������� ����� ��������� �������� ������ �������� ������
    void addRefMarkup(std::vector<int> &axiomTypes, const bool comments);
    
    // �������� ���� �������� � ������ ������
    signed int clear();
    
    // ������� �������� - �������� �� ����� ���� �� �������� ������ � ������� ������������ �������� �� ������ ���������
    signed int check (const double newMaxDistance, std::vector<int> &axiomTypes, long int upTo);

	// �������� ������������
    RefMarkupSetSimple& operator= (const RefMarkupSetSimple& second);
        
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_REF_MARKUP_SET_Simple_HXX */
