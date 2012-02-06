/***************************************************************************
*					PreprocessSimple
*
*	Description:	����� �������������, ������� �� �������� ������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Preprocess_Simple_HXX
#define _Preprocess_Simple_HXX

#include "AxiomLibException.h"
#include "Round.h"
#include "DataSet.h"
#include "Environment.h"
#include "Preprocess.h"

#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class PreprocessSimple : public Preprocess {

 private:

	// ��������� ��������
	int someParameter;

 protected:

 public:

	// ����������� ������
	PreprocessSimple (void);

	// ���������� ������
	~PreprocessSimple (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� ������������� ����������� ��� ������� ������ ���������
	int setParams (int sp);

	// ������� �������� ����� ��������� �������������
	Preprocess* copy (void);

	// ������� �������� ������� ��������� �������������
	double learn (TemplateRecognizer &templateRecognizer);

	// �������� ������� ������ - ������ ��������� �������������
	int run (DataSet &dataSet);

}; // end of class

}; // end of namespace

#endif /* _Preprocess_Simple_HXX */
