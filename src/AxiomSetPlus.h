/****************************************************************************
*					AxiomSetPlus
*
*	Description:	������� ������ + ���������� �� ��� + ��������� �� ���
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_SET_PLUS_HXX
#define __AXIOMS_LIB_AXIOM_SET_PLUS_HXX

#include "AxiomSet.h"

namespace AxiomLib {

struct AxiomSetPlus {

 private:

 protected: 
  
 public:

	// ������� ������
	AxiomSet* axiomSet; 

	// ���������� �� ������� ������
	double goal; // �������� ������� �������
	int nFirst;  // ���-�� ������ ������� ����
	int nSecond; // ���-�� ������ ������� ����
	std::vector <double> axiomProfit; // ���������� ������ �������

	// ����������� ���������� - ��������� ������� ������
	double goalRatio;

	// ��������� �����������
	double asCrossProb; // ������� ������� (+-0.5)
	double axCrossProb; // ������ ���� ������ (+-0.8) ��� ��� �� ������� ������ � ��� � ������� 1-2 ������� ����� ���� �������, � ����� ��� � ������ ����������� �����������
	double ecCrossProb; // ������ ���� ������ (+-0.8) ��� ��� � ��� � ������� 1-2 �� ����� ���� �������, � ����� ��� � ������ ����������� �����������

	// ��������� �������
	// ������� �� ������ ���������� ������ � ������� 
	double prTransProb; // ���� �������� (0.2) ��� ��� ����������� ��� ���� ������ � �������
	double asTransProb; // ����� ������ (0.05) ��� ��� ����������� ��� ������ ������� � ����� ���������������
	double axTransProb; // ������ (+-0.1) ��� ��� � ������ ������������ ���������� ������ �� � �������
	double ecTransProb; // ������ (+-0.1) ��� ��� ����������� � ������� �� � �������

	// ********************* ������� ��������� ***************************

	// ����������� ���������
	AxiomSetPlus (void);

	// ���������� ���������
	~AxiomSetPlus (void);

	// ������� ������� ����������� ������� ���������
	int clear (void);

	// ������� ������� ���������� �� ������� ������
	int clearStat (void);

	AxiomSetPlus(const AxiomSetPlus& second);
	
	// �������� ������������ ������� ������
    AxiomSetPlus & operator= (const AxiomSetPlus &second);
    
}; // end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AXIOM_SET_PLUS_HXX */
