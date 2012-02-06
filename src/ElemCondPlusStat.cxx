/****************************************************************************
*					������� ������ ElemCondPlusStat
*
*	Description:	������������ ������� �� ����������� - ����������
*	Author:			dk
*	History:	
*
****************************************************************************/
#include "ElemCondPlusStat.h"
#include "ECStatistics.h"

//#include <boost/archive/xml_iarchive.hpp>
//#include <boost/archive/xml_oarchive.hpp>

using namespace AxiomLib;



// ����������� � �������� �������� ���������� �� ���������
ElemCondPlusStat::ElemCondPlusStat () {
	setDefaultStatistics();
}

ElemCondPlusStat::ElemCondPlusStat(const ElemCondPlus &ecPlus) : ElemCondPlus(ecPlus) {
	setDefaultStatistics();
}

// ������ ����������
ElemCondPlusStat::~ElemCondPlusStat(void) {
	
}

void ElemCondPlusStat::setDefaultStatistics() {
	goal = -1.0;
	statAbnorm = -1.0;
	statNormal = -1.0;
	statOccurence = -1.0;
}

// ������� �������� �������� ���� ����������� ����������
void ElemCondPlusStat::clear(void) {
	ElemCondPlus::clear();
	setDefaultStatistics();
}


/****************************************************************************
*					ElemCondPlusStat::operator=
*
*	Description:	�������� ������������ ������������� ������� �� �����������
*	Parameters:		second - ������������� �������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
//ElemCondPlusStat& ElemCondPlusStat::operator= (const ElemCondPlusStat &second) {
//	// ������� ���������� ���������� ������� ������
//	this->clear();
//	// �������� ���������� ���������� ������
//	this->statAbnorm = second.statAbnorm;
//	this->statNormal = second.statNormal;
//	this->goal = second.goal;
//	this->statOccurence = second.statOccurence;
//	// �������� ���������� ���������� �������������� ������
//	ElemCondPlus::operator =(second);
////	this->dimension = second.dimension;
////	this->sign = second.sign;
////	// �������� ������������ �������
////	if (second.elemCondition != NULL) {
////		ElemConditionsFactory ecf;
////		this->elemCondition = ecf.createAxiomByName (second.elemCondition->name().c_str());
////		std::map<std::string, std::string> params;
////		if (second.elemCondition->getParams(params) >= 0)
////			this->elemCondition->setParams (params);
////	}
//	return *this;
//}

/****************************************************************************
*					ElemCondPlusStat::saveECToFile
*
*	Description:	���������� � ����
*	Parameters:		
*	Returns:		void
*	Throws:			-
*	Author:			wictor
*	History:
*
****************************************************************************/
signed int ElemCondPlusStat::saveECToFile(std::string baseDir, std::string ecName, const std::vector<std::string> &paramNames) const {
	ElemCondPlus::saveECToFile(baseDir, ecName, paramNames);

	ECStatistics ecStatistics(*this);
	
	ecStatistics.saveToFile(baseDir + "/" +ecName + ".stat");

	return 0;
}

/****************************************************************************
*					ElemCondPlusStat::initFromFile
*
*	Description:	�������� �� �����
*	Parameters:		
*	Returns:		int
*	Throws:			-
*	Author:			wictor
*	History:
*
****************************************************************************/
signed int ElemCondPlusStat::initFromFile(std::string baseDir, std::string ecName, 
										  const std::vector<std::string> &paramNames) {
	ElemCondPlus::initFromFile(baseDir, ecName, paramNames);
	std::string fullPath = baseDir + "/" + ecName;	
	
	ECStatistics ecStatistics;
	ecStatistics.initFromFile(fullPath + ".stat");
	ecStatistics.copyTo(*this);
	
	return 0;		
}
