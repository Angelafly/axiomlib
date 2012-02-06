/****************************************************************************
*
*			������� ������ DerivativeDecreaseRight
*
****************************************************************************/

#include <string>
#include "DerivativeDecreaseRight.h"
#include "DerivativeIncreaseRight.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001
#define double_default_Min_Level_Limit	0.001

/****************************************************************************
*			DerivativeDecreaseRight::DerivativeDecreaseRight
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
DerivativeDecreaseRight::DerivativeDecreaseRight (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	derivDecreaseMinLevel = double_default_Min_Level_Limit;
	
	// ������������� ����������� ��������� ������� ������������� �������
	derivDecreaseMaxLevel = 10;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	derivDecreaseLeft = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	derivDecreaseRight = 2;

	// ����������� ������� ��������� ��� ��������� derivDecreaseMinLevel, ������� ����������� ��� ������� ���������� ���������
	derivDecreaseMinLevelLimit = derivDecreaseMinLevel;
	
	// ������������� ������� ��������� ��� ��������� derivDecreaseMaxLevel, ������� ����������� ��� ������� ���������� ���������
	derivDecreaseMaxLevelLimit = derivDecreaseMaxLevel;

	// ��� ��������� ���������� derivDecreaseMinLevel � derivDecreaseMaxLevel, ��� ������� ���������� ���������
	levelDelta = derivDecreaseMaxLevel - derivDecreaseMinLevel;
	
	// ������������ �������� ��������� derivDecreaseLeft, ������� ����������� ��� ������� ���������� ���������
	derivDecreaseLeftLimit = derivDecreaseLeft;
	
	// ������������ �������� ��������� derivDecreaseRight, ������� ����������� ��� ������� ���������� ���������
	derivDecreaseRightLimit = derivDecreaseRight;
}

/****************************************************************************
*			DerivativeDecreaseRight::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string DerivativeDecreaseRight::name (void) const {
	std::string toReturn("derivative_decrease_right");
	return toReturn;
}


/****************************************************************************
*			DerivativeDecreaseRight::check
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
signed int DerivativeDecreaseRight::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (((int) k < derivDecreaseLeft) || ((int) (x.size() - k) <= derivDecreaseRight)) {
		toReturn = -1;
		//throw AxiomLibException("Error in DerivativeDecreaseRight::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	for (int i = 0; i < derivDecreaseRight; i++) {
		if (((x[k+i] - x[k+i+1]) < derivDecreaseMinLevel) || ((x[k+i] - x[k+i+1]) > derivDecreaseMaxLevel)) {
			return 0;
		}
	}
	
	return toReturn;
}

/****************************************************************************
*			DerivativeDecreaseRight::operator<
*
*	Description:	��������� ���� DerivativeDecreaseRight �� ��������� (x < y).
*	Parameters:	DerivativeDecreaseRight& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� DerivativeDecreaseRight
*				-1 ���� > � second - ��� DerivativeDecreaseRight
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� DerivativeDecreaseRight)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double DerivativeDecreaseRight::operator< (const ElemCondition& second) const{
	// �������� �������� y ���� DerivativeDecreaseRight* �� second ���� el_axiom
	const DerivativeDecreaseRight* y = dynamic_cast<const DerivativeDecreaseRight*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� DerivativeDecreaseRight&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ((derivDecreaseMaxLevel < y->derivDecreaseMaxLevel + 0.01) && (derivDecreaseMinLevel > y->derivDecreaseMinLevel - 0.01))
			return 1;
		else if ((y->derivDecreaseMaxLevel < derivDecreaseMaxLevel + 0.01) && (y->derivDecreaseMinLevel > derivDecreaseMinLevel - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			DerivativeDecreaseRight::transmute
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
signed int DerivativeDecreaseRight::transmute (const double p) {
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
			derivDecreaseMaxLevel = derivDecreaseMaxLevel * (1 + change);
			toReturn = 1;
		}
		else {
			// ��������� ������ �������
			derivDecreaseMinLevel = derivDecreaseMinLevel * (1 - change);
			toReturn = 1;
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			derivDecreaseMaxLevel = derivDecreaseMaxLevel - (derivDecreaseMaxLevel  - derivDecreaseMinLevel) * change;
			toReturn = 1;
		}
		else {
			// ����������� ������ �������
			derivDecreaseMinLevel = derivDecreaseMinLevel + (derivDecreaseMaxLevel  - derivDecreaseMinLevel) * change;
			toReturn = 1;
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			DerivativeDecreaseRight::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DerivativeDecreaseRight::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in DerivativeDecreaseRight::setParams : zero parameter map.");
	}
	int flag = 0;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("derivDecreaseMinLevel")) {
			// ok, �����, ������� value
			derivDecreaseMinLevel = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("derivDecreaseMaxLevel")) {
			// ok, �����, ������� value
			derivDecreaseMaxLevel = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("derivDecreaseLeft")) {
			// ok, �����, ������� value
			derivDecreaseLeft = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("derivDecreaseRight")) {
			// ok, �����, ������� value
			derivDecreaseRight = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in DerivativeDecreaseRight::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			DerivativeDecreaseRight::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DerivativeDecreaseRight::getParams (std::map<std::string,std::string> &params) const {
	params.clear();
	char temp[64];
	sprintf( temp, "%f", derivDecreaseMinLevel);
	std::string str1 (temp);
	params["derivDecreaseMinLevel"] = str1;
	sprintf( temp, "%f", derivDecreaseMaxLevel);
	std::string str2 (temp);
	params["derivDecreaseMaxLevel"] = str2;
	sprintf( temp, "%d", derivDecreaseLeft);
	std::string str3 (temp);
	params["derivDecreaseLeft"] = str3;
	sprintf( temp, "%d", derivDecreaseRight);
	std::string str4 (temp);
	params["derivDecreaseRight"] = str4;
	return 0;
}


/****************************************************************************
*			DerivativeDecreaseRight::setParamValue
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
signed int DerivativeDecreaseRight::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "derivDecreaseMinLevel") {
		derivDecreaseMinLevel = param;
		return 0;
	}
	if (paramName == "derivDecreaseMaxLevel") {
		derivDecreaseMaxLevel = param;
		return 0;
	}
	if (paramName == "derivDecreaseLeft") {
		derivDecreaseLeft = round (param);
		return 0;
	}
	if (paramName == "derivDecreaseRight") {
		derivDecreaseRight = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			DerivativeDecreaseRight::getParamValue
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
signed int DerivativeDecreaseRight::getParamValue (double &param, const std::string &paramName) const {
	if (paramName == "derivDecreaseMinLevel") {
		param = derivDecreaseMinLevel;
		return 0;
	}
	if (paramName == "derivDecreaseMaxLevel") {
		param = derivDecreaseMaxLevel;
		return 0;
	}
	if (paramName == "derivDecreaseLeft") {
		param = (double) derivDecreaseLeft;
		return 0;
	}
	if (paramName == "derivDecreaseRight") {
		param = (double) derivDecreaseRight;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			DerivativeDecreaseRight::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int DerivativeDecreaseRight::getECParamNames (std::vector<std::string> &ecParamNames) const{
	ecParamNames.resize(4);
	ecParamNames[0] = "derivDecreaseMinLevel";
	ecParamNames[1] = "derivDecreaseMaxLevel";
	ecParamNames[2] = "derivDecreaseLeft";
	ecParamNames[3] = "derivDecreaseRight";
	return 0;
}


/****************************************************************************
*			DerivativeDecreaseRight::checkAndCompatibility
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
signed int DerivativeDecreaseRight::checkAndCompatibility (const ElemCondition& second) const{
	// �������� �������� y ���� DerivativeDecreaseRight* �� second ���� el_axiom
	const DerivativeDecreaseRight* y = dynamic_cast<const DerivativeDecreaseRight*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� DerivativeDecreaseRight&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((derivDecreaseMaxLevel >= y->derivDecreaseMaxLevel) && (derivDecreaseMinLevel <= y->derivDecreaseMaxLevel)) ||
		     ((derivDecreaseMaxLevel >= y->derivDecreaseMinLevel) && (derivDecreaseMinLevel <= y->derivDecreaseMinLevel)) )
			return 1;
		else
			return 0;
	}
	else {
		const DerivativeIncreaseRight* x = dynamic_cast<const DerivativeIncreaseRight*> (&second);
		if (x != NULL)
			return 0;
		return 1;
	}
	return -1;
}


/****************************************************************************
*					DerivativeDecreaseRight::setLimits
*
*	Description:	������� ������������� ������������ �������� ���������� derivDecreaseLeftLimit � derivDecreaseRightLimit
*	Parameters:		newLeftLimit - ����� �������� ��� ��������� derivDecreaseLeftLimit
*					newRightLimit - ����� �������� ��� ��������� derivDecreaseRightLimit
*	Returns:		0
*	Throws:			AxiomLibException - ���� ���� �� ���� �� ������� ���������� 
*					����� ������������� ��������. 
*	Author:			dk
*	History:
*
****************************************************************************/
signed int DerivativeDecreaseRight::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in DerivativeDecreaseRight::setLimits : parameters have a wrong values.");
	derivDecreaseLeftLimit = newLeftLimit;
	derivDecreaseRightLimit = newRightLimit;
	// ����� ������ �� ����������� ������� ������ ���� ����� 0
	derivDecreaseLeftLimit = 0;
	return 0; 
}


/****************************************************************************
*					DerivativeDecreaseRight::setLimits
*
*	Description:	������� ������������� �������� ���������� derivDecreaseMinLevelLimit, derivDecreaseMaxLevelLimit � levelDelta
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
signed int DerivativeDecreaseRight::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	// ������� �������� ������������� �������
	if (row.size() < 2) 
		return -1;
	derivDecreaseMinLevelLimit = min (row[0] - row[1], double_default_Min_Level_Limit);
	derivDecreaseMaxLevelLimit = derivDecreaseMinLevelLimit;
	for (int k = 2; k < (int) row.size(); k++) {
		derivDecreaseMinLevelLimit = min (row[k-1] - row[k], derivDecreaseMinLevelLimit);
		derivDecreaseMinLevelLimit = max (double_default_Min_Level_Limit, derivDecreaseMinLevelLimit);
		derivDecreaseMaxLevelLimit = max (row[k-1] - row[k], derivDecreaseMaxLevelLimit);
	}
	// ����������� ����������� � ������������ ��������
	if (reserve < 0.001)
		return -1;
	if (derivDecreaseMinLevelLimit < 0)
		derivDecreaseMinLevelLimit *= reserve;
	else 
		derivDecreaseMinLevelLimit /= reserve;
	if (derivDecreaseMaxLevelLimit > 0)
		derivDecreaseMaxLevelLimit *= reserve;
	else 
		derivDecreaseMaxLevelLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in DerivativeDecreaseRight::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (derivDecreaseMaxLevelLimit - derivDecreaseMinLevelLimit) / (double) numOfLevels;
	return 0;
}


/****************************************************************************
*					DerivativeDecreaseRight::changeParams
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
signed int DerivativeDecreaseRight::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		derivDecreaseMinLevel = derivDecreaseMinLevelLimit;
		derivDecreaseMaxLevel = derivDecreaseMinLevelLimit;
		derivDecreaseLeft = 0;
		derivDecreaseRight = 0;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((derivDecreaseMaxLevel+levelDelta - derivDecreaseMaxLevelLimit) > eps) {
				if ((derivDecreaseMinLevel+levelDelta - derivDecreaseMaxLevelLimit) > eps) {
					if (derivDecreaseLeft >= derivDecreaseLeftLimit) {
						if (derivDecreaseRight >= derivDecreaseRightLimit)
							return -1;
						else {
							derivDecreaseRight++;
							derivDecreaseLeft = 0;
							derivDecreaseMinLevel = derivDecreaseMinLevelLimit;
							derivDecreaseMaxLevel = derivDecreaseMinLevelLimit;
						}
					} else {
						derivDecreaseLeft++;
						derivDecreaseMinLevel = derivDecreaseMinLevelLimit;
						derivDecreaseMaxLevel = derivDecreaseMinLevelLimit;
					}
				} else {
					derivDecreaseMinLevel += levelDelta;
					derivDecreaseMaxLevel = derivDecreaseMinLevel;
				}
			} else
				derivDecreaseMaxLevel += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((derivDecreaseMaxLevel-levelDelta - derivDecreaseMinLevel) < -eps) {
				if ((derivDecreaseMinLevel-levelDelta - derivDecreaseMinLevelLimit) < -eps) {
					if (derivDecreaseLeft == 0) {
						if (derivDecreaseRight == 0)
							return -1;
						else {
							derivDecreaseRight--;
							derivDecreaseLeft = derivDecreaseLeftLimit;
							derivDecreaseMinLevel = derivDecreaseMaxLevelLimit;
							derivDecreaseMaxLevel = derivDecreaseMaxLevelLimit;
						}
					} else {
						derivDecreaseLeft--;
						derivDecreaseMinLevel = derivDecreaseMaxLevelLimit;
						derivDecreaseMaxLevel = derivDecreaseMaxLevelLimit;
					}
				} else {
					derivDecreaseMinLevel -= levelDelta;
					derivDecreaseMaxLevel = derivDecreaseMaxLevelLimit;
				}
			} else
				derivDecreaseMaxLevel -= levelDelta;
		}
		return 0;
	}

	return -1;
}
