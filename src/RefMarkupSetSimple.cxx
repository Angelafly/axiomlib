/****************************************************************************
*			������� ������ RefMarkupSetSimple.h
*
*	Description:	��������� �������� ��� ������ �������������� - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "RefMarkupSetSimple.h"

using namespace AxiomLib;

// �����������  - � �������� ���������� ���������� �� ���������
RefMarkupSetSimple::RefMarkupSetSimple () {
	refMarkups.resize(0);
}


// ������ ����������
RefMarkupSetSimple::~RefMarkupSetSimple () {
}


/****************************************************************************
*                       RefMarkupSetSimple::setParamsFromEnv
*
*       Description:	���������������� ��������� ������� �� env
*       Parameters:	const Environment& env - �����
*       Returns:		0
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int RefMarkupSetSimple::setParamsFromEnv (const Environment& env) {
	// �������� ��������� ���������� �� ������� ����
	rmu.setParamsFromEnv (env);
	return 0;
}


/****************************************************************************
*					RefMarkupSetSimple::addRefMarkup
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
void RefMarkupSetSimple::addRefMarkup(std::vector<double>& refPattern, AxiomSet& as, const bool comments) {
	// ������� ��� ��� �������� ��������
	std::vector<int> axiomTypes;
	// ��������� ��������� ���
	as.enter(axiomTypes, refPattern, 0, (unsigned long) refPattern.size());
	// �������� ����� �������� ��������� ����������
	if (comments) {
		std::cout << "\n	Axioms";
		for (unsigned int i = 0; i  < axiomTypes.size(); i++) {
			std::cout << "  " << axiomTypes[i];
		}
	}
	// ������ ������������� ��������
	int flag = 0;
	std::vector<RefMarkupSimple>::iterator setIter;
	// ������� �������� ��������� � ��� ������������� ����� �� ���������
	for (setIter = refMarkups.begin(); setIter != refMarkups.end(); ++setIter) {
		if (*setIter == axiomTypes) {
			flag = 1;
			break;
		}
	}
	// ���� ����� �� �������� �� ������� - �� ��������� �� ��� �����
	if (flag == 0) {
		//RefMarkupSimple rmu;
		rmu.create(axiomTypes);
		refMarkups.resize(refMarkups.size() + 1);
		refMarkups[refMarkups.size() - 1] = rmu;
	}
	if (comments) std::cout << "New size is " << refMarkups.size() << "\n";
}


/****************************************************************************
*					RefMarkupSetSimple::addRefMarkup
*
*	Description:	���������� ����� ��������� �������� � ������� ������
*	Parameters:	refRes - ����������� �������� � ����������� ���� - ������������������ ������ ��� �����������
*	Returns:		void
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkupSetSimple::addRefMarkup(std::vector<int> &axiomTypes, const bool comments) {
	// �������� ����� �������� ��������� ����������
	if (comments) {
		std::cout << "\n	Axioms";
		for (unsigned int i = 0; i  < axiomTypes.size(); i++) {
			std::cout << "  " << axiomTypes[i];
		}
	}
	// ������ ������������� ��������
	int flag = 0;
	std::vector<RefMarkupSimple>::iterator setIter;
	// ������� �������� ��������� � ��� ������������� ����� �� ���������
	for (setIter = refMarkups.begin(); setIter != refMarkups.end(); ++setIter) {
		if (*setIter == axiomTypes) {
			flag = 1;
			break;
		}
	}
	// ���� ����� �� �������� �� ������� - �� ��������� �� ��� �����
	if (flag == 0) {
		//RefMarkupSimple rmu;
		rmu.create(axiomTypes);
		refMarkups.resize(refMarkups.size() + 1);
		refMarkups[refMarkups.size() - 1] = rmu;
		//rmu.setOut();
	}
	if (comments) std::cout << "New size is " << refMarkups.size() << "\n";
}


/****************************************************************************
*					RefMarkupSetSimple::clear
*
*	Description:	������� �������� ������ - �������� ���� ����������� �������� 
*	Parameters:	-
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int RefMarkupSetSimple::clear () {
	if (refMarkups.size() > 0) {
		for (unsigned int i = 0; i < refMarkups.size(); i++) {
			(refMarkups[i]).clear();
		}
		refMarkups.clear();
	}
	return 0;
}


/****************************************************************************
*					RefMarkupSetSimple::check 
*
*	Description:	������� �������� - �������� �� ����� ���� �� ��������
*				������ � ������� ������������ �������� �� ������ ���������.
*				�� ���� ����������� - �������� �� � ����� �������� ���� ������ 
*				����� ���� �� ������� ������� ������. (���������� � ����� - 
*				�������������, ��� ��������� ������� �������� 
*				�������� ���� ������ ��������� � ��������� �������� 
*				��������� ��������)
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int RefMarkupSetSimple::check (const double newMaxDistance, std::vector<int> &axiomTypes, long int upTo) {
	// ��������� �� ���� ��������� � ������� - �������� �� ����������
	//RefMarkupSimple temp;
	std::vector<RefMarkupSimple>::iterator setIter;
	int flag = 0;
	// ���������� � ������ �� ��������
	for (setIter = refMarkups.begin(); setIter != refMarkups.end(); ++setIter) {
		//temp = *setIter; 
		//flag = temp.check(axiomTypes, upTo);
		flag = (*setIter).check(newMaxDistance, axiomTypes, upTo);
		if (flag != 0) {
			return flag;
		}
	}
	return 0;
}


/****************************************************************************
*					RefMarkupSetSimple::operator=
*
*	Description:	�������� ������������
*	Parameters:		������������� ������ ������ ��������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
RefMarkupSetSimple& RefMarkupSetSimple::operator= (const RefMarkupSetSimple& second) {
	rmu = second.rmu;
	refMarkups.clear();
	refMarkups = second.refMarkups;
	return *this;
}
