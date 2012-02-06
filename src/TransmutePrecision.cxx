/****************************************************************************
*			TransmutePrecision
*
*	Description:	������� ������ TransmutePrecision - ����������
*	Author:		gevor
*	History:		
*
****************************************************************************/
#include "TransmutePrecision.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			TransmutePrecision::setParamsFromEnv
*
*	Description:	��������� ���������� ������� �� �����
*	Parameters:	const Environment& env - ������ �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ��������� ���
*	Author:		gevor
*	History:
*
****************************************************************************/
int TransmutePrecision::setParamsFromEnv (const Environment& env)
{
  if (env.getDoubleParamValue(ecTransLevel, "transLevel") < 0) {
    throw AxiomLibException ("Error in TransmutePrecision::setParamsFromEnv - can not find all needed parameners.");
  }
  if (ecTransLevel < 0 || ecTransLevel > 1)
    throw AxiomLibException ("Error in TransmutePrecision::setParamsFromEnv - ecTransLevel out of range.");
  return 0;
}

/****************************************************************************
*			TransmutePrecision::select
*
*	Description:	������� ������� ��� ������ ��������� - ������� ������ �� ������ ������������ ������� (����)
*	Parameters:	AxiomSetPop - ���������� ���������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int TransmutePrecision::transmute (AxiomSetPop& pop) {
	// �������� ������������ ���������� ��������
	if (ecTransLevel < 0 || ecTransLevel > 1)
		throw AxiomLibException ("Error in TransmutePrecision::transmute.");
	// ����� �� ���������, �������� ��, ������� ��������� ����������
	// ������� �� ������ ���������� ������������ �������
	if (ecTransLevel > 0) {
		// �������� ������
		initTransmute (pop);
		int curPopSize = (int) axiomSets.size();
		// ����� �� ���� ������� ������
		for (int i = 0; i < curPopSize; i++) {
			int nOfAxioms = (int) (axiomSets[i])->size();
			int resTransmuteAxiom = 0;
			// ����� �� ���� �������� � �������
			for (int j = 0; j < nOfAxioms; j++) {
				resTransmuteAxiom = (*(axiomSets[i]))[j]->transmute(ecTransLevel, 0.0);
			}
			(*asPop)[i].voidStats();
		}
	}
	
	return 0;
}
