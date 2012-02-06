/****************************************************************************
*			������� ������ RefMarkupSimple.h
*
*	Description:	��������� �������� - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "RefMarkupSimple.h"
#include "Defines.h"
using namespace AxiomLib;

// ����������� ������ � ��������� ��������� ��������
RefMarkupSimple::RefMarkupSimple () {
	axiomTypes.clear();
	maxDistance = 0.0;
	dtwUse = false; // ��������� �� ��������� ������������ ������� ������� ��� �������� �� ������ DTW
	// ����� �� ���������� DTW ������������ - ���������� ����� ���� �� ���� �� ���������� ��� ������������ ��� dtwUse = True
	// ���� ��� ����� true - �� ������������� ��������� ���� �� ���, � ������ ��� �� ����� �������� - �� ��� ���������� �� ����� ������
	useDTWwideSearch = true; 
	useDTWdeepSearch = false;
	delUse = true;
	// ������� ������� �������������
	delPrice.resize(2);
	delPrice [0] = 1.0; // ���� �������� �������� ������� �� �������� ����
	delPrice [1] = 1.0; // ���� �������� ���������� ������� �� �������� ����
	insUse = true;
	insPrice.resize(2);
	insPrice [0] = 1.0; // ���� ������� �������� ������� � �������� ����
	insPrice [1] = 1.0; // ���� ������� ���������� ������� � �������� ����
	chgUse = true;
	chgPrice.resize(3);
	chgPrice [0] = 1.0; // 2.0; // ���� ������ �������� ������� � �������� ���� �� ���������
	chgPrice [1] = 1.0; // 2.0; // ���� ������ ���������� ������� � �������� ���� �� ���������
	chgPrice [2] = 1.0; // 2.0; // ���� ������ ���������� ������� � �������� ���� �� �������
	// ������� ������� ������� - ������� ���� �� �������� �������
	averDel = 0.0;
	for (unsigned int i = 0; i < delPrice.size(); i++) averDel += (float) delPrice[i];
	averDel /= delPrice.size();
	// ������� �������� �� ������� �������
	averIns = 0.0;
	for (unsigned int i = 0; i < insPrice.size(); i++) averIns += (float) insPrice[i];
	averIns /= insPrice.size();
	// ������� �������� �� ��������� �������
	averChg = 0.0;
	for (unsigned int i = 0; i < chgPrice.size(); i++) averChg += (float) chgPrice[i];
	averChg /= chgPrice.size();
	// ��������� �� �������� �������� ����������, ������� �� ����� ������������� �������� � ��� ������������� ����� ��������������
	a1 = 0.0; 
	a2 = 0.0;
	b1 = 0.0;
	b2 = 0.0;
	// ��� ������������ �� �������� ������� ������������ ����� � ������� ���������� � ��������� DTW
	// ������ �� ������������� �������� ��������� ������ ������� ������� ����� ��� ��������� DTW 
	//  ��������, ���� truncationAxioms = 0.0  - �� ��������� ����������� �������� �� �����
	//							= 1.0 - �� ��������� �� ������ ���� � ������� ����������
	// ����� ����� - � ����� � ��������� ���� - �������� ����������� �� ������� [0.1, 0.9] ! 
	// 			   ����� �������� ������ ��������� ����� � ������� ���������� ��� ��������� ������� �� ����� ������� � 0.
	truncationAxioms = 0.5;
	truncationRow = 0.5;
	// ��������� ������ � ���������� � �������� ������� ������ ������������ ��������� ��������
	float coef1 = (float) 0.9; 
	float coef2 = (float) 1.1;
}


/****************************************************************************
*                       RefMarkupSimple::getCopy
*
*       Description:	������ ������� ��������� ��������
*       Parameters:		axTypes - ����������� ������ ��������� ��������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int RefMarkupSimple::getCopy (std::vector<int> &axTypes) const {
	axTypes = axiomTypes;
	return 0;
}


/****************************************************************************
*                       RefMarkupSimple::setParamsFromEnv
*
*       Description:	���������������� ��������� ������� �� env
*       Parameters:	const Environment& env - �����
*       Returns:		0
*       Throws:		AxiomLibException - ���� ��� ������-�� ���������
*       Author:		dk
*       History:
*
****************************************************************************/
int RefMarkupSimple::setParamsFromEnv (const Environment& env) {
	std::string tempStr;
	double tempDB1, tempDB2, tempDB3;
	
	if (env.getDoubleParamValue(tempDB1, "MaxDistance") >= 0) {
		if (tempDB1 < 0) {
			throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : wrong value of MaxDistance in config file.");
		} else {
			maxDistance = tempDB1;
		}
	}
	
	if (env.getStringParamValue(tempStr, "Recognizer") < 0)
		throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : recognizer not set.");
	if ( (tempStr == "DTW") || (tempStr == "dtw") ) {
		dtwUse = true;
		useDTWwideSearch = true;  // �������� ��������������� �� ��������� - ��� ��� ����� � ������� ��� ������� ��������� ����� ��������� ��������� ������ � �������
		useDTWdeepSearch = false;
	} else {
		if ( (tempStr == "Metric") || (tempStr == "metric") || (tempStr == "METRIC") ) { 
			dtwUse = false; // ��������� ������������ ������� �������
		} else {
			throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : recognizer not set.");
		}
	}
	// ������� ������� �������������
	if (env.getStringParamValue(tempStr, "DelUse") >= 0) {
		if ( (tempStr == "true") || (tempStr == "True") || (tempStr == "TRUE") ) {
			if ( (env.getDoubleParamValue(tempDB1, "DelPriceNull") < 0) || (env.getDoubleParamValue(tempDB2, "DelPriceNoNull") < 0) ) 
				throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : delete costs are not set.");
			setDelPrice (true, tempDB1, tempDB2);
		}
		if ( (tempStr == "false") || (tempStr == "False") || (tempStr == "FALSE") ) {
			delUse = false;
		}
	}
	
	if (env.getStringParamValue(tempStr, "InsUse") >= 0) {
		if ( (tempStr == "true") || (tempStr == "True") || (tempStr == "TRUE") ) {
			if ( (env.getDoubleParamValue(tempDB1, "InsPriceNull") < 0) || (env.getDoubleParamValue(tempDB2, "InsPriceNoNull") < 0) ) 
				throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : insert costs are not set.");
			setInsPrice (true, tempDB1, tempDB2);
		}
		if ( (tempStr == "false") || (tempStr == "False") || (tempStr == "FALSE") ) {
			insUse = false;
		}
	}
	
	if (env.getStringParamValue(tempStr, "ChgUse") >= 0) {
		if ( (tempStr == "true") || (tempStr == "True") || (tempStr == "TRUE") ) {
			if ( (env.getDoubleParamValue(tempDB1, "ChgNullToNoNull") < 0) || (env.getDoubleParamValue(tempDB2, "ChgNoNullToNoNull") < 0) || (env.getDoubleParamValue(tempDB3, "ChgNoNullToNull") < 0) ) 
				throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : change costs are not set.");
			setChgPrice (true, tempDB1, tempDB2, tempDB3);
		}
		if ( (tempStr == "false") || (tempStr == "False") || (tempStr == "FALSE") ) {
			chgUse = false;
		}
	}
	
	if (env.getDoubleParamValue(tempDB1, "TruncationRef") >= 0) {
		if ( (tempDB1 >= 0.1) && (tempDB1 <= 0.9) ) {
			truncationAxioms = (float) tempDB1; 
		} else {
			throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : wrong value of truncationAxioms in config file.");
		}
	}
	
	if (env.getDoubleParamValue(tempDB1, "TruncationRow") >= 0) {
		if ( (tempDB1 >= 0.1) && (tempDB1 <= 0.9) ) {
			truncationRow = (float) tempDB1; 
		} else {
			throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : wrong value of truncationRow in config file.");
		}
	}
	
	if (env.getDoubleParamValue(tempDB1, "CompressionDegree") >= 0) {
		if ( (tempDB1 > 0.0) && (tempDB1 <= 1.0) ) {
			coef1 = (float) tempDB1; 
		} else {
			throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : wrong value of CompressionDegree in config file.");
		}
	}
	
	if (env.getDoubleParamValue(tempDB1, "StretchingDegree") >= 0) {
		if (tempDB1 >= 1.0) {
			coef2 = (float) tempDB1; 
		} else {
			throw AxiomLibException("RefMarkupSimple::setParamsFromEnv : wrong value of StretchingDegree in config file.");
		}
	}
	
	return 0;
}

// ������ (����) ����������
RefMarkupSimple::~RefMarkupSimple () {
}


// ������� ���������� ����� ������� � ��������� ���������
int RefMarkupSimple::size (void) const {
	return (int) axiomTypes.size();
}


// �������� ���� ����������� ��������� �� ������
int RefMarkupSimple::clear () {
	axiomTypes.clear();
	return 0;
}


/****************************************************************************
*					RefMarkupSimple::setDTWUse
*
*	Description:	������� ������� �������� ���������� dtwUse
*	Parameters:	newDTWUse - ������������ �� �������� DTW ��� ����������� ���������� ����� ������������� ��������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSimple::setDTWUse (bool newDTWUse) {
	dtwUse = newDTWUse;
}


/****************************************************************************
*					RefMarkupSimple::setDelPrice
*
*	Description:	������� ������� ��� �� �������� ��������
*	Parameters:	newUse - ������������ �� ��������
*				delNull - ���� �������� �������� �������
*				delNoNull - ���� �������� ���������� �������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSimple::setDelPrice (bool newUse, double delNull, double delNoNull) {
	delUse = newUse;
	delPrice[0] = delNull;
	delPrice[1] = delNoNull;
}


/****************************************************************************
*					RefMarkupSimple::setInsPrice
*
*	Description:	������� ������� ��� �� �������� �������
*	Parameters:	newUse - ������������ �� ��������
*				insNull - ���� ������� �������� �������
*				insNoNull - ���� ������� ���������� �������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSimple::setInsPrice (bool newUse, double insNull, double insNoNull) {
	insUse = newUse;
	insPrice[0] = insNull;
	insPrice[1] = insNoNull;
}


/****************************************************************************
*					RefMarkupSimple::setChgPrice
*
*	Description:	������� ������� ��� �� �������� ������
*	Parameters:	newUse - ������������ �� ��������
*				chgNullToNoNull - ���� ��������� �������� ������� �� ���������
*				chgNoNullToNoNull - ���� ��������� ���������� ������� �� ���������
*				chgNoNullToNull - ���� ��������� ���������� ������� �� �������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSimple::setChgPrice (bool newUse, double chgNullToNoNull, double chgNoNullToNoNull, double chgNoNullToNull) {
	chgUse = newUse;
	chgPrice[0] = chgNullToNoNull;
	chgPrice[1] = chgNoNullToNoNull;
	chgPrice[2] = chgNoNullToNull;
}


/****************************************************************************
*					RefMarkupSimple::create
*
*	Description:	������� ���������� ���������� ������
*	Parameters:	newAxiomTypes - ������ �������� ���� ���������
*	Returns:		void
*	Throws:		AxiomLibException - ���� ������� ��������� ������������ �����
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSimple::create (std::vector<signed int> &newAxiomTypes) {
	// �������� ������������ ����������
	if (newAxiomTypes.size() <= 0) {
		throw AxiomLibException ("Error in RefMarkupSimple::create: wrong parameters to create RefMarkupSimple. ");
	}
	// ��������� ���������� ������� ������
	this->axiomTypes.resize(0);
	this->axiomTypes = newAxiomTypes;
}


/****************************************************************************
*					RefMarkupSimple::operator ==
*
*	Description:	������� ������� ��������� ���� ��������
*	Parameters:	second - ������ ������������ �������
*	Returns:		bool - ��������� ������� ���������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
bool RefMarkupSimple::operator == (const RefMarkupSimple& second) const {
	if ( this->axiomTypes.size() != second.size() ) return false;
	for (unsigned int i = 0; i < this->axiomTypes.size(); i++) {
		if (this->axiomTypes[i] != second.axiomTypes[i]) return false;
	}
	return true;
}


/****************************************************************************
*					RefMarkupSimple::operator ==
*
*	Description:	������� ������� ��������� ���� ��������
*	Parameters:	second - ������ ������������ ������ ����� ������
*	Returns:		bool - ��������� ������� ���������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
bool RefMarkupSimple::operator == (std::vector <int>& second) const {
	if ( axiomTypes.size() != second.size() ) return false;
	for (unsigned int i = 0; i < axiomTypes.size(); i++) {
		if (axiomTypes[i] != second[i]) return false;
	}
	return true;
}


/****************************************************************************
*					RefMarkupSimple::check 
*
*	Description:	������� �������� - �������� �� ��������
*				������ � ������� �������� �� ������ ���������.
*				�� ���� ����������� - �������� �� � ����� �������� ���� ������ 
*				������� ������� ������. (���������� � ����� - �������������,
*				��� ��������� ������� �������� 
*				�������� ���� ������ ��������� � ��������� �������� 
*				��������� ��������)
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*				newMaxDistance - ������������ ���������� ����� ��������� ����� 
*	Returns:		0 - ���� �������� �� ���������
*				1 - ���� �������� ��������� � �������� ���������
*	Throws:		AxiomLibException - ���� ����� ����������� �������� upTo
*	Author:		dk
*	History:
*
****************************************************************************/
int RefMarkupSimple::check (const double newMaxDistance, std::vector<int> &rowAxiomTypes, long int upTo) {
	// �������� �� ������������ ������� ������
	if (upTo >= (long int) rowAxiomTypes.size()) {
		throw AxiomLibException ("Error in RefMarkupSimple::check: wrong parameters of check functuion");
	}
	// ����� �������� � ����
	double toReturn;
	maxDistance = newMaxDistance;
	if (dtwUse) {
		toReturn = subcheckDTW (rowAxiomTypes, upTo);
	}
	else {
		toReturn = subcheck (0.0, rowAxiomTypes, upTo, (int) axiomTypes.size() - 1);
	}
	if (toReturn < maxDistance) return 1;
	return 0;
}


/****************************************************************************
*					RefMarkupSimple::subcheck 
*
*	Description:	��������������� ������� ��� check - ����������� ����� 
*				������� �������� ���������� �������� - ��� ����� 
*				����, ��� ����� ��������� ������� ����� ���� ������ 0.
*				�������� �� ������ ������� ��� ��������� ���������� ����� ����������
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				axiomLen - ��������� ��������
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*				curEnd - ������� ��������� � ��������� �������� ��� ���������
*	Returns:		0 - false
*				1 - true
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double RefMarkupSimple::subcheck (const double curDist, std::vector<int> &rowAxiomTypes, long int upTo, int curEnd) {
	if (curEnd < 0) {
		return 0;
	}
	long int j = upTo;
	// ���� ������� ��������� ����� �������� ������ ����������� ����������� - �� �� ���������� ��������� - � ��� ������� - ��� ���������� � �������� ���������
	if (curDist > maxDistance) return curDist;
	for (int i = curEnd; i >= 0; i--) {
		// �������� ��������� ��������� ������������������ � ��������
		if (axiomTypes[i] != rowAxiomTypes[j]) {
			// ����������� ���� ������ - ���������� ��������� �������� ������ ���������� - ��� ������������ ���������� ����� ������������� ��������
			std::vector <double> dist; // ��������� �������� ���������� ����� ����� ��������� ��������
			dist.resize(3);
			for (unsigned int k = 0; k < dist.size(); k++) dist[k] = -1.0;
			// � ����������� �� ����, ����� �������� ������������  - ��������� ����� ����������� ����������
			if (delUse) {
				if (rowAxiomTypes[j] == -1) // ��� ������ ��� ������� ������� -  �������
					dist[0] = subcheck (delPrice[0] + curDist, rowAxiomTypes, j-1, i);
				else 
					dist[0] = subcheck (delPrice[1] + curDist, rowAxiomTypes, j-1, i);
			}
			if (insUse) {
				if (axiomTypes[i] == -1)
					dist[1] = subcheck (insPrice[0] + curDist, rowAxiomTypes, j, i-1);
				else 
					dist[1] = subcheck (insPrice[1] + curDist, rowAxiomTypes, j, i-1);
			}
			if (chgUse) {
				if (axiomTypes[i] == -1)
					dist[2] = subcheck (chgPrice[0] + curDist, rowAxiomTypes, j-1, i-1);
				else {
					if (axiomTypes[i] == -1)
						dist[2] = subcheck (chgPrice[2] + curDist, rowAxiomTypes, j-1, i-1);
					else 
						dist[2] = subcheck (chgPrice[1] + curDist, rowAxiomTypes, j-1, i-1);
				}
			}
			// ����� ����������� ���������� �� ����������
			int wayToDo = 0;
			for (int f = 1; f  < (int) dist.size(); f++) {
				if ( (dist[f] < dist[wayToDo]) || (dist[wayToDo] == -1) ){
					wayToDo = f;
				}
			}
			// �������� - ��� ���� �� ���� ���������� ���������
			if (dist[wayToDo] < 0)
				throw AxiomLibException ("Error in RefMarkupSimple::subcheck: no operation to use. ");
			// ���������� ���������
			return dist[wayToDo];
		}
		// ���������� �� ���� - ��� ��������� ��������� �������
		j--;
		if ((j < 0) && (i != 0)) {
			// ��� ������� �������� ����� ���-�� ����������
			return curDist + i*delPrice[0]; // ���������� ���� �������� ���� ���������� ��� ������������ �������� � ��������� ��������
		}
	}
	// ��� �� ���������� �� ������� ������ - ������ �������� ������� ���������� - ������� � ������� ��������� ����������
	return curDist;
}


/****************************************************************************
*					RefMarkupSimple::subcheckDTW
*
*	Description:	��������������� ������� ��� check - ������� ���������� ����� 
*				������� ��������� �������� � ���������� �������������� ����, 
*				������� ������������� � upTo.
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*	Returns:		double - ���������� ����� ���������
*	Throws:		AxiomLibException - ���� �� ������ ���� ������� �� ��� 
*							       �������� ������ �������� ��������� ����������
*	Author:		dk
*	History:
*
****************************************************************************/
double RefMarkupSimple::subcheckDTW (std::vector<int> &rowAxiomTypes, long int upTo) {
	// ����� ������ �������
	//  - ����������� ��������� ���������� �������� ������������� ���� ��� ��������� � ��������� ���������, ������� ������������� � upTo
	//  - ������ ������ ����������� ���������� ��� ������ �� �������� 
	//  - ����� ����������� ����������
	const bool debug = false;
	// �������� �� ������� ����� �������� ��������� ����������
	if (axiomTypes.size() < 1) throw AxiomLibException ("Error in RefMarkupSimple::subcheckDTW: RefMarkupSimple not initialised.");
	// ����������� ������ � ������� ������ ������������
	long int leftMin, leftMax;
	leftMin = upTo - (long int) (axiomTypes.size() * coef1);
	if (leftMin < 0) leftMin = 0;
	leftMax = upTo - (long int) (axiomTypes.size() * coef2);
	if (leftMax < 0) leftMax = 0;
	// ��� ������������� - ������ �������� ������� ���, ����� leftMax ��� ������ leftMin
	if (leftMin < leftMax) {
		long int temp = leftMax;
		leftMax = leftMin;
		leftMin = temp;
	}
	// ������� ����������
	std::vector < std::vector <float> > distMatrix;
	distMatrix.resize(axiomTypes.size());
	// ���������� ������� ����������
	for (int i = 0; i < (int) distMatrix.size(); i++) {
		(distMatrix[i]).resize(upTo - leftMax + 1);
		for (int j= 0; j < (int) (distMatrix[i]).size(); j++) {
			distMatrix[i][j] = distance (axiomTypes[i], rowAxiomTypes[upTo-j]);
		}
	}
	// �������� ������� ��������������� �����������
	std::vector <double> preResult;
	preResult.resize(leftMin - leftMax + 1);
	// ������ ��������������� ������� ���������� � distMatrix ������� DTW � ������ ����������� � preResult
	// ����� DTW-�������� ������������ - ������������ ��� �� � ���� �������
	runDistDTW (preResult, distMatrix);
	// ����� ����������� ���������� � ��� ������� � �������� ����������
	double curDist;
	curDist = preResult[0];
	for (unsigned int g = 1; g < preResult.size(); g++) {
		if (curDist > preResult[g]) curDist = preResult[g];
	}
	if (debug) {
		std::cout << "\r	curDist is " << curDist /*<< "		preResult.size() is " << preResult.size() */<< "\r";
	}
	return curDist;
}


/****************************************************************************
*					RefMarkupSimple::distance
*
*	Description:	��������������� ������� ��� subcheckDTW - 
*				����������� ���������� ����� ����� ������� 
*	Parameters:	symbA - ������ ������������ ������
*				symbB - ������ ������������ ������
*	Returns:		float - ���������� ����� ���������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
inline float RefMarkupSimple::distance (int symbA, int symbB) {
	if (symbA == symbB) return (float) 0.0;
	if (symbA == -1) return (float) chgPrice[2];
	if (symbB == -1) return (float) chgPrice[0];
	return (float) chgPrice[1];
}


/****************************************************************************
*					RefMarkupSimple::runDistDTW
*
*	Description:	��������������� ������� ��� check - ������� ���������� ����� 
*				������� ��������� �������� � ���������� �������������� ����, 
*				������� ������������� � upTo.
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*	Returns:		int
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int RefMarkupSimple::runDistDTW (std::vector <double> &preResult, std::vector < std::vector <float> > &distMatrix) {
	const bool debug = false;
	if (useDTWwideSearch)  {
		// ������� �������� ����������, ����������� ��� ����������� ��� ������ ����������� ����������
		// ����������� �������� - ��� ��������� ������������ ����� � ������� ����������, ����� ������� ���� ������ ��� ������ ����
		// ���������� ��� ������� ������������ ���� ������ ���������(a1 � b1) � ������� (a2 � b2)
		a1 = axiomTypes.size() * truncationAxioms / (truncationRow * (distMatrix[0]).size());
		a2 = axiomTypes.size() * (1- truncationAxioms);
		b1 = axiomTypes.size() * truncationAxioms / (truncationRow * ((distMatrix[0]).size() - preResult.size()));
		b2 = b1* ((truncationRow - 1) * (distMatrix[0]).size() - truncationRow * preResult.size());
		// ����� ������ ���������� � ������� distMatrix
		distDTWwideSearch (preResult, distMatrix);
		return 1;
	}
	if (useDTWdeepSearch)  {
		int deep;
		for (int t = 0; t < (int) preResult.size(); t++) {
			// ������� �������� ����������, ����������� ��� ����������� ��� ������ ����������� ����������
			// ����������� �������� - ��� ��������� ������������ ����� � ������� ����������, ����� ������� ���� ������ ��� ������ ����
			// ���������� ��� ������� ������������ ���� ������ ���������(a1 � b1) � ������� (a2 � b2)
			a1 = axiomTypes.size() * truncationAxioms / (truncationRow * ( (distMatrix[0]).size() - t ) );
			a2 = axiomTypes.size() * (1- truncationAxioms);
			b1 = a1;
			b2 = a1* (truncationRow - 1) * ((distMatrix[0]).size() - t);
			deep = 1; // �������� �� ��������� ��� ����� ���� � ������� ����������
			preResult[t] = distDTWdeepSearch (0.0, deep, distMatrix, 0, t);
			// ������������ ���� - �� ��� ����� � ����������� ��������� DTW
			preResult[t] = sqrt (preResult[t]) / (double) deep; // ���, ���� ���������� ���������� ���������� ����� ��������, �� ������������ �������� ����� �������.
		}
		return 2;
	}
	if (debug) {
		std::cout << "\n	useDTWdeepSearch is " << useDTWdeepSearch;
		std::cout << "\n	useDTWwideSearch is " << useDTWwideSearch;
	}
	throw AxiomLibException ("Error in RefMarkupSimple::runDistDTW: No DTW algorithm was chosen to recognize.");
	return 0;
}


/****************************************************************************
*					RefMarkupSimple::distDTWdeepSearch
*
*	Description:	��������������� ������� ��� subcheckDTW - 
*				����������� ���������� ����� ����� ����������� 
*				� �������������� ��������� DTW - ����� ������ � �����
*	Parameters:	curDist - ������� ���������� ����� ������������� �������� - ���������� ����� �� ���������� �����������
*				curDeep - ������� �������� ������� ���������� ����
*				distMatrix - ������� ���������� ����� ��������� �����
*				i - ������� ��������� ��������, � �������� ���� ������� ���������� 
*				j - ������� � ������������� ������, � �������� ���� ������� ����������
*	Returns:		double - ���������� ����� �����������
*	Throws:		AxiomLibException -  ���� ������ � ������ ������ ����������� ���������� ����� ����� ���������� ������� � ���������, 
*								����� ��� ���������� ��� �������� �� ������� � ����� ���������� �������� ������������� ��������
*	Author:		dk
*	History:
*
****************************************************************************/
double RefMarkupSimple::distDTWdeepSearch (const double curDist, int &curDeep, std::vector < std::vector <float> > &distMatrix, const int i, const int j) const {
	bool debug = false;
	if (debug) std::cout << "\n	( " << i << ", " << j << " )";
	// ���� �������� �������� �������� - �� ���������� ���������� ��������� - �������� ���������������
	if ( ( i == ( distMatrix.size() - 1 ) )  && ( j == ( (distMatrix[i]).size() - 1) ) ) {
		curDeep = 1; // ������� ������� �� ������ ������
		return curDist + distMatrix[i][j];
	}
	// ��������� ����� �� �� ���� ����� - ���� �� - �� ������� ���� ���� ������ ����
	double up = -1.0;
	int curDeepUp = curDeep;
	if ( ( (i + 1.5) - (j+0.5)*a1- a2 < 0.01) && ( (i + 1.5) - (j+0.5)*b1 - b2 > -0.01) && ( (i + 1) < (int) distMatrix.size()) ) {
		if (debug)std::cout << "	up";
		up = distDTWdeepSearch(curDist + distMatrix[i][j], curDeepUp, distMatrix, i+1, j);
	}
	// ��������� ����� �� �� ���� ������ - ���� �� - �� ������� ���� ���� ������ ����
	double right = -1.0;
	int curDeepRight = curDeep;
	if ( ((i+0.5) - (j + 1.5)*a1- a2 < 0.01) && ((i+0.5) - (j + 1.5)*b1 - b2 > -0.01) && ((j + 1) < (int) (distMatrix[i]).size()) ) {
		if (debug)std::cout << "	right";
		right = distDTWdeepSearch(curDist + distMatrix[i][j], curDeepRight, distMatrix, i, j+1);
	}
	// ��������� ����� �� �� ���� �� ��������� - ���� �� - �� ������� ���� ���� ������ ����
	double diag = -1.0;
	int curDeepDiag = curDeep;
	if ( ( (i+1.5) - (j + 1.5)*a1- a2 < 0.01) && ( (i+1.5) - (j + 1.5)*b1 - b2 > -0.01) && ((j + 1) < (int) (distMatrix[i]).size()) && ( (i + 1) < (int) distMatrix.size()) ) {
		if (debug)std::cout << "	diag";
		diag = distDTWdeepSearch(curDist + distMatrix[i][j], curDeepDiag, distMatrix, i+1, j+1);
	}
	// �������� � up ���������� ��������
	curDeep = curDeepUp;
	if (right >= -0.01) {
		if ( (right < up) || (up < -0.01) ) { 
			up = right;
			curDeep = curDeepRight;
		}
	}
	if (diag >= -0.01) {
		if ( (diag < up) || (up < -0.01) ) {
			up = diag;
			curDeep = curDeepDiag;
		}
	}
	// ��������� - ������� �� ���� ���� ����� �� �������� ������������� ��������
	if (up < -0.01) {
		if (debug) {
			std::cout << "\n 	 up is " << up << "\n";
			std::cout << " 	 curDeep is " << curDeep << "\n";
			std::cout << " 	 i is " << i << "\n";
			std::cout << " 	 j is " << j << "\n";
			std::cout << " 	 distMatrix.size() is " << distMatrix.size() << "\n";
			std::cout << " 	 (distMatrix[i]).size() is " << (distMatrix[i]).size() << "\n";
			std::cout << " 	 (i + 1.5) - (j+0.5)*a1- a2 is " << (i + 1.5) - (j+0.5)*a1- a2 << "\n";
			std::cout << " 	 (i + 1.5) - (j+0.5)*b1 - b2 is " << (i + 1.5) - (j+0.5)*b1 - b2  << "\n";
			std::cout << " 	 (i+0.5) - (j+1.5)*a1- a2 is " << (i+0.5) - (j+1.5)*a1- a2 << "\n";
			std::cout << " 	 (i+0.5)- (j+1.5)*b1 - b2 is " << (i+0.5)- (j+1.5)*b1 - b2  << "\n";
			std::cout << " 	 (i + 1.5) - (j+1.5)*a1- a2 " << (i + 1.5) - (j+1.5)*a1- a2 << "\n";
			std::cout << " 	 (i + 1.5) - (j+1.5)*b1 - b2 is " << (i + 1.5) - (j+1.5)*b1 - b2  << "\n"; 
		}
		 throw AxiomLibException ("Error in RefMarkupSimple::distDTWdeepSearch: wrong result of subfunction distDTWdeepSearch. ");
	}
	// ���������� ���������� ���������� ����� ��������� � (i,j) � (max_i, max_j) - ������� ������������ �������
	if (debug) std::cout << "	down";
	curDeep++;
	return up;
}


/****************************************************************************
*					RefMarkupSimple::distDTWwideSearch
*
*	Description:	��������������� ������� ��� subcheckDTW - 
*				����������� ���������� ����� ����� ����������� 
*				� �������������� ��������� DTW - ����� ������ � ������
*	Parameters:	preResult - ������ ��� ������ ��������������� ����������� ���������� �� ������� distMatrix
*				distMatrix - ������� ���������� ����� ��������� �����
*	Returns:		double - ������� ��� ������������ ������ � ������� � �� ����� ����������
*	Throws:		AxiomLibException - ���� ��������� ������ ��������� � ������ � ������
*	Author:		dk
*	History:
*
****************************************************************************/
double RefMarkupSimple::distDTWwideSearch (std::vector <double> &preResult, std::vector < std::vector <float> > &distMatrix) const {
	const bool debug = false;
	// �������������� ��������� ����������
	const int dimY = (int) distMatrix.size();
	const int dimX = (int) distMatrix[distMatrix.size() - 1].size();
	// ������� ������� ��� ������ ���� (������� ���������� � ������� ������������) � ������������� ��
	std::vector < std::vector <float> > nomin; // ��������� ��� ������� � ������ ��������� - ��� �������� ���������� 
	std::vector <std::vector <float> > denom; // � ��� �������� ������������ (���������� ����� ����� ���� � ���������� DTW-���������� �� ������ ������) - ����������� �� ������� ���� ���������� 
	nomin.resize(dimY);
	denom.resize(dimY);
	for (int i = 0; i < dimY; i++) {
		(nomin[i]).resize(dimX);
		(denom[i]).resize(dimX);
	}
	nomin[dimY - 1][dimX - 1] = distMatrix[dimY - 1][dimX - 1];
	denom[dimY - 1][dimX - 1] = 1.0;
	// �������� ������� ������������ - ��� �������� Ture � ��� �������, ������� ����� ������� � ���������� ����
	std::vector <std::vector <bool> > permit; 
	permit.resize(dimY);
	for (int i = 0; i < dimY; i++) {
		(permit[i]).resize(dimX);
		for (int j = 0; j < dimX; j++) 
			permit[i][j] = checkInRange(i, j);
	}
	// ������ ��������� ������ ����������� ����������
	// ���������� �������� ������� ������� - ���������� ������ � ������ ������� ������� �� ����������� ������
	for (int i = dimY - 2; i >= 0; i--) {
		if (permit[i][dimX-1]) {
			denom[i][dimX-1] = denom[i+1][dimX-1] + 1;
			if (distMatrix[i][dimX-1] < 0.01)
				nomin[i][dimX-1] = nomin[i+1][dimX-1];
			else
				nomin[i][dimX-1] = nomin[i+1][dimX-1] + averIns;
		}
		else
			break;
	}
	// ���������� ������� ������ - ���������� ������ � ������ ������ ������� �� ����� ��������
	for (int j = dimX - 2; j >=0; j--) {
		if (permit[dimY-1][j]) {
			denom[dimY-1][j] = denom[dimY-1][j+1] + 1;
			if (distMatrix[dimY-1][j] < 0.01)
				nomin[dimY-1][j] = nomin[dimY-1][j+1];
			else
				nomin[dimY-1][j] = nomin[dimY-1][j+1] + averDel;
		}
		else
			break;
		
	}
	// ���������� ���������� ������ ������ nomin � denim
	int j = 0; 
	float tempUp, tempDiag, tempRight;
	for (int sum = dimX + dimY - 4; sum >=0; sum--) {
		if (debug) std::cout << "\n	sum is " << sum << "	dimX is " << dimX << "	dimY is " << dimY;
		for (int i = min (sum, dimY - 2); i >= 0; i--) {
			j = sum - i; 
			// ���� ������ ������� �� ������� - �� ����������� �� ����� ���������� ��������
			if (j > (dimX - 2)) break;
			// ���� ������ � ������ - ����������� ����������� �������������
			if (permit[i][j] == false) continue;
			if (debug) std::cout << "\n	( " << i << ", " << j << " )" << "	sum is " << sum << "	dimX is " << dimX << "	dimY is " << dimY;
			// ���� ������� ����������� - �� ��������� ��������� ����� ����� ��������� �����������
			// ���� ������ ������
			if (permit[i+1][j]) 
				if (distMatrix[i][j] < 0.01) tempUp = (float) ((nomin[i+1][j]) / (denom[i+1][j] + 1.0));
				else tempUp = (float) ((nomin[i+1][j] + averIns) / (denom[i+1][j] + 1.0));
			else
				tempUp = -1.0;
			// ���� ������ ������
			if  (permit[i][j+1]) 
				if (distMatrix[i][j] < 0.01) tempRight = (float) ((nomin[i][j+1]) / (denom[i][j+1] + 1.0));
				else tempRight = (float) ((nomin[i][j+1] + averDel) / (denom[i][j+1] + 1.0));
			else
				tempRight = -1.0;
			// ���� ������ �� ���������
			if  (permit[i+1][j+1]) 
				if (distMatrix[i][j] < 0.01) tempDiag = (float) ((nomin[i+1][j+1]) / (denom[i+1][j+1] + 1.0));
				else tempDiag = (float) ((nomin[i+1][j+1] + averChg) / (denom[i+1][j+1] + 1.0));
			else
				tempDiag = -1.0;
			// �������� ���� � ���������� ���������� � ��� ���������� � ������ ������
			if ( (tempDiag > -0.01) && ((tempUp >= tempDiag) || (tempUp < -0.01)) && ((tempRight >= tempDiag) || (tempRight < -0.01)) ) {
				denom[i][j] = denom[i+1][j+1] + (float) 1.0;
				if (distMatrix[i][j] < 0.01) nomin[i][j] = nomin[i+1][j+1];
				else nomin[i][j] = nomin[i+1][j+1] + averChg;
				continue;
			}
			if ( (tempRight > -0.01) && ( (tempUp >= tempRight)|| (tempUp < -0.01) ) ) {
				denom[i][j] = denom[i][j+1] + (float) 1.0;
				if (distMatrix[i][j] < 0.01) nomin[i][j] = nomin[i][j+1];
				else nomin[i][j] = nomin[i][j+1] + averDel;
				continue;
			}
			if (tempUp > -0.01) {
				denom[i][j] = denom[i+1][j] + (float) 1.0;
				if (distMatrix[i][j] < 0.01) nomin[i][j] = nomin[i+1][j];
				else nomin[i][j] = nomin[i+1][j] + averIns;
				continue;
			}
			if (debug) {
				std::cout << "\n 	 i is " << i << "\n";
				std::cout << " 	 j is " << j << "\n";
				std::cout << " 	 tempUp is " << tempUp << "\n";
				std::cout << " 	 tempRight is " << tempRight << "\n";
				std::cout << " 	 tempDiag is " << tempDiag << "\n";
				/*std::cout << " 	 a1 is " << a1 << "\n";
				std::cout << " 	 a2 is " << a2 << "\n";
				std::cout << " 	 b1 is " << b1 << "\n";
				std::cout << " 	 b2 is " << b2 << "\n";
				std::cout << "\n	Permit is \n";
				for (int i = permit.size() - 1; i >=0 ; i--) {
					for (int j = 0; j < (permit[i]).size(); j++)
						std::cout << "  " << permit[i][j]; 
					std::cout << "\n";
				}*/
			}
			throw AxiomLibException ("Error in RefMarkupSimple::distDTWwideSearch: no way was selected. ");
		}
	}
	// ��������� ������ preResult ����������� ������������
	for (unsigned int t = 0; t < preResult.size(); t++) {
		preResult[t] = sqrt (nomin[0][t]) / denom[0][t];
	}
	return 0.0;
}


/****************************************************************************
*					RefMarkupSimple::checkInRange
*
*	Description:	��������������� ������� ��� distDTWwideSearch
*				������������ - ������������ �� ���������� ������ � ������� ���������� 
*				� ������������� ����������� �� ���������� ���� � ������ �������
*	Parameters:	i - ���������� � ������� �� ��������� (����� ������) 
*				j - ���������� � ������� �� ����������� (����� �������)
*	Returns:		bool - ��������� �������� �����������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
inline bool RefMarkupSimple::checkInRange (const int i, const int j) const{
	if ( ( (i+0.5) - (j + 0.5)*a1- a2 < 0.01) && ( (i+0.5) - (j + 0.5)*b1 - b2 > -0.01) ) {
		return true;
	}
	return false;
}


// �������� ������� - ����� ��������
int RefMarkupSimple::setOut () {
	std::cout << "\n	RefMarkupSimple::setOut:\n";
	
    std::cout << "\n	dtwUse = " << dtwUse;
    std::cout << "	useDTWwideSearch = " << useDTWwideSearch;
    std::cout << "	useDTWdeepSearch = " << useDTWdeepSearch;
    std::cout << "\n	a1= " << a1 << "	a2 = " << a2 << "	 b1 = " << b1 << "	b2 = " << b2;

    // ���� ��������
    std::cout << "\n	Axioms:";
    for (unsigned int i = 0; i < axiomTypes.size(); i++ ) {
    	std::cout << "   " << axiomTypes[i];
    }
    //std::cout << "\n";
    
    // �������� �������� ������� ������� ���������, �� �������� ������� ������������ ������� ������������� ������� ������� ����� ����������
    std::cout << "\n	maxDistance = " << maxDistance;
    // �������� ����� �������� ������������
    std::cout << "\n	delUse = " << delUse;
    std::cout << "	insUse = " << insUse;
    std::cout << "	chgUse = " << chgUse;
    // ���� �� ��������, �������, ������ ������� � ������ ��������� �������� � � ������ �������� ����.
    std::cout << "\n	delPrice:";
    for (unsigned int i = 0; i < delPrice.size(); i++) {
    	std::cout << " " << delPrice[i];
    }
    std::cout << "\n	insPrice:";
    for (unsigned int i = 0; i < insPrice.size(); i++) {
    	std::cout << " " << insPrice[i];
    }
    std::cout << "\n	chgPrice:";
    for (unsigned int i = 0; i < chgPrice.size(); i++) {
    	std::cout << " " << chgPrice[i];
    }
        
    // ���������� ��� ��������, ������� � ������ �������- ������������ � ��������� DTW � ���������� ������ � ������ 
    std::cout << "\n	averDel = " << averDel;
    std::cout << "	averIns = " << averIns;
    std::cout << "	averChg = " << averChg;
    
    // ������������ �� �������� ������� ������������ ����� � ������� ���������� � ��������� DTW
    std::cout << "\n	truncationAxioms = " << truncationAxioms;
    std::cout << "	truncationRow = " << truncationRow;
    std::cout << "\n	coef1 = " << coef1;
    std::cout << "	coef2 = " << coef2;    
    std::cout << "\n";
	//std::cout << "To continue press Enter.";
	//std::cin.get();
	return 0;
}


/****************************************************************************
*					RefMarkupSimple::operator=
*
*	Description:	�������� ������������
*	Parameters:		������������� ������ ������ ��������� ��������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
RefMarkupSimple& RefMarkupSimple::operator= (const RefMarkupSimple& second) {
	axiomTypes.clear();
	axiomTypes = second.axiomTypes;
	maxDistance = second.maxDistance;
	delPrice.clear();
	delPrice = second.delPrice;
	insPrice.clear();
	insPrice = second.insPrice;
	chgPrice.clear();
	chgPrice = second.chgPrice;
	averDel = second.averDel;
	averIns = second.averIns;
	averChg = second.averChg;
	delUse = second.delUse;
	insUse = second.insUse;
	chgUse = second.chgUse;
	truncationAxioms = second.truncationAxioms;
	truncationRow = second.truncationRow;
	coef1 = second.coef1;
	coef2 = second.coef2;
	dtwUse = second.dtwUse;
	useDTWwideSearch = second.useDTWwideSearch;
	useDTWdeepSearch = second.useDTWdeepSearch;
	return *this;
}
