/****************************************************************************
*
*				������� ������ PreprocessSimple
*
****************************************************************************/

#include "PreprocessSimple.h"
#include "TemplateRecognizer.h"

using namespace AxiomLib;

// ���������� ������
PreprocessSimple::PreprocessSimple (void) {
	someParameter = 0;
} 

// ���������� ������
PreprocessSimple::~PreprocessSimple (void) {
	
}

/****************************************************************************
*						PreprocessSimple::initFromEnv
*
*       Description:	������� ������������� ��������� ������ �� ������, 
*						���������� �� ������ ���������
*       Parameters:		env - ���������, ���������� ����������� ���������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSimple::initFromEnv (const Environment& env) {
	
	return 0;
}


/****************************************************************************
*						PreprocessSimple::copy
*
*       Description:	������� �������� ����� ��������� �������������
*       Parameters:		void
*       Returns:		Preprocess* - ��������� �� ��������� ����� ������� ���������������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
Preprocess* PreprocessSimple::copy (void) {
	PreprocessSimple* toReturn;
	toReturn = new PreprocessSimple;
	toReturn->setParams (this->someParameter);
	return (Preprocess*) toReturn;
}


/****************************************************************************
*						PreprocessSimple::setParams
*
*       Description:	������� ������������� ����������� ��� ������� ������ ���������
*       Parameters:		sp - ������������� ���������� ������ someParameter
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSimple::setParams (int sp) {
	someParameter = sp;
	return 0;
}


/****************************************************************************
*						PreprocessSimple::learn
*
*       Description:	������� �������� ������� ��������� �������������
*       Parameters:		templateRecognizer - ������ ��������������, �� �������
*						��������� �������� �������� ������� ��������� �������������
*       Returns:		-1
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
double PreprocessSimple::learn (TemplateRecognizer &templateRecognizer) {

	return -1;
}


/****************************************************************************
*						PreprocessSimple::run
*
*       Description:	�������� ������� ������ - ������ ��������� �������������
*       Parameters:		dataSet - ������������� ����� ������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int PreprocessSimple::run (DataSet &dataSet) {

	return 0;
}
