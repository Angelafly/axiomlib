/****************************************************************************
*						AxiomExprStructure
*
*	Description:	�������� ��������� �������
*	Author:			dk
*	History:     	-
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AxiomExprStructure_HXX
#define __AXIOMS_LIB_AxiomExprStructure_HXX

#include "ElemCondPlusStruture.h"
#include <vector>
#include <string>

namespace AxiomLib {

class AxiomExprStructure {

public:
	
	// �������� ������ ������� ������
	std::string axiomName;
	// �������� ������������� ����� ������������ ������� (����� ���� �� ������)
	std::string elemCondBankName;
	// �������� ��������� ��� ������������� ���������, ������������� ������ �������. 
	// � �������� - ����� �� �������������� ���� and, ����� ��������� - ���� or. ��������� �������� � ��������� ElemCondPlusStruture
	std::vector <std::vector <ElemCondPlusStruture> > elemConds;

		// ������� ������: //

	// ����������� ������ - ������� ��� ���� �������
	AxiomExprStructure() {
		axiomName.clear();
		elemCondBankName.clear();
		elemConds.clear();
	}

	// ������� ������� ���������� ����� ������
	void clear() {
		axiomName.clear();
		elemCondBankName.clear();
		for (unsigned int i = 0; i < elemConds.size(); i++) {
			for (unsigned int j = 0; j < elemConds[i].size(); j++)
				elemConds[i][j].clear();
			elemConds[i].clear();
		}
		elemConds.clear();
	}
	
}; //end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AxiomExprStructure_HXX */
