/****************************************************************************
*			AxiomLibException
*
*	Description:	����� ���������� ��� ���������� AxiomLib
*	Author:		Eugene A. Vasin
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_AXIOM_LIB_EXCEPTION_HXX
#define __AXIOMS_LIB_AXIOM_LIB_EXCEPTION_HXX

#include <string>
#include <stdexcept>

namespace AxiomLib {

class AxiomLibException : std::logic_error {
  private:

  protected:
    std::string errText;

  public:
	AxiomLibException() : logic_error("unspecified AxiomLibException") {}

    // ����������� �����������
	AxiomLibException(const std::string& errorText): logic_error(errorText), errText(errorText) {}
    
    // ����������� ����������
	~AxiomLibException() throw() {}

    // ���������� ����� ������
	std::string error (void) const { return errText; }

};

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_AXIOM_LIB_EXCEPTION_HXX */
