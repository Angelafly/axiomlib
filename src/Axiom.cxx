/****************************************************************************
*			������� ������ Axiom.h
*
*	Description:	���������� ������������ ������ - ����������
*	Author:		dk
*	History:	
*
****************************************************************************/
#include "Axiom.h"

using namespace AxiomLib;
using namespace std;

// ������ ����������� - � �������� ����� ������ ��� no_name
Axiom::Axiom () {
	//this->nameOfAxiom.resize(7);
	this->nameOfAxiom = std::string("no_name");
	elemConditions.resize(0);
	// ��������� �������� ��������� ������ �� ���������
	minNumOfEC = 1;
	maxNumOfEC = 7;
}

// ������ ����������� � �������� ����� ������� �� ������� ���������
Axiom::Axiom (const std::string sName) {
	this->nameOfAxiom.resize(sName.size());
	this->nameOfAxiom = sName;
	elemConditions.resize(0);
	// ��������� �������� ��������� ������ �� ���������
	minNumOfEC = 1;
	maxNumOfEC = 7;
}

// ���������� ������ - ������� ��� ������������ ��������� �������
Axiom::~Axiom(void) {
	for (unsigned int i = 0; i < this->elemConditions.size(); i++) {
		delete(this->elemConditions[i]);
	}
}
  
// �������� ������� (���������� ���������� �� ����������, ������������ ��� ������� ��������� ������)
std::string Axiom::name (void) const {
	return this->nameOfAxiom;
}

// ������� ������������� �������� ��������� ������
signed int Axiom::setAttrValue (const unsigned int min, const unsigned int max) {
	// �������� ������������ ����������
	if ( max < min ) 
		throw AxiomLibException("Error in Axiom::initAxiomByNums : cannot creat elementary condition with given number.");
	minNumOfEC = min;
	maxNumOfEC = max;
	return 0;
}

// ������� ���������� �������� ��������� ������
signed int Axiom::getAttrValue (unsigned int &min, unsigned int &max) {
	// �������� ������������ ����������
	min = minNumOfEC;
	max = maxNumOfEC;
	return 0;
}

// ������� ���������� ����� ������������ ������� � ������ �������
//int Axiom::getNumOfEC (void) const {
//	return elemConditions.size();
//}
  
// ������� ������������� �������� ������������� ��������� ������������� ������������� �������
signed int Axiom::setParamValue (const double param, const int ecNum, const std::string &paramName) {
	// �������� ������������ ����������
	if ( (ecNum >= 0) && (ecNum < (int) elemConditions.size()) ) {
		return ( (elemConditions[ecNum])->setParamValue(param, paramName) );
	}
	return -1;
}
     
// ������� ���������� �������� ������������� ��������� ������������� ������������� �������
signed int Axiom::getParamValue (double &param, const int ecNum, const std::string &paramName) const {
	// �������� ������������ ����������
	if ( (ecNum >= 0) && (ecNum < (int) elemConditions.size()) ) {
		return ( (elemConditions[ecNum])->getParamValue(param, paramName) );
	}
	return -1;
}


/****************************************************************************
*				Axiom::clear
*
*	Description:	������� ������� ��� ����������� ��������� ������������ �������
*	Parameters:	-
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::clear (void) {
	for (unsigned int i = 0; i < elemConditions.size(); i++) {
		delete (elemConditions[i]);
	}
	elemConditions.resize(0);
	return 0;
}


/****************************************************************************
*				Axiom::check 
*
*	Description:	���������, ����������� �� ������ ������� �������� ����� k ���� ts
*	Parameters:	k - ����� � ���� ��� ������� ����������� ������������ �������
*				ts - ����������� ���
*	Returns:		1  - ���� �����������
*				0  ���� �� �����������
*				-1  ���� ������ ������� ������� �� ���������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::check (const unsigned long k, const std::vector<double>& ts) {
	int res = 1;
	for (unsigned int i = 0; i < this->elemConditions.size(); i++) {
		res = this->elemConditions[i]->check(k,ts);
		if (res != 1) {
			break;
		}
	}  
	return res;
}
  

/****************************************************************************
*				Axiom::initAxiomByNums
*
*	Description:	������ ������ ������� ��� ���������� ������������ ������ � ��������, ��������� � ������� axiom_nums. 
*				������ ������������� ������� ������������ ������� � ������ ���� ������������ ������������ �������.
*				��� ����� ������������ ��� �������� "���������" ������. 
*				��� �������� ������������ ������������ ������������ ������� ��� ����������
*	Parameters:	axiomNums - ������ ������� �������������� ������������ ��������, ������� ����� ��������� � �������.
*	Returns:		0 - ���� ��� ��������� �������, � ��������� ������ - ����� �� ������� �� throw
*	Throws:		AxiomLibException -  1)���� �� ������� ������� ��������� �� ������������ �������, ������������� � ������
*								2)���� ������������ �� ������� ��������� ������� �� ����������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::initAxiomByNums (const std::vector<signed int>& axiomNums) {
	ElemConditionsFactory ecf;
	
	elemConditions.resize(axiomNums.size());
	for (unsigned int i = 0; i < axiomNums.size(); i++) {
		// �������� ������������� �������
		elemConditions[i] = ecf.createAxiomByNum(axiomNums[i]);
		if (elemConditions[i] == NULL) {
			//std::cerr << "Wrong number of elementary condition in Axiom::init AxiomByNums\n";
			throw AxiomLibException("Error in Axiom::initAxiomByNums : cannot creat elementary condition with given number.");
			//exit(1);
		}
		// �������� ������������� ���������� ������������� ������� � ��� ��������������� � �������
		for (unsigned int j=0; j < i; j++) {
			if (elemConditions[i]->checkAndCompatibility(*(elemConditions[j])) != 1) {
				//std::cerr << "Incompatible axioms in Axiom::init AxiomByNums\n";
				throw AxiomLibException("Error in Axiom::initAxiomByNums : incompatible elementary conditions.");
				return 1;
			}
		}
	}
	
	return 0;
}

/****************************************************************************
*				Axiom::initAxiomByNames
*
*	Description:	������ ������ ������� ��� ���������� ������������ ������ � �������, �������������� � ������� axiom_nums. 
*				��� ����� ������������ ��� �������� ��������� ������. ��� �������� ������������ ������������ ������������ �������  ��� ����������
*	Parameters:	axiomNums - ������ ������� �������������� ������������ ��������, ������� ����� ��������� � �������.
*	Returns:		0 - ���� ��� ��������� �������, � ��������� ������ - ����� �� ������� �� throw
*	Throws:		AxiomLibException -  1)���� �� ������� ������� ��������� �� ������������ �������, ������������� � ������
*								2)���� ������������ �� ������� ��������� ������� �� ����������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::initAxiomByNames (const std::vector<std::string>& axiomNames) {
	ElemConditionsFactory ecf;
	
	elemConditions.resize(axiomNames.size());
	for (unsigned int i = 0; i < axiomNames.size(); i++) {
		//�������� ������������� �������
		elemConditions[i] = ecf.createAxiomByName(axiomNames[i].c_str());
		if (elemConditions[i] == NULL) {
			//std::cerr << "Wrong number of elementary condition in Axiom::init AxiomByNames\n";
			throw AxiomLibException("Error in Axiom::initAxiomByNames : cannot creat elementary condition with given name.");
			//exit(1);
		}
		//�������� ������������� ���������� ������������� ������� � ��� ������������� � ������ �������
		for (unsigned int j=0; j < i; j++) {
			if (elemConditions[i]->checkAndCompatibility(*(elemConditions[j])) != 1) {
				//std::cerr << "Incompatible axioms in Axiom::init AxiomByNames\n";
				throw AxiomLibException("Error in Axiom::initAxiomByNames : incompatible elementary conditions.");
				return 1;
			}
		}
	}
	
	return 0;
}


/****************************************************************************
*				Axiom::initAxiomByRand
*
*	Description:	������� ������ ������ ������� ��� ���������� ������������ ������� ��������� ��������� �������
*				���������� ������������ ��������� � ����� ������� ������ - ������ �������� ������������ ������� - 
*				� ��������� �������� ���������� ������ ����������� ������������ �������, 
*				����� ������������ ������� ��� ���� �������� ��� ��������
*	Parameters:	numOfEC - ����� ������������ �������, ������� ����� ������� � ������ �������
*	Returns:		-1 - ����  ������� �� �������
*				 0 - ���� ��� ������ ���������
*	Throws:		AxiomLibExeption - ���� ���������� ��������� numOfEC ����������� ������������ ������� 
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::initAxiomByRand (const int numOfEC) {
	ElemConditionsFactory ecf;

	ecf.createAxiomByRand(elemConditions, numOfEC);

  	return 0;
}


/****************************************************************************
*				Axiom::setECParameters
*
*	Description:	������������� ���������� ������������ �������, �������� � �������
*	Parameters:	setECParameters - ������ ����������� �� ���� ������������ ������� � "������ �����������"
*				"������ �����������" - ��� ����������� �� ���� ���������� ������������ ������� � �������� ���� ����������
*				�������� ���������� �������� � ���� �����, ��� �������� ( ���� ��� ����� �� ��� ����������� �������� atof() )
*	Returns:		0  - ���� ���� �� ���� ������� ��� �������
*				-2 - � ecParams �� ������� �� ������ ���������� �����������
*	Throws:		AxiomLibExeption - ���� ����������� ecParams �����
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::setECParameters (const std::map<std::string, std::map<std::string, std::string> >& ecParams) {
	signed int toReturn=-2;

	// ���������, ������ �� �����?
	if (ecParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in Axiom::setECParameters : zero parameter map.");
	}

	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map<std::string, std::map<std::string, std::string> >::const_iterator i = ecParams.begin(); i != ecParams.end(); i++) {
		std::string ecName = i->first;
		std::map<std::string, std::string> paramMap = i->second;

		for (unsigned int j = 0; j < this->elemConditions.size(); j++) {
			// ����, ������ ��������� ������������� i
			if (ecName == this->elemConditions[j]->name()) {
				// ok, �����, ������� value
				toReturn = 0;
				this->elemConditions[j]->setParams(paramMap);
			}
		}
	}

  	return toReturn;
}
  

/****************************************************************************
*				Axiom::getECParameters
*
*	Description:	������� ���������� ��������� ������
*	Parameters:	ecParams - ����������� - ������ ����������� �� ���� ������������ ������� � "������ �����������"
*				"������ �����������" - ��� ����������� �� ���� ���������� ������������ ������� � �������� ���� ����������
*				�������� ���������� �������� � ���� �����, ��� �������� ( ���� ��� ����� �� ��� ����������� �������� atof() )
*	Returns:		0 
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::getECParameters (std::map <std::string, std::map <std::string, std::string> > &ecParams) const {
	ecParams.clear();
	std::string curEC;
	for (unsigned int j = 0; j < this->elemConditions.size(); j++) {
		curEC = this->elemConditions[j]->name();
		std::map <std::string, std::string> newMap;
		(this->elemConditions[j])->getParams(newMap);
		ecParams[curEC] = newMap;
	}
	return 0;    
}


/****************************************************************************
*				Axiom::getECParamNames
*
*	Description:	������� ��������� ������� ������ ������� ���������� ������������� 
*				������� � �������� ������� � ������� ������������ �������
*	Parameters:	ecNum - ����� ������������� �������, � ������� ������������� ����������
*				ecParamNames - ����������� ������ ����� � ���������� ����������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ����� ���������� ������������� ������� 
*				������� �� ������� ����� ������������ ������� ������ �������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::getECParamNames (int ecNum, std::vector<std::string> &ecParamNames) const {
	if ((ecNum >= 0) && (ecNum < (int) elemConditions.size())) {
		return ( (elemConditions[ecNum])->getECParamNames (ecParamNames) );
	}
	else {
		throw AxiomLibException("Error in Axiom::getECParamNames : given ecNum out of range.");
	}
	return -1;
}


/****************************************************************************
*				Axiom::getECNumByName
*
*	Description:	������� ���������� ����� ������������� ������� � ������� ������� �� ��� �����
*	Parameters:	ecName - ��� ������������� �������
*	Returns:		>= 0 - ���� ������������� ������� � �������� ������ �������
*				< 0 - ���� ������������� ������� � �������� ������ �� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::getECNumByName (std::string &ecName) const {
	for (unsigned int i = 0; i < elemConditions.size(); i++) {
		if ( ecName == (elemConditions[i])->name() ) {
			return i;
		}
	}
	return -1;
}


/****************************************************************************
*				Axiom::getECNames
*
*	Description:	������� ��������� ������� ������ ������� ������������ ������� ������ �������
*	Parameters:	ecParamNames - ����������� ������ ����� � ���������� ������������ �������
*	Returns:		������������� ����� - ����� ��������� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
 signed int Axiom::getECNames(std::vector<std::string> &ecNames) const {
 	signed int toReturn;
	toReturn = (int) elemConditions.size();
 	ecNames.resize(toReturn);
	for (int i = 0; i < toReturn; i++) {
		ecNames[i] = (elemConditions[i])->name();
	}
 	return 0;
 }


 /****************************************************************************
*					Axiom::createAxiomStructure
*
*	Description:	������� ��������� ��������� ������ ������� ������ � AxiomStructure
*	Parameters:		as - ���������, � ������� ���������� �������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int Axiom::createAxiomStructure (AxiomStructure &as) const {
	// ��������� ���� ��������
	as.axiomName = this->nameOfAxiom;
	this->getECParameters (as.axiomParameters);	
	return 0;
}


 /****************************************************************************
*				Axiom::transmute
*
*	Description:	������� �� ������ ���������� ������������ �������, �������� � ������ ������� (ectp)
*				� �� ������ ��������� ����� ������� (�������� atp)
*				������� �� ������ ������������� ������� ���������� (��� �������) ��������� ��������� ����������
*				������� �� ������ ������� ������������ ������, �������� ��� ��������� ���������� ������� ���������� ������������� �������
*	Parameters:	ectp - �������� ���������� ������� ������� �� ������ ������������ ������� - �������� ��������� ������� ������������� �� 0 �� 1
*				atp  - �������� ���������� ������� ������� �� ������ ������� - �������� ��������� ������� ������������� �� 0 �� 1
*	Returns:		0 - ���� ������� ������ �������, � ��������� ������ - ���� ���-�� ���� �� ��� - ������� ������ �� ����������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int Axiom::transmute (const double ectp, const double atp) { 
	ElemConditionsFactory ecf;
	int flagOfConset = 0; //������������ ��� ���� �������������

	//��������� �� ������� ������������� ������� ������ ������� � �������� �������� - ���������� ��� ���
	for (unsigned int i = 0; i < this->elemConditions.size(); i++) {
		if ((double) rand() / (double) RAND_MAX  <  atp) {
			//������ ������������ ������� ������� ��� ������� �� ������ ������� - �� ���� ���������� ��� ������
			std::vector <ElemCondition* > compatibleEC;
			compatibleEC.resize(0);
			ElemCondition* tempUrl;
			// �������� ������� ����������� ������������ ������� - �� ���� �������, �� ������� ���� �������� ��������� ��� �������
			for (int k = 1; k <= N_EL_AXIOMS; k++) {
				tempUrl = ecf.createAxiomByNum(k);
				int flag = 0; //���� - ������ ����������� ����������� �������
				for (unsigned int j = 0; j < this->elemConditions.size(); j++) {
					if (i!=j) {
						if (this->elemConditions[j]->checkAndCompatibility(*tempUrl) != 1) {
							flag = 1;
							break;
						}
					}
				}
				if ((flag == 0) && (this->elemConditions.size() != 1)) {
					compatibleEC.push_back(tempUrl);
				}
			}
			// ������ � compatibleEC �������� ��� �������, �� ������� ����� ���� �������� ��������� ��� ������� ����������� �������
			int doNotDel = (int) compatibleEC.size();//����� �����  �������� ������������ ������� �� ������ ���������� ������ ���������� ��� �������
			//������ - ���� �������� - ���������� ��� ������� ������������� ������� �� �������� ��������� �� ������ �����������
			if (compatibleEC.size() != 0) {
				int cngEC = round(((double) rand() / (double) RAND_MAX ) * ((double) compatibleEC.size() - 1.0));
				doNotDel = cngEC;
				this->elemConditions[i] = compatibleEC[cngEC];
			}
			else {
				flagOfConset = 1;
				if (this->elemConditions.size() > minNumOfEC) {
					delete (this->elemConditions[i]);
					for (unsigned int temp = (i+1); temp < this->elemConditions.size(); temp++) {
						this->elemConditions[temp-1] = this->elemConditions[temp];
					}
					this->elemConditions.pop_back();
				}
				else {
					//�������� ��������� ������������ �������  �� ������� - ������� /*������ ���*/ ��������� ��� ���� �������
					this->elemConditions[i]->transmute(ectp);
				}
			}
			//�������� �� �������������� ������������ �������
			for (unsigned int del = 0; del < compatibleEC.size(); del++) {
				if (del != doNotDel) {
					delete(compatibleEC[del]);
				}
			}			
		}
	}
	
	//������ ������� �������� ��� ���� ����������� ������� � ������� (����� �������)
	if (((double) rand() / (double) RAND_MAX  <  atp) && ((flagOfConset == 0) || (this->elemConditions.size() == 1))) {
		if (this->elemConditions.size() < maxNumOfEC) {
			std::vector <ElemCondition* > compatibleEC;
			compatibleEC.resize(0);
			ElemCondition* tempUrl;
			//�������� ������ ������������ �������, ������� �������� ���� �� �������� � ��� �������������� � �������
			for (int k = 1; k <= N_EL_AXIOMS; k++) {
				tempUrl = ecf.createAxiomByNum(k);
				int flag = 0;
				for (unsigned int j = 0; j < this->elemConditions.size(); j++) {
					if (this->elemConditions[j]->checkAndCompatibility(*tempUrl) != 1) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					compatibleEC.push_back(tempUrl);
				}
			}
			//������ ����������� ���� ������� ������ - ��������� ��������� �� ���
			int doNotDel = (int) compatibleEC.size();
			if (compatibleEC.size() > 0) {
				int addEC = round(((double) rand() / (double) RAND_MAX ) * ( (double) compatibleEC.size() - 1.0));
				this->elemConditions.push_back(compatibleEC[addEC]);
				doNotDel = addEC;
			} 
			for (unsigned int del = 0; del < compatibleEC.size(); del++) {
				if (del != doNotDel) {
					delete(compatibleEC[del]);
				}
			}
		}
	}
  
	//������ ������� �� ������ ������������ �������
	for (unsigned int i=0; i < this->elemConditions.size(); i++ ){
		this->elemConditions[i]->transmute(ectp);
	}
	
  	return 0;
}


/****************************************************************************
*				Axiom::operator<
*
*	Description:	��������� ������ ������� � �� ����������� ����������� � ������ ��������.
*				�� �������, ��������� - ������
*	Parameters:	-
*	Returns:		�������������� ����� �� 0 �� 1 - ��� ����� � 1 - ��� � ������� ������������ ����� �������, 
*				��� ������ ������� ������ �������� �� ������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
double Axiom::operator< (const Axiom& second) {
	double res, curres, tmpres;
	if (this->elemConditions.size() == 0)  {
		return 0;
	}
	if ((this->elemConditions.size() != 0) && (second.elemConditions.size() == 0)) {
		return 1;
	}
	res = 0;
	for (unsigned int i = 0; i < this->elemConditions.size(); i++) {
		curres = 0.0;
		tmpres = 0.0;
		for (unsigned int j = 0; j < second.elemConditions.size(); j++) {
			if (this->elemConditions[i]->name() == second.elemConditions[j]->name()) {
				tmpres = (*(this->elemConditions[i])) < *(second.elemConditions[j]);
				if (tmpres > curres) {
					curres = tmpres;
				}
			}
		}
		if (curres == 0.0) {
			return 0;
		}
		else {
			res += curres;
		}
	}
	res = res / (double) this->elemConditions.size();	
	return res;
};


/****************************************************************************
*				Axiom::operator=
*
*	Description:	�������� ������������
*	Parameters:	������������� �������
*	Returns:		*this
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
Axiom & Axiom::operator= (const Axiom& second) {
	ElemConditionsFactory ecf;
	// ����������� �������� �������
	nameOfAxiom = second.name();
	// ���������� ������������ ������� ����� �������� ����� �� � ������ �� �����������
	std::map <std::string, std::map <std::string, std::string> > ecParams;
	second.getECParameters (ecParams);
	int num;
	std::vector<std::string> ecNames;
	second.getECNames(ecNames);
	num = (int) ecNames.size();
	elemConditions.resize (num);
	for (int i = 0; i < num; i++) {
		elemConditions[i] = ecf.createAxiomByName( (ecNames[i]).c_str() );
	}
	setECParameters (ecParams);
	return *this;
}

Axiom::Axiom(const Axiom &other)
{
	*this = other;
}


/****************************************************************************
*				Axiom::checkWeakEquivalence
*
*	Description:	�������� (������) ����������� ������������ ���� ������:
*			��������� ������������ ������� ���������. � ���� ����,
*			��� ���� ��������� ������ ������ "�", ��������
*			������ �� ������.
*	Parameters:	onst Axiom& second - �������, � ������� ���� ��������� ���������
*	Returns:	0 ���� ����������� ��������������� ���
*			1 ���� ����������� ��������������� ����
*	Throws:		-
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int Axiom::checkWeakEquivalence(const Axiom& second) const
{
  // �������� �� ���������� ������������ �������
  if (elemConditions.size() != second.elemConditions.size()) {
    return 0;
  }
  
  // �������� ������ �������� ������������ �������
  vector<string> thisNames;
  vector<string> secondNames;
  getECNames(thisNames);
  second.getECNames(secondNames);

  // ��������� �����
  //sort(thisNames.begin(), thisNames.end());
  //sort(secondNames.begin(), secondNames.end());
  
  // ����������
  if (thisNames == secondNames)
    return 1;
  else
    return 0;
}
