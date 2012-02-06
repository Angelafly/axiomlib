/****************************************************************************
*
*			������� ������ BetweenTwoLevels
*
****************************************************************************/

#include <string>
#include "BetweenTwoLevels.h"
#include "DecreaseLeftElemCondition.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;
using namespace std;



#define eps		0.0000000001


/****************************************************************************
*			BetweenTwoLevels::BetweenTwoLevels
*
*	Description:	��������� �����������, ��������� ����������
*				����������� � ������������ ���������� (��� ��������)
*	Parameters:	-
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
BetweenTwoLevels::BetweenTwoLevels (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	Min = -100;
	
	// ������������� ����������� ��������� ������� ������������� �������
	Max = 100;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	Left = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	Right = 0;

	// ����������� ������� ��������� ��� ��������� Min, ������� ����������� ��� ������� ���������� ���������
	minLimit = Min;
	
	// ������������� ������� ��������� ��� ��������� Max, ������� ����������� ��� ������� ���������� ���������
	maxLimit = Max;

	// ��� ��������� ���������� Min � Max, ��� ������� ���������� ���������
	levelDelta = Max - Min;
	
	// ������������ �������� ��������� Left, ������� ����������� ��� ������� ���������� ���������
	leftLimit = Left;
	
	// ������������ �������� ��������� Right, ������� ����������� ��� ������� ���������� ���������
	rightLimit = Right;
}

/****************************************************************************
*			BetweenTwoLevels::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string BetweenTwoLevels::name (void) const {
	std::string toReturn("between_two_levels");
	return toReturn;
}


/****************************************************************************
*			BetweenTwoLevels::check
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
signed int BetweenTwoLevels::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	
	// �������� ������
	if (( (int) k < Left) || ( (int) (x.size() - k) <= Right)) {
		toReturn = -1;
		//throw AxiomLibException("Error in BetweenTwoLevels::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = 1;
	for (int i = (int) k - Left; i <= (int) k + Right; i++) {
		if ( (x[i] > Max) || (x[i] < Min) ) {
			return 0;
		}
	}
	
	return toReturn;
}

/****************************************************************************
*			BetweenTwoLevels::operator<
*
*	Description:	��������� ���� BetweenTwoLevels �� ��������� (x < y).
*	Parameters:	BetweenTwoLevels& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� BetweenTwoLevels
*				-1 ���� > � second - ��� BetweenTwoLevels
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� BetweenTwoLevels)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double BetweenTwoLevels::operator< (const ElemCondition& second) const {
	// �������� �������� y ���� BetweenTwoLevels* �� second ���� el_axiom
	const BetweenTwoLevels* y = dynamic_cast<const BetweenTwoLevels*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� BetweenTwoLevels&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ((Max < y->Max + 0.01) && (Min > y->Min - 0.01))
			return 1;
		else if ((y->Max < Max + 0.01) && (y->Min > Min - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			BetweenTwoLevels::transmute
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
signed int BetweenTwoLevels::transmute (const double p) {
	signed int toReturn=0;

	// �������� ������ p
	if (p < 0 || p > 1) {
		return -1;
	}
	
	// �������� ������� ���������
	double change = (((double)rand())/ ((double) RAND_MAX)) * p;
	
	if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
		// ����������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ����������� ������� �������
			Max = Max + (Max - Min) * change;
			toReturn = 1;
		}
		else {
			// ��������� ������ �������
			Min = Min -  (Max - Min) * change;
			toReturn = 1;
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			Max = Max - (Max  - Min) * change;
			toReturn = 1;
		}
		else {
			// ����������� ������ �������
			Min = Min + (Max  - Min) * change;
			toReturn = 1;
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			BetweenTwoLevels::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int BetweenTwoLevels::setParams (const std::map<std::string, std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in BetweenTwoLevels::setParams : zero parameter map.");
	}
	int flag = 0;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("Min")) {
			// ok, �����, ������� value
			Min = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("Max")) {
			// ok, �����, ������� value
			Max = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("Left")) {
			// ok, �����, ������� value
			Left = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("Right")) {
			// ok, �����, ������� value
			Right = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in BetweenTwoLevels::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			BetweenTwoLevels::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int BetweenTwoLevels::getParams (std::map<std::string,std::string> &params) const{
	params.clear();
	char temp[64];
	sprintf( temp, "%f", Min);
	std::string str1 (temp);
	params["Min"] = str1;
	sprintf( temp, "%f", Max);
	std::string str2 (temp);
	params["Max"] = str2;
	sprintf( temp, "%d", Left);
	std::string str3 (temp);
	params["Left"] = str3;
	sprintf( temp, "%d", Right);
	std::string str4 (temp);
	params["Right"] = str4;
	return 0;
}


/****************************************************************************
*			BetweenTwoLevels::setParamValue
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
signed int BetweenTwoLevels::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "Min") {
		Min = param;
		return 0;
	}
	if (paramName == "Max") {
		Max = param;
		return 0;
	}
	if (paramName == "Left") {
		Left = round (param);
		return 0;
	}
	if (paramName == "Right") {
		Right = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			BetweenTwoLevels::getParamValue
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
signed int BetweenTwoLevels::getParamValue (double &param, const std::string &paramName) const {
	if (paramName == "Min") {
		param = Min;
		return 0;
	}
	if (paramName == "Max") {
		param = Max;
		return 0;
	}
	if (paramName == "Left") {
		param = (double) Left;
		return 0;
	}
	if (paramName == "Right") {
		param = (double) Right;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			BetweenTwoLevels::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int BetweenTwoLevels::getECParamNames (std::vector<std::string> &ecParamNames) const {
	ecParamNames.resize(4);
	ecParamNames[0] = "Min";
	ecParamNames[1] = "Max";
	ecParamNames[2] = "Left";
	ecParamNames[3] = "Right";
	return 0;
}


/****************************************************************************
*			BetweenTwoLevels::checkAndCompatibility
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
signed int BetweenTwoLevels::checkAndCompatibility (const ElemCondition& second) const{
	// �������� �������� y ���� BetweenTwoLevels* �� second ���� el_axiom
	const BetweenTwoLevels* y = dynamic_cast<const BetweenTwoLevels*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� BetweenTwoLevels&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( ((Max >= y->Max) && (Min <= y->Max)) || ((Max >= y->Min) && (Min <= y->Min)) )
			return 1;
		else
			return 0;
	}
	else {
		return 1;
	}
	return -1;
}


/****************************************************************************
*					BetweenTwoLevels::setLimits
*
*	Description:	������� ������������� ������������ �������� ���������� leftLimit � rightLimit
*	Parameters:		newLeftLimit - ����� �������� ��� ��������� leftLimit
*					newRightLimit - ����� �������� ��� ��������� rightLimit
*	Returns:		0
*	Throws:			AxiomLibException - ���� ���� �� ���� �� ������� ���������� 
*					����� ������������� ��������. 
*	Author:			dk
*	History:
*
****************************************************************************/
signed int BetweenTwoLevels::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in BetweenTwoLevels::setLimits : parameters have a wrong values.");
	leftLimit = newLeftLimit;
	rightLimit = newRightLimit;
	return 0; 
}


/****************************************************************************
*					BetweenTwoLevels::setLimits
*
*	Description:	������� ������������� �������� ���������� minLimit, maxLimit � levelDelta
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
signed int BetweenTwoLevels::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	if (row.size() < 1)
		return -1;
	// ����������� ����������� � ������������ ��������
	minLimit = row [0];
	maxLimit = row [0];
	for (unsigned int j = 1; j < row.size(); j++) {
		minLimit = min (row [j], minLimit);
		maxLimit = max (row [j], maxLimit);
	}
	if (reserve < 0.001)
		return -1;
	if (minLimit < 0)
		minLimit *= reserve;
	else 
		minLimit /= reserve;
	if (maxLimit > 0)
		maxLimit *= reserve;
	else 
		maxLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in BetweenTwoLevels::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (maxLimit - minLimit) / (double) numOfLevels;
	if(levelDelta < 1e-8) {
		return -1;
	}	
	return 0;
}


/****************************************************************************
*					BetweenTwoLevels::changeParams
*
*	Description:	������� �������� ��������� ������������� ������� �� ����� ����� numOfSteps
*	Parameters:		numOfSteps - ����� �����, �� ������� �������� ��������� ������������� �������
*	Returns:		0 - ���� ��������� ��������� ������
*					-1 - ���� ��������� �� ��������� ����� ���� ����������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int BetweenTwoLevels::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		Min = minLimit;
		Max = minLimit;
		Left = 0;
		Right = 0;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((Max+levelDelta - maxLimit) > eps) {
				if ((Min+levelDelta - maxLimit) > eps) {
					if (Left >= leftLimit) {
						if (Right >= rightLimit)
							return -1;
						else {
							Right++;
							Left = 0;
							Min = minLimit;
							Max = minLimit;
						}
					} else {
						Left++;
						Min = minLimit;
						Max = minLimit;
					}
				} else {
					Min += levelDelta;
					Max = Min;
				}
			} else
				Max += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((Max-levelDelta - Min) < -eps) {
				if ((Min-levelDelta - minLimit) < -eps) {
					if (Left == 0) {
						if (Right == 0)
							return -1;
						else {
							Right--;
							Left = leftLimit;
							Min = maxLimit;
							Max = maxLimit;
						}
					} else {
						Left--;
						Min = maxLimit;
						Max = maxLimit;
					}
				} else {
					Min -= levelDelta;
					Max = maxLimit;
				}
			} else
				Max -= levelDelta;
		}
		return 0;
	}

	return -1;
}
