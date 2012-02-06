/****************************************************************************
*
*				������� ������ PreprocessFactory
*
****************************************************************************/

#include "PreprocessFactory.h"

using namespace AxiomLib;

// ���������� ������� �������
PreprocessFactory::PreprocessFactory(void) {
	classNum = 0;
	// ��������! ��� ������ ������ ���� �������� � ������ ������.
	classNames.resize (2);
	classNames[0] = "Simple";
	classNames[1] = "Squeeze";
}

// ���������� ������� �������
PreprocessFactory::~PreprocessFactory(void) {
	classNames.clear();
}

/****************************************************************************
*				PreprocessFactory::create
*
*	Description:	������� ����� ������������� �� �����
*	Parameters:		name - ��� ������, ��������� �������� �������
*	Returns:		Preprocess* - ��������� �� ��������� �����
*	Throws:			AxiomLibException - ���� ������ ���
*	Author:			dk
*	History:
*
****************************************************************************/
Preprocess* PreprocessFactory::create (const std::string name) {
	
	if ( (name == "Simple") || (name == "SIMPLE") || (name == "simple") ) {
		PreprocessSimple* toReturn;
		toReturn = new PreprocessSimple;
		return toReturn;
	}

	if ( (name == "Squeeze") || (name == "SQUEEZE") || (name == "squeeze") ) {
		PreprocessSqueeze* toReturn;
		toReturn = new PreprocessSqueeze;
		return toReturn;
	}

	throw AxiomLibException ("PreprocessFactory::create : can not find the requested preprocessor.");
}


/****************************************************************************
*				PreprocessFactory::create
*
*	Description:	������� ��������� ����� �������������
*	Parameters:		-
*	Returns:		Preprocess* - ��������� �� ��������� �����
*	Throws:			AxiomLibException - ���� ������ ���
*	Author:			dk
*	History:
*
****************************************************************************/
Preprocess* PreprocessFactory::create (void) {
	// �������� �������� �������� ����������, ���������� �� ����������� � �������� �������
	if (classNum >= classNames.size())
		throw AxiomLibException ("PreprocessFactory::create : no preprocessors to create one.");
	// �������� ������ �������������
	Preprocess* toReturn;
	toReturn = create (classNames[classNum]);
	// ���������� �������� ������� �������������
	classNum++;
	if (classNum >= classNames.size())
		classNum = 0;
	// ���������� ��������� ����� �������������
	return toReturn;
}


/****************************************************************************
*				PreprocessFactory::getSize
*
*	Description:	���������� ����� ������� �������������, ������� ����� ���� 
*					������� � ������ ������.
*	Parameters:		-
*	Returns:		int - ����� ������� �������������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int PreprocessFactory::getSize (void) {
	return (int) classNames.size();
}
