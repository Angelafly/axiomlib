/****************************************************************************
*			Nets
*
*	Description:	����� ���������� ������ ����������. 
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Nets_HXX
#define __AXIOMS_LIB_Nets_HXX

#include "Environment.h"
#include "AxiomSetPop.h"
//#include "AxiomSetPopExt.h"
#include "RefMarkupSetSimple.h"
#include "RefMarkupSimple.h"
#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class Nets {

 private:

	 // ���������� ������ ������ ������ - ���� ���� ���������, �� ����� true, ����� false.
	 bool status; 

 protected:
 
 public:
 
	// ����������� ����������� - ������� �������� �� ���������
	Nets (void);

	// ����������� ���������� - ������
	~Nets ();

	// ������� �������� ������� ����� �� �������� ��������� ��� ������� ������ ����������� ���������
	int prepare (std::vector <RefMarkupSetSimple> &refMarkupSetSimple);

	// ������� �������� - ���� �� � ���� markUp �������� �� ����������� �����, ������ ������� ������������� � ���� �� ������� i
	double check (int &resNum, std::vector <int> &row, long int &i) const;
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Nets_STRATEGY_HXX */
