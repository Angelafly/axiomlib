/****************************************************************************
*
*			������� ������ IncreaseRightElemCondition
*
****************************************************************************/

#include <string>
#include "IncreaseRightElemCondition.h"
#include "DecreaseRightElemCondition.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001
#define double_default_Min_Level_Limit	0.001

/****************************************************************************
*			IncreaseRightElemCondition::IncreaseRightElemCondition
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
IncreaseRightElemCondition::IncreaseRightElemCondition (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	increaseMinLevel = double_default_Min_Level_Limit;
	
	// ������������� ����������� ��������� ������� ������������� �������
	increaseMaxLevel = 10;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	increaseLeft = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	increaseRight = 1;

	// ����������� ������� ��������� ��� ��������� increaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	increaseMinLevelLimit = increaseMinLevel;
	
	// ������������� ������� ��������� ��� ��������� increaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	increaseMaxLevelLimit = increaseMaxLevel;

	// ��� ��������� ���������� increaseMinLevel � increaseMaxLevel, ��� ������� ���������� ���������
	levelDelta = increaseMaxLevel - increaseMinLevel;
	
	// ������������ �������� ��������� increaseLeft, ������� ����������� ��� ������� ���������� ���������
	increaseLeftLimit = increaseLeft;
	
	// ������������ �������� ��������� increaseRight, ������� ����������� ��� ������� ���������� ���������
	increaseRightLimit = increaseRight;
}

/****************************************************************************
*			IncreaseRightElemCondition::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string IncreaseRightElemCondition::name (void) const {
	std::string toReturn("increase_right_elementary_condition");  
	return toReturn;
}


/****************************************************************************
*			IncreaseRightElemCondition::check
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
signed int IncreaseRightElemCondition::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (((int) k < increaseLeft) || ((int) (x.size() - k) <= increaseRight)) {
		toReturn = -1;
		//throw AxiomLibException("Error in IncreaseRightElemCondition::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	for (int i = 0; i < increaseRight; i++) {
		if (((x[k+i+1] - x[k]) < increaseMinLevel*(i+1)) || ((x[k+i+1] - x[k]) > increaseMaxLevel*(i+1))) {
			return 0;
		}
	}
	
	return toReturn;
}

/****************************************************************************
*			IncreaseRightElemCondition::operator<
*
*	Description:	��������� ���� IncreaseRightElemCondition �� ��������� (x < y).
*	Parameters:	IncreaseRightElemCondition& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� IncreaseRightElemCondition
*				-1 ���� > � second - ��� IncreaseRightElemCondition
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� IncreaseRightElemCondition)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double IncreaseRightElemCondition::operator< (const ElemCondition& second) const{
	// �������� �������� y ���� IncreaseRightElemCondition* �� second ���� el_axiom
	const IncreaseRightElemCondition* y = dynamic_cast<const IncreaseRightElemCondition*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� IncreaseRightElemCondition&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ((increaseMaxLevel < y->increaseMaxLevel + 0.01) && (increaseMinLevel > y->increaseMinLevel - 0.01))
			return 1;
		else if ((y->increaseMaxLevel < increaseMaxLevel + 0.01) && (y->increaseMinLevel > increaseMinLevel - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			IncreaseRightElemCondition::transmute
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
signed int IncreaseRightElemCondition::transmute (const double p) {
	signed int toReturn=0;
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
			increaseMaxLevel = increaseMaxLevel * (1 + change);
			toReturn = 1;
		}
		else {
			// ��������� ������ �������
			increaseMinLevel = increaseMinLevel * (1 - change);
			toReturn = 1;
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			increaseMaxLevel = increaseMaxLevel - (increaseMaxLevel  - increaseMinLevel) * change;
			toReturn = 1;
		}
		else {
			// ����������� ������ �������
			increaseMinLevel = increaseMinLevel + (increaseMaxLevel  - increaseMinLevel) * change;
			toReturn = 1;
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			IncreaseRightElemCondition::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int IncreaseRightElemCondition::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in IncreaseRightElemCondition::setParams : zero parameter map.");
	}
	int flag = 0;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("increaseMinLevel")) {
			// ok, �����, ������� value
			increaseMinLevel = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("increaseMaxLevel")) {
			// ok, �����, ������� value
			increaseMaxLevel = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("increaseLeft")) {
			// ok, �����, ������� value
			increaseLeft = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("increaseRight")) {
			// ok, �����, ������� value
			increaseRight = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in IncreaseRightElemCondition::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			IncreaseRightElemCondition::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int IncreaseRightElemCondition::getParams (std::map<std::string,std::string> &params) const {
	params.clear();
	char temp[64];
	sprintf( temp, "%f", increaseMinLevel);
	std::string str1 (temp);
	params["increaseMinLevel"] = str1;
	sprintf( temp, "%f", increaseMaxLevel);
	std::string str2 (temp);
	params["increaseMaxLevel"] = str2;
	sprintf( temp, "%d", increaseLeft);
	std::string str3 (temp);
	params["increaseLeft"] = str3;
	sprintf( temp, "%d", increaseRight);
	std::string str4 (temp);
	params["increaseRight"] = str4;
	return 0;
}


/****************************************************************************
*			IncreaseRightElemCondition::setParamValue
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
signed int IncreaseRightElemCondition::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "increaseMinLevel") {
		increaseMinLevel = param;
		return 0;
	}
	if (paramName == "increaseMaxLevel") {
		increaseMaxLevel = param;
		return 0;
	}
	if (paramName == "increaseLeft") {
		increaseLeft = round (param);
		return 0;
	}
	if (paramName == "increaseRight") {
		increaseRight = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			IncreaseRightElemCondition::getParamValue
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
signed int IncreaseRightElemCondition::getParamValue (double &param, const std::string &paramName) const {
	if (paramName == "increaseMinLevel") {
		param = increaseMinLevel;
		return 0;
	}
	if (paramName == "increaseMaxLevel") {
		param = increaseMaxLevel;
		return 0;
	}
	if (paramName == "increaseLeft") {
		param = (double) increaseLeft;
		return 0;
	}
	if (paramName == "increaseRight") {
		param = (double) increaseRight;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			IncreaseRightElemCondition::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int IncreaseRightElemCondition::getECParamNames (std::vector<std::string> &ecParamNames) const {
	ecParamNames.resize(4);
	ecParamNames[0] = "increaseMinLevel";
	ecParamNames[1] = "increaseMaxLevel";
	ecParamNames[2] = "increaseLeft";
	ecParamNames[3] = "increaseRight";
	return 0;
}


/****************************************************************************
*			IncreaseRightElemCondition::checkAndCompatibility
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
signed int IncreaseRightElemCondition::checkAndCompatibility (const ElemCondition& second) const{
	// �������� �������� y ���� IncreaseRightElemCondition* �� second ���� el_axiom
	const IncreaseRightElemCondition* y = dynamic_cast<const IncreaseRightElemCondition*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� IncreaseRightElemCondition&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((increaseMaxLevel >= y->increaseMaxLevel) && (increaseMinLevel <= y->increaseMaxLevel)) ||
		     ((increaseMaxLevel >= y->increaseMinLevel) && (increaseMinLevel <= y->increaseMinLevel)) )
			return 1;
		else
			return 0;
	}
	else {
		const DecreaseRightElemCondition* x = dynamic_cast<const DecreaseRightElemCondition*> (&second);
		if (x != NULL)
			return 0;
		return 1;
	}
	return -1;
}


/****************************************************************************
*					IncreaseRightElemCondition::setLimits
*
*	Description:	������� ������������� ������������ �������� ���������� increaseLeftLimit � increaseRightLimit
*	Parameters:		newLeftLimit - ����� �������� ��� ��������� increaseLeftLimit
*					newRightLimit - ����� �������� ��� ��������� increaseRightLimit
*	Returns:		0
*	Throws:			AxiomLibException - ���� ���� �� ���� �� ������� ���������� 
*					����� ������������� ��������. 
*	Author:			dk
*	History:
*
****************************************************************************/
signed int IncreaseRightElemCondition::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in IncreaseRightElemCondition::setLimits : parameters have a wrong values.");
	increaseLeftLimit = newLeftLimit;
	increaseRightLimit = newRightLimit;
	// ����� ������ �� ����������� ������� ������ ���� ����� 0
	increaseLeftLimit = 0;
	return 0; 
}


/****************************************************************************
*					IncreaseRightElemCondition::setLimits
*
*	Description:	������� ������������� �������� ���������� increaseMinLevelLimit, increaseMaxLevelLimit � levelDelta
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
signed int IncreaseRightElemCondition::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	double tmp;
	// ������� �������� ������������� �������
	if (row.size() < 2) 
		return -1;
	increaseMinLevelLimit = max (row[1] - row[0], double_default_Min_Level_Limit);
	increaseMaxLevelLimit = increaseMinLevelLimit;
	for (int k = increaseLeftLimit; k < (int) row.size() - increaseRightLimit; k++) {
		for (int i = 1; i <= increaseRightLimit; i++) {
			tmp = (row[k+i] - row[k]) / (double) i;
			increaseMinLevelLimit = min (tmp, increaseMinLevelLimit);
			increaseMinLevelLimit = max (double_default_Min_Level_Limit, increaseMinLevelLimit); // �������� - ����� �� ���� � ������������� ����
			increaseMaxLevelLimit = max (tmp, increaseMaxLevelLimit);
		}
	}
	// ����������� ����������� � ������������ ��������
	if (reserve < 0.001)
		return -1;
	if (increaseMinLevelLimit < 0)
		increaseMinLevelLimit *= reserve;
	else 
		increaseMinLevelLimit /= reserve;
	if (increaseMaxLevelLimit > 0)
		increaseMaxLevelLimit *= reserve;
	else 
		increaseMaxLevelLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in IncreaseRightElemCondition::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (increaseMaxLevelLimit - increaseMinLevelLimit) / (double) numOfLevels;
	return 0;
}


/****************************************************************************
*					IncreaseRightElemCondition::changeParams
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
signed int IncreaseRightElemCondition::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		increaseMinLevel = increaseMinLevelLimit;
		increaseMaxLevel = increaseMinLevelLimit;
		increaseLeft = 0;
		increaseRight = 0;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((increaseMaxLevel+levelDelta - increaseMaxLevelLimit) > eps) {
				if ((increaseMinLevel+levelDelta - increaseMaxLevelLimit) > eps) {
					if (increaseLeft >= increaseLeftLimit) {
						if (increaseRight >= increaseRightLimit)
							return -1;
						else {
							increaseRight++;
							increaseLeft = 0;
							increaseMinLevel = increaseMinLevelLimit;
							increaseMaxLevel = increaseMinLevelLimit;
						}
					} else {
						increaseLeft++;
						increaseMinLevel = increaseMinLevelLimit;
						increaseMaxLevel = increaseMinLevelLimit;
					}
				} else {
					increaseMinLevel += levelDelta;
					increaseMaxLevel = increaseMinLevel;
				}
			} else
				increaseMaxLevel += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((increaseMaxLevel-levelDelta - increaseMinLevel) < -eps) {
				if ((increaseMinLevel-levelDelta - increaseMinLevelLimit) < -eps) {
					if (increaseLeft == 0) {
						if (increaseRight == 0)
							return -1;
						else {
							increaseRight--;
							increaseLeft = increaseLeftLimit;
							increaseMinLevel = increaseMaxLevelLimit;
							increaseMaxLevel = increaseMaxLevelLimit;
						}
					} else {
						increaseLeft--;
						increaseMinLevel = increaseMaxLevelLimit;
						increaseMaxLevel = increaseMaxLevelLimit;
					}
				} else {
					increaseMinLevel -= levelDelta;
					increaseMaxLevel = increaseMaxLevelLimit;
				}
			} else
				increaseMaxLevel -= levelDelta;
		}
		return 0;
	}

	return -1;
}
