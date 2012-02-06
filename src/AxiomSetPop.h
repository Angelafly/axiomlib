/****************************************************************************
*				AxiomSetPop
*
*	Description:	��������� � ������� - �������� ������
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_SET_POP_HXX
#define __AXIOMS_LIB_AXIOM_SET_POP_HXX

#include "AxiomSetWithStats.h"
#include "Environment.h"
#include <vector>
#include <set>

namespace AxiomLib {

class AxiomSetPop {

private:

  friend class CrossoverStrategy;
  
  friend class TransmuteStrategy;

protected:

  std::vector<AxiomSetWithStats>  asPop; // ��������� - axiom sets
  
  int popSize; // ������ ���������, ��� �� ������ � �������
  
public:

  // ������ ����������� � �������� ���������� �� ���������
  AxiomSetPop (void) {popSize = -1; /*asPop = new std::vector<AxiomSetWithStats>;*/ asPop.resize(0);}

  // ���������� - ��������� ��� ��������� � ������ ������� ������ ������������ �������
  ~AxiomSetPop(void) {}
  
  // ������������� ��������� ��������� ������ ������ �� ���������
  int initFromEnv (const Environment& env);
    
  // ��������� ��������������� ������ ������ � ��������
  void clear ();
  
  // ��������� ��������������� ������ ������ � ��������
  void addSets (std::vector < std::vector<AxiomSetWithStats> > &newSets, std::vector < std::vector <int> > &setsMask);
  // ��������� ��������������� ������ ������ � ��������
  void addSets (std::vector<AxiomSetWithStats>  &newSets); 
  
  // ����� ���������� ������� ������ ��� ������� asNum � ������� asPop
  void voidStats (const int asNum);
  // ��������� ���������� ������� ������ ��� ������� asNum � ������� asPop
  void setStats (const int asNum, signed int nf, signed int ns, double g);
  // ��������� ����������, ��� ��������, ��� ������� ������ ��� ������� asNum � ������� asPop
  void getStats (const int asNum, signed int& nf, signed int& ns, double& g) const;
  // copy axiom set with statistic by number asNum from this class
  AxiomSetWithStats& copyAxiomSet (const int asNum);
  
  // ������� ��������� - ���������� �� ������� ������ � ������� �������� ���� ��������� ��������, ������� �������� ������ ������� ������
  bool exit (double exitCond, AxiomSetWithStats &bestAxiomSet);
  
  // ������� ������������ ��������� - �������� �� ��������� ��� ����� - �� ������� ������� ����� false
  int cutDown (std::vector <bool> &stayAlive);
  
  // ��������� ��� row ������� � ������� begin ���������� �������� end �������� ������ ����� asNum �� ������� asPop � �������� ���������� �������� � ��� markup
  int enter (std::vector<int>& markup, const std::vector<double>& row, const unsigned long begin, const unsigned long end, int asNum);
  
  // �������� ������� ������ ������ ������ (�� �������)
  int getBasePopSize(void) const;
  // �������� ������� ������ ���������
  int size(void) const;

  // ������� ������������ ��� ������� ������ �� ������ � ���������
  std::string name (int index) const;

  // ���������� ������ ������ � �����
  int saveAxiomSetPopToFiles (Environment &env);
  
  // �������� ����������� ���������
  //AxiomSetPop & operator= (AxiomSetPop& second);

}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_BASE_GENETICS_HXX */
