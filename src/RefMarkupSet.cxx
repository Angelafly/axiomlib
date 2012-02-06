/****************************************************************************
*			������� ������ RefMarkupSet.h
*
*	Description:	��������� �������� ��� ������ �������������� - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "RefMarkupSet.h"
#include "RefMarkup.h"

using namespace AxiomLib;

// �����������  - � �������� ���������� ���������� �� ���������
RefMarkupSet::RefMarkupSet () {
	precision = 0.0;
	refMarkups.resize(0);
}


// ������ (����) ����������
RefMarkupSet::~RefMarkupSet () {

}


// ������� ������� ����� �������� ���������� ����������
int RefMarkupSet::setPrecision (const double newPrecision) {
	if ((newPrecision < 0.0) || (newPrecision > 1.0)) {
		throw AxiomLibException ("Error in RefMarkupSet::setPrecision: input parameter out of range");
	}
	precision = newPrecision;
	return 0;
}


/****************************************************************************
*					RefMarkupSet::processRes
*
*	Description:	������� ��������� ����������� �������� ���� �� 
*				������� �������� - ����� ������ ����� ������������ 
*				�����-���� �������
*	Parameters:	resRef - �������� �������� ����
*				axiomTypes - ����������� �������� ����
*				axiomMinLen - ����������� ��������� ��������
*				axiomMaxLen - ����������� ��������� ��������
*	Returns:		0
*	Throws:		AxiomLibException - ���� �������� �������� �����
*	Author:		dk
*	History:
*
****************************************************************************/
signed int RefMarkupSet::processRes (std::vector<int> &resRef, std::vector<int> &axiomTypes, std::vector<int> &axiomMinLen, std::vector<int> &axiomMaxLen) {
	// �������� �� ������������ ����������
	if (resRef.size() <= 0) {
		throw AxiomLibException ("Error in RefMarkupSet::processRes: empty resRef - nothing to process.");
	}
	// ������������ ����� ��������� � ����������� ��������
	int numOfCng = 1;
	for (unsigned int j = 1; j < resRef.size(); j++) {
		if (resRef[j] != resRef[j-1]) {
			numOfCng++;
		}
	}
	// �������������� � ���������� �������
	int curMin = 1;
	int curMax = 1;
	axiomTypes.resize(numOfCng);
	axiomTypes[0] = resRef[0];
	axiomMinLen.resize(numOfCng);
	axiomMaxLen.resize(numOfCng);
	// ��������� � ������������ ���������� ��������
	int h = 1;
	for (int i = 1; i < (int) resRef.size(); i++) {
		if (resRef[i] != resRef[i-1]) {
			axiomTypes[h] = resRef[i];
			axiomMinLen[h-1] = (int) ( (double) curMin * (1.0 - precision) );
			//std::cout << "\n 		curMin is " << curMin << "\n";
			//std::cout << " 		minLen[h-1] is " << axiomMinLen[h-1] << "\n";
			curMin = 1;
			axiomMaxLen[h-1] =(int) ( (double) curMax * (1.0 + precision) );
			curMax = 1;
			h++;
		}
		else {
			curMin++;
			curMax++;
		} 
	}
	axiomMinLen[h-1] = (int) ( (double) curMin * (1.0 - precision) );
	axiomMaxLen[h-1] = (int) ( (double) curMax * (1.0 + precision) );
	return 0;
}


/****************************************************************************
*					RefMarkupSet::addRefMarkup
*
*	Description:	���������� ����� ��������� �������� ������ �������� ������
*	Parameters:	refPattern - ��������� ��� ��� �������� �������� ������ � ���������� 
*				� ��������� ��������
*				as - ������� ������ ��� ��������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSet::addRefMarkup(std::vector<double>& refPattern, AxiomSet& as, const bool comments) {
	//std::cout << "Adding reference markup to the set with size " << refMarkups.size() << ": ";

	// ������� ��� ��� �������� ��������
	std::vector<int> resRef;
	// ��������� ��������� ���
	as.enter(resRef, refPattern, 0, (unsigned long) refPattern.size());
	// �������������� ���� ��� ������ ����������� ��������
	std::vector<int> axiomTypes;
	std::vector<int> axiomMinLen;
	std::vector<int> axiomMaxLen;
	// ������������ ����������� ��������
	processRes (resRef, axiomTypes, axiomMinLen, axiomMaxLen);
	if (comments) {
		std::cout << "\n	Axioms";
		for (unsigned int i = 0; i  < axiomTypes.size(); i++) {
			std::cout << "  " << axiomTypes[i];
		}
		std::cout << "\n	MinLen";
		for (unsigned int i = 0; i  < axiomMinLen.size(); i++) {
			std::cout << "  " << axiomMinLen[i];
		}
		std::cout << "\n	MaxLen";
		for (unsigned int i = 0; i  < axiomMaxLen.size(); i++) {
			std::cout << "  " << axiomMaxLen[i];
		}
		std::cout << "\n";
	}
	// ������ ������������� ��������
	int flag = 0;
	RefMarkup temp;
	std::vector<RefMarkup>::iterator setIter;
	// ������� �������� ��������� � ��� ������������� ����� �� ���������
	for (setIter = refMarkups.begin(); setIter != refMarkups.end(); ++setIter) {
		temp = *setIter; 
		flag = temp.addLens (axiomTypes, axiomMinLen, axiomMaxLen);
		if (flag != 0) {
			break;
		}
	}
	// ���� ����� �� �������� �� ������� - �� ��������� �� ��� �����
	if (flag == 0) {
		RefMarkup rmu;
		rmu.create(axiomTypes, axiomMinLen, axiomMaxLen);
		refMarkups.resize(refMarkups.size() + 1);
		refMarkups[refMarkups.size() - 1] = rmu;
//		cout << " flag = 0.";
	}
//	cout << "New size is " << refMarkups.size() << "\n";	
}


/****************************************************************************
*					RefMarkupSet::addRefMarkup
*
*	Description:	���������� ����� ��������� �������� � ������� ������
*	Parameters:	refRes - ����������� �������� � ����������� ���� - ������������������ ������ ��� �����������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSet::addRefMarkup(std::vector<int> &resRef, const bool comments) {
	// �������������� ���� ��� ������ ����������� ��������
	std::vector<int> axiomTypes;
	std::vector<int> axiomMinLen;
	std::vector<int> axiomMaxLen;
	// ������������ ����������� ��������
	processRes (resRef, axiomTypes, axiomMinLen, axiomMaxLen);
	if (comments) {
		std::cout << "\n	Axioms";
		for (unsigned int i = 0; i  < axiomTypes.size(); i++) {
			std::cout << "  " << axiomTypes[i];
		}
		std::cout << "\n	MinLen";
		for (unsigned int i = 0; i  < axiomMinLen.size(); i++) {
			std::cout << "  " << axiomMinLen[i];
		}
		std::cout << "\n	MaxLen";
		for (unsigned int i = 0; i  < axiomMaxLen.size(); i++) {
			std::cout << "  " << axiomMaxLen[i];
		}
		std::cout << "\n";
	}
	// ������ ������������� ��������
	int flag = 0;
	RefMarkup temp;
	std::vector<RefMarkup>::iterator setIter;
	// ������� �������� ��������� � ��� ������������� ����� �� ���������
	for (setIter = refMarkups.begin(); setIter != refMarkups.end(); ++setIter) {
		temp = *setIter; 
		flag = temp.addLens (axiomTypes, axiomMinLen, axiomMaxLen);
		if (flag != 0) {
			break;
		}
	}
	// ���� ����� �� �������� �� ������� - �� ��������� �� ��� �����
	if (flag == 0) {
		RefMarkup rmu;
		rmu.create(axiomTypes, axiomMinLen, axiomMaxLen);
		refMarkups.resize(refMarkups.size() + 1);
		refMarkups[refMarkups.size() - 1] = rmu;
//		cout << " flag = 0.";
	}
//	cout << "New size is " << refMarkups.size() << "\n";	
}


/****************************************************************************
*					RefMarkupSet::check 
*
*	Description:	������� �������� - �������� �� ����� ���� �� ��������
*				������ � ������� ������������ �������� �� ������ ���������.
*				�� ���� ����������� - �������� �� � ����� �������� ���� ������ 
*				����� ���� �� ������� ������� ������. (���������� � ����� - 
*				�������������, ��� ��������� ������� ����������� �������� 
*				�������� ���� ������ ��������� � ��������� �������� 
*				��������� ��������)
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				axiomLen - ��������� ��������
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int RefMarkupSet::check (std::vector<int> &axiomTypes, std::vector<int> &axiomLen, long int upTo) {
	// ��������� �� ���� ��������� � ������� - �������� �� ����������
	RefMarkup temp;
	std::vector<RefMarkup>::iterator setIter;
	int flag = 0;
//	cout << "Checking the set with size" << refMarkups.size() << ": ";
	// ������� �������� ��������� � ��� ������������� ����� �� ���������
	for (setIter = refMarkups.begin(); setIter != refMarkups.end(); ++setIter) {
		temp = *setIter; 
		flag = temp.check(axiomTypes, axiomLen, upTo);
		if (flag != 0) {
			return flag;
		}
//		cout << ".";
	}
//	cout << "\n";
	return 0;
}
