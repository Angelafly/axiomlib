/***************************************************************************
*					Preprocess
*
*	Description:	����������� ����� �������� ���������� �������������
*					�������� ������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Preprocess_HXX
#define _Preprocess_HXX

#include "AxiomLibException.h"
#include "Round.h"
#include "DataSet.h"
#include "Environment.h"

#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class TemplateRecognizer;

class Preprocess {

 private:

 protected:

 public:

	// ����������
	virtual ~Preprocess () {}

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	virtual int initFromEnv (const Environment& env) = 0;

	// ������� �������� ����� ��������� �������������
	virtual Preprocess* copy (void) = 0;

	// ������� �������� ������� ��������� �������������
	virtual double learn (TemplateRecognizer &templateRecognizer) = 0;

	// �������� ������� ������ - ������ ��������� �������������
	virtual int run (DataSet &dataSet) = 0;

}; // end of class

}; // end of namespace

#endif /* _Preprocess_HXX */
