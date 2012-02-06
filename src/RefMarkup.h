/****************************************************************************
*			RefMarkup
*
*	Description:	���� ��������� ��������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_REF_MARKUP_HXX
#define __AXIOMS_LIB_REF_MARKUP_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "all_axioms.h"
#include "Axiom.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

// ���� ������������ ������ ���������� � -1, �� �� ��������������
#define MARKUPINF = -1

class RefMarkup {

 private:

 protected:

    // ���� ����������� ��������
    std::vector<int> axiomTypes;
    std::vector<int> axiomMinLen;
    std::vector<int> axiomMaxLen;
    
    // �������� �������� ������� ������� ���������, �� �������� ������� ������������ ������� ������������� ������� ������� ����� ����������
    int errorLimit;

 public:
 
    // ������ �����������
    RefMarkup(void);

    // ���������� ��������� ��� ��������� � ������ ������� ������ ������������ �������
    ~RefMarkup(void);

    // ���������� ����� ��������� �������� ������ �������� ������
    void create(std::vector<signed int> &newAxiomTypes, std::vector<signed int> &newMinLen, std::vector<signed int> &newMaxLen);
    
    // ��� ������� ��� ������������� � ��� ������������ ����������� ������ - ����� ��������� ���� �������� - ���� �������� ���� ��������
    signed int addLens (std::vector<signed int> &newAxiomTypes, std::vector<signed int> &newMinLen, std::vector<signed int> &newMaxLen);
    
    // ������� �������� - �������� �� �������� ������� ������ � ������� ������������ �������� �� ������ ���������
    signed int check (std::vector<int> &axiomTypes, std::vector<int> &axiomLen, long int upTo);

	// ������ ������� ��������� ��������
	signed int getCopy (std::vector<int> &axTypes, std::vector<int> &axMinLen, std::vector<int> &axMaxLen) const;
    
    // ��������������� ������� ���������� � ��������� ��������� ��� check
    signed int subcheck (std::vector<int> &rowAxiomTypes, std::vector<int> &rowAxiomLen, long int upTo, int curEnd);

    // ��������������� ������� - ��� ����������� �������� �������� � ���� set'a, � �� � ���� vector'a
    bool operator < (const RefMarkup second) const {return 0;};
        
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_REF_MARKUP_HXX */
