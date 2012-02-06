/****************************************************************************
*
*			������� ������ ElemCondition
*
****************************************************************************/
#include "ElemCondition.h"
#include "../AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*                       ElemCondition::crossEC
*
*       Description:    ������� (���� � ������ ��������� � �� �������������)
*			������� ����������� ���� ������������ ������� ������
*			����. ������ ��� ������������ �������.
*       Parameters:     const ElemCondition& second - ������ ������������ �������
*			double crossProb - ����������� ����������� ���� �������
*			double crossParam - �������� ����������� (=0 - ���, =1 - ����������)
*       Returns:        
*       Throws:         AxiomLibException - ���� ������� ������ �����
*       Author:         dk
*       History:
*
****************************************************************************/
void ElemCondition::crossEC(ElemCondition& second, const double crossProb, const double crossParam)
{
  // 1. ����������, �������� �� ��� ��������� ������ ����
  if (name() != second.name()) {
    throw AxiomLibException("ElemCondition::crossEC - trying to cross parameters of differenc ECs.");
  }

  // ��������� ������ ���������  
  if (crossParam > 1 || crossParam < 0) {
    throw AxiomLibException("ElemCondition::crossEC - crossParam out of range.");
  }
  if (crossProb > 1 || crossProb < 0) {
    throw AxiomLibException("ElemCondition::crossEC - crossProb out of range.");
  }
  
  // 2. �������� ������ ���� ����������
  std::vector<std::string> ecParamNames;
  getECParamNames (ecParamNames);
  
  // 3. ����� ��������������� �� ���� ����������
  for (std::vector<std::string>::const_iterator curParamName = ecParamNames.begin(); curParamName != ecParamNames.end(); curParamName++) {
    // �������, ����� �� ��� �������
    if ((( (double) rand() / (double) RAND_MAX )) < crossProb) {
      // ��, ������� �����

      // �������� �������� ��������� � ������ curParamName
      double p1;
      getParamValue(p1, *curParamName);
      double p2;
      second.getParamValue(p2, *curParamName);
      
      // ���������� ��
      p1 = p1 * (1 - crossParam) + p2 * crossParam;
      p2 = p2 * (1 - crossParam) + p1 * crossParam;
      
      // ���������
      setParamValue(p1, *curParamName);
      second.setParamValue(p2, *curParamName);
    }
  }
}

