/****************************************************************************
*			RefMarkupSet
*
*	Description:	����� ��������� ��������, ��������������� ������
*				������ ��������������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_REF_MARKUP_SET_HXX
#define __AXIOMS_LIB_REF_MARKUP_SET_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "all_axioms.h"
#include "Axiom.h"
#include "AxiomSet.h"
#include "RefMarkup.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

namespace AxiomLib {

class RefMarkupSet {

 private:

 protected:

    // �������� ������� ����������� �������� - �� ���� ������������ � ���������� ����� �������� ������� ����������� �� (1+-precision)
    double precision;

 public:
 
    // ����������� ���������  �������� 
    std::vector<RefMarkup> refMarkups;
  
    // ������ �����������
    RefMarkupSet(void);  

    // ���������� ��������� ��� ��������� � ������ ������� ������ ������������ �������
    ~RefMarkupSet(void);
    
    // ������� ������� ����� �������� ���������� ����������
    int setPrecision (const double newPrecision);

    // ���������� ����� ��������� �������� ������ �������� ������
    void addRefMarkup(std::vector<double>& refPattern, AxiomSet& as, const bool comments);
    
    // ���������� ����� ��������� �������� ������ �������� ������
    void addRefMarkup(std::vector<int> &resRef, const bool comments);
    
    // ��������������� ������� - �������������� ���������� �������� �� �����������
    signed int processRes (std::vector<int> &resRef, std::vector<int> &axiomTypes, std::vector<int> &axiomMinLen, std::vector<int> &axiomMaxLen);
    
    // ������� �������� - �������� �� ����� ���� �� �������� ������ � ������� ������������ �������� �� ������ ���������
    signed int check (std::vector<int> &axiomTypes, std::vector<int> &axiomLen, long int upTo);
        
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_REF_MARKUP_SET_HXX */
