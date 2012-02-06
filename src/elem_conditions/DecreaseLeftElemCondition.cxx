/****************************************************************************
*
*			������� ������ DecreaseLeftElemCondition
*
****************************************************************************/

#include <string>
#include "DecreaseLeftElemCondition.h"
#include "IncreaseLeftElemCondition.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001
#define double_default_Min_Level_Limit	0.001

/****************************************************************************
*			DecreaseLeftElemCondition::DecreaseLeftElemCondition
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
DecreaseLeftElemCondition::DecreaseLeftElemCondition (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	decreaseMinLevel = double_default_Min_Level_Limit;
	
	// ������������� ����������� ��������� ������� ������������� �������
	decreaseMaxLevel = 10;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	decreaseLeft = 1;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	decreaseRight = 0;

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
*			DecreaseLeftElemCondition::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string DecreaseLeftElemCondition::name (void) const {
	std::string toReturn("decrease_left_elementary_condition");  
	return toReturn;
}


/****************************************************************************
*			DecreaseLeftElemCondition::check
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
signed int DecreaseLeftElemCondition::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (((int) k < decreaseLeft) || ((int) (x.size() - k) <= decreaseRight)) {
		toReturn = -1;
		//throw AxiomLibException("Error in DecreaseLeftElemCondition::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	for (int i = 0; i < decreaseLeft; i++) {
		if (((x[k-i-1] - x[k]) < decreaseMinLevel*(i+1)) || ((x[k-i-1] - x[k]) > decreaseMaxLevel*(i+1))) {
			return 0;
		}
	}
	
	return toReturn;
}

/****************************************************************************
*			DecreaseLeftElemCondition::operator<
*
*	Description:	��������� ���� DecreaseLeftElemCondition �� ��������� (x < y).
*	Parameters:	DecreaseLeftElemCondition& second - ������ ����� ���������
*	Returns:		1 ����  <  � second - ��� DecreaseLeftElemCondition
*				-1 ����  >  � second - ��� DecreaseLeftElemCondition
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� DecreaseLeftElemCondition)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double DecreaseLeftElemCondition::operator< (const ElemCondition& second) const{
	// �������� �������� y ���� DecreaseLeftElemCondition* �� second ���� el_axiom
	const DecreaseLeftElemCondition* y = dynamic_cast<const DecreaseLeftElemCondition*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� DecreaseLeftElemCondition&, �� ����� ���-�� ���������
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
*			DecreaseLeftElemCondition::transmute
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
signed int DecreaseLeftElemCondition::transmute (const double p) {
	signed int toReturn = 0;
	// �������� ������ p
	if (p < 0 || p > 1) {
		return -1;
	}
	
	// �������� ������� ���������
	double change = (((double) rand())/ ((double) RAND_MAX)) * p ;
	
	if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
		// ����������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ����������� ������� �������
			decreaseMaxLevel = decreaseMaxLevel * (1 + change);
			toReturn = 1;
		}
		else {
			// ��������� ������ �������
			decreaseMinLevel = decreaseMinLevel * (1 - change);
			toReturn = 1;
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			decreaseMaxLevel = decreaseMaxLevel - (decreaseMaxLevel  - decreaseMinLevel) * change;
			toReturn = 1;
		}
		else {
			// ����������� ������ �������
			decreaseMinLevel = decreaseMinLevel + (decreaseMaxLevel  - decreaseMinLevel) * change;
			toReturn = 1;
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			DecreaseLeftElemCondition::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DecreaseLeftElemCondition::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in DecreaseLeftElemCondition::setParams : zero parameter map.");
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
			throw AxiomLibException("Error in DecreaseLeftElemCondition::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			DecreaseLeftElemCondition::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DecreaseLeftElemCondition::getParams (std::map<std::string,std::string> &params) const {
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
*			DecreaseLeftElemCondition::setParamValue
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
signed int DecreaseLeftElemCondition::setParamValue (const double param, const std::string &paramName) {
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
*			DecreaseLeftElemCondition::getParamValue
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
signed int DecreaseLeftElemCondition::getParamValue (double &param, const std::string &paramName) const{
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
*			DecreaseLeftElemCondition::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DecreaseLeftElemCondition::getECParamNames (std::vector<std::string> &ecParamNames) const{
	ecParamNames.resize(4);
	ecParamNames[0] = "decreaseMinLevel";
	ecParamNames[1] = "decreaseMaxLevel";
	ecParamNames[2] = "decreaseLeft";
	ecParamNames[3] = "decreaseRight";
	return 0;
}


/****************************************************************************
*			DecreaseLeftElemCondition::checkAndCompatibility
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
signed int DecreaseLeftElemCondition::checkAndCompatibility (const ElemCondition& second) const {
	// �������� �������� y ���� DecreaseLeftElemCondition* �� second ���� el_axiom
	const DecreaseLeftElemCondition* y = dynamic_cast<const DecreaseLeftElemCondition*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� DecreaseLeftElemCondition&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((decreaseMaxLevel >= y->decreaseMaxLevel) && (decreaseMinLevel <= y->decreaseMaxLevel)) || ((decreaseMaxLevel >= y->decreaseMinLevel) && (decreaseMinLevel <= y->decreaseMinLevel)) )
			return 1;
		else
			return 0;
	}
	else {
		const IncreaseLeftElemCondition* x = dynamic_cast<const IncreaseLeftElemCondition*> (&second);
		if (x != NULL)
			return 0;
		return 1;
	}
	return -1;
}


/****************************************************************************
*					DecreaseLeftElemCondition::setLimits
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
signed int DecreaseLeftElemCondition::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in DecreaseLeftElemCondition::setLimits : parameters have a wrong values.");
	decreaseLeftLimit = newLeftLimit;
	decreaseRightLimit = newRightLimit;
	// ������ ������ �� ����������� ������� ������ ���� ����� 0
	decreaseRightLimit = 0;
	return 0; 
}


/****************************************************************************
*					DecreaseLeftElemCondition::setLimits
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
signed int DecreaseLeftElemCondition::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	double tmp;
	// ������� �������� ������������� �������
	if (row.size() < 2) 
		return -1;
	// ����������� ��������� �������� ������� ����������
	//  max ������� - ��� ������ �� ����, ����� �������� decreaseMinLevelLimit � decreaseMaxLevelLimit �� ����� ����� ��������������, ���� ���������� �� ����
	decreaseMinLevelLimit = max (row[0] - row[1], double_default_Min_Level_Limit);
	decreaseMaxLevelLimit = decreaseMinLevelLimit;
	for (int k = decreaseLeftLimit; k < (int) row.size() - decreaseRightLimit; k++) {
		for (int i = 1; i <= decreaseLeftLimit; i++) {
			tmp = (row[k-i] - row[k]) / (double) i;
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
		throw AxiomLibException("Error in DecreaseLeftElemCondition::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (decreaseMaxLevelLimit - decreaseMinLevelLimit) / (double) numOfLevels;
	return 0;
}


/****************************************************************************
*					DecreaseLeftElemCondition::changeParams
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
signed int DecreaseLeftElemCondition::changeParams (const int numOfSteps) {
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
