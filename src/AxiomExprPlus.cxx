/****************************************************************************
*					������� ������ AxiomExprPlus
*
*	Description:	���������� ������ ������������ �� AxiomExpr � ����������� ����������
*	Author:			dk
*	History:	
*
****************************************************************************/
#include "AxiomExprPlus.h"
#include "AxiomStatistics.h"

using namespace AxiomLib;



// ������ ����������� � �������� ����� ������� �� default'�
AxiomExprPlus::AxiomExprPlus () {
}


// ������ ����������� � �������� ����� ������� � ���������
AxiomExprPlus::AxiomExprPlus (const std::string sName) : AxiomExpr(sName) {
	setDefaultStatistics();
}

AxiomExprPlus::AxiomExprPlus(const ElemCondPlus &ec) {
	this->expression.resize(1);
	this->expression[0].resize(1);
	this->expression[0][0] = ec;
	
	setDefaultStatistics();	
}

AxiomExprPlus::AxiomExprPlus(const AxiomExpr &ax) : AxiomExpr(ax) {
	setDefaultStatistics();
}


// ���������� ������
AxiomExprPlus::~AxiomExprPlus(void) {
	
}

void AxiomExprPlus::setDefaultStatistics() {
	goal = -1.0;
	statAbnorm = -1.0;
	statNormal = -1.0;		
	statOccurence = -1.0;
	statVariation = -1.0;
}


// ������� ���������� ���� ��������� � �������
void AxiomExprPlus::clear(void) {
	AxiomExpr::clear();
	setDefaultStatistics();
}


/****************************************************************************
*					AxiomExprPlus::operator=
*
*	Description:	�������� ������������ ������� �� �����������
*	Parameters:		second - ������������� ������� ������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
//AxiomExprPlus& AxiomExprPlus::operator= (const AxiomExprPlus &second) {
//	// �������� ����������� ������ �������
//	this->clear();
//	// �������� ���������� �� �������
//	this->goal = second.goal;
//	this->statAbnorm = second.statAbnorm;
//	this->statNormal = second.statNormal;
//	this->statVariation = second.statVariation;
//	// ����������� �������� ������� � ������������� ����� ������������ �������
//	// ����� ����� �������� ������ AxiomExpr::operator=(second);
//	this->nameOfAxiomExpr = second.name();
//	this->nameOfECBank = second.nameOfECBank;
//	// ���������� ���������� ������������ �������
//	this->expression.resize (second.expression.size());
//	for (unsigned int i = 0; i < second.expression.size(); i++) {
//		this->expression[i].resize (second.expression[i].size());
//		for (unsigned int j = 0; j < second.expression[i].size(); j++)
//			this->expression[i][j] = second.expression[i][j];
//	}
//	return *this;
//}

/****************************************************************************
*					AxiomExprPlus::saveAxiomToFile
*
*	Description:	���������� ������� � ����������
*	Parameters:		baseDir, axiomName, paramNames
*	Returns:		0
*	Throws:			-
*	Author:			wictor
*	History:
*
****************************************************************************/
int AxiomExprPlus::saveAxiomToFile(const std::string &baseDir, const std::string &axiomName, const std::vector<std::string> &paramNames) const {
	std::vector<std::string> temp(paramNames);
	AxiomExpr::saveAxiomToFile(baseDir, axiomName, temp);
	
	AxiomStatistics axiomStatistics(*this);
	axiomStatistics.saveToFile(baseDir + "/" + axiomName + ".stat");
	return 0;
}

int AxiomExprPlus::initAxiomFromFile(std::string baseDir, std::string axiomName, const std::vector<std::string> &paramNames) {
	AxiomExpr::initAxiomFromFile(baseDir, axiomName, paramNames);
	
	AxiomStatistics axiomStatistics;
	axiomStatistics.initFromFile(baseDir + "/" + axiomName + ".stat");
	axiomStatistics.copyTo(*this);
	return 0;
}
