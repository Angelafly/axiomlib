/****************************************************************************
*					������� ������ AxiomExprSetPlus
*
*	Description:	������� ������ �� ������ ���� AxiomExpr �� ����������� - ����������
*	Author:			dk
*	History:	
*
****************************************************************************/
#include "AxiomExprSetPlus.h"
#include "AxiomSetStatistics.h"

using namespace AxiomLib;




bool AxiomExprSetPlus::operator <(const AxiomExprSetPlus &other) const
{
	return this->goal < other.goal;
}

// ������ ����������� � �������� ����� ������� ������ �� default'�
AxiomExprSetPlus::AxiomExprSetPlus () {
	goal = -1.0;
	errFirst = -1;
	errSecond = -1;
	axiomsIndex.clear();
	markUps.clear();
}


// ������ ����������� � �������� ����� ������� ������ � ���������
AxiomExprSetPlus::AxiomExprSetPlus (const std::string sName) {
	goal = -1.0;
	errFirst = -1;
	errSecond = -1;
	axiomsIndex.clear();
	markUps.clear();
	nameOfAxiomSet = sName;
}

AxiomExprSetPlus::AxiomExprSetPlus(const AxiomExprSet &as) : AxiomExprSet(as) {
	goal = -1.0;
	errFirst = -1;
	errSecond = -1;	
}

// ���������� - ������� ��� ��������� ������������ ������ � ������� ������
AxiomExprSetPlus::~AxiomExprSetPlus(void) {
	axiomsIndex.clear();
	markUps.clear();
}

// ������� ������� ��� ��������� ������������ ������ � ������� ������
void AxiomExprSetPlus::clear(void) {
	AxiomExprSet::clear();
	goal = -1.0;
	errFirst = -1;
	errSecond = -1;
	axiomsIndex.clear();
	markUps.clear();
}


/****************************************************************************
*					AxiomExprSetPlus::operator=
*
*	Description:	�������� ������������ ������� ������ �� �����������
*	Parameters:		second - ������������� ������� ������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
//AxiomExprSetPlus& AxiomExprSetPlus::operator= (const AxiomExprSetPlus &second) {
//	AxiomExprSet::operator =(second);
////	// �������� ��� ������� ������
////	this->nameOfAxiomSet = second.nameOfAxiomSet;
////	this->nameOfAxiomBank = second.nameOfAxiomBank;
////	// �������� �������� � ������� �������
////	this->clear();
////	axioms.resize(second.axioms.size());
////	for (unsigned int i = 0; i < second.axioms.size(); i++) {
////		axioms[i] = new AxiomExpr;
////		*axioms[i] = *(second.axioms[i]);
////	}
//	// �������� ���������� �� ������� ������
//	this->goal = second.goal;
//	this->errFirst = second.errFirst;
//	this->errSecond = second.errSecond;
//	this->axiomsIndex = second.axiomsIndex;
//	this->markUps = second.markUps;
//	return *this;
//}

int AxiomExprSetPlus::saveAxiomSetToFile(std::string baseDir, std::string axiomSetName, 
										  const std::vector<std::string> &dataSetParams, 
										  int first, int second) const {
	// TODO: ������� ����� const std::vector<std::string>& dataSetParams
	std::vector<std::string> temp(dataSetParams);
	int result = AxiomExprSet::saveAxiomSetToFile(baseDir, axiomSetName, temp, first, second);
	
	AxiomSetStatistics statistics(*this);
	statistics.saveToFile(
			baseDir + "/" +axiomSetName + ".stat"
			);
	//std::cout<<"saving as with stat"<<std::endl;
	
	return result;
}

int AxiomExprSetPlus::initAxiomSetFromFile(std::string axiomSetBaseDir, std::string axiomSetName,
										   const std::vector<std::string> &paramNames) {
	
	int result = AxiomExprSet::initAxiomSetFromFile(axiomSetBaseDir,
									   axiomSetName, paramNames);
	
	AxiomSetStatistics statistics;
	statistics.initFromFile(
			axiomSetBaseDir + "/" +axiomSetName + ".stat"
			);
	
	statistics.copyTo(*this);
	
	this->nameOfAxiomSet = axiomSetName;
	
	//std::cout<<"loading as with stat"<<std::endl;
	
	return result;
}

