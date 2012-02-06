/****************************************************************************
*				
*
*	Description:	
*	Author:		
*	History:		
*
****************************************************************************/

#ifndef __AXIOMS_LIB_POP_ELEM_EXT_HXX
#define __AXIOMS_LIB_POP_ELEM_EXT_HXX

#include "AxiomSetWithStats.h"
#include "Environment.h"
#include <vector>
#include <set>

namespace AxiomLib {

class PopElemExt {

private:

protected:

public:

  AxiomSetWithStats axSet;
  
  double prec;
  
  // ������ ����������� � �������� ���������� �� ���������
  PopElemExt (void) {}

  // ���������� - ��������� ��� ��������� � ������ ������� ������ ������������ �������
  ~PopElemExt (void) {}
  
  // ������������� ��������� ��������� ������ ������ �� ���������
  int initFromEnv (const Environment& env);
    
  // ����� ���������� ������� ������ ��� ������� asNum � ������� asPop
  void voidStats ();
  
  // ��������� ����������
  void setStats (signed int nf, signed int ns, double g);
  
  // ��������� ����������, ��� ��������
  void getStats (signed int& nf, signed int& ns, double& g) const;
  
  // ��������� ��� row ������� � ������� begin ���������� �������� end �������� ������ ����� asNum �� ������� asPop � �������� ���������� �������� � ��� markup
  int enter (std::vector<int>& markup, const std::vector<double>& row, const unsigned long begin, const unsigned long end);
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_POP_ELEM_EXT_HXX */
