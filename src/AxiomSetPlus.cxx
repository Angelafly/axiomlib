/****************************************************************************
*
*				������� ��������� AxiomSetPlus
*
****************************************************************************/

#include "AxiomSetPlus.h"

#include "Logger.h"

using namespace AxiomLib;

// ����������� ���������
AxiomSetPlus::AxiomSetPlus (void) {
	axiomSet = NULL;
	goal = -1.0;
	nFirst = -1;
	nSecond = -1;
	axiomProfit.clear();
	goalRatio = 0.0;
	asCrossProb = 0.0;
	axCrossProb = 0.0;
	ecCrossProb = 0.0;
	prTransProb = 0.0;
	asTransProb = 0.0;
	axTransProb = 0.0;
	ecTransProb = 0.0;
}

// ���������� ���������
AxiomSetPlus::~AxiomSetPlus (void) {
	if (axiomSet != NULL) {
		axiomSet->clear ();
		delete axiomSet;
		axiomSet = NULL;
	}
}

// �������� ������������ ������� ������
AxiomSetPlus & AxiomSetPlus::operator= (const AxiomSetPlus &second) {
	//Logger::getInstance()->writeDebug("Entering AxiomSetPlus::operator= ");
	if(this == &second) {
		//Logger::getInstance()->writeDebug("Leaving AxiomSetPlus::operator= (self-assignment)");
		return *this;
	}
	
	if (second.axiomSet != NULL) {
		if (axiomSet == NULL) {
			axiomSet = new AxiomSet;
		}
		*axiomSet = *(second.axiomSet);
	}
	else {
		if(axiomSet) delete axiomSet;
		axiomSet = NULL;
	}
	goal = second.goal;
	nFirst = second.nFirst;
	nSecond = second.nSecond;
	axiomProfit = second.axiomProfit;
	// ��������� �������� ������� �������
	goalRatio = second.goalRatio;
	// ��������� ��������� �����������
	asCrossProb = second.asCrossProb;
	axCrossProb = second.asCrossProb;
	ecCrossProb = second.ecCrossProb;
	// ��������� ��������� �������
	prTransProb = second.prTransProb;
	asTransProb = second.asTransProb;
	axTransProb = second.axTransProb;
	ecTransProb = second.ecTransProb;
	//Logger::getInstance()->writeDebug("Leaving AxiomSetPlus::operator= ");
	return *this;
}

// ������� ������� ���������� �� ������� ������
int AxiomSetPlus::clearStat (void) {
	goal = -1.0;
	nFirst = -1;
	nSecond = -1;
	axiomProfit.clear();
	goalRatio = 0.0;
	asCrossProb = 0.0;
	axCrossProb = 0.0;
	ecCrossProb = 0.0;
	prTransProb = 0.0;
	asTransProb = 0.0;
	axTransProb = 0.0;
	ecTransProb = 0.0;
	return 0;
}

AxiomSetPlus::AxiomSetPlus(const AxiomSetPlus &second)
{
	//Logger::getInstance()->writeDebug("Copying AxiomSetPlus");
	axiomSet = 0;
	*this = second;
	//Logger::getInstance()->writeDebug("Finished copying AxiomSetPlus");
}

// ������� ������� ����������� ������� ���������
int AxiomSetPlus::clear (void) {
	if (axiomSet != NULL) {
		axiomSet->clear();
		delete axiomSet;
		axiomSet = NULL;
	}
	goal = -1.0;
	nFirst = -1;
	nSecond = -1;
	axiomProfit.clear();
	goalRatio = 0.0;
	asCrossProb = 0.0;
	axCrossProb = 0.0;
	ecCrossProb = 0.0;
	prTransProb = 0.0;
	asTransProb = 0.0;
	axTransProb = 0.0;
	ecTransProb = 0.0;
	return 0;
}
