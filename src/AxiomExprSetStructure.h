/****************************************************************************
*						AxiomExprSetStructure
*
*	Description:	�������� ��������� ������� ������
*	Author:			dk
*	History:     	-
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AxiomExprSetStructure_HXX
#define __AXIOMS_LIB_AxiomExprSetStructure_HXX

#include "AxiomExprStructure.h"
#include <vector>
#include <string>

namespace AxiomLib {

class AxiomExprSetStructure {

public:
	
	// �������� ������ ������� ������
	std::string axiomSetName;
	// �������� ������������� ����� ������
	std::string axiomBankDir;
	// �������� ������, ������������ ������ ������� ������
	std::vector <AxiomExprStructure> axioms;
	
}; //end of class

}; //  end of namespace

#endif /* __AXIOMS_LIB_AxiomExprSetStructure_HXX */
