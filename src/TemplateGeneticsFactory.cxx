/****************************************************************************
*
*				������� ������ TemplateGeneticsFactory
*
****************************************************************************/

#include "TemplateGeneticsFactory.h"

using namespace AxiomLib;

/****************************************************************************
*					TemplateGeneticsFactory::create
*
*	Description:	������� ����� ��������� �������� ��������
*	Parameters:		name - ��� ������, ��������� �������� �������
*	Returns:		TemplateGenetics* - ��������� �� ��������� �����
*	Throws:			AxiomLibException - ���� ������ ���
*	Author:			dk
*	History:
*
****************************************************************************/
TemplateGenetics* TemplateGeneticsFactory::create (const std::string name) {
	
	if ( (name == "Extend") || (name == "EXTEND") || (name == "extend") ) {
		TemplateGeneticsExt* toReturn;
		toReturn = new TemplateGeneticsExt;
		return toReturn;
	}

	throw AxiomLibException ("TemplateGeneticsFactory::create : can not find the requested TemplateGenetics class.");
}
