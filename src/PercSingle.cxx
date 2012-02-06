/****************************************************************************
*						PercSingle.cxx
*
*       Description:	����������� �������-������ ��������� PercSingle
*       Author:			wictor
*       History:
*
****************************************************************************/

#include "PercSingle.h"

namespace AxiomLib {

/****************************************************************************
*					PercSingleSettings::initFromEnv()
*
*	Description:	������������� �� ���������. ��� ���������� ���������
*					 ��������� � ������� ����� ��������� (��� �����, �� ���������� �����������).
*					��� numAux ��������� 0,
*					��� auxDiff 0.1
*	Parameters:	Environment& env - ���������
*	Returns:	-
*	Throws:		AxiomLibException, ���� ��������, ��� �������� ��� ���������, �� ����������
*	Author:		wictor
*	History:
*
****************************************************************************/
void PercSingleSettings::initFromEnv(const Environment &env) {
	commonSettings.initFromEnv(env);
	coderSettings.initFromEnv(env);
	percSettings.initFromEnv(env);	

	// ��������� numAux, �� ��������� - 0
	if(env.getIntParamValue(numAux ,"numAux") < 0) {
		numAux = 0;
	}

	// ��������� auxDiff, �� ��������� - 0.1
	if(env.getDoubleParamValue(auxDiff ,"auxDiff") < 0) {
		auxDiff = 0.1;
	};
}

};
