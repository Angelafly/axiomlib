/****************************************************************************
*			Net
*
*	Description:	����� ����������� ���������. 
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Net_HXX
#define __AXIOMS_LIB_Net_HXX

#include "AxiomLibException.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <set>

namespace AxiomLib {

class Net {

 private:

	 // ���������� ������ ������ ������ - ���� ���� ���������, �� ����� true, ����� false.
	 bool status; 

	 // ���������� �������� q-������ ����� - q
	 unsigned int q;

	 // ����� ������� ��������
	 unsigned int inNum;

	 // ����� �������� ��������
	 unsigned int outNum;

	 // ������������ ���������� ����� �� �������� q �� outNum
	 std::vector <int> outCoeff;

	 // ������������ ������, ������� ����� ���� ����������� � outNum ����� ��������
	 int maxOutNum;

	 // ������� ������������� ��� ���� ������ ����
	 std::vector <std::vector <std::vector <std::vector <char> > > > matrices;

	 std::map <int, int> distValues;
	 std::set <int> usedDistValues;

	 // ��������������� ������� ������������� ����� � ���� ��������� �������
	 inline void code (int input, std::vector <char> &res) const;

	 // ��������������� ������� ������������� ������� ����� �� 0 �� q  � ���� �������� �������
	 inline void code (std::vector <int> &input, std::vector <std::vector <char> > &res) const;

	 // ������� ����������� ������ ������ � q-������ ������
	 inline void codeRes (int input, std::vector <int> &res);

	 // ������� ������������� ������ ������ � q-������ ������
	 inline void decodeRes (int &output, std::vector <int> &res) const;

	 // ��������������� ������� ������������� ������� � ������, ������ �� ������� ����������� � ���� �������
	 inline void mult (std::vector <char> &x, std::vector <char> &y, std::vector <std::vector <char> > &res) const;

 protected:
 
 public:
 
	// ����������� ����������� - ������� �������� �� ���������
	Net (unsigned int newQ);

	// ����������� ���������� - ������
	~Net ();

	// ������� �������� ������� ����� �� �������� ��������� ��� ������� ������ ����������� ���������
	int prepare (std::vector <int> &in, int out);

	// ������� �������� - ���� �� � ���� markUp �������� �� ����������� �����, ������ ������� ������������� � ���� �� ������� i
	double check (int &resNum, std::vector <int> &row, long int &i) const;
  
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_Net_STRATEGY_HXX */
