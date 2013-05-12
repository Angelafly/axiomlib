/****************************************************************************
*
*			������� ������ Variance
*
****************************************************************************/

#include <string>
#include "Variance.h"
#include <stdlib.h>
#include <math.h>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <boost/accumulators/framework/accumulator_base.hpp>
#include <boost/accumulators/framework/parameters/sample.hpp>

#include "../AxiomLibException.h"
#include "../Round.h"
#include "../Defines.h"

using namespace AxiomLib;



#define eps		0.0000000001


/****************************************************************************
*			Variance::Variance
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
Variance::Variance (void) {
	// ���������� �� ��������� �������� ����������
	
	// ����������� ����������� ���������� ������� ������������� �������
//	minLevel = 10.0;
	
	// ������������� ����������� ��������� ������� ������������� �������
	maxLevel = 100.0;
	
	// ����� ����� ��������� ��� ������������� ������� - �����
	left = 0;
	
	// ����� ����� ��������� ��� ������������� ������� - ������
	right = 0;

	// ����������� ������� ��������� ��� ��������� minLevel, ������� ����������� ��� ������� ���������� ���������
	//minLevelLimit = minLevel;
	
	// ������������� ������� ��������� ��� ��������� maxLevel, ������� ����������� ��� ������� ���������� ���������
	maxLevelLimit = maxLevel;

	// ��� ��������� ���������� minLevel � maxLevel, ��� ������� ���������� ���������
	levelDelta = maxLevel;// - minLevel;
	
	// ������������ �������� ��������� left, ������� ����������� ��� ������� ���������� ���������
	leftLimit = left;
	
	// ������������ �������� ��������� right, ������� ����������� ��� ������� ���������� ���������
	rightLimit = right;
}

/****************************************************************************
*			Variance::name
*
*	Description:	������� ���������� ��� �������
*	Parameters:	-
*	Returns:		std::string - ��� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string Variance::name (void) const {
    std::string toReturn("variance");
	return toReturn;
}


/****************************************************************************
*			Variance::check
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
signed int Variance::check (const unsigned long k, const std::vector<double>& x) const{
	signed int toReturn;
	double curRes = 0;
	
	// �������� ������
	if (( (int) k < left) || ( (int) (x.size() - k) <= right)) {
		toReturn = -1;
        //throw AxiomLibException("Error in Variance::check : out of range.");
		return toReturn;
	}

    // �������� ������������� �������

    boost::accumulators::accumulator_set<double,
        boost::accumulators::features<boost::accumulators::tag::variance> > varianceAccumulator;

    for (int i = (int) k - left; i <= (int) k + right; i++) {
        varianceAccumulator(x[i]);
	}
	
    double variance = boost::accumulators::variance(varianceAccumulator);

	if(variance > maxLevel/* || variance < minLevel*/) {
        return 0;
    } else {
        return 1;
    }
}

/****************************************************************************
*			Variance::operator<
*
*	Description:	��������� ���� Variance �� ��������� (x < y).
*	Parameters:	Variance& second - ������ ����� ���������
*	Returns:		1 ���� < � second - ��� Variance
*				-1 ���� > � second - ��� Variance
*				0 ����� (�� ����, ���� ��������� ���������� �� �����, ��� �� 0.01 ��� ���� second - ��� �� Variance)
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double Variance::operator< (const ElemCondition& second) const {
    // �������� �������� y ���� Variance* �� second ���� el_axiom
    const Variance* y = dynamic_cast<const Variance*> (&second);
  
    // ���� y != NULL, �.�. second ������������� ����� ��� Variance&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if (( maxLevel < y->maxLevel + 0.01)/* && ( minLevel > y->minLevel - 0.01)*/)
			return 1;
		else if ((y->maxLevel <  maxLevel + 0.01)/* && (y->minLevel >  minLevel - 0.01)*/)
				return -1;
			else
				return 0;
	}
	else
		return 0;
}

/****************************************************************************
*			Variance::transmute
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
signed int Variance::transmute (const double p) {
	signed int toReturn=0;

	// �������� ������ p
	if (p < 0 || p > 1) {
		return -1;
	}
	
	// �������� ������� ���������
	double change = (((double)rand())/ ((double) RAND_MAX)) * p;
	
	if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
		// ����������� ������� �������
//		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ����������� ������� �������
			maxLevel = maxLevel * (1 + change);
			toReturn = 1;
//		}
//		else {
//			// ��������� ������ �������
//			minLevel = minLevel * (1 - change);
//			toReturn = 1;
//		}
	}
	else {
		// ��������� ������� �������
//		if (((double)rand())/ ((double) RAND_MAX) > 0.5) {
			// ��������� ������� �������
			maxLevel = maxLevel - (maxLevel/*  - minLevel*/) * change;
			toReturn = 1;
//		}
//		else {
//			// ����������� ������ �������
//			minLevel = minLevel + (maxLevel  - minLevel) * change;
//			toReturn = 1;
//		}
	}
	
	// �������� left � right
	// �������� ������� ���������
	double change1 = (((double)rand())/ ((double) RAND_MAX)) * p;
	if (((double)rand())/ ((double) RAND_MAX) > 0.5) { // ��� ��� �������� left � right �� ������
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
*			Variance::setParams
*
*	Description:	������ �������� ���������� ������������� ������� 
*	Parameters:	map<string,string> - ����� �������� ��������� <-> ������� ��������
*	Returns:		0 ���� ��� ok (����� ���� �� ��� ��������� ���� � �����)
*	Throws:		AxiomLibException ���� ����� ������ ��� � ����� ���� ������ ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Variance::setParams (const std::map<std::string,std::string>& newParams) {
	signed int toReturn=0;
	
	// ���������, ������ �� �����?
	if (newParams.size() == 0) {
		toReturn = -1;
        throw AxiomLibException("Error in Variance::setParams : zero parameter map.");
	}
	int flag;
	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string,std::string>::const_iterator i = newParams.begin(); i != newParams.end(); i++) {
		std::string paramName = i->first;
		std::string paramValue = i->second;
		flag = 0;
		// ����, ������ ��������� ������������� i
//		if (paramName == (const std::string)("minLevel")) {
//			// ok, �����, ������� value
//			minLevel = atof(paramValue.c_str());
//			flag = 1;
//		}
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
            throw AxiomLibException("Error in Variance::setParams : unknown parameter.");
		}
	}
	
	return toReturn;
}


/****************************************************************************
*			Variance::getParams
*
*	Description:	���������� �������� ����������
*	Parameters:	map<string,string> - ����������� ����� �������� ��������� <-> ������� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Variance::getParams (std::map<std::string,std::string> &params) const{
	params.clear();
	char temp[64];
//	sprintf( temp, "%f", minLevel);
//	std::string str1 (temp);
//	params["minLevel"] = str1;
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
*			Variance::setParamValue
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
signed int Variance::setParamValue (const double param, const std::string &paramName) {
//	if (paramName == "minLevel") {
//		minLevel = param;
//		return 0;
//	}
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
*			Variance::getParamValue
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
signed int Variance::getParamValue (double &param, const std::string &paramName) const {
//	if (paramName == "minLevel") {
//		param = minLevel;
//		return 0;
//	}
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
*			Variance::getECParamNames
*
*	Description:	������� ��������� ������� ������ �������� � ���������� ���������� ������ ������������ �������
*	Parameters:	ecParamNames - ����������� ������ � ���������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Variance::getECParamNames (std::vector<std::string> &ecParamNames) const {
	ecParamNames.resize(4);
	ecParamNames[0] = "minLevel";
	ecParamNames[1] = "maxLevel";
	ecParamNames[2] = "left";
	ecParamNames[3] = "right";
	return 0;
}


/****************************************************************************
*			Variance::checkAndCompatibility
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
signed int Variance::checkAndCompatibility (const ElemCondition& second) const{
    // �������� �������� y ���� Variance* �� second ���� el_axiom
    const Variance* y = dynamic_cast<const Variance*> (&second);
  
    // ���� y != NULL, �.�. second ������������� ����� ��� Variance&, �� ����� ���-�� ���������
	// ���� y == NULL, �.�. �� ��������� ��� ������ ������� ������� ����, ����� ���������� 0
	if (y != NULL) {
		if ( (( maxLevel >= y->maxLevel) /*&& ( minLevel <= y->maxLevel)*/) /*|| (( maxLevel >= y->minLevel) && ( minLevel <= y->minLevel)) */)
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
*					Variance::setLimits
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
signed int Variance::setLimits (const int newLeftLimit, const int newRightLimit) {
	if ((newLeftLimit < 0) || (newRightLimit < 0))
        throw AxiomLibException("Error in Variance::setLimits : parameters have a wrong values.");
	leftLimit = newLeftLimit;
	rightLimit = newRightLimit;
	return 0; 
}

/****************************************************************************
*					Variance::setLimits
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
signed int Variance::setLimits (const std::vector<double>& row, const double reserve, const unsigned int numOfLevels) {
	if (row.size() < 2)
		return -1;

    boost::accumulators::accumulator_set<double,
        boost::accumulators::features<boost::accumulators::tag::variance> > varianceAccumulator;

    minLevelLimit = -1;
    maxLevelLimit = 0;
	for (unsigned int i = 2; i < row.size(); i++) {

        double mean = (row[i] - row[i-1]) / 2;

        double variance = ((row[i] - mean)*(row[i] - mean) + (row[i-1] - mean)*(row[i-1] - mean))/2;

        minLevelLimit = minLevelLimit >= 0 ? min (variance, minLevelLimit) : variance;
        maxLevelLimit = max (variance, maxLevelLimit);
	}
	if (reserve < 0.001)
		return -1;
	maxLevelLimit *= reserve;
	minLevelLimit /= reserve;
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
*					Variance::changeParams
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
signed int Variance::changeParams (const int numOfSteps) {
	if (numOfSteps == 0) { // ���� ������� �������� = 0 - �� ������������� ��������� �������� ��� ���������� ������
//		minLevel = minLevelLimit;
		maxLevel = minLevelLimit; // maxLevelLimit;
		left = 0; //leftLimit;
		right = 0; //rightLimit;
		return 0;
	}
	if (numOfSteps > 0) { // �������� �� ������������ ����� ����� ������ (����������� ����������)
		for (int num = 0; num < numOfSteps; num++) {
			if ((maxLevel+levelDelta - maxLevelLimit) > eps) {
//				if ((minLevel+levelDelta - maxLevelLimit) > eps) {
					if (left >= leftLimit) {
						if (right >= rightLimit) {
							return -1;
						} else {
							right++;
							left = 0;
//							minLevel = minLevelLimit;
							maxLevel = minLevelLimit;
						}
					} else {
						left++;
//						minLevel = minLevelLimit;
						maxLevel = minLevelLimit;
					}
//				} else {
//					minLevel += levelDelta;
//					maxLevel = minLevel;
//				}
			} else {
				maxLevel += levelDelta;
			}
		}
		return 0;
	} else { // �������� �� ������������ ���������� ����� �����
		for (int num = 0; num > numOfSteps; num--) {
			if ((maxLevel-levelDelta /*- minLevel*/) < -eps) {
//				if ((minLevel-levelDelta - minLevelLimit) < -eps) {
					if (left == 0) {
						if (right == 0)
							return -1;
						else {
							right--;
							left = leftLimit;
//							minLevel = maxLevelLimit;
							maxLevel = maxLevelLimit;
						}
					} else {
						left--;
//						minLevel = maxLevelLimit;
						maxLevel = maxLevelLimit;
					}
//				} else {
//					minLevel -= levelDelta;
//					maxLevel = maxLevelLimit;
//				}
			} else {
				maxLevel -= levelDelta;
			}
		}
		return 0;
	}

	return -1;
}
