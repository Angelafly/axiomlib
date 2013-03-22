/****************************************************************************
*
*			������� ������ ElemCondition
*
****************************************************************************/
#include "ElemCondition.h"
#include "../AxiomLibException.h"
#include "../FuzzyDataSet.h"

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

int ElemCondition::setLimits(FuzzyDataSet *fuzzyDataSet, bool oneVsAllMode, const double reserve, const unsigned int numOfLevels)
{
	// ������� ����������� ��� �������� ���
	std::vector<double> teachRow;
	int numOfMultiTS;
	std::vector <int> numOfTS;
	if(!oneVsAllMode) {
		fuzzyDataSet->getNormalClassSize (numOfMultiTS, numOfTS);
		if ((numOfTS.size() != numOfMultiTS) || (numOfMultiTS < 1))
			throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::selectElemCond : not enough data for the algorithm.");
		for (int i = 0; i < numOfMultiTS; i++) {
			if (numOfTS[i] > dimension) {
				fuzzyDataSet->getNormalTSFromClass (teachRow, i, dimension);
				if (teachRow.size() > 0)
					break;
			}
		}
	} else {
		if(fuzzyDataSet->getClassCount() <= 0) {
			throw AxiomLibException("FuzzyMultiDataExtAlgorithm::selectElemCond() : abnormal classes not found");
		}

		fuzzyDataSet->getTSByIndexFromClass(teachRow, 0, 0, dimension);
	}
	if (teachRow.size() < 1)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::selectElemCond : incorrect normal data set.");

	return setLimits(teachRow, reserve, numOfLevels);
}

