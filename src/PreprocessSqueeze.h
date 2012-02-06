/***************************************************************************
*					PreprocessSqueeze
*
*	Description:	����� �������������, ������� � ������������� �� ���������
*					���������� ������ ��� ���������� ���������� �� ������ ������.
*	Author:			dk
*	History:
*
****************************************************************************/

#ifndef _Preprocess_Squeeze_HXX
#define _Preprocess_Squeeze_HXX

#include "AxiomLibException.h"
#include "Round.h"
#include "DataSet.h"
#include "Environment.h"
#include "Preprocess.h"
#include "GoalStrategyFactory.h"
#include "GoalStrategy.h"

#include <string>
#include <vector>
#include <map>

namespace AxiomLib {

class PreprocessSqueeze : public Preprocess {

 private:

	// �������� ������ / ���������� ���������� (��� 1.0 - ���������� �� ����������)
	double squeezeParam;

	// ����������� � ������������ �������� ��� ��������� squeezeParam
	double squeezeParamMin;
	double squeezeParamMax;

	// ������� �������� �������� ��� ���������� ������������ 
	// ( ���� ��� � (����� ����� ������������ - 1) )
	unsigned int LagrangeDegree;

	// ������ ����������� ��� ���������� �������� ���������� ��������
	std::vector <double> factorials;

	GoalStrategy* goalStrategy; // ��������� �������� ������� ������� �� ������� I � II ����

	// ��������������� ������� �������� - ���������� �������� �������� precision
	double sublearn (TemplateRecognizer &templateRecognizer, std::vector <double> &curRes, std::vector <double> &curSqueezeParams);

	// ��������������� ������� - ����� �������������� ��� ��������� ���������
	inline bool chooseBest (std::vector <double> &curRes, std::vector <double> &curSqueezeParams) const;

 protected:

 public:

	// ����������� ������
	PreprocessSqueeze (void);

	// ���������� ������
	~PreprocessSqueeze (void);

	// ������� ������������� ��������� ������ �� ������, ���������� �� ������ ���������
	int initFromEnv (const Environment& env);

	// ������� ������������� ����������� ��� ������� ������ ���������
	int setParams (double sp);

	// ������� ������������� ��� ����������� ��� ������� ������ ���������
	int setParams (double sp, double spMin, double spMax, unsigned int ld);

	// ������� �������� ����� ��������� �������������
	Preprocess* copy (void);

	// ���������� ������� ���������� ������������� ��� ������ �������� ������� ����
	bool apply (std::vector <double> &newVec, std::vector <double> &vec) const;

	// ���������� ������� ���������� ������������� ��� ������ �������� ������� ����
	bool apply (std::vector <int> &newVec, std::vector <int> &vec) const;

	// ������� �������� ������� ��������� ������������� � ������ ���������� �������
	double learn (TemplateRecognizer &templateRecognizer);

	// �������� ������� ������ - ������ ��������� �������������
	int run (DataSet &dataSet);

}; // end of class

}; // end of namespace

#endif /* _Preprocess_Squeeze_HXX */
