/****************************************************************************
*					PreprocessFactory
*
*	Description:	��������� ����� �������� ������� �������������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef __AXIOMS_LIB_Preprocess_FACTORY_HXX
#define __AXIOMS_LIB_Preprocess_FACTORY_HXX

#include "AllPreprocess.h"
#include "AxiomLibException.h"

#include <string>
#include <vector>

namespace AxiomLib {

class PreprocessFactory {

  private:

	// �������� ���� ������� �������������, ������� ����� ���� ������� � ������ ������
	std::vector <std::string> classNames;

	// ���������, ����� ����� ��������� ��� ������ � ������� ������� create (void) ��� 0 �������
	unsigned int classNum;

  protected:

  public:

	// ���������� ������� �������
	PreprocessFactory(void);

	// ���������� ������� �������
	~PreprocessFactory(void);

	// ���������� ����� ������� �������������, ������� ����� ���� ������� ������ �������
	int getSize (void);

	// ������� �������� ������� �� �����
	Preprocess* create (const std::string name);

	// ������� �������� ������� ���������� ������ �������������
	Preprocess* create (void);

};

}; /* namespace AxiomLib */

#endif /* __AXIOMS_LIB_Preprocess_FACTORY_HXX */
