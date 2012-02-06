/****************************************************************************
*			AxiomSetwithStats
*
*	Description:	������� ������ + ���������� �� ���
*	Author:		gevor
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_SET_WITH_STATS_HXX
#define __AXIOMS_LIB_AXIOM_SET_WITH_STATS_HXX

#include "AxiomSet.h"
#include "AxiomBase.h"
#include <string>

namespace AxiomLib {

class AxiomSetWithStats:public AxiomSet {

 private:

 protected:
 
  double goal; // �������� ������� �������
  int nFirst;  // ���-�� ������ ������� ����
  int nSecond; // ���-�� ������ ������� ����

 public:

  // ��������� ������ ����� ������ �������� ������� ��������� �����������
  friend class CrossoverStrategy;
  
  // ��������� ������ ����� ������ �������� ������� ��������� �������
  friend class TransmuteStrategy;
 
  // ������ ����������� � �������� ����� �� default'�
  AxiomSetWithStats(void) : AxiomSet () {nFirst = -1; nSecond = -1; goal = -1;}
  
  // ������ ����������� � �������� ����� � ���������
  AxiomSetWithStats(const std::string sName) : AxiomSet (sName) {nFirst = -1; nSecond = -1; goal = -1;}
	 
  // ���������� ��������� ��� ��������� � ������ ������ ������� ������ ������������ �������
  ~AxiomSetWithStats(void) {};
  
  // ��������� �� goal
  bool operator< (const AxiomSetWithStats& second) const;
  
  // ����� ����������
  inline void voidStats(void) {nFirst = -1; nSecond = -1; goal = -1;}
  // ��������� ����������
  inline void setStats(signed int nf, signed int ns, double g) {nFirst = nf; nSecond = ns; goal = g;}
  // ��������� ����������, ��� ��������
  inline void unsafeGetStats(signed int& nf, signed int& ns, double& g) const {nf = nFirst; ns = nSecond; g = goal;}
  
//   // �������� �� ������������ �������� ������� �������
//   bool exit (double exitCond);
  
  // ������������� ������ ������ �� �����
  signed int initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, AxiomBase &axiomBase);
  
  // ����� ������ ������ �� �������
  int outputToCout (void) const;
  
  // �������� goal, nFirst � nSecond
  AxiomSetWithStats & operator= (const AxiomSetWithStats &second);
    
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_SET_WITH_STATS_HXX */
