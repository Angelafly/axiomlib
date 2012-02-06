/****************************************************************************
*
*			������� ������ DecreaseRightElemCondition
*
****************************************************************************/

#include <string>
#include "DecreaseRightElemCondition.h"
#include "IncreaseRightElemCondition.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001
#define double_default_Min_Level_Limit	0.001

/****************************************************************************
*			DecreaseRightElemCondition::DecreaseRightElemCondition
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
DecreaseRightElemCondition::DecreaseRightElemCondition (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	decreaseMinLevel = double_default_Min_Level_Limit;
	
	// ������������� ����������� ��������� ������� ������������� �������
	decreaseMaxLevel = 10;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	decreaseLeft = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	decreaseRight = 1;

	// ����������� ������� ��������� ��� ��������� decreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	decreaseMinLevelLimit = decreaseMinLevel;
	
	// ������������� ������� ��������� ��� ��������� decreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	decreaseMaxLevelLimit = decreaseMaxLevel;

	// ��� ��������� ���������� decreaseMinLevel � decreaseMaxLevel, ��� ������� ���������� ���������
	levelDelta = decreaseMaxLevel - decreaseMinLevel;
	
	// ������������ �������� ��������� decreaseLeft, ������� ����������� ��� ������� ���������� ���������
	decreaseLeftLimit = decreaseLeft;
	
	// ������������ �������� ��������� decreaseRight, ������� ����������� ��� ������� ���������� ���������
	decreaseRightLimit = decreaseRight;
}

/****************************************************************************
*			DecreaseRightElemCondition::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string DecreaseRightElemCondition::name (void) const {
	std::string toReturn("decrease_right_elementary_condition");  
	return toReturn;
}


/****************************************************************************
*			DecreaseRightElemCondition::check
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
signed int DecreaseRightElemCondition::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (((int) k < decreaseLeft) || ((int)(x.size() - k) <= decreaseRight)) {
		toReturn = -1;
		//throw AxiomLibException("Error in DecreaseRightElemCondition::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	for (int i = 0; i < decreaseRight; i++) {
		if (((x[k] - x[k+i+1]) < decreaseMinLevel*(i+1)) || ((x[k] - x[k+i+1]) > decreaseMaxLevel*(i+1))) {
			return 0;
		}
	}
	
	return toReturn;
}

/****************************************************************************
*			DecreaseRightElemCondition::operator<
*
*	Description:	��������� ���� DecreaseRightElemCondition �� ��������� (x < y).
*	Parameters:	DecreaseRightElemCondition& second - ������ ����� ���������
*	Returns:		1 ����  <  � second - ��� DecreaseRightElemCondition
*				-1 ����  >  � second - ��� DecreaseRightElemCondition
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� DecreaseRightElemCondition)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double DecreaseRightElemCondition::operator< (const ElemCondition& second) const{
	// �������� �������� y ���� DecreaseRightElemCondition* �� second ���� el_axiom
	const DecreaseRightElemCondition* y = dynamic_cast<const DecreaseRightElemCondition*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� DecreaseRightElemCondition&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ((decreaseMaxLevel < y->decreaseMaxLevel + 0.01) && (decreaseMinLevel > y->decreaseMinLevel - 0.01))
			return 1;
		else if ((y->decreaseMaxLevel < decreaseMaxLevel + 0.01) && (y->decreaseMinLevel > decreaseMinLevel - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			DecreaseRightElemCondition::transmute
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
signed int DecreaseRightElemCondition::transmute (const double p) {
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
			decreaseMaxLevel = decreaseMaxLevel * (1 + change);
		}
		else {
			// ��������� ������ �������
			decreaseMinLevel = decreaseMinLevel * (1 - change);
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			decreaseMaxLevel = decreaseMaxLevel - (decreaseMaxLevel  - decreaseMinLevel) * change;
		}
		else {
			// ����������� ������ �������
			decreaseMinLevel = decreaseMinLevel + (decreaseMaxLevel  - decreaseMinLevel) * change;
		}
	}
	
	return 0;
}


/****************************************************************************
*			DecreaseRightElemCondition::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DecreaseRightElemCondition::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in DecreaseRightElemCondition::setParams : zero parameter map.");
	}
	int flag = 0;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("decreaseMinLevel")) {
			// ok, �����, ������� value
			decreaseMinLevel = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("decreaseMaxLevel")) {
			// ok, �����, ������� value
			decreaseMaxLevel = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("decreaseLeft")) {
			// ok, �����, ������� value
			decreaseLeft = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("decreaseRight")) {
			// ok, �����, ������� value
			decreaseRight = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in DecreaseRightElemCondition::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			DecreaseRightElemCondition::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DecreaseRightElemCondition::getParams (std::map<std::string,std::string> &params) const{
	params.clear();
	char temp[64];
	sprintf( temp, "%f", decreaseMinLevel);
	std::string str1 (temp);
	params["decreaseMinLevel"] = str1;
	sprintf( temp, "%f", decreaseMaxLevel);
	std::string str2 (temp);
	params["decreaseMaxLevel"] = str2;
	sprintf( temp, "%d", decreaseLeft);
	std::string str3 (temp);
	params["decreaseLeft"] = str3;
	sprintf( temp, "%d", decreaseRight);
	std::string str4 (temp);
	params["decreaseRight"] = str4;
	return 0;
}


/****************************************************************************
*			DecreaseRightElemCondition::setParamValue
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
signed int DecreaseRightElemCondition::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "decreaseMinLevel") {
		decreaseMinLevel = param;
		return 0;
	}
	if (paramName == "decreaseMaxLevel") {
		decreaseMaxLevel = param;
		return 0;
	}
	if (paramName == "decreaseLeft") {
		decreaseLeft = round (param);
		return 0;
	}
	if (paramName == "decreaseRight") {
		decreaseRight = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			DecreaseRightElemCondition::getParamValue
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
signed int DecreaseRightElemCondition::getParamValue (double &param, const std::string &paramName) const {
	if (paramName == "decreaseMinLevel") {
		param = decreaseMinLevel;
		return 0;
	}
	if (paramName == "decreaseMaxLevel") {
		param = decreaseMaxLevel;
		return 0;
	}
	if (paramName == "decreaseLeft") {
		param = (double) decreaseLeft;
		return 0;
	}
	if (paramName == "decreaseRight") {
		param = (double) decreaseRight;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			DecreaseRightElemCondition::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DecreaseRightElemCondition::getECParamNames (std::vector<std::string> &ecParamNames) const{
	ecParamNames.resize(4);
	ecParamNames[0] = "decreaseMinLevel";
	ecParamNames[1] = "decreaseMaxLevel";
	ecParamNames[2] = "decreaseLeft";
	ecParamNames[3] = "decreaseRight";
	return 0;
}


/****************************************************************************
*			DecreaseRightElemCondition::checkAndCompatibility
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
signed int DecreaseRightElemCondition::checkAndCompatibility (const ElemCondition& second) const {
	// �������� �������� y ���� DecreaseRightElemCondition* �� second ���� el_axiom
	const DecreaseRightElemCondition* y = dynamic_cast<const DecreaseRightElemCondition*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� DecreaseRightElemCondition&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((decreaseMaxLevel >= y->decreaseMaxLevel) && (decreaseMinLevel <= y->decreaseMaxLevel)) ||
		     ((decreaseMaxLevel >= y->decreaseMinLevel) && (decreaseMinLevel <= y->decreaseMinLevel)) )
			return 1;
		else
			return 0;
	}
	else {
		const IncreaseRightElemCondition* x = dynamic_cast<const IncreaseRightElemCondition*> (&second);
		if (x != NULL)
			return 0;
		return 1;
	}
	return -1;
}


/****************************************************************************
*					DecreaseRightElemCondition::setLimits
*
*	Description:	������� ������������� ������������ �������� ���������� decreaseLeftLimit � decreaseRightLimit
*	Parameters:		newLeftLimit - ����� �������� ��� ��������� decreaseLeftLimit
*					newRightLimit - ����� �������� ��� ��������� decreaseRightLimit
*	Returns:		0
*	Throws:			AxiomLibException - ���� ���� �� ���� �� ������� ���������� 
*					����� ������������� ��������. 
*	Author:			dk
*	History:
*
****************************************************************************/
signed int DecreaseRightElemCondition::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in DecreaseRightElemCondition::setLimits : parameters have a wrong values.");
	decreaseLeftLimit = newLeftLimit;
	decreaseRightLimit = newRightLimit;
	// ����� ������ �� ����������� ������� ������ ���� ����� 0
	decreaseLeftLimit = 0;
	return 0; 
}


/****************************************************************************
*					DecreaseRightElemCondition::setLimits
*
*	Description:	������� ������������� �������� ���������� decreaseMinLevelLimit, decreaseMaxLevelLimit � levelDelta
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
signed int DecreaseRightElemCondition::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	double tmp;
	// ������� �������� ������������� �������
	if (row.size() < 2) 
		return -1;
	decreaseMinLevelLimit = max (row[0] - row[1], double_default_Min_Level_Limit);
	decreaseMaxLevelLimit = decreaseMinLevelLimit;
	for (int k = decreaseLeftLimit; k < (int) row.size() - decreaseRightLimit; k++) {
		for (int i = 1; i <= decreaseRightLimit; i++) {
			tmp = (row[k] - row[k+i]) / (double) i;
			decreaseMinLevelLimit = min (tmp, decreaseMinLevelLimit);
			decreaseMinLevelLimit = max (double_default_Min_Level_Limit, decreaseMinLevelLimit); // �������� - ����� �� ���� � ������������� ����
			decreaseMaxLevelLimit = max (tmp, decreaseMaxLevelLimit);
		}
	}
	// ����������� ����������� � ������������ ��������
	if (reserve < 0.001)
		return -1;
	if (decreaseMinLevelLimit < 0)
		decreaseMinLevelLimit *= reserve;
	else 
		decreaseMinLevelLimit /= reserve;
	if (decreaseMaxLevelLimit > 0)
		decreaseMaxLevelLimit *= reserve;
	else 
		decreaseMaxLevelLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in DecreaseRightElemCondition::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (decreaseMaxLevelLimit - decreaseMinLevelLimit) / (double) numOfLevels;
	return 0;
}


/****************************************************************************
*					DecreaseRightElemCondition::changeParams
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
signed int DecreaseRightElemCondition::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		decreaseMinLevel = decreaseMinLevelLimit;
		decreaseMaxLevel = decreaseMinLevelLimit;
		decreaseLeft = 0;
		decreaseRight = 0;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((decreaseMaxLevel+levelDelta - decreaseMaxLevelLimit) > eps) {
				if ((decreaseMinLevel+levelDelta - decreaseMaxLevelLimit) > eps) {
					if (decreaseLeft >= decreaseLeftLimit) {
						if (decreaseRight >= decreaseRightLimit)
							return -1;
						else {
							decreaseRight++;
							decreaseLeft = 0;
							decreaseMinLevel = decreaseMinLevelLimit;
							decreaseMaxLevel = decreaseMinLevelLimit;
						}
					} else {
						decreaseLeft++;
						decreaseMinLevel = decreaseMinLevelLimit;
						decreaseMaxLevel = decreaseMinLevelLimit;
					}
				} else {
					decreaseMinLevel += levelDelta;
					decreaseMaxLevel = decreaseMinLevel;
				}
			} else
				decreaseMaxLevel += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((decreaseMaxLevel-levelDelta - decreaseMinLevel) < -eps) {
				if ((decreaseMinLevel-levelDelta - decreaseMinLevelLimit) < -eps) {
					if (decreaseLeft == 0) {
						if (decreaseRight == 0)
							return -1;
						else {
							decreaseRight--;
							decreaseLeft = decreaseLeftLimit;
							decreaseMinLevel = decreaseMaxLevelLimit;
							decreaseMaxLevel = decreaseMaxLevelLimit;
						}
					} else {
						decreaseLeft--;
						decreaseMinLevel = decreaseMaxLevelLimit;
						decreaseMaxLevel = decreaseMaxLevelLimit;
					}
				} else {
					decreaseMinLevel -= levelDelta;
					decreaseMaxLevel = decreaseMaxLevelLimit;
				}
			} else
				decreaseMaxLevel -= levelDelta;
		}
		return 0;
	}

	return -1;
}
