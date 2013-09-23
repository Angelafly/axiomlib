/****************************************************************************
*					AxiomExprSet 
*
*	Description:	����� �������� ������� ������ �� ������ ��������� �� 
*					������������ �������.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_EXPR_SET_HXX
#define __AXIOMS_LIB_AXIOM_EXPR_SET_HXX

#include "./elem_conditions/ElemCondition.h"
#include "all_axioms.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomBase.h"
#include "AxiomSetBase.h"
#include "AxiomExpr.h"
#include "AxiomExprSetStructure.h"
#include "AxiomExprStructure.h"
#include "Environment.h"
#include "EnvDataSet.h"
#include "./MultiMarking/multimarks.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

namespace AxiomLib {

class AxiomExprSet {

 private:

	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive &archive, const unsigned int /*version*/) {
		archive & BOOST_SERIALIZATION_NVP(nameOfAxiomSet);
		archive & BOOST_SERIALIZATION_NVP(nameOfAxiomBank);
		archive & BOOST_SERIALIZATION_NVP(axioms);
	}
	
	// ��������� ������ ����� ����������� ��� ��������� ���������
	friend class CrossoverTpl;

	// ��������� ������ ����� ������� ��� ��������� ���������
	friend class TransmuteTpl;

 protected:
    
    // ��� ������ ������� ������ (������������� �������������� ������� ������ ����������� ������������)
    std::string nameOfAxiomSet;
    
    // ��� ������������� ����� ������
    std::string nameOfAxiomBank;
    
 public:

	 // ������� �������� � ������� � ������� ���������� ���������� 
    std::vector < AxiomExpr* > axioms;

	//		������� ������:
 
    // ������ ����������� � �������� ����� �� default'�
    AxiomExprSet(void);  
  
    // ������ ����������� � �������� ����� � ���������
    AxiomExprSet(const std::string sName);
	
	// ����������� �����������
	AxiomExprSet(const AxiomExprSet &other);
 
    // ���������� ��������� ��� ��������� � ������ ������ ������� ������ ������������ �������
    ~AxiomExprSet(void);
	
    // ������� �������� ���� ������ �� ������� ������
    void clear (void);
    
    // ������� ������������ ��� ������� ������
    std::string name (void) const;

	// ������� ������������� ��� ������� ������
	void setName(const std::string& value) { nameOfAxiomSet = value; }

	// ������� ������������ ����� ������ � �������
	int size (void) const;

	// ��������� ������� � ������������� ������ ������ � ������ �������
	void addAxiom (const AxiomExpr &newAxiom);

	// ��������� ������� � ������������� ������ ������ � ������ �������
	void addAxioms (std::vector <AxiomExpr*> &newAxiomExpr);

	// ������������� ����� ������ ������
	void setAxioms (std::vector <AxiomExpr*> &newAxiomExpr);
    
	// ������� �������� ����
	int enter (std::vector<int>& marked, const std::vector < std::vector<double> > &row, const unsigned long begin, const unsigned long end) const;

    // ������� �������� ����
    int enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end);

	// ������� �������� ���� � ��������� ����������
    int enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat);

    // ������� �������� ���� � ������ �������� �������������� ������
    int enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat);

    // �������� ������������ ������� ������
    AxiomExprSet& operator= (const AxiomExprSet &second);

	// ������� ��������� ��������� ������ ������� ������ � AxiomExprSetStructures
	int createAxiomExprSetStructure (AxiomExprSetStructure &aess) const;
	
	// ������� ������� ������� ������ ������ ������������ ������� (�� ������������ �� �����)
    int initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName);
	
	// ������� ������� ������� ������ ������ ������������ ������� (�� ������������ �� �����)
    int initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, const std::vector<std::string> &paramNames);

	// ������� ������� ������� ������ �� ������ �� ��������� � ��������� ������� ������
	int initAxiomSetFromStructure (AxiomExprSetStructure &aess);

	// ������� ������� ������� ������ ��������� �������
	int initAxiomSetByRand (const int numOfAxioms, const int maxECinAxiom);

	// ������� ������� ������� ������ ��������� ������� � ��������� ����������� ������ ������ ��� ������� ������� �� ��������� ���������
	int initAxiomSetByRand (const int numOfAxioms, const int maxECinAxiom, const int dimensionMin, const int dimensionMax);

	// ������� ���������� ������� ������ � ����
	int saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, int first = -1, int second = -1) const;

	// ������� ���������� ������� ������ � ���� (� ��������� ���������� �������� ������ ��� ������� ������������� �������)
	int saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, std::vector <std::string> &dataSetParams, int first = -1, int second = -1) const;
        
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_EXPR_SET_HXX */
