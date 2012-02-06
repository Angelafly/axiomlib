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

namespace AxiomLib {

class AxiomLibException {
  private:

  protected:
    std::string errText;

  public:
	AxiomLibException() {};

    // ����������� �����������
    AxiomLibException(const std::string& errorText): errText(errorText) {};
    
    // ����������� ����������
    ~AxiomLibException() {};

    // ���������� ����� ������
    std::string error (void) const {return errText;}

};

}; // namespace AxiomLib

#endif /* __AXIOMS_LIB_AXIOM_LIB_EXCEPTION_HXX */
