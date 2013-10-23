/***************************************************************************
*			Axiom
*
*	Description:	���������� ������������ ������
*	Author:		Eugene A. Vasin
*	History:
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_HXX
#define __AXIOMS_LIB_AXIOM_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomStructure.h"
#include "Round.h"
#include "all_axioms.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class Axiom {

 private:
  
 // ������ ���� ��������� ��� �������� ������������� �������� ������������ ������� � �������� 
    friend class Check;

 protected:
 
  // ������, �������� ���������� ������������ ������, �������� � ������ �������
    std::vector<ElemCondition *> elemConditions;
    
  // �������� �������
    std::string nameOfAxiom;

  // ����������� ����� ������������ ������� � �������
	unsigned int minNumOfEC;

  // ������������ ����� ������������ ������� � �������
	unsigned int maxNumOfEC;

 public:
	double weight;
 
  // ������ �����������: ������� ������� ������
    Axiom(void);  
  
  // ������ �����������: ������� ������� ������ � ���������� ��� ������ �������
    Axiom(const std::string sName);
  
  // �����������, ������������ ��� ���������� ��������
    ~Axiom(void);

  // �������� ������� (���������� ���������� �� ����������, ������������ ��� ������� ��������� ������)
    std::string name (void) const;

  // ������� ������������� �������� ��������� ������
	signed int setAttrValue (const unsigned int min, const unsigned int max);

  // ������� ���������� �������� ��������� ������
	signed int getAttrValue (unsigned int &min, unsigned int &max);

  // ������� ���������� �������� ����� ����������� ������� � ������ �������
    inline int size(void) const {return (int) elemConditions.size();};
    inline int getNumOfEC (void) const {return (int) elemConditions.size();};

  // ������� ������� ��� ����������� ��������� ������������ �������
    signed int clear (void);
    
  // ���������, ����������� �� ������ ������� �������� ����� k ���� ts
  // ����������:  1  ���� �����������
  //              0  ���� �� �����������
  //		 -1  ���� ������ ������� ������� �� ���������
    signed int check (const unsigned long k, const std::vector<double>& ts);
  
  // ������ ������ ������� ��� ���������� ������������ ������ � ��������, ��������� � ������� axiom_nums
  // ��� ����� ������������ ��� �������� "���������" ������
  // ��� �������� ������������ ������������ ������ ��� ����������
    signed int initAxiomByNums (const std::vector<signed int>& axiomNums);

  // ������� ����������� ������� ��������� ������� ������������� ���������
  // �� ���� ����������� �������� - ����� ���� ������� � ����������� �������
  // � ������ - ���� �� ������� ���������� ���������� ������� ����� �������  - ������� ���������� -1
    signed int initAxiomByRand (const int numOfEC);

  // ������ ������ ������� ��� ���������� ������������ ������ � �������, ��������� � ������� axiom_names
  // � ��������, ���� � ��������� ����� ��������
  // ��� ����� ������������ ��� �������� ������������� ��������� ������
  // ��� �������� ������������ ������������ ������ ��� ����������
    signed int initAxiomByNames (const std::vector<std::string>& axiomNames);

  // ������������� ���������� ������������ �������, �������� � �������
    signed int setECParameters (const std::map <std::string, std::map <std::string, std::string> >& ecParams);
    
  // ������� ���������� ��������� ������
    signed int getECParameters (std::map <std::string, std::map <std::string, std::string> > &ecParams) const;
    
  // ������� ������������� �������� ������������� ��������� ������������� ������������� �������
    signed int setParamValue (const double param, const int ecNum, const std::string &paramName);
    
  // ������� ���������� �������� ������������� ��������� ������������� ������������� �������
    signed int getParamValue (double &param, const int ecNum, const std::string &paramName) const;
  
  // ������� ��������� ������� ������ ������� ���������� ������������� ������� � �������� ������� � ������� ������������ �������
    signed int getECParamNames(int ecNum, std::vector<std::string> &ecParamNames) const;
    
  // ������� ���������� ����� ������������� ������� � ������� ������� �� ��� �����
    signed int getECNumByName (std::string &ecName) const;
    
  // ������� ��������� ������� ������ ������� ������������ ������� ������ �������
    signed int getECNames(std::vector<std::string> &ecNames) const;

  // ������� ��������� ��������� ������ ������� � AxiomStructure
	signed int createAxiomStructure (AxiomStructure &as) const;

  // ������� �� ������ ���������� ������������ �������, �������� � ������ ������� (ectp). � ��
  // ������ ��������� ����� ������� (�������� atp)
    signed int transmute (const double ectp, const double atp);
      
  // ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
  // �� �������, ������ ���������
    double operator< (const Axiom& second);
    
  // �������� ������������
    Axiom & operator= (const Axiom& second);
	
	Axiom(const Axiom& other);

  // �������� ������ ����������� ������������ ���� ������
    signed int checkWeakEquivalence(const Axiom& second) const;
    
};

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_HXX */
