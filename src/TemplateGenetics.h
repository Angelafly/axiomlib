/****************************************************************************
*					TemplateGenetics
*
*	Description:	������� ������������ ����� ��� ���������� �������� 
*					�������� ���������������.
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef _AXIOMS_LIB_TEMPLATE_GENETICS_HXX
#define _AXIOMS_LIB_TEMPLATE_GENETICS_HXX

#include "Environment.h"
#include "TemplateRecognizer.h"
#include "AxiomLibException.h"

namespace AxiomLib {

class TemplateGenetics {

  private:

  protected:

  public:

	// ����������
	virtual ~TemplateGenetics () {}

	// ��������� ���������� ���������
	virtual int initFromEnv (const Environment& env) = 0;

	// ������ ������������� ���������
	virtual double run (TemplateRecognizer& templateRecognizer) = 0;

}; // end of class

}; //  end of namespace

#endif /* _AXIOMS_LIB_TEMPLATE_GENETICS_HXX */
