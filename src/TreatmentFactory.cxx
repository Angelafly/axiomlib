/****************************************************************************
*
*			������� ������ TreatmentFactory
*
****************************************************************************/

#include "TreatmentFactory.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			TreatmentFactory::create
*
*	Description:	������� ����� �������������
*	Parameters:	name - ��� ������, ��������� �������� �������
*	Returns:		Treatment* - ��������� �� ��������� �����
*	Throws:		AxiomLibException - ���� ������ ���
*	Author:		dk
*	History:
*
****************************************************************************/
Treatment* TreatmentFactory::create (const std::string name) {
  if (name == "TreatmentSample") {
    TreatmentSample* toReturn;
    toReturn = new TreatmentSample;
    return toReturn;
  }
  throw AxiomLibException ("TreatmentFactory::create : can not find the requested treatment strategy.");
}
