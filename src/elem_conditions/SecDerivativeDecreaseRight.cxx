/****************************************************************************
*
*			������� ������ SecDerivativeDecreaseRight
*
****************************************************************************/

#include <string>
#include "SecDerivativeDecreaseRight.h"
#include "SecDerivativeIncreaseRight.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001
#define double_default_Min_Level_Limit	0.001

/****************************************************************************
*			SecDerivativeDecreaseRight::SecDerivativeDecreaseRight
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
SecDerivativeDecreaseRight::SecDerivativeDecreaseRight (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	secDerivDecreaseMinLevel = double_default_Min_Level_Limit;
	
	// ������������� ����������� ��������� ������� ������������� �������
	secDerivDecreaseMaxLevel = 10;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	secDerivDecreaseLeft = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	secDerivDecreaseRight = 3;

	// ����������� ������� ��������� ��� ��������� secDerivDecreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	secDerivDecreaseMinLevelLimit = secDerivDecreaseMinLevel;
	
	// ������������� ������� ��������� ��� ��������� secDerivDecreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	secDerivDecreaseMaxLevelLimit = secDerivDecreaseMaxLevel;

	// ��� ��������� ���������� secDerivDecreaseMinLevel � secDerivDecreaseMaxLevel, ��� ������� ���������� ���������
	levelDelta = secDerivDecreaseMaxLevel - secDerivDecreaseMinLevel;
	
	// ������������ �������� ��������� secDerivDecreaseLeft, ������� ����������� ��� ������� ���������� ���������
	secDerivDecreaseLeftLimit = secDerivDecreaseLeft;
	
	// ������������ �������� ��������� secDerivDecreaseRight, ������� ����������� ��� ������� ���������� ���������
	secDerivDecreaseRightLimit = secDerivDecreaseRight;
}

/****************************************************************************
*			SecDerivativeDecreaseRight::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string SecDerivativeDecreaseRight::name (void) const {
	std::string toReturn("second_derivative_decrease_right");
	return toReturn;
}


/****************************************************************************
*			SecDerivativeDecreaseRight::check
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
signed int SecDerivativeDecreaseRight::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (((int) k < secDerivDecreaseLeft) || ((int) (x.size() - k) <= secDerivDecreaseRight) || (secDerivDecreaseRight < 2)) {
		toReturn = -1;
		//throw AxiomLibException("Error in SecDerivativeDecreaseRight::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	double prevRes = x[k+1] - x[k];
	double currRes;
	for (int i = 1; i < secDerivDecreaseRight; i++) {
		currRes = x[k+i+1] - x[k+i];
		if (((prevRes - currRes) < secDerivDecreaseMinLevel) || ((prevRes - currRes) > secDerivDecreaseMaxLevel)) {
			return 0;
		}
		prevRes = currRes;
	}
	
	return toReturn;
}

/****************************************************************************
*			SecDerivativeDecreaseRight::operator<
*
*	Description:	��������� ���� SecDerivativeDecreaseRight �� ��������� (x < y).
*	Parameters:	SecDerivativeDecreaseRight& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� SecDerivativeDecreaseRight
*				-1 ���� > � second - ��� SecDerivativeDecreaseRight
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� SecDerivativeDecreaseRight)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double SecDerivativeDecreaseRight::operator< (const ElemCondition& second) const{
	// �������� �������� y ���� SecDerivativeDecreaseRight* �� second ���� el_axiom
	const SecDerivativeDecreaseRight* y = dynamic_cast<const SecDerivativeDecreaseRight*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� SecDerivativeDecreaseRight&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ((secDerivDecreaseMaxLevel < y->secDerivDecreaseMaxLevel + 0.01) && (secDerivDecreaseMinLevel > y->secDerivDecreaseMinLevel - 0.01))
			return 1;
		else if ((y->secDerivDecreaseMaxLevel < secDerivDecreaseMaxLevel + 0.01) && (y->secDerivDecreaseMinLevel > secDerivDecreaseMinLevel - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			SecDerivativeDecreaseRight::transmute
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
signed int SecDerivativeDecreaseRight::transmute (const double p) {
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
			secDerivDecreaseMaxLevel = secDerivDecreaseMaxLevel * (1 + change);
		}
		else {
			// ��������� ������ �������
			secDerivDecreaseMinLevel = secDerivDecreaseMinLevel * (1 - change);
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			secDerivDecreaseMaxLevel = secDerivDecreaseMaxLevel - (secDerivDecreaseMaxLevel  - secDerivDecreaseMinLevel) * change;
		}
		else {
			// ����������� ������ �������
			secDerivDecreaseMinLevel = secDerivDecreaseMinLevel + (secDerivDecreaseMaxLevel  - secDerivDecreaseMinLevel) * change;
		}
	}
	
	return 1;
}


/****************************************************************************
*			SecDerivativeDecreaseRight::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeDecreaseRight::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in SecDerivativeDecreaseRight::setParams : zero parameter map.");
	}
	int flag = 0;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("secDerivDecreaseMinLevel")) {
			// ok, �����, ������� value
			secDerivDecreaseMinLevel = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("secDerivDecreaseMaxLevel")) {
			// ok, �����, ������� value
			secDerivDecreaseMaxLevel = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("secDerivDecreaseLeft")) {
			// ok, �����, ������� value
			secDerivDecreaseLeft = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("secDerivDecreaseRight")) {
			// ok, �����, ������� value
			secDerivDecreaseRight = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in SecDerivativeDecreaseRight::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			SecDerivativeDecreaseRight::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeDecreaseRight::getParams (std::map<std::string,std::string> &params) const{
	params.clear();
	char temp[64];
	sprintf( temp, "%f", secDerivDecreaseMinLevel);
	std::string str1 (temp);
	params["secDerivDecreaseMinLevel"] = str1;
	sprintf( temp, "%f", secDerivDecreaseMaxLevel);
	std::string str2 (temp);
	params["secDerivDecreaseMaxLevel"] = str2;
	sprintf( temp, "%d", secDerivDecreaseLeft);
	std::string str3 (temp);
	params["secDerivDecreaseLeft"] = str3;
	sprintf( temp, "%d", secDerivDecreaseRight);
	std::string str4 (temp);
	params["secDerivDecreaseRight"] = str4;
	return 0;
}


/****************************************************************************
*			SecDerivativeDecreaseRight::setParamValue
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
signed int SecDerivativeDecreaseRight::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "secDerivDecreaseMinLevel") {
		secDerivDecreaseMinLevel = param;
		return 0;
	}
	if (paramName == "secDerivDecreaseMaxLevel") {
		secDerivDecreaseMaxLevel = param;
		return 0;
	}
	if (paramName == "secDerivDecreaseLeft") {
		secDerivDecreaseLeft = round (param);
		return 0;
	}
	if (paramName == "secDerivDecreaseRight") {
		secDerivDecreaseRight = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			SecDerivativeDecreaseRight::getParamValue
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
signed int SecDerivativeDecreaseRight::getParamValue (double &param, const std::string &paramName) const{
	if (paramName == "secDerivDecreaseMinLevel") {
		param = secDerivDecreaseMinLevel;
		return 0;
	}
	if (paramName == "secDerivDecreaseLevel") {
		param = secDerivDecreaseMaxLevel;
		return 0;
	}
	if (paramName == "secDerivDecreaseLeft") {
		param = (double) secDerivDecreaseLeft;
		return 0;
	}
	if (paramName == "secDerivDecreaseRight") {
		param = (double) secDerivDecreaseRight;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			SecDerivativeDecreaseRight::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int SecDerivativeDecreaseRight::getECParamNames (std::vector<std::string> &ecParamNames) const{
	ecParamNames.resize(4);
	ecParamNames[0] = "secDerivDecreaseMinLevel";
	ecParamNames[1] = "secDerivDecreaseMaxLevel";
	ecParamNames[2] = "secDerivDecreaseLeft";
	ecParamNames[3] = "secDerivDecreaseRight";
	return 0;
}


/****************************************************************************
*			SecDerivativeDecreaseRight::checkAndCompatibility
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
signed int SecDerivativeDecreaseRight::checkAndCompatibility (const ElemCondition& second) const{
	// �������� �������� y ���� SecDerivativeDecreaseRight* �� second ���� el_axiom
	const SecDerivativeDecreaseRight* y = dynamic_cast<const SecDerivativeDecreaseRight*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� SecDerivativeDecreaseRight&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((secDerivDecreaseMaxLevel >= y->secDerivDecreaseMaxLevel) && (secDerivDecreaseMinLevel <= y->secDerivDecreaseMaxLevel)) ||
		     ((secDerivDecreaseMaxLevel >= y->secDerivDecreaseMinLevel) && (secDerivDecreaseMinLevel <= y->secDerivDecreaseMinLevel)) )
			return 1;
		else
			return 0;
	}
	else {
		const SecDerivativeIncreaseRight* x = dynamic_cast<const SecDerivativeIncreaseRight*> (&second);
		if (x != NULL)
			return 0;
		return 1;
	}
	return -1;
}


/****************************************************************************
*					SecDerivativeDecreaseRight::setLimits
*
*	Description:	������� ������������� ������������ �������� ���������� secDerivDecreaseLeftLimit � secDerivDecreaseRightLimit
*	Parameters:		newLeftLimit - ����� �������� ��� ��������� secDerivDecreaseLeftLimit
*					newRightLimit - ����� �������� ��� ��������� secDerivDecreaseRightLimit
*	Returns:		0
*	Throws:			AxiomLibException - ���� ���� �� ���� �� ������� ���������� 
*					����� ������������� ��������. 
*	Author:			dk
*	History:
*
****************************************************************************/
signed int SecDerivativeDecreaseRight::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in SecDerivativeDecreaseRight::setLimits : parameters have a wrong values.");
	secDerivDecreaseLeftLimit = newLeftLimit;
	secDerivDecreaseRightLimit = newRightLimit;
	// ����� ������ �� ����������� ������� ������ ���� ����� 0
	secDerivDecreaseLeftLimit = 0;
	return 0; 
}


/****************************************************************************
*					SecDerivativeDecreaseRight::setLimits
*
*	Description:	������� ������������� �������� ���������� secDerivDecreaseMinLevelLimit, secDerivDecreaseMaxLevelLimit � levelDelta
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
signed int SecDerivativeDecreaseRight::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	// ������� �������� ������������� �������
	if (row.size() < 3) 
		return -1;
	secDerivDecreaseMinLevelLimit = max (2*row[1] - row[0] - row[2], double_default_Min_Level_Limit);
	secDerivDecreaseMaxLevelLimit = secDerivDecreaseMinLevelLimit;
	for (int k = 2; k < (int) (row.size() - 1); k++) {
		secDerivDecreaseMinLevelLimit = min (2*row[k] - row[k-1] - row[k+1], secDerivDecreaseMinLevelLimit);
		secDerivDecreaseMinLevelLimit = max (double_default_Min_Level_Limit, secDerivDecreaseMinLevelLimit);
		secDerivDecreaseMaxLevelLimit = max (2*row[k] - row[k-1] - row[k+1], secDerivDecreaseMaxLevelLimit);
	}
	// ����������� ����������� � ������������ ��������
	if (reserve < 0.001)
		return -1;
	if (secDerivDecreaseMinLevelLimit < 0)
		secDerivDecreaseMinLevelLimit *= reserve;
	else 
		secDerivDecreaseMinLevelLimit /= reserve;
	if (secDerivDecreaseMaxLevelLimit > 0)
		secDerivDecreaseMaxLevelLimit *= reserve;
	else 
		secDerivDecreaseMaxLevelLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in SecDerivativeDecreaseRight::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (secDerivDecreaseMaxLevelLimit - secDerivDecreaseMinLevelLimit) / (double) numOfLevels;
	return 0;
}


/****************************************************************************
*					SecDerivativeDecreaseRight::changeParams
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
signed int SecDerivativeDecreaseRight::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		secDerivDecreaseMinLevel = secDerivDecreaseMinLevelLimit;
		secDerivDecreaseMaxLevel = secDerivDecreaseMinLevelLimit;
		secDerivDecreaseLeft = 0;
		secDerivDecreaseRight = 0;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((secDerivDecreaseMaxLevel+levelDelta - secDerivDecreaseMaxLevelLimit) > eps) {
				if ((secDerivDecreaseMinLevel+levelDelta - secDerivDecreaseMaxLevelLimit) > eps) {
					if (secDerivDecreaseLeft >= secDerivDecreaseLeftLimit) {
						if (secDerivDecreaseRight >= secDerivDecreaseRightLimit)
							return -1;
						else {
							secDerivDecreaseRight++;
							secDerivDecreaseLeft = 0;
							secDerivDecreaseMinLevel = secDerivDecreaseMinLevelLimit;
							secDerivDecreaseMaxLevel = secDerivDecreaseMinLevelLimit;
						}
					} else {
						secDerivDecreaseLeft++;
						secDerivDecreaseMinLevel = secDerivDecreaseMinLevelLimit;
						secDerivDecreaseMaxLevel = secDerivDecreaseMinLevelLimit;
					}
				} else {
					secDerivDecreaseMinLevel += levelDelta;
					secDerivDecreaseMaxLevel = secDerivDecreaseMinLevel;
				}
			} else
				secDerivDecreaseMaxLevel += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((secDerivDecreaseMaxLevel-levelDelta - secDerivDecreaseMinLevel) < -eps) {
				if ((secDerivDecreaseMinLevel-levelDelta - secDerivDecreaseMinLevelLimit) < -eps) {
					if (secDerivDecreaseLeft == 0) {
						if (secDerivDecreaseRight == 0)
							return -1;
						else {
							secDerivDecreaseRight--;
							secDerivDecreaseLeft = secDerivDecreaseLeftLimit;
							secDerivDecreaseMinLevel = secDerivDecreaseMaxLevelLimit;
							secDerivDecreaseMaxLevel = secDerivDecreaseMaxLevelLimit;
						}
					} else {
						secDerivDecreaseLeft--;
						secDerivDecreaseMinLevel = secDerivDecreaseMaxLevelLimit;
						secDerivDecreaseMaxLevel = secDerivDecreaseMaxLevelLimit;
					}
				} else {
					secDerivDecreaseMinLevel -= levelDelta;
					secDerivDecreaseMaxLevel = secDerivDecreaseMaxLevelLimit;
				}
			} else
				secDerivDecreaseMaxLevel -= levelDelta;
		}
		return 0;
	}

	return -1;
}
