/***************************************************************************
*					AxiomExpr
*
*	Description:	������� � ���� ��������� �� ������������ �������
*					(� ������������, ������������ � �����������)
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Axiom_Expr_HXX
#define _Axiom_Expr_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemCondPlus.h"
#include "AxiomBase.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomExprStructure.h"
#include "Round.h"
#include "all_axioms.h"

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

namespace AxiomLib {

class AxiomExpr {

 private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &archive, const unsigned int /*version*/) {
		archive & BOOST_SERIALIZATION_NVP(nameOfAxiomExpr);
		archive & BOOST_SERIALIZATION_NVP(nameOfECBank);
		archive & BOOST_SERIALIZATION_NVP(expression);
	}
  
	// ������ ���� ��������� ��� �������� ������������� �������� ������������ ������� � ��������
    friend class Check;

	// ������������� ��������� ��� ��� ���������� ������
	signed int setDefaultName (void);

 protected:

 public:

	// �������� �������
    std::string nameOfAxiomExpr;

	// ��� ������������� ����� ������������ �������
    std::string nameOfECBank;

	// ������� �� - ��������� ��� ��. ������� � �������� ������� ������������, ������� ����� ����� - �����������.
	// ��������� � ��������� ��������� �������.
	std::vector< std::vector <ElemCondPlus> > expression;
	
		// ������� ������: //

	int index;
	
	// �����������: �� ������� ��������� ��� �������
    AxiomExpr(void);  
  
	// �����������: �� ������� ��������� ��� �������, ���������� ����� ��� �������
    AxiomExpr(const std::string sName);

	AxiomExpr(const ElemCondPlus& ec);
  
	// �����������, ������������ ��� ���������� ��������
    ~AxiomExpr(void);

	AxiomExpr *clone() const { return new AxiomExpr(*this); }

	// ���������� �������� ������� (���������� ���������� �� ����������, ������������ ��� ������� ��������� ������)
    std::string name (void) const;
	
	// ������� ���������� �������� ����� ����������� ������� � ������ �������
    signed int size(void) const;
    signed int getNumOfEC (void) const;

	// ������� ������� ��� ������������ ������� � ������� ���������� �������
    signed int clear (void);
    
	// ���������, ����������� �� ������ ������� �������� ����� k ���� ts (��� ����������� � ������������ �����)
	// ����������:  1  ���� �����������
	//              0  ���� �� �����������
	//				-1  ���� ������ ������� ������� �� ���������
    signed int check (const unsigned long k, const std::vector<double>& ts) const;
	signed int check (const unsigned long k, const std::vector < std::vector<double> >& ts) const;

	// ������������� ���������� ������������ �������, �������� � �������
    //signed int setECParameters (const std::map <std::string, std::map <std::string, std::string> >& ecParams);
    
	// ������� ���������� ��������� ������
    //signed int getECParameters (std::map <std::string, std::map <std::string, std::string> > &ecParams) const;
  
	// ��������� ������ ������� � �� ����������� ����������� � ������ �������� ���� �� ����.
	// �� �������, ������ ���������
    double operator< (const AxiomExpr& second);
    
	// �������� ������������
	// � ����� ���� ��� ������� ������ ��� �������� �� ���������
    // AxiomExpr & operator= (const AxiomExpr& second);

	// ������ ������ �� ��������� �
	AxiomExpr operator& (const AxiomExpr& second) const;

	// ������ ������ �� ��������� ���
	AxiomExpr operator| (const AxiomExpr& second) const;

	AxiomExpr& operator &=(const AxiomExpr& second);

	AxiomExpr& operator |=(const AxiomExpr& second);

	// ������ ������ �� ��������� � � ������ �������
	signed int andExpr (const AxiomExpr& first, const AxiomExpr& second);

	// ������ ������ �� ��������� ��� � ������ �������
	signed int orExpr (const AxiomExpr& first, const AxiomExpr& second);

	// ���������� ����������� ������������ ��������
	void getDimensions(std::vector<bool> &v) const;
	
	// �������� ������ ����������� ������������ ���� ������
    signed int checkWeakEquivalence(const AxiomExpr& second) const;

	// ������� ��������� ��������� ��������� ������� � AxiomExprStructures
	signed int createAxiomExprStructure (AxiomExprStructure &aes) const;

	// ������� ����������� ������� ��������� ������� ������������� ���������
	// �� ���� ����������� �������� - ����� ���� ������� � ����������� �������
	// � ������ - ���� �� ������� ���������� ���������� ������� ����� �������  - ������� ���������� -1
    signed int initAxiomByRand (const int numOfEC);

	// ��������� ����������� ������� � ��������� ����������� ������ ������ ��� ������� ������� �� ��������� ���������
	signed int initAxiomByRand (const int numOfEC, const int dimensionMin, const int dimensionMax);

	// ������� ������� ������� �� ������ �� ��������� � ��������� ������� ������
	signed int initAxiomFromStructure (AxiomExprStructure &aes);

	// ������� ������� ������� �� �����
	signed int initAxiomFromFile (const std::string baseDir, const std::string axiomName);

	// ������� ������� ������� �� ����� (� ��������� ����������� ������ ������, �� ������� ����������� ������ ������������ ������� �� ������ �������)
	signed int initAxiomFromFile (const std::string baseDir, const std::string axiomName, const std::vector <std::string> &paramNames);

	// ������� ���������� ������� ������ � ����
	signed int saveAxiomToFile (const std::string baseDir, const std::string axiomName) const;

	// ������� ���������� ������� ������ � ���� (� ��������� ����������� ������ ������, �� ������� ����������� ������ ������������ ������� �� ������ �������)
	signed int saveAxiomToFile (const std::string baseDir, const std::string axiomName, std::vector <std::string> &paramNames) const;

	std::string toString() const;
    
};

}; //  end of namespace

std::ostream& operator<<(std::ostream& ostr, const AxiomLib::AxiomExpr& axiomExpr);

#endif /* _Axiom_Expr_HXX */
