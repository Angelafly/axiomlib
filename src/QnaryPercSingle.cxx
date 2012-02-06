/****************************************************************************
*						QnaryPercSingle.cxx
*
*       Description:	����������� �������-������ ��������� QnaryPercSingleSettings
*       Author:			wictor
*       History:
*
****************************************************************************/
#include "QnaryPercSingle.h"

namespace AxiomLib {

/****************************************************************************
*					QnaryPercSingleSettings::initFromEnv()
*
*	Description:	������������� �� ���������. ��� ���������� ���������
*					 ��������� � ������� ����� ��������� (��� �����, �� ���������� �����������).
*					��� qO ��������� 0, ��� ��������, ��� ����� ��������� ����� ����� ����� ��������� �����
*	Parameters:	Environment& env - ���������
*	Returns:	-
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
void QnaryPercSingleSettings::initFromEnv(const Environment &env) {
	commonSettings.initFromEnv(env);
	coderSettings.initFromEnv(env);	

	// ��������� qO, �� ��������� - 0
	if(env.getIntParamValue(qO ,"qOutput") < 0) {
		qO = 0;
	}
}

};

