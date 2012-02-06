/****************************************************************************
*
*			������� ������ SecDerivativeIncreaseRight
*
****************************************************************************/

#include <string>
#include "SecDerivativeIncreaseRight.h"
#include "SecDerivativeDecreaseRight.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001
#define double_default_Min_Level_Limit	0.001

/****************************************************************************
*			SecDerivativeIncreaseRight::SecDerivativeIncreaseRight
*
*	Description:	��������� �����������, ��������� ����������
*				����������� � ������������ ����������
*	Parameters:	-
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
SecDerivativeIncreaseRight::SecDerivativeIncreaseRight (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	secDerivIncreaseMinLevel = double_default_Min_Level_Limit;
	
	// ������������� ����������� ��������� ������� ������������� �������
	secDerivIncreaseMaxLevel = 10;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	secDerivIncreaseLeft = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	secDerivIncreaseRight = 3;

	// ����������� ������� ��������� ��� ��������� secDerivIncreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	secDerivIncreaseMinLevelLimit = secDerivIncreaseMinLevel;
	
	// ������������� ������� ��������� ��� ��������� secDerivIncreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	secDerivIncreaseMaxLevelLimit = secDerivIncreaseMaxLevel;

	// ��� ��������� ���������� secDerivIncreaseMinLevel � secDerivIncreaseMaxLevel, ��� ������� ���������� ���������
	levelDelta = secDerivIncreaseMaxLevel - secDerivIncreaseMinLevel;
	
	// ������������ �������� ��������� secDerivIncreaseLeft, ������� ����������� ��� ������� ���������� ���������
	secDerivIncreaseLeftLimit = secDerivIncreaseLeft;
	
	// ������������ �������� ��������� secDerivIncreaseRight, ������� ����������� ��� ������� ���������� ���������
	secDerivIncreaseRightLimit = secDerivIncreaseRight;
}

/****************************************************************************
*			SecDerivativeIncreaseRight::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string SecDerivativeIncreaseRight::name (void) const {
	std::string toReturn("second_derivative_increase_right");
	return toReturn;
}


/****************************************************************************
*			SecDerivativeIncreaseRight::check
*
*	Description:	�������� ���������� ������� ����. ������� ��� ����� � ������� k (������� � ����) ���� x
*	Parameters:	unsigned long k		- ����� �����, ��� ������� ����������� ������������, ������� � 0
*				std::vector<double> x	- ��������������� ��������� ���
*	Returns:		1 ���� ������� �����������
*				0 ���� ������� �� �����������
*				XXX: -1 ���� ������� �� ���������, �������� ��-�� ����, ��� ����� k ������� ������ � ������� ����
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (((int) k < secDerivIncreaseLeft) || ((int) (x.size() - k) <= secDerivIncreaseRight) || (secDerivIncreaseRight < 2)) {
		toReturn = -1;
		//throw AxiomLibException("Error in SecDerivativeIncreaseRight::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	double prevRes = x[k+1] - x[k];
	double currRes;
	for (int i = 1; i < secDerivIncreaseRight; i++) {
		currRes = x[k+i+1] - x[k+i];
		if (((currRes - prevRes) < secDerivIncreaseMinLevel) || ((currRes - prevRes) > secDerivIncreaseMaxLevel)) {
			return 0;
		}
		prevRes = currRes;
	}
	
	return toReturn;
}

/****************************************************************************
*			SecDerivativeIncreaseRight::operator<
*
*	Description:	��������� ���� SecDerivativeIncreaseRight �� ��������� (x < y).
*	Parameters:	SecDerivativeIncreaseRight& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� SecDerivativeIncreaseRight
*				-1 ���� > � second - ��� SecDerivativeIncreaseRight
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� SecDerivativeIncreaseRight)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double SecDerivativeIncreaseRight::operator< (const ElemCondition& second) const{
	// �������� �������� y ���� SecDerivativeIncreaseRight* �� second ���� el_axiom
	const SecDerivativeIncreaseRight* y = dynamic_cast<const SecDerivativeIncreaseRight*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� SecDerivativeIncreaseRight&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ((secDerivIncreaseMaxLevel < y->secDerivIncreaseMaxLevel + 0.01) && (secDerivIncreaseMinLevel > y->secDerivIncreaseMinLevel - 0.01))
			return 1;
		else if ((y->secDerivIncreaseMaxLevel < secDerivIncreaseMaxLevel + 0.01) && (y->secDerivIncreaseMinLevel > secDerivIncreaseMinLevel - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			SecDerivativeIncreaseRight::transmute
*
*	Description:	�������, �������� �������� ���������� � �� ����� ��� p ���
*	Parameters:	double p - ������� ��������� ���������
*	Returns:		0 ���� ��� ok
*				XXX: -1 ���� p<0 ��� p>1 ��� �� �����-�� �������� �� ���������� ��������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::transmute (const double p) {
	// �������� ������ p
	if (p < 0 || p > 1) {
		return -1;
	}
	
	// �������� ������� ���������
	double change = (((double)rand())/ ((double) RAND_MAX)) * p ;
	
	if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
		// ����������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ����������� ������� �������
			secDerivIncreaseMaxLevel = secDerivIncreaseMaxLevel * (1 + change);
		}
		else {
			// ��������� ������ �������
			secDerivIncreaseMinLevel = secDerivIncreaseMinLevel * (1 - change);
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			secDerivIncreaseMaxLevel = secDerivIncreaseMaxLevel - (secDerivIncreaseMaxLevel  - secDerivIncreaseMinLevel) * change;
		}
		else {
			// ����������� ������ �������
			secDerivIncreaseMinLevel = secDerivIncreaseMinLevel + (secDerivIncreaseMaxLevel  - secDerivIncreaseMinLevel) * change;
		}
	}
	
	return 1;
}


/****************************************************************************
*			SecDerivativeIncreaseRight::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in SecDerivativeIncreaseRight::setParams : zero parameter map.");
	}
	int flag = 0;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("secDerivIncreaseMinLevel")) {
			// ok, �����, ������� value
			secDerivIncreaseMinLevel = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("secDerivIncreaseMaxLevel")) {
			// ok, �����, ������� value
			secDerivIncreaseMaxLevel = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("secDerivIncreaseLeft")) {
			// ok, �����, ������� value
			secDerivIncreaseLeft = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("secDerivIncreaseRight")) {
			// ok, �����, ������� value
			secDerivIncreaseRight = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in SecDerivativeIncreaseRight::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			SecDerivativeIncreaseRight::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::getParams (std::map<std::string,std::string> &params) const{
	params.clear();
	char temp[64];
	sprintf( temp, "%f", secDerivIncreaseMinLevel);
	std::string str1 (temp);
	params["secDerivIncreaseMinLevel"] = str1;
	sprintf( temp, "%f", secDerivIncreaseMaxLevel);
	std::string str2 (temp);
	params["secDerivIncreaseMaxLevel"] = str2;
	sprintf( temp, "%d", secDerivIncreaseLeft);
	std::string str3 (temp);
	params["secDerivIncreaseLeft"] = str3;
	sprintf( temp, "%d", secDerivIncreaseRight);
	std::string str4 (temp);
	params["secDerivIncreaseRight"] = str4;
	return 0;
}


/****************************************************************************
*			SecDerivativeIncreaseRight::setParamValue
*
*	Description:	������� ������������� �������� ������ ��������� ������������� �������, 
*				���� ����� �������� ������������ � ������ ������������ �������
*	Parameters:	param - ��������, �� ������� ����� �������� ��������
*				paramName - ��� ���������, �������� �������� ����� ��������
*	Returns:		0 - ���� ������� �������� �������� ���������
*				-1 - ���� �� ������� �������� �������� �������� (�������� 
*				       �������� � ����� ������ ��� � ������ ������������ �������)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "secDerivIncreaseMinLevel") {
		secDerivIncreaseMinLevel = param;
		return 0;
	}
	if (paramName == "secDerivIncreaseMaxLevel") {
		secDerivIncreaseMaxLevel = param;
		return 0;
	}
	if (paramName == "secDerivIncreaseLeft") {
		secDerivIncreaseLeft = round (param);
		return 0;
	}
	if (paramName == "secDerivIncreaseRight") {
		secDerivIncreaseRight = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			SecDerivativeIncreaseRight::getParamValue
*
*	Description:	������� ���������� �������� ������ ��������� ������������� �������, 
*				���� ����� �������� ������������ � ������ ������������ �������
*	Parameters:	param - ����������� �������� ���������
*				paramName - ����� � ������ ���������, �������� �������� ��������� ��������
*	Returns:		0 - ���� ������� ��������� ��������
*				-1 - ���� �� ������� ������� �������� (�������� ��� ������ ��� � ������ ������������ �������)
*				1 - ���� �������� ������������� ����
*				2 - ���� �������� �� ��������� � �� ������������ ����
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::getParamValue (double &param, const std::string &paramName) const {
	if (paramName == "secDerivIncreaseMinLevel") {
		param = secDerivIncreaseMinLevel;
		return 0;
	}
	if (paramName == "secDerivIncreaseLevel") {
		param = secDerivIncreaseMaxLevel;
		return 0;
	}
	if (paramName == "secDerivIncreaseLeft") {
		param = (double) secDerivIncreaseLeft;
		return 0;
	}
	if (paramName == "secDerivIncreaseRight") {
		param = (double) secDerivIncreaseRight;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			SecDerivativeIncreaseRight::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::getECParamNames (std::vector<std::string> &ecParamNames) const{
	ecParamNames.resize(4);
	ecParamNames[0] = "secDerivIncreaseMinLevel";
	ecParamNames[1] = "secDerivIncreaseMaxLevel";
	ecParamNames[2] = "secDerivIncreaseLeft";
	ecParamNames[3] = "secDerivIncreaseRight";
	return 0;
}


/****************************************************************************
*			SecDerivativeIncreaseRight::checkAndCompatibility
*
*	Description:	��������� ������������� ������������ ������� � ������� �� ���������� ������ "�"
*	Parameters:	const ElemCondition& second - �������, � ������� ��������� �� �������������
*	Returns:		1 ���� ����������
*				0 ���� �� ����������
*				-1 ���� undefined
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::checkAndCompatibility (const ElemCondition& second) const{
	// �������� �������� y ���� SecDerivativeIncreaseRight* �� second ���� el_axiom
	const SecDerivativeIncreaseRight* y = dynamic_cast<const SecDerivativeIncreaseRight*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� SecDerivativeIncreaseRight&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((secDerivIncreaseMaxLevel >= y->secDerivIncreaseMaxLevel) && (secDerivIncreaseMinLevel <= y->secDerivIncreaseMaxLevel)) ||
		     ((secDerivIncreaseMaxLevel >= y->secDerivIncreaseMinLevel) && (secDerivIncreaseMinLevel <= y->secDerivIncreaseMinLevel)) )
			return 1;
		else
			return 0;
	}
	else {
		const SecDerivativeDecreaseRight* x = dynamic_cast<const SecDerivativeDecreaseRight*> (&second);
		if (x != NULL) 
			return 0;
		return 1;
	}
	return -1;
}


/****************************************************************************
*					SecDerivativeIncreaseRight::setLimits
*
*	Description:	������� ������������� ������������ �������� ���������� secDerivIncreaseLeftLimit � secDerivIncreaseRightLimit
*	Parameters:		newLeftLimit - ����� �������� ��� ��������� secDerivIncreaseLeftLimit
*					newRightLimit - ����� �������� ��� ��������� secDerivIncreaseRightLimit
*	Returns:		0
*	Throws:			AxiomLibException - ���� ���� �� ���� �� ������� ���������� 
*					����� ������������� ��������. 
*	Author:			dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in SecDerivativeIncreaseRight::setLimits : parameters have a wrong values.");
	secDerivIncreaseLeftLimit = newLeftLimit;
	secDerivIncreaseRightLimit = newRightLimit;
	// ����� ������ ��� ������� ������� �� ����������� ������������� ������ 0.
	secDerivIncreaseLeftLimit = 0;
	return 0; 
}


/****************************************************************************
*					SecDerivativeIncreaseRight::setLimits
*
*	Description:	������� ������������� �������� ���������� secDerivIncreaseMinLevelLimit, secDerivIncreaseMaxLevelLimit � levelDelta
*	Parameters:		row - ������� ����������, �� �������� ����������� �������� ����������
*					reserve - �������� �������������� ������ �� ������� ������� ��������� �������� ��������� ����������
*					numOfLevels - ����� ����� �� ���������� ���������� ������������� ������� 
*						� ������������ �� �������������, �� �������� ������������ levelDelta
*	Returns:		0 - ���� ����� ��������� �����������
*					-1 - ���� �� ������� ���������� ����� ���������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	// ������� �������� ������������� �������
	if (row.size() < 3) 
		return -1;
	secDerivIncreaseMinLevelLimit = max (row[0] - 2*row[1] + row[2], double_default_Min_Level_Limit);
	secDerivIncreaseMaxLevelLimit = secDerivIncreaseMinLevelLimit;
	for (int k = 2; k < (int) (row.size() - 1); k++) {
		secDerivIncreaseMinLevelLimit = min (row[k-1] - 2*row[k] + row[k+1], secDerivIncreaseMinLevelLimit);
		secDerivIncreaseMinLevelLimit = max (double_default_Min_Level_Limit, secDerivIncreaseMinLevelLimit);
		secDerivIncreaseMaxLevelLimit = max (row[k-1] - 2*row[k] + row[k+1], secDerivIncreaseMaxLevelLimit);
	}
	// ����������� ����������� � ������������ ��������
	if (reserve < 0.001)
		return -1;
	if (secDerivIncreaseMinLevelLimit < 0)
		secDerivIncreaseMinLevelLimit *= reserve;
	else 
		secDerivIncreaseMinLevelLimit /= reserve;
	if (secDerivIncreaseMaxLevelLimit > 0)
		secDerivIncreaseMaxLevelLimit *= reserve;
	else 
		secDerivIncreaseMaxLevelLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in SecDerivativeIncreaseRight::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (secDerivIncreaseMaxLevelLimit - secDerivIncreaseMinLevelLimit) / (double) numOfLevels;
	return 0;
}


/****************************************************************************
*					SecDerivativeIncreaseRight::changeParams
*
*	Description:	������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
*	Parameters:		numOfSteps - ����� �����, �� ������� �������� ��������� ������������� �������
*	Returns:		0 - ���� ��������� ��������� ������
*					-1 - ���� ��������� �� ��������� ����� ���� ����������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int SecDerivativeIncreaseRight::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		secDerivIncreaseMinLevel = secDerivIncreaseMinLevelLimit;
		secDerivIncreaseMaxLevel = secDerivIncreaseMinLevelLimit;
		secDerivIncreaseLeft = 0;
		secDerivIncreaseRight = 0;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((secDerivIncreaseMaxLevel+levelDelta - secDerivIncreaseMaxLevelLimit) > eps) {
				if ((secDerivIncreaseMinLevel+levelDelta - secDerivIncreaseMaxLevelLimit) > eps) {
					if (secDerivIncreaseLeft >= secDerivIncreaseLeftLimit) {
						if (secDerivIncreaseRight >= secDerivIncreaseRightLimit)
							return -1;
						else {
							secDerivIncreaseRight++;
							secDerivIncreaseLeft = 0;
							secDerivIncreaseMinLevel = secDerivIncreaseMinLevelLimit;
							secDerivIncreaseMaxLevel = secDerivIncreaseMinLevelLimit;
						}
					} else {
						secDerivIncreaseLeft++;
						secDerivIncreaseMinLevel = secDerivIncreaseMinLevelLimit;
						secDerivIncreaseMaxLevel = secDerivIncreaseMinLevelLimit;
					}
				} else {
					secDerivIncreaseMinLevel += levelDelta;
					secDerivIncreaseMaxLevel = secDerivIncreaseMinLevel;
				}
			} else
				secDerivIncreaseMaxLevel += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((secDerivIncreaseMaxLevel-levelDelta - secDerivIncreaseMinLevel) < -eps) {
				if ((secDerivIncreaseMinLevel-levelDelta - secDerivIncreaseMinLevelLimit) < -eps) {
					if (secDerivIncreaseLeft == 0) {
						if (secDerivIncreaseRight == 0)
							return -1;
						else {
							secDerivIncreaseRight--;
							secDerivIncreaseLeft = secDerivIncreaseLeftLimit;
							secDerivIncreaseMinLevel = secDerivIncreaseMaxLevelLimit;
							secDerivIncreaseMaxLevel = secDerivIncreaseMaxLevelLimit;
						}
					} else {
						secDerivIncreaseLeft--;
						secDerivIncreaseMinLevel = secDerivIncreaseMaxLevelLimit;
						secDerivIncreaseMaxLevel = secDerivIncreaseMaxLevelLimit;
					}
				} else {
					secDerivIncreaseMinLevel -= levelDelta;
					secDerivIncreaseMaxLevel = secDerivIncreaseMaxLevelLimit;
				}
			} else
				secDerivIncreaseMaxLevel -= levelDelta;
		}
		return 0;
	}

	return -1;
}
