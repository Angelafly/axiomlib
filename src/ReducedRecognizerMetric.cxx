/****************************************************************************
*					ReducedRecognizerMetric.cxx
*
*	Description:	������� ������ ReducedRecognizerMetric - ����������
*	Author:			dk
*	History:		
*
****************************************************************************/
#include "ReducedRecognizerMetric.h"
#include "Defines.h"

using namespace AxiomLib;


// ����������� ����������� - ������� �������� ���������� ���������� �� ���������
ReducedRecognizerMetric::ReducedRecognizerMetric (void) { 
	
}


// ����������� ���������� - �������� ���� ������������ �������� ������
ReducedRecognizerMetric::~ReducedRecognizerMetric (void) { 
	
}


/****************************************************************************
*                       ReducedRecognizerMetric::setParamsFromEnv
*
*       Description:    ���������������� ��������� ������� �� env
*       Parameters:     const Environment& env - ���������
*       Returns:        0
*       Throws:         AxiomLibException - ���� ��� ������-�����������
*       Author:         gevor
*       History:
*
****************************************************************************/
int ReducedRecognizerMetric::setParamsFromEnv (const Environment& env) {
	// ��������� ��������� �������� �������������� �� �������
	ReducedRecognizerDistance::setParamsFromEnv(env);
	
	return 0;
}


/****************************************************************************
*                       ReducedRecognizerMetric::run
*
*       Description:    ������� �������� ������ �������� ethalon � markUp � ������ ����������� ������ � result
*       Parameters:     markUp - ������ ��� ������� ��� ��������
*						etalon - �������� ���������� ���������� ���������, �.�. ������� ������� ����
*						result - ������ � ������������ �������������
*       Returns:        0
*       Throws:         -
*       Author:         dk
*       History:
*
****************************************************************************/
int ReducedRecognizerMetric::run (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) {
	return countMetricDist(markUp, etalon, result);
}

/****************************************************************************
*                       ReducedRecognizerMetric::countMetricDist
*
*       Description:    ������� ��������� ����� �������� ethalon � markUp � ������ ����������� ������ � result
*       Parameters:     markUp - ������ ��� ������� ��� ��������
*						etalon - �������� ���������� ���������� ���������, �.�. ������� ������� ����
*						result - ������ � ������������ �������������
*       Returns:        0
*       Throws:         -
*       Author:         dk
*       History:
*
****************************************************************************/
int ReducedRecognizerMetric::countMetricDist (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) const {
	// ��������� ����� ����, ����� ������ ��� � ������� �� ����������
	int len = (int) markUp.size();
	result.resize(len, 0);
	int etLen = etalon.size();
	if (etLen < 1)
		return 0;
	int etLenMinusOne = etLen - 1;
	double curPrecisionF = precision() * (double) etLen;
	//int curPrecision = round (curPrecisionF);
	// ���������� ������ ��� ������ ������������ ��� ���� etalon
	int minLen, maxLen;
	maxLen = round ((1.0 + precision()) * ((double) etLen));
	minLen = round ((1.0 - precision()) * ((double) etLen));
	if (minLen < 1)
		minLen = 1;
	// �������������� ������� ��� �������� ������������ ����������
	std::vector <std::vector <int> > matrix;
	matrix.resize (etLen);
	// ������� �������� ������������ ������� �� 0 �� maxLen � ���� markUp
	int upTo, j, l, curVal;
	int rightDist, diagDist, downDist, minDist;
	upTo = min (len, maxLen);
	for (int i = minLen-1; i < upTo; i++) {
		// �������� ������ ������� � ������������ � ������� �������������� �������� i
		for (j = 0; j < etLen; j++) {
			matrix[j].resize(i+1);
		}
		// ��������� �������� � ������� �������� �������
		if (markUp[i] == etalon[etLenMinusOne]) {
			matrix[etLenMinusOne][i] = 0;
			curVal = 0;
		} else {
			matrix[etLenMinusOne][i] = 1;
			curVal = 1;
		}
		// ��������� �������� � ������� ��������� �������
		for (int k = etLen - 2; k >= 0; k--) {
			curVal++;
			matrix[k][i] = curVal;
		}
		curVal = matrix[etLenMinusOne][i];
		for (int k = i-1; k >= 0; k--) {
			curVal++;
			matrix[etLenMinusOne][k] = curVal;
		}
		// ��������� �������� � ��������� �������
		for (int k = 1; k <= min(etLenMinusOne, i); k++) {
			// ���� �� �����������
			j = etLenMinusOne - k;
			for (l = i - k; l >= 0; l--) {
				// ��������� �������� ��� ���� ��������� �����
				rightDist = matrix[j][l+1] + 1;
				diagDist =  matrix[j+1][l+1];
				if (markUp[l] != etalon[j])
					diagDist++;
				downDist =  matrix[j+1][l] + 1;
				if ((diagDist <= rightDist) && (diagDist <= downDist)) {
					matrix[j][l] = diagDist;
				} else {
					if (rightDist <= downDist) {
						matrix[j][l] = rightDist;
					} else {
						matrix[j][l] = downDist;
					}
				}
			}
			// ���� �� ���������
			l = i - k;
			for (j = etLenMinusOne - k - 1; j >= 0; j--) {
				// ��������� �������� ��� ���� ��������� �����
				rightDist = matrix[j][l+1] + 1;
				diagDist =  matrix[j+1][l+1];
				if (markUp[l] != etalon[j])
					diagDist++;
				downDist =  matrix[j+1][l] + 1;
				if ((diagDist <= rightDist) && (diagDist <= downDist)) {
					matrix[j][l] = diagDist;
				} else {
					if (rightDist <= downDist) {
						matrix[j][l] = rightDist;
					} else {
						matrix[j][l] = downDist;
					}
				}
			}
		}
		// �������� ���������� �������� ����� ��������� ������������
		minDist = matrix[0][0];
		for (int k = 1; k <= (i - minLen + 1); k++) {
			if (matrix[0][k] < minDist)
				minDist = matrix[0][k];
		}
		result[i] = (double)minDist / (double) etLen;
//		// ���������� ���� �� ����������
//		if (minDist <= curPrecision) {
//			result[i] = 1;
//		} else {
//			result[i] = 0;
//		}
	}
	// ���������� ���� �� ���� - ������ ������ ��� ����� �������� ������ maxLen
	int maxLenMinusOne = maxLen - 1;
	upTo = min(etLenMinusOne, maxLenMinusOne);
	for (int i = maxLen; i < len; i++) {
		// ��������� �������� � ������� �������� �������
		if (markUp[i] == etalon[etLenMinusOne]) {
			matrix[etLenMinusOne][maxLenMinusOne] = 0;
			curVal = 0;
		} else {
			matrix[etLenMinusOne][maxLenMinusOne] = 1;
			curVal = 1;
		}
		// ��������� �������� � ������� ��������� �������
		for (int k = etLen - 2; k >= 0; k--) {
			curVal++;
			matrix[k][maxLenMinusOne] = curVal;
		}
		curVal = matrix[etLenMinusOne][maxLenMinusOne];
		for (int k = maxLen - 2; k >= 0; k--) {
			curVal++;
			matrix[etLenMinusOne][k] = curVal;
		}
		// ��������� �������� � ��������� �������
		for (int k = 1; k <= upTo; k++) {
			// ���� �� �����������
			j = etLenMinusOne - k;
			for (l = maxLenMinusOne - k; l >= 0; l--) {
				// ��������� �������� ��� ���� ��������� �����
				rightDist = matrix[j][l+1] + 1;
				diagDist =  matrix[j+1][l+1];
				if (markUp[i - maxLenMinusOne + l] != etalon[j])
					diagDist++;
				downDist =  matrix[j+1][l] + 1;
				if ((diagDist <= rightDist) && (diagDist <= downDist)) {
					matrix[j][l] = diagDist;
				} else {
					if (rightDist <= downDist) {
						matrix[j][l] = rightDist;
					} else {
						matrix[j][l] = downDist;
					}
				}
			}
			// ���� �� ���������
			l = maxLenMinusOne - k;
			for (j = etLenMinusOne - k - 1; j >= 0; j--) {
				// ��������� �������� ��� ���� ��������� �����
				rightDist = matrix[j][l+1] + 1;
				diagDist =  matrix[j+1][l+1];
				if (markUp[i - k] != etalon[j])
					diagDist++;
				downDist =  matrix[j+1][l] + 1;
				if ((diagDist <= rightDist) && (diagDist <= downDist)) {
					matrix[j][l] = diagDist;
				} else {
					if (rightDist <= downDist) {
						matrix[j][l] = rightDist;
					} else {
						matrix[j][l] = downDist;
					}
				}
			}
		}
		// �������� ���������� �������� ����� ��������� ������������
		minDist = matrix[0][0];
		for (int k = 1; k <= (maxLen - minLen); k++) {
			if (matrix[0][k] < minDist)
				minDist = matrix[0][k];
		}
		result[i] = (double)minDist / (double)etLen;
		// ���������� ���� �� ����������
//		if (minDist <= curPrecision) {
//			result[i] = 1;
//		} else {
//			result[i] = 0;
//		}
	}

	return 0;
}
