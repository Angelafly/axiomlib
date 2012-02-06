/****************************************************************************
*				TemplateGeneticsFactory
*
*	Description:	��������� ����� �������� ������� �������������
*	Author:		dk
*	History:	
*
****************************************************************************/

#ifndef _AXIOMS_LIB_TEMPLATE_GENETICS_FACTORY_HXX
#define _AXIOMS_LIB_TEMPLATE_GENETICS_FACTORY_HXX

#include "AllTemplateGenetics.h"
#include "AxiomLibException.h"
#include <string>

namespace AxiomLib {

class TemplateGeneticsFactory {

 private:

 protected:
 
 public:

	// ����������� ������ �������
	TemplateGeneticsFactory(void) {}

	// ���������� ������ �������
	~TemplateGeneticsFactory(void) {}
  
	// ������� �������� ������� ������ TemplateGenetics �� �������� ������
	TemplateGenetics* create (const std::string name);
  
}; // end of class

}; // end of namespace AxiomLib

#endif /* _AXIOMS_LIB_TEMPLATE_GENETICS_FACTORY_HXX */
