/***************************************************************************
*					TemplateRecognizer
*
*	Description:	�����, ����������� ������ ��������������, ������� �������� 
*					�������� �������������, ������� ������, �������� ������
*					��������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Template_Recognizer_HXX
#define _Template_Recognizer_HXX

#include "AxiomLibException.h"
#include "Round.h"
#include "Environment.h"
#include "DataSet.h"
#include "Preprocess.h"
#include "AxiomSet.h"
#include "Recognizer.h"
#include "RefTrajFinder.h"

#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class TemplateRecognizer {

 private:

 protected:

 public:

	// �������� ������� ������:
	// ����� ������������� 
	Preprocess* preprocess; 

	// ������� ������
	AxiomSet* axiomSet;

	// ����� ��������������
	Recognizer* recognizer;

	// �������� ��������� ��������� ����������
	RefTrajFinder* refTrajFinder;

	// ������ �������� ������: 
	// ����� ��������� �����
	DataSet dataSet;

	// ����� ��������� ����� - ����� ��������� ���������� �������������
	DataSet prepDataSet;

	// ������ �������� ���������� � ������������ ������������� �� ������� ����� ������ - ����� ��������� ��������
	// ��� ���������� ���������� - �������� ����������� ���� � ��������� ����� ����������� ��������� �������
	std::vector<int> params;

	/**********************************************************************************/
	// ������� ������:
	// ����������� ������ � �������� ���� ���������� �� ���������
	TemplateRecognizer (void);  
  
	// �����������, ������������ ��� ���������� �������� ������ �������
	~TemplateRecognizer (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� ��������� ������ ������ ������ � ������� ����� ��������������
	int setDataSet (const DataSet &newDataSet);

	// ������� ��������� ������ ��������� ��������� ������, ������� ��������� ����� ���� �� ������ ������ ������������
	int setParamsOfDataSet (std::vector<int> &newParams);

	// �������� ������� ������ - ������ ������ ��������������
	int run (int &resFirst, int &resSecond, const int asIndex = -1);

	// ������ ������ �������������� � ������� ���� �������� � �����
	int runWithOutput (int &resFirst, int &resSecond, const int asIndex = -1);

	// ������ ��������� ������������� ��������� ������ ������
	int runPreprocessor (void);

	// �������� ������ �������� ������������� �� ��� ����������������� ������ ������
	int runRecognizer (int &resFirst, int &resSecond);

	// �������� ������ �������� ������������� � ��������� ���������� �� ��� ����������������� ������ ������
	int runRecognizer (int &resFirst, int &resSecond, std::vector<double> &stat);

	// ������� �������� �������
	int learn (void);

	// �������� ������������ ��������
    TemplateRecognizer & operator= (const TemplateRecognizer &second);

}; // end of class

}; // end of namespace

#endif /* _Template_Recognizer_HXX */
