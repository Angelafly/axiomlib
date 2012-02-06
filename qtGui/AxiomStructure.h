/****************************************************************************
*					AxiomStructure
*
*	Description:	�����, ���������� �������� ��������� ������� � �� ���������
*	Author:			dk
*	History:     	-
*
****************************************************************************/
#ifndef __qtGui_AxiomStructure_HXX
#define __qtGui_AxiomStructure_HXX

#include <stdlib.h>
#include <map>

// ���������� ��� ��������
typedef std::map<std::string, std::map<std::string, std::string> >::const_iterator	c_iter;

class AxiomStructure {

public:
	
	// �������� �������
	std::string axiomName;
	// ����� �������� ������������ �������, ������������ �������
	std::vector <std::string> ecNames;
	// ��������� ������������ �������, �������� � ������� ������
	std::map<std::string, std::map<std::string, std::string> > axiomParameters;
	
}; //end of class

#endif /* __qtGui_AxiomStructure_HXX */
