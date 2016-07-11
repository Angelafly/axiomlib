/****************************************************************************
*					ReducedRecognizerDTW.cxx
*
*	Description:	������� ������ ReducedRecognizerDTW - ����������
*	Author:			dk
*	History:		
*
****************************************************************************/
#include <boost/lexical_cast.hpp>

#include "ReducedRecognizerDTW.h"
#include "Logger.h"
#include "Defines.h"

using namespace AxiomLib;

#define QUESTION_MARK_LOCAL_INDEX -1

// ����������� ����������� - ������� �������� ���������� ���������� �� ���������
ReducedRecognizerDTW::ReducedRecognizerDTW (void) { 
    //precision = 0.0;
	stretch = 1.0;
}

// ����������� ���������� - �������� ���� ������������ �������� ������
ReducedRecognizerDTW::~ReducedRecognizerDTW (void) { 
	
}


/****************************************************************************
*                       ReducedRecognizerDTW::setParamsFromEnv
*
*       Description:    ���������������� ��������� ������� �� env
*       Parameters:     const Environment& env - �����
*       Returns:        0
*       Throws:         AxiomLibException - ���� ��� ������-�����������
*       Author:         wictor
*       History:
*
****************************************************************************/
int ReducedRecognizerDTW::setParamsFromEnv (const Environment& env) {
	// precision ��������������� � ��������
	ReducedRecognizerDistance::setParamsFromEnv(env);
	
	if (env.getDoubleParamValue(stretch, "Stretch") < 0) {
		throw AxiomLibException("ReducedRecognizerDTW::setParamsFromEnv : stretch is undefined.");
	}
	if (stretch < 1.0)
		throw AxiomLibException("ReducedRecognizerDTW::setParamsFromEnv : stretch has invalid value .");

	return 0;
}


// �������� �� ������� ���� const
int ReducedRecognizerDTW::run (const std::vector <int> &markUp, const std::vector<int> &etalon, std::vector <double> &result) {
	return countDTWDist (markUp, etalon, result);
}


/****************************************************************************
*                       ReducedRecognizerDTW::run
*
*       Description:    ������� ������ �������� ethalon � markUp � ������ ����������� ������ � result
*       Parameters:     markUp - ������ ��� ������� ��� ��������
*						etalon - �������� ���������� ���������� ���������, �.�. ������� ������� ����
*						result - ������ � ������������ �������������
*       Returns:        0
*       Throws:         -
*       Author:         dk, wictor (������ � result ������ ����������)
*       History:
*
****************************************************************************/
int ReducedRecognizerDTW::countDTWDist (const std::vector <int> &markup, const std::vector<int> &etalon, std::vector <double> &result) const {
	// ���������� ������ ��� ������ ������������ ��� ���� etalon
	int minLen, maxLen;
	maxLen = round (stretch * (double) etalon.size());
	minLen = round (((double) etalon.size()) / stretch);
	if (minLen < 1)
		minLen = 1;
	// ��������� ����� ����, ����� ������ ��� � ������� �� ����������
	int len = (int) markup.size();
	result.assign(len, -1);
	// �������������� ������� ��� ������ � ����� ����
	std::vector <std::vector <int> > matrixLen, matrixErr;
	int etLen = etalon.size();
	if (etLen < 1)
		return 0;
	int etLenMinusOne = etLen - 1;
	matrixLen.resize (etLen);
	matrixErr.resize (etLen);
	// ������� �������� ������������ ������� �� 0 �� maxLen � ���� markUp
	int upTo, j, l, curVal;
	double minDist, curDist;
	int rightDist, diagDist, downDist;
	upTo = min (len, maxLen);
	for (int i = minLen-1; i < upTo; i++) {
		// ���������� ������� ��� ����� � ������ � ����
		for (j = 0; j < etLen; j++) {
			matrixErr[j].resize(i+1);
			matrixLen[j].resize(i+1);
		}
		// ��������� �������� � ������� �������� �������
        if (markup[i] == etalon[etLenMinusOne] || etalon[etLenMinusOne] == QUESTION_MARK_LOCAL_INDEX)
			curVal = 0;
		else
			curVal = 1;
		matrixErr[etLenMinusOne][i] = curVal;
		matrixLen[etLenMinusOne][i] = 1;
		// ��������� �������� � ������� ��������� �������
		for (int k = etLen - 2; k >= 0; k--) {
            if (markup[i] != etalon[k] && etalon[k] != QUESTION_MARK_LOCAL_INDEX)
				curVal++;
			matrixErr[k][i] = curVal;
			matrixLen[k][i] = etLen - k;
		}
		curVal = matrixErr[etLenMinusOne][i];
		for (int k = i-1; k >= 0; k--) {
            if (markup[k] != etalon[etLenMinusOne] && etalon[etLenMinusOne] != QUESTION_MARK_LOCAL_INDEX)
				curVal++;
			matrixErr[etLenMinusOne][k] = curVal;
			matrixLen[etLenMinusOne][k] = i - k + 1;
		}
		// ��������� �������� � ��������� �������
		for (int k = 1; k <= min(etLenMinusOne, i); k++) {
			// ���� �� �����������
			j = etLenMinusOne - k;
			for (l = i - k; l >= 0; l--) {
                if (markup[l] == etalon[j] || etalon[j] == QUESTION_MARK_LOCAL_INDEX)
					curVal = 0;
				else
					curVal = 1;
				// ��������� �������� ��� ���� ��������� �����
				rightDist = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l+1] + 1);
				diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j][l+1] + 1);
				if (diagDist <= rightDist) {
					// ����� ������������ ������� ���������� � ������
					diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j+1][l+1] + 1);
					if (diagDist <= downDist) {
						matrixErr[j][l] = matrixErr[j+1][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				} else {
					// ����� ������ ������� ���������� � ������
					rightDist  = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j][l+1] + 1);
					if (rightDist <= downDist) {
						matrixErr[j][l] = matrixErr[j][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				}
				/*
				rightDist = ((double) (matrixErr[j][l+1] + curVal))   / ((double)(matrixLen[j][l+1] + 1));
				diagDist =  ((double) (matrixErr[j+1][l+1] + curVal)) / ((double)(matrixLen[j+1][l+1] + 1));
				downDist =  ((double) (matrixErr[j+1][l] + curVal))   / ((double)(matrixLen[j+1][l] + 1));
				if ((diagDist <= rightDist) && (diagDist <= downDist)) {
					matrixErr[j][l] = matrixErr[j+1][l+1] + curVal;
					matrixLen[j][l] = matrixLen[j+1][l+1] + 1;
				} else {
					if (rightDist <= downDist) {
						matrixErr[j][l] = matrixErr[j][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				}
				*/
			}
			// ���� �� ���������
			l = i - k;
			for (j = etLenMinusOne - k - 1; j >= 0; j--) {
                if (markup[l] == etalon[j] || etalon[j] == QUESTION_MARK_LOCAL_INDEX)
					curVal = 0;
				else
					curVal = 1;
				// ��������� �������� ��� ���� ��������� �����
				rightDist = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l+1] + 1);
				diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j][l+1] + 1);
				if (diagDist <= rightDist) {
					// ����� ������������ ������� ���������� � ������
					diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j+1][l+1] + 1);
					if (diagDist <= downDist) {
						matrixErr[j][l] = matrixErr[j+1][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				} else {
					// ����� ������ ������� ���������� � ������
					rightDist  = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j][l+1] + 1);
					if (rightDist <= downDist) {
						matrixErr[j][l] = matrixErr[j][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				}
			}
		}
		// �������� ���������� �������� ����� ��������� ������������
		minDist = ((double) matrixErr[0][0]) / ((double) matrixLen[0][0]);
		for (int k = 1; k <= (i - minLen + 1); k++) {
			curDist = ((double) matrixErr[0][k]) / ((double) matrixLen[0][k]);
			if (curDist < minDist)
				minDist = curDist;
		}
		result[i] = minDist;
		// ���������� ���� �� ����������
//		if (minDist <= precision) {
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
        if (markup[i] == etalon[etLenMinusOne] || etalon[etLenMinusOne] == QUESTION_MARK_LOCAL_INDEX)
			curVal = 0;
		else
			curVal = 1;
		matrixErr[etLenMinusOne][maxLenMinusOne] = curVal;
		//matrixLen[etLenMinusOne][maxLenMinusOne] = 1; // ��� ������������ ��� � ��� ���� �������
		// ��������� �������� � ������� ��������� �������
		for (int k = etLen - 2; k >= 0; k--) {
            if (markup[i] != etalon[k] && etalon[k] != QUESTION_MARK_LOCAL_INDEX)
				curVal++;
			matrixErr[k][maxLenMinusOne] = curVal;
			//matrixLen[k][maxLenMinusOne] = etLen - k; // ��� ������������ ��� � ��� ���� �������
		}
		curVal = matrixErr[etLenMinusOne][maxLenMinusOne];
		for (int k = maxLen - 2; k >= 0; k--) {
            if (markup[i - maxLenMinusOne + k] != etalon[etLenMinusOne] && etalon[etLenMinusOne] != QUESTION_MARK_LOCAL_INDEX)
				curVal++;
			matrixErr[etLenMinusOne][k] = curVal;
			//matrixLen[etLenMinusOne][k] = maxLen - k; // ��� ������������ ��� � ��� ���� �������
		}
		// ��������� �������� � ��������� �������
		for (int k = 1; k <= upTo; k++) {
			// ���� �� �����������
			j = etLenMinusOne - k;
			for (l = maxLenMinusOne - k; l >= 0; l--) {
                if (markup[i - maxLenMinusOne + l] == etalon[j] || etalon[j] == QUESTION_MARK_LOCAL_INDEX)
					curVal = 0;
				else
					curVal = 1;
				// ��������� �������� ��� ���� ��������� �����
				rightDist = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l+1] + 1);
				diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j][l+1] + 1);
				if (diagDist <= rightDist) {
					// ����� ������������ ������� ���������� � ������
					diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j+1][l+1] + 1);
					if (diagDist <= downDist) {
						matrixErr[j][l] = matrixErr[j+1][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				} else {
					// ����� ������ ������� ���������� � ������
					rightDist  = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j][l+1] + 1);
					if (rightDist <= downDist) {
						matrixErr[j][l] = matrixErr[j][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				}
			}
			// ���� �� ���������
			l = maxLenMinusOne - k;
			for (j = etLenMinusOne - k - 1; j >= 0; j--) {
                if (markup[i - k] == etalon[j] || etalon[j] == QUESTION_MARK_LOCAL_INDEX)
					curVal = 0;
				else
					curVal = 1;
				// ��������� �������� ��� ���� ��������� �����
				rightDist = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l+1] + 1);
				diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j][l+1] + 1);
				if (diagDist <= rightDist) {
					// ����� ������������ ������� ���������� � ������
					diagDist  = (matrixErr[j+1][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j+1][l+1] + 1);
					if (diagDist <= downDist) {
						matrixErr[j][l] = matrixErr[j+1][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				} else {
					// ����� ������ ������� ���������� � ������
					rightDist  = (matrixErr[j][l+1] + curVal)*(matrixLen[j+1][l] + 1);
					downDist = (matrixErr[j+1][l] + curVal)*(matrixLen[j][l+1] + 1);
					if (rightDist <= downDist) {
						matrixErr[j][l] = matrixErr[j][l+1] + curVal;
						matrixLen[j][l] = matrixLen[j][l+1] + 1;
					} else {
						matrixErr[j][l] = matrixErr[j+1][l] + curVal;
						matrixLen[j][l] = matrixLen[j+1][l] + 1;
					}
				}
			}
		}
		// �������� ���������� �������� ����� ��������� ������������
		minDist = ((double) matrixErr[0][0]) / ((double) matrixLen[0][0]);
		for (int k = 1; k <= (maxLen - minLen); k++) {
			curDist = ((double) matrixErr[0][k]) / ((double) matrixLen[0][k]);
			if (curDist < minDist)
				minDist = curDist;
		}
		result[i] = minDist;
		// ���������� ���� �� ����������
//		if (minDist <= precision) {
//			result[i] = 1;
//		}  else {
//			result[i] = 0;
//		}
	}	

	return 0;
}
