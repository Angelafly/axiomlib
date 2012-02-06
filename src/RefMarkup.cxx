/****************************************************************************
*			������� ������ RefMarkup.h
*
*	Description:	��������� �������� - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "RefMarkup.h"

using namespace AxiomLib;

// ������ �����������
RefMarkup::RefMarkup () {
	errorLimit = 1;
}

// ������ (����) ����������
RefMarkup::~RefMarkup () {

}


/****************************************************************************
*					RefMarkup::create
*
*	Description:	������� ���������� ���������� ������
*	Parameters:	newAxiomTypes - ������ �������� ���� ���������
*				newMinLen - ������ ���������� �������� (����������� ����� ����������� ������)
*				newMaxLen - ������ ���������� �������� (������������ ����� ����������� ������)
*	Returns:		void
*	Throws:		AxiomLibException - ���� ������� ��������� ������ �����
*	Author:		dk
*	History:
*
****************************************************************************/
void RefMarkup::create (std::vector<signed int> &newAxiomTypes, std::vector<signed int> &newMinLen, std::vector<signed int> &newMaxLen) {
	// �������� ������������ ����������
	if ((newAxiomTypes.size() <= 0) || (newAxiomTypes.size() != newMinLen.size()) || (newAxiomTypes.size() != newMinLen.size())) {
		throw AxiomLibException ("Error in RefMarkup::create: wrong parameters to create RefMurkup ");
	}
	// ��������� ���������� ������� ������
	this->axiomTypes.resize(0);
	this->axiomTypes = newAxiomTypes;
	this->axiomMinLen.resize(0);
	this->axiomMinLen = newMinLen;
	this->axiomMaxLen.resize(0);
	this->axiomMaxLen = newMaxLen;
}

// ������� ��������� ���������� ��������
/****************************************************************************
*					RefMarkup::addLens
*
*	Description:	������� ��������� ���������� ��������. ���� ������� �������� 
*				��������� � ���������, ���������� � ������, �� ��������� �������� 
*				������������.
*	Parameters:	newAxiomTypes - ������ �������� ���� ���������
*				newMinLen - ������ ���������� �������� (����������� ����� ����������� ������)
*				newMaxLen - ������ ���������� �������� (������������ ����� ����������� ������)
*	Returns:		0 - ���� �������� �� ������� � ���������� �������� �� ����
*				1 - ���� �������� ������� � ��������� �������� ���� ����������
*	Throws:		AxiomLibException - ���� ������� ��������� ������ ����� ��� �����
*	Author:		dk
*	History:
*
****************************************************************************/
signed int RefMarkup::addLens (std::vector<signed int> &newAxiomTypes, std::vector<signed int> &newMinLen, std::vector<signed int> &newMaxLen) {
	// �������� ������������ ����������
	if ((newAxiomTypes.size() <= 0) || (newAxiomTypes.size() != newMinLen.size()) || (newAxiomTypes.size() != newMinLen.size())) {
		throw AxiomLibException ("Error in RefMarkup::addLens: wrong parameters to change RefMurkup ");
	}
	if (this->axiomTypes.size() <= 0) {
		throw AxiomLibException ("Error in RefMarkup::addLens: wrong proccessing of addLens functuion");
	}
	// ��������� �������� ����� 
	if (newAxiomTypes.size() != this->axiomTypes.size()) {
		return 0;
	}
	int flag = 0;
	for (unsigned int i = 0; i < this->axiomTypes.size(); i++) {
		if (this->axiomTypes[i]!= newAxiomTypes[i]) {
			flag = 1;
			break;
		}
	}
	// ���� �������� ������ - �� ������� � false - �� ���� � 0
	if (flag != 0) {
		return 0;
	}
	// ���� �������� ���� ���������� - �� ��������� ������ � ������ �������� � ������� � true - �� ���� 1
	for (unsigned int j = 0; j < this->axiomMinLen.size(); j++) {
		if (this->axiomMinLen[j] > newMinLen[j]) {
			this->axiomMinLen[j] = newMinLen[j];
		}
		if (this->axiomMaxLen[j] < newMaxLen[j]) {
			this->axiomMaxLen[j] = newMaxLen[j];
		}
	}
	return 1;
}


/****************************************************************************
*					RefMarkup::getCopy 
*
*	Description:	������ ������� ��������� ��������
*	Parameters:		axTypes - ����������� ��� ������ (�������� ����)
*					axMinLen - ����������� ����� ����������� ��������� ������ � ��������
*					axMaxLen - ����������� ����� ������������ ��������� ������ � ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int RefMarkup::getCopy (std::vector<int> &axTypes, std::vector<int> &axMinLen, std::vector<int> &axMaxLen) const {
	axTypes = axiomTypes;
	axMinLen = axiomMinLen;
	axMaxLen = axiomMaxLen;
	return 0;
}


/****************************************************************************
*					RefMarkup::check 
*
*	Description:	������� �������� - �������� �� ��������
*				������ � ������� ������������ �������� �� ������ ���������.
*				�� ���� ����������� - �������� �� � ����� �������� ���� ������ 
*				������� ������� ������. (���������� � ����� - �������������,
*				��� ��������� ������� ����������� �������� 
*				�������� ���� ������ ��������� � ��������� �������� 
*				��������� ��������)
*	Parameters:	axiomTypes - ������� ��� ������ (�������� ����)
*				axiomLen - ��������� ��������
*				upTo - ����� ���� (��� �������� �������� ���� �� ���� �����)
*	Returns:		0 - false
*				1 - true
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int RefMarkup::check (std::vector<int> &rowAxiomTypes, std::vector<int> &rowAxiomLen, long int upTo) {
	// �������� �� ������������ ������� ������
	if (upTo >= (long int) rowAxiomTypes.size()) {
		throw AxiomLibException ("Error in RefMarkup::check: wrong parameters of check functuion");
	}
	// ����� �������� � ����
	int toReturn = 0;
	toReturn = subcheck (rowAxiomTypes, rowAxiomLen, upTo, (int) axiomTypes.size() - 1);
	return toReturn;
}


/****************************************************************************
*					RefMarkup::subcheck 
*
*	Description:	��������������� ������� ��� check - ����������� ����� 
*				������� �������� ���������� �������� - ��� ����� 
*				����, ��� ����� ��������� ������� ����� ���� ������ 0.
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
signed int RefMarkup::subcheck (std::vector<int> &rowAxiomTypes, std::vector<int> &rowAxiomLen, long int upTo, int curEnd) {
	if (curEnd < 0) {
		return 1;
	}
	long int j = upTo;
	int flag = 0;
	for (int i = curEnd; i >= 0; i--) {
		// �� ��������� �������� ���������� ���� - ���� ��� -1
		if (axiomTypes[i] == -1) {
			continue;
		}
		// �� ��������� �������� ���� - ���� ��� -1 - ��� �������� ������ �������
		if ((rowAxiomTypes[j] == -1) && (j != upTo))  {
			j--;
			if (j < 0) {
				// ��� ������� �������� ����� ���-�� ����������
				return 0;
			}
		}
		// ���� ������� ����� � �� ������� � ������� - �� ��������� - ��������� �� ���� �� ��� ���������� 
		if (axiomMinLen[i] == 0) {
			flag = this->subcheck (rowAxiomTypes, rowAxiomLen, j, curEnd - 1);
			if (flag != 0) {
				return flag;
			}
		}
		// ������ �������� ��������� ��������� ������������������ � ��������
		if (axiomTypes[i] != rowAxiomTypes[j]) {
			// ����������� ���� ������
			return 0;
		}
		// �������� - ��������� �� ��� ���� ����� - �� ���� ������������ ����������� ��������
		if ((axiomMinLen[i] > rowAxiomLen[j]) || ((axiomMaxLen[i] < rowAxiomLen[j]) && (i != 0) ) ) {
			// ���������� ����� �������
			return 0;
		}
		// ���������� �� ���� - ��� ��������� ��������� �������
		j--;
		if ((j < 0) && (i != 0)) {
			// ��� ������� �������� ����� ���-�� ����������
			return 0;
		}
	}
	// ��� �� ���������� �� ������� ������ - ������ �������� �������� - ������� � true	
	return 1;
}
