/****************************************************************************
*
*				������� ������ TemplateRecognizer
*
****************************************************************************/

#include "TemplateRecognizer.h"

using namespace AxiomLib;

// ���������� ������
TemplateRecognizer::TemplateRecognizer (void) {
	preprocess = NULL;
	axiomSet = NULL;
	recognizer = NULL;
	refTrajFinder = NULL;
} 


// ���������� ������ - �������� ��� ��������� � ������ ������ ������������ �������
TemplateRecognizer::~TemplateRecognizer (void) {
	if (preprocess != NULL) delete preprocess;
	if (axiomSet != NULL) delete axiomSet;
	if (recognizer != NULL) delete recognizer;
	if (refTrajFinder != NULL) delete refTrajFinder;	
}


/****************************************************************************
*						TemplateRecognizer::setDataSet
*
*       Description:	������� ��������� ������ ������ ������ � ������� ����� ��������������
*       Parameters:		newDataSet - ����� ����� ������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int TemplateRecognizer::setDataSet (const DataSet &newDataSet) {
	dataSet = newDataSet;
	return 0;
}


/****************************************************************************
*                       TemplateRecognizer::setParamsOfDataSet
*
*       Description:	������� ��������� ������ ��������� ��������� ������ params
*       Parameters:	newParams - ����� �������� �������
*       Returns:		0
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int TemplateRecognizer::setParamsOfDataSet (std::vector<int> &newParams) {
	params = newParams;
	return 0;
}


/****************************************************************************
*				TemplateRecognizer::initFromEnv
*
*	Description:	������� ������������� ��������� ������� �� ������ �� 
*					���������, ������� �������� ��� �������� �������
*	Parameters:	env - ������ ������ ���������, ������ ������� ��������� ������� ������
*	Returns:	0
*	Throws:		AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::initFromEnv (const Environment& env) {
	
	return 0;
}


/****************************************************************************
*					TemplateRecognizer::run
*
*	Description:	�������� ������� � ������ - ��������� �������� ������
*					��������������.
*	Parameters:		resFirst - ����������� ����� ������ I ����
*					resSecond - ����������� ����� ������ II ����
*					asIndex - ������, ��� ������� ��������� ������� ������
*	Returns:		0 - ���� ������� ���������� ���������
*					-1 - ���� �������������� �� ��������� (���������� �����-���� �� ��� ���������)
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::run (int &resFirst, int &resSecond, const int asIndex) {
	// �������� ���������� ��������������
	if ((preprocess == NULL) || (axiomSet == NULL) || (recognizer == NULL))
		return -1;
	
	// �������� ����� ������ ������ ��� �� ��������� � �������������
	prepDataSet = dataSet;

	// ������������� ������ ������
	preprocess->run (prepDataSet);
	
	// ������ ������������� � ��������� ����� ������ ������� � ������� ����
	recognizer->run (*axiomSet, prepDataSet, params, resFirst, resSecond, asIndex);
	
	// ����� �� �������
	return 0;
}


/****************************************************************************
*					TemplateRecognizer::runWithOutput
*
*	Description:	��������� �������� ������ �������������� � ������� ����������� � ����.
*	Parameters:		resFirst - ����������� ����� ������ I ����
*					resSecond - ����������� ����� ������ II ����
*					asIndex - ������, ��� ������� ��������� ������� ������
*						(�������������� ��������, �������� �� ��������� -1 - ��� ����
*						 ������, ��� ������� ��������� ���������� ��������������� �� ��� ������������)
*	Returns:		0 - ���� ������� ���������� ���������
*					-1 - ���� �������������� �� ��������� (���������� �����-���� �� ��� ���������)
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::runWithOutput (int &resFirst, int &resSecond, const int asIndex) {
	int toRet;
	recognizer->setOutputToFile(true);
	toRet = run (resFirst, resSecond, asIndex);
	recognizer->setOutputToFile(false);
	// ����� �� �������
	return toRet;
}


/****************************************************************************
*					TemplateRecognizer::runPreprocessor
*
*	Description:	������ ��������� ������������� ��������� ������ ������
*	Parameters:		void
*	Returns:		0 - ���� ������� ���������� ���������
*					-1 - ���� �������������� �� ��������� (���������� �����-���� �� ��� ���������)
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::runPreprocessor (void) {
	// �������� ���������� ��������������
	if (preprocess == NULL)
		return -1;
	
	// �������� ����� ������ ������ ��� �� ��������� ���������� �������������
	prepDataSet = dataSet;

	// ������������� ������ ������
	preprocess->run (prepDataSet);

	// ����� �� �������
	return 0;
}


/****************************************************************************
*					TemplateRecognizer::runRecognizer
*
*	Description:	�������� ������ �������� ������������� �� ��� ����������������� ������ ������
*	Parameters:		resFirst - ����������� ����� ������ I ����
*					resSecond - ����������� ����� ������ II ����
*	Returns:		0 - ���� ������� ���������� ���������
*					-1 - ���� �������������� �� ��������� (���������� �����-���� �� ��� ���������)
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::runRecognizer (int &resFirst, int &resSecond) {
	// �������� ���������� ��������������
	if ((axiomSet == NULL) || (recognizer == NULL))
		return -1;
	
	// ������ ������������� � ��������� ����� ������ ������� � ������� ����
	recognizer->run (*axiomSet, prepDataSet, params, resFirst, resSecond);
	
	// ����� �� �������
	return 0;
}


/****************************************************************************
*					TemplateRecognizer::runRecognizer
*
*	Description:	�������� ������ �������� ������������� �� ��� ����������������� ������ ������
*					��� ���� ���� ������� ���������� �� �������� ������������ �������.
*	Parameters:		resFirst - ����������� ����� ������ I ����
*					resSecond - ����������� ����� ������ II ����
*	Returns:		0 - ���� ������� ���������� ���������
*					-1 - ���� �������������� �� ��������� (���������� �����-���� �� ��� ���������)
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::runRecognizer (int &resFirst, int &resSecond, std::vector<double> &stat) {
	// �������� ���������� ��������������
	if ((axiomSet == NULL) || (recognizer == NULL))
		return -1;
	
	// ������ ������������� � ��������� ����� ������ ������� � ������� ����
	recognizer->run (*axiomSet, prepDataSet, params, resFirst, resSecond, stat);
	
	// ����� �� �������
	return 0;
}


/****************************************************************************
*					TemplateRecognizer::learn
*
*	Description:	������� �������� �������
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateRecognizer::learn (void) {
	// �������� ����� �� ������ ���������
	if ((preprocess == NULL) || (axiomSet == NULL) || (recognizer == NULL))
		return -1;

	return 0;
}


/****************************************************************************
*				TemplateRecognizer::operator=
*
*	Description:	�������� ������������ ��������
*	Parameters:		second - ������������� ������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
TemplateRecognizer & TemplateRecognizer::operator= (const TemplateRecognizer &second) {
	// ����������� ��������� �������������
	if (preprocess != NULL) {
		delete preprocess;
		preprocess = NULL;
	}
	if (second.preprocess != NULL)
		preprocess = second.preprocess->copy();
	
	// ����������� ������� ������
	if (axiomSet == NULL)
		axiomSet = new AxiomSet;
	if (second.axiomSet == NULL) {
		delete axiomSet;
		axiomSet = NULL;
	} else {
		*axiomSet = *(second.axiomSet);
	}

	// ����������� ��������������
	if (recognizer != NULL) {
		delete recognizer;
		recognizer = NULL;
	}
	if (second.recognizer != NULL)
		recognizer = second.recognizer->copy();

	// ����������� ��������� ��������� ��������� ����������
	if (refTrajFinder != NULL) {
		delete refTrajFinder;
		refTrajFinder = NULL;
	}
	if (second.refTrajFinder != NULL)
		refTrajFinder = second.refTrajFinder->copy();

	// ����������� DataSet'� � ��� ����������
	dataSet = second.dataSet;
	params.clear();
	params = second.params;

	return *this;
}
