/****************************************************************************
*
*			������� ������ Integral
*
****************************************************************************/

#include <string>
#include "Integral.h"
#include <stdlib.h>
#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;

#define eps		0.0000000001


/****************************************************************************
*			Integral::Integral
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
Integral::Integral (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
	minLevel = -100;
	
	// ������������� ����������� ��������� ������� ������������� �������
	maxLevel = 100;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	left = 5;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	right = 5;

	// ����������� ������� ��������� ��� ��������� minLevel, ������� ����������� ��� ������� ���������� ���������
	minLevelLimit = minLevel;
	
	// ������������� ������� ��������� ��� ��������� maxLevel, ������� ����������� ��� ������� ���������� ���������
	maxLevelLimit = maxLevel;

	// ��� ��������� ���������� minLevel � maxLevel, ��� ������� ���������� ���������
	levelDelta = maxLevel - minLevel;
	
	// ������������ �������� ��������� left, ������� ����������� ��� ������� ���������� ���������
	leftLimit = left;
	
	// ������������ �������� ��������� right, ������� ����������� ��� ������� ���������� ���������
	rightLimit = right;
}

/****************************************************************************
*			Integral::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string Integral::name (void) const {
	std::string toReturn("integral");
	return toReturn;
}


/****************************************************************************
*			Integral::check
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
signed int Integral::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	double curRes = 0;
	
	// �������� ������
	if (( (int) k < left) || ( (int) (x.size() - k) <= right)) {
		toReturn = -1;
		//throw AxiomLibException("Error in Integral::check : out of range.");
		return toReturn;
	}
	// �������� ������������� �������
	toReturn = -1;
	curRes = 0.5 * (x[k-left] + x[k+right]);
	for (int i = (int) k - left + 1; i < (int) k + right; i++) {
		curRes += x[i];
	}
	//if ((left == 0) && (right == 0)) curRes = 0; // �������� � �����
	if ((curRes <= maxLevel) && (curRes >= minLevel)) {
		return 1;
	} 
	else {
		return 0;
	}
	return toReturn;
}

/****************************************************************************
*			Integral::operator<
*
*	Description:	��������� ���� Integral �� ��������� (x < y).
*	Parameters:	Integral& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� Integral
*				-1 ���� > � second - ��� Integral
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� Integral)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double Integral::operator< (const ElemCondition& second) const {
	// �������� �������� y ���� Integral* �� second ���� el_axiom
	const Integral* y = dynamic_cast<const Integral*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� Integral&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if (( maxLevel < y->maxLevel + 0.01) && ( minLevel > y->minLevel - 0.01))
			return 1;
		else if ((y->maxLevel <  maxLevel + 0.01) && (y->minLevel >  minLevel - 0.01))
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			Integral::transmute
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
signed int Integral::transmute (const double p) {
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
			maxLevel =  maxLevel + ( maxLevel -  minLevel) * change;
			toReturn = 1;
		}
		else {
			// ��������� ������ �������
			minLevel =  minLevel -  ( maxLevel -  minLevel) * change;
			toReturn = 1;
		}
	}
	else {
		// ��������� ������� �������
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			maxLevel =  maxLevel - ( maxLevel  -  minLevel) * change;
			toReturn = 1;
		}
		else {
			// ����������� ������ �������
			minLevel =  minLevel + ( maxLevel  -  minLevel) * change;
			toReturn = 1;
		}
	}
	
	// �������� left � right
	// �������� ������� ���������
	double change1 = (((double)rand())/ ((double) RAND_MAX)) * p;
	if ( ( (double)rand() )/ ( (double) RAND_MAX ) > 0.5 ) { // ��� ��� �������� left � right �� ������
		int newVal = 0; 
		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ����������� ������� �������
			if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
				// ����������� right
				newVal = right + round ((right -  left) * change1);
				if ((right > 0) && (newVal > 0)) right = newVal;
				toReturn = 1;
			}
			else {
				// ����������� left
				newVal = left -  round ((right -  left) * change1);
				if ((left > 0)&&(newVal > 0)) left = newVal;
				toReturn = 1;
			}
		}
		else {
			// ��������� ������� �������
			if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
				// ��������� right
				newVal =  right - round ((right -  left) * change1);
				if ((right > 0)&&(newVal > 0)) right = newVal;
				toReturn = 1;
			}
			else {
				// ��������� left
				newVal = left + round ((right -  left) * change1);
				if ((left > 0)&&(newVal > 0)) left = newVal;
				toReturn = 1;
			}
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			Integral::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Integral::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in Integral::setParams : zero parameter map.");
	}
	int flag;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
		if (paramName == (const std::string)("minLevel")) {
			// ok, �����, ������� value
			minLevel = atof(paramValue.c_str());
			flag = 1;
		}
		if (paramName == (const std::string)("maxLevel")) {
			// ok, �����, ������� value
			maxLevel = atof(paramValue.c_str());
			flag = 2;
		}
		if (paramName == (const std::string)("left")) {
			// ok, �����, ������� value
			left = atoi(paramValue.c_str());
			flag = 3;
		}
		if (paramName == (const std::string)("right")) {
			// ok, �����, ������� value
			right = atoi(paramValue.c_str());
			flag = 4;
		}
		if (flag == 0) {
			// ����������� ��������
			throw AxiomLibException("Error in Integral::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			Integral::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Integral::getParams (std::map<std::string,std::string> &params) const{
	params.clear();
	char temp[64];
	sprintf( temp, "%f", minLevel);
	std::string str1 (temp);
	params["minLevel"] = str1;
	sprintf( temp, "%f", maxLevel);
	std::string str2 (temp);
	params["maxLevel"] = str2;
	sprintf( temp, "%d", left);
	std::string str3 (temp);
	params["left"] = str3;
	sprintf( temp, "%d", right);
	std::string str4 (temp);
	params["right"] = str4;
	return 0;
}


/****************************************************************************
*			Integral::setParamValue
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
signed int Integral::setParamValue (const double param, const std::string &paramName) {
	if (paramName == "minLevel") {
		minLevel = param;
		return 0;
	}
	if (paramName == "maxLevel") {
		maxLevel = param;
		return 0;
	}
	if (paramName == "left") {
		left = round (param);
		return 0;
	}
	if (paramName == "right") {
		right = round (param);
		return 0;
	}
	return -1;
} 


/****************************************************************************
*			Integral::getParamValue
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
signed int Integral::getParamValue (double &param, const std::string &paramName) const {
	if (paramName == "minLevel") {
		param = minLevel;
		return 0;
	}
	if (paramName == "maxLevel") {
		param = maxLevel;
		return 0;
	}
	if (paramName == "left") {
		param = (double) left;
		return 0;
	}
	if (paramName == "right") {
		param = (double) right;
		return 0;
	}
	return -1;
}


/****************************************************************************
*			Integral::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Integral::getECParamNames (std::vector<std::string> &ecParamNames) const {
	ecParamNames.resize(4);
	ecParamNames[0] = "minLevel";
	ecParamNames[1] = "maxLevel";
	ecParamNames[2] = "left";
	ecParamNames[3] = "right";
	return 0;
}


/****************************************************************************
*			Integral::checkAndCompatibility
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
signed int Integral::checkAndCompatibility (const ElemCondition& second) const{
	// �������� �������� y ���� Integral* �� second ���� el_axiom
	const Integral* y = dynamic_cast<const Integral*> (&second);
  
	// ���� y != NULL, �.�. second ������������� ����� ��� Integral&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( (( maxLevel >= y->maxLevel) && ( minLevel <= y->maxLevel)) || (( maxLevel >= y->minLevel) && ( minLevel <= y->minLevel)) )
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
*					Integral::setLimits
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
signed int Integral::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
		throw AxiomLibException("Error in Integral::setLimits : parameters have a wrong values.");
	leftLimit = newLeftLimit;
	rightLimit = newRightLimit;
	return 0; 
}


/****************************************************************************
*					Integral::setLimits
*
*	Description:	������� ������������� �������� ���������� minLevelLimit, maxLevelLimit � levelDelta
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
signed int Integral::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	std::vector <double> curRes;
	int len;
	// �������� ������������� ����� ����
	len = row.size() - rightLimit - leftLimit;
	if (len < 1)
		return -1;
	// ������� �������� �������� ���������
	curRes.resize (len);
	for (int k = leftLimit; k < (int) row.size() - rightLimit; k++) {
		curRes[k-leftLimit] = 0.5 * (row[k-leftLimit] + row[k+rightLimit]);
		for (int i = k - leftLimit + 1; i < k + rightLimit; i++) {
			curRes[k-leftLimit] += row[i];
		}
	}
	// ����������� ����������� � ������������ ��������
	minLevelLimit = min (curRes[0], row [leftLimit]);
	maxLevelLimit = max (curRes[0], row [leftLimit]);
	for (int j = 1; j < len; j++) {
		minLevelLimit = min3 (curRes[j], row [j + leftLimit], minLevelLimit);
		maxLevelLimit = max3 (curRes[j], row [j + leftLimit], maxLevelLimit);
	}
	if (reserve < 0.001)
		return -1;
	if (minLevelLimit < 0)
		minLevelLimit *= reserve;
	else 
		minLevelLimit /= reserve;
	if (maxLevelLimit > 0)
		maxLevelLimit *= reserve;
	else 
		maxLevelLimit /= reserve;
	// ����������� ���� ��������� ����������
	if (numOfLevels < 1)
		throw AxiomLibException("Error in Integral::setLimits : parameter 'numOfLevels' has a wrong value.");
	levelDelta = (maxLevelLimit - minLevelLimit) / (double) numOfLevels;
	if(levelDelta < 1e-8) {
		return -1;
	}
	return 0;
}


/****************************************************************************
*					Integral::changeParams
*
*	Description:	������� �������� ��������� ������������� ������� �� ����� �����, ��������� � ����������
*	Parameters:		numOfSteps - ����� �����, �� ������� �������� ��������� ������������� �������
*	Returns:		0 - ���� ��������� ��������� ������
*					-1 - ���� ��������� �� ��������� ����� ����� ����������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int Integral::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
		minLevel = minLevelLimit;
		maxLevel = minLevelLimit; // maxLevelLimit;
		left = 0; //leftLimit;
		right = 0; //rightLimit;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((maxLevel+levelDelta - maxLevelLimit) > eps) {
				if ((minLevel+levelDelta - maxLevelLimit) > eps) {
					if (left >= leftLimit) {
						if (right >= rightLimit)
							return -1;
						else {
							right++;
							left = 0;
							minLevel = minLevelLimit;
							maxLevel = minLevelLimit;
						}
					} else {
						left++;
						minLevel = minLevelLimit;
						maxLevel = minLevelLimit;
					}
				} else {
					minLevel += levelDelta;
					maxLevel = minLevel;
				}
			} else
				maxLevel += levelDelta;
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((maxLevel-levelDelta - minLevel) < -eps) {
				if ((minLevel-levelDelta - minLevelLimit) < -eps) {
					if (left == 0) {
						if (right == 0)
							return -1;
						else {
							right--;
							left = leftLimit;
							minLevel = maxLevelLimit;
							maxLevel = maxLevelLimit;
						}
					} else {
						left--;
						minLevel = maxLevelLimit;
						maxLevel = maxLevelLimit;
					}
				} else {
					minLevel -= levelDelta;
					maxLevel = maxLevelLimit;
				}
			} else
				maxLevel -= levelDelta;
		}
		return 0;
	}

	return -1;
}
