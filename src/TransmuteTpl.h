/****************************************************************************
*					TransmuteTpl
*
*	Description:	����� ������� ��� ��������� ������� ��������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Transmute_Tpl_HXX
#define __AXIOMS_LIB_Transmute_Tpl_HXX

#include "Environment.h"
#include "Axiom.h"
#include "AxiomSet.h"
#include "AxiomSetPlus.h"
#include "AxiomLibException.h"
#include "Round.h"
#include <string>
#include <vector>

namespace AxiomLib {

class TransmuteTpl {

 private:

	/*std::vector < std::vector <Axiom *>* > axioms;

	std::vector < std::vector <double> > axiomsProfit;

	std::vector < std::vector <unsigned short> > axiomsMask;

	std::vector < std::vector <double> > axiomsMaskProb;
	*/
	
	// ���� true, �� ��������� ������� ������ �������� ��������� ��������. 
	// ����� ��������� ������� ��������� �� ��������� �������
	bool canMutateOrder;

 protected:
  
 public:

	// ����������� ����������� - ������
	TransmuteTpl ();

	// ����������� ���������� - ������
	~TransmuteTpl ();

	// ���������� ��� ������� ������ �������
	std::string name (void) const;

	// ������� ������� �������� ���������� ������ �� ���������
	int setParamsFromEnv (const Environment& env);

	// ������� ���������� ������� vec �� ������������� � ������� ����������� � ���� �������� � ������ mask
	inline void sort (std::vector <double> &vec, std::vector <unsigned short> &mask) const;

	// ������� ������� ��������� ������� ������
	int transmute (AxiomSetPlus &axiomSetPlus) const;

	// �������� �������� ������� ��� ��������� ���������
	int transmute (std::vector <AxiomSetPlus> &population) const;
  
}; // end of class

}; // end of namespace

#endif /* __AXIOMS_LIB_Transmute_Tpl_HXX */
