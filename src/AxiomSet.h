/****************************************************************************
*			Axiom Set 
*
*	Description:	����� �������� ������� ������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_SET_HXX
#define __AXIOMS_LIB_AXIOM_SET_HXX

#include "./elem_conditions/ElemCondition.h"
#include "ElemConditionsFactory.h"
#include "AxiomLibException.h"
#include "AxiomBase.h"
#include "AxiomSetBase.h"
#include "AxiomSetStructure.h"
#include "AxiomStructure.h"
#include "all_axioms.h"
#include "Axiom.h"
#include "Environment.h"
#include "EnvDataSet.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "./MultiMarking/multimarks.h"
namespace AxiomLib {

class AxiomSet {

 private:

 protected:

	// ��������� ������ ����� ����������� ��� ��������� ���������
	friend class CrossoverTpl;

	// ��������� ������ ����� ������� ��� ��������� ���������
	friend class TransmuteTpl;
 
    // ������� �������� � ������� � ������� ���������� ���������� 
    std::vector<Axiom *> axioms;
    
    // ��� ������ ������� ������ (������������� �������������� ������� ������ ����������� ������������)
    std::string nameOfAxiomSet;
    
    // ��� ������������� ����� ������
    std::string nameOfAxiomBank;
    
    // ����������� ��� ��� �������� ����������� ��� ������� ������������� ���������� - ���� �� ��������� - �� ���������� 
    // �������� ����������� ���������� ������ ������ �������� � ���������� ������ ��� �������������� �������� ���������
    int crossoverControlBit;
    
 public:
 
    // ������ ����������� � �������� ����� �� default'�
    AxiomSet(void);  
  
    // ������ ����������� � �������� ����� � ���������
    AxiomSet(const std::string sName);
 
    // ���������� ��������� ��� ��������� � ������ ������ ������� ������ ������������ �������
    ~AxiomSet(void);

    // ������� �������� ���� ������ �� ������� ������
    void clear (void);
    
    // ������� ������������ ��� ������� ������
    std::string name (void) const;

	// ������� ������������ ����� ������ � �������
	int size (void) const;
    
    // ������� ��������� �������� ������������ ���� �����������
    int setCrossoverControlBit (const int newCrossoverControlBit);
    
    // ������� ���������� ������� �������� ������������ ���� �����������
    int getCrossoverControlBit (void) const;

    // ������� �������� ����
    int enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end);

    // ������� �������� ���� � ��������� ����������
    int enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat);

    // for MultiMarking
    int enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end);

    // for MultiMarking
    int enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat);

    // ������� ������� ������ ������� ��������� �������
    signed int initAxiomSetByRand (const int numOfAxioms, const int maxECinAxiom);
    
    // ������� ������� ������ ������� ������ ������������ ������� (������� ���� ������������ ������� �������� � ������ ������� � ����������)
    signed int initAxiomSetByNames (const std::vector < std::vector < std::string > > &axiomEC, const std::vector <std::string>& axiomNames);

    // ������� ������� ������ ������� ������ ������������ ������� (������� ������� ������������ ������� �������� � ������ ������� � ����������)
    signed int initAxiomSetByNums (const std::vector < std::vector < int > > &axiomEC, const std::vector <std::string>& axiomNames);
    
    // ������� ������� ������ ������� ������ ������������ ������� (�� ������������ �� �����)
    signed int initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, AxiomBase &axiomBase);
    
    // ������� �������� ����� ������� ������ ����� ����������� � �������������� ������ �� �������� �������
    signed int initAxiomSetByCopy (const std::vector <Axiom*> &axiomVec, double levelOfSub);
    
    // ������� ���������� ������� ������ � ����
    signed int saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, int first = -1, int second = -1);
    
    // ������� ���������� ������� ������ � ����
    signed int saveAxiomSetToFile (Environment &env, EnvDataSet &envDataSet, int first = -1, int second = -1);

	// ������� ��������� ��������� ������ ������� ������ � AxiomSetStructures
	signed int createAxiomSetStructure (AxiomSetStructure &ass) const;
    
    // ������� ������� ���������� ������� ������
    signed int setAxiomParameters (const std::map < std::string , std::map <std::string, std::map <std::string, std::string> > >& axiomParams);
    
    // ������� ���������� �������� ������������� ���������� ������� ������
    signed int getAxiomParameters (std::map < std::string , std::map <std::string, std::map <std::string, std::string> > > &axiomParams);
    
    // ������� ������������� �������� ��������� �� ������ ������� � ������� ������, �� ������ ������������� ������� � ������ ������, �� ����� ���������
    signed int setParamValue (double param, int axiomNum, int ecNum, std::string &paramName);
    
    // ������� ���������� �������� ��������� �� ������ ������� � ������� ������, �� ������ ������������� ������� � ������ ������, �� ����� ���������
    signed int getParamValue (double &param, int axiomNum, int ecNum, std::string &paramName);
    
    // ������� ���������� ����� ��������� � ������� axioms ������� ������ ������
    signed int getNumOfAxioms (void) const;
    
    // ������� ���������� ������� � �������� ������� � ������� ������ ������ �������
    Axiom& getAxiom (int i) const;
    
    // ������� �������� ������� � �������� �������  ������� ������
    signed int setAxiom (Axiom& axiom, const int i);
    
    // ������� ������������ ������ ������ ������
    signed int slide (const int i, const int j);
    
    // �������� ������������ ������� ������
    AxiomSet & operator= (const AxiomSet &second);
	
	AxiomSet(const AxiomSet &second);
    
    // �������� ������� (�������� �� ������!!!) ����������� ������������ ���� ������� ������
    signed int checkWeakEquivalence(const AxiomSet& second) const;

	// Transmute selected axiom
	signed int transmuteAxiom (const int axiomNum, double ectl, double atl);

	void initAxiomWeights();

	std::vector<double> getAxiomWeights() const;

	// Normalizes weights
	void setAxiomWeights(const std::vector<double>& w);
    
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_SET_HXX */
