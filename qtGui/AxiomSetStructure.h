/****************************************************************************
*					AxiomSetStructure
*
*	Description:	�����, ���������� �������� ��������� ������� ������
*	Author:			dk
*	History:     	-
*
****************************************************************************/
#ifndef __qtGui_AxiomSetStructure_HXX
#define __qtGui_AxiomSetStructure_HXX

#include "AxiomStructure.h"

class AxiomSetStructure {

public:
	
	// �������� ������� ������
	std::string axiomSetName;
	// �������� ����� ������ �� �������� ����� ���� ����� �������
	std::string axiomBankDir;
	// ����� ������, ������������ ������� ������
	std::vector <AxiomStructure> axioms;
	
}; //end of class

#endif /* __qtGui_AxiomSetStructure_HXX */
