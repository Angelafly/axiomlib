/****************************************************************************
*					������� ������ AxiomExprSet
*
*	Description:	������� ������ �� ������ ���� AxiomExpr - ����������
*	Author:			dk
*	History:	
*
****************************************************************************/
#include "AxiomExprSet.h"

using namespace AxiomLib;

#define str_default_axiomSet_name	"no_name_AxiomExprSet"
#define str_default_axiomBank_name	"no_name_AxiomBank"


// ������ ����������� � �������� ����� ������� ������ �� default'�
AxiomExprSet::AxiomExprSet () {
	nameOfAxiomSet.assign (str_default_axiomSet_name);
	nameOfAxiomBank.assign (str_default_axiomBank_name);
	axioms.clear();
}


// ������ ����������� � �������� ����� ������� ������ � ���������
AxiomExprSet::AxiomExprSet (const std::string sName) {
	nameOfAxiomSet = sName;
	nameOfAxiomBank.assign (str_default_axiomBank_name);
	axioms.clear();
}

// ����������� ����������� - ��������� �������
AxiomExprSet::AxiomExprSet(const AxiomExprSet &other) {
	this->nameOfAxiomBank = other.nameOfAxiomBank;
	this->nameOfAxiomSet = other.nameOfAxiomSet;
	
	this->axioms.reserve(other.axioms.size());
	
	for(unsigned int i = 0; i < other.axioms.size(); i++) {
		this->axioms.push_back(other.axioms[i]->clone());
	}
}

// ���������� - ������� ��� ��������� ������������ ������ � ������� ������
AxiomExprSet::~AxiomExprSet(void) {
	this->clear();
}


// ������� ������� ��� ��������� ������������ ������ � ������� ������
void AxiomExprSet::clear(void) {
	for (unsigned int i = 0; i < this->axioms.size(); i++) {
		(axioms[i])->clear();
		delete (axioms[i]);
	}
	axioms.clear();
}


// ��������� ������� � ������������� ������ ������ � ������ �������
void AxiomExprSet::addAxiom (const AxiomExpr& newAxiom) {
	unsigned int oldSize = axioms.size();
	axioms.resize (oldSize + 1);
	axioms[oldSize] = new AxiomExpr;
	*(axioms[oldSize]) = newAxiom;
}


// ��������� ������� � ������������� ������ ������ � ������ �������
void AxiomExprSet::addAxioms (std::vector <AxiomExpr*> &newAxiomExpr) {
	unsigned int oldSize = axioms.size();
	axioms.resize (oldSize + newAxiomExpr.size());
	unsigned int j = 0;
	for (unsigned int i = oldSize; i < axioms.size(); i++, j++) {
		*axioms[i] = *newAxiomExpr[j];
	}
}


// ������������� ����� ������ ������
void AxiomExprSet::setAxioms (std::vector <AxiomExpr*> &newAxiomExpr) {
	clear();
	addAxioms (newAxiomExpr);
}


// ���������� �������� ������� ������
std::string AxiomExprSet::name (void) const {
	return nameOfAxiomSet;
}


// ������� ���������� ����� ������ � ������ �������
int AxiomExprSet::size (void) const {
	return axioms.size();
}


/****************************************************************************
*					AxiomExprSet::enter
*
*	Description:	������� �������� ������������ ���� row ������� � ������� begin �� ������� end
*	Parameters:		marked - ������, ���� ������ ��������
*					row    - ����������� ���
*					begin  - ������ � �������� �������� ��������
*					end    - ������ �� �������� ����� ��������
*	Returns:		0
*	Throws:			AxiomLibExeption - ���� begin � end ������ �����������
*	Author:			dk
*	History:
*
****************************************************************************/
int AxiomExprSet::enter (std::vector<int>& marked, const std::vector < std::vector<double> > &row, const unsigned long begin, const unsigned long end) const {
	// �������� ������� ����������
	unsigned int len = 0;
	for (unsigned int i = 0; i < row.size(); i++) {
		if (row[i].size() > len)
			len = row[i].size();
	}
	if ((end <= begin) || (begin < 0) || (end > len)) {
		throw AxiomLibException("Error in AxiomExprSet::enter : wrong input parameters.");
	}	
	// �������� ���� ��������� �������
	int curRes;
	marked.resize(end - begin);
	unsigned long k = 0;
	for (unsigned long i = begin; i < end; i++, k++) {
		marked[k] = 0;
		for (unsigned int j = 0; j < axioms.size(); j++) {
			curRes = (axioms[j])->check (i, row);
			if (curRes > 0) {
				marked[k] = j + 1;
				break;
			}
		}
	}
 	return 0;
}


/****************************************************************************
*				AxiomExprSet::enter
*
*	Description:	������� �������� ���� row ������� � ������� begin �� ������� end
*	Parameters:		marked - ������, ���� ������ ��������
*					row    - ��� ��� ��������
*					begin - ������ � �������� �������� ��������
*					end    - ������ �� �������� ����� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*					-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:			AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:			dk
*	History:
*
****************************************************************************/
int AxiomExprSet::enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end) {
	// �������� ������� ����������
	if ((end <= begin) || (row.size() < end)) {
		marked.clear();
		throw AxiomLibException("Error in AxiomExprSet::enter : wrong input parameters.");
	}
	// �������� ���� ��������� �������
	int curRes;
	marked.resize(end - begin);
	unsigned long k = 0;
	for (unsigned long i = begin; i < end; i++, k++) {
		marked[k] = 0;
		curRes = 0;
		for (unsigned int j = 0; j < axioms.size(); j++ ) {
			curRes = (axioms[j])->check(i, row);
			if (curRes > 0) {
				marked[k] = j + 1;
				break;
			}
		}
	}
 	return 0;
}


/****************************************************************************
*				AxiomExprSet::enter
*
*	Description:	������� �������� ���� row ������� � ������� begin �� ������� end
*	Parameters:		marked - ������, ���� ������ ��������
*					row    - ��� ��� ��������
*					begin - ������ � �������� �������� ��������
*					end    - ������ �� �������� ����� ��������
*					stat - ���������� �� �������� �������, ����� ���� ������������ ��� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*					-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:			AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:			dk
*	History:
*
****************************************************************************/
int AxiomExprSet::enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat) {
	// �������� ������� ����������
	if ((end <= begin) || (row.size() < end)) {
		marked.resize(0);
		throw AxiomLibException("Error in AxiomExprSet::enter : wrong parameters.");
	}
	// �������� ���� ��������� ������� �� ������ ����������
	int curRes;
	marked.resize(end - begin);
	stat.resize(axioms.size());
	for (unsigned int i = 0; i < axioms.size(); i++)
		stat[i] = false;
	unsigned long k = 0;
	for (unsigned long i = begin; i < end; i++, k++) {
		marked[k] = 0;
		curRes = 0;
		for (unsigned int j = 0; j < axioms.size(); j++ ) {
			curRes = (axioms[j])->check(i, row);
			if (curRes > 0) {
				marked[k] = j + 1;
				if (!stat[j]) stat[j] = true;
				break;
			}
		}
	}
 	return 0;
}

/****************************************************************************
*				AxiomExprSet::enter
*
*	Description:	������� �������� ���� � ������ �������� �������������� ������
*                               row ������� � ������� begin �� ������� end
*	Parameters:		marked - ������, ���� ������ ��������
*					row    - ��� ��� ��������
*					begin - ������ � �������� �������� ��������
*					end    - ������ �� �������� ����� ��������
*					stat - ���������� �� �������� �������, ����� ���� ������������ ��� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*					-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:			AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:			armkor
*	History:
*
****************************************************************************/

int AxiomExprSet::enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat){
    if ((end <= begin) || (row.size() < end)) {
            marked.resize(0);
            throw AxiomLibException("Error in AxiomExprSet::enter : wrong parameters.");
    }
    else {
        int curRes;
        marked.resize(end - begin);
        for (unsigned long i=begin;i<end;i++){
            marked[i-begin].resize(axioms.size());
            for (int j=0;j<axioms.size();j++){
                marked[i-begin][j]=false;
            }
        }
        stat.resize(axioms.size());
        for (unsigned int i = 0; i < axioms.size(); i++){
                stat[i] = false;
            }
        for (unsigned long i = begin; i < end; i++) {
                curRes = 0;
                for (unsigned int j = 0; j < axioms.size(); j++ ) {
                        curRes = (axioms[j])->check(i, row);
                        if (curRes > 0) {
                                marked[i-begin][j] = true;
                                if (!stat[j]) stat[j] = true;
                        }
                }
        }

}
    return 0;

}


/****************************************************************************
*				AxiomExprSet::operator=
*
*	Description:	�������� ������������ ������� ������
*	Parameters:		second - ������������� ������� ������
*	Returns:		*this
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
AxiomExprSet& AxiomExprSet::operator= (const AxiomExprSet &second) {
	// �������� ��� ������� ������
	this->nameOfAxiomSet = second.nameOfAxiomSet;
	this->nameOfAxiomBank = second.nameOfAxiomBank;
	// �������� �������� � ������� �������
	this->clear();
	axioms.resize(second.axioms.size());
	for (unsigned int i = 0; i < second.axioms.size(); i++) {
		axioms[i] = second.axioms[i]->clone();
	}
	return *this;
}


/****************************************************************************
*					AxiomExprSet::createAxiomExprSetStructure
*
*	Description:	������� ��������� ��������� ������ ������� ������ � AxiomExprSetStructures
*	Parameters:		aess - ����������� ���� ��������� ������� ������
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int AxiomExprSet::createAxiomExprSetStructure (AxiomExprSetStructure &aess) const {
	aess.axiomBankDir = nameOfAxiomBank;
	aess.axiomSetName = nameOfAxiomSet;
	aess.axioms.clear();
	aess.axioms.resize(this->axioms.size());
	for (unsigned int i = 0; i < this->axioms.size(); i++)
		axioms[i]->createAxiomExprStructure (aess.axioms[i]);
	return 0;
}


/****************************************************************************
*					AxiomExprSet::initAxiomSetFromFile
*
*	Description:	������� �������� ������� ������ ��� ����� ������ ������������ ������
*	Parameters:		axiomBaseDir - ����������, ��� ��������� ������������� ���� �������� ������� ������
*					axiomSetName - ��� ������� ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int AxiomExprSet::initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName) {
	AxiomSetBase axiomSetBase;
	AxiomExprSetStructure aess;
	// ��������� ������ ���� � �����
	std::string axiomSetPath = axiomSetBaseDir;
	axiomSetPath.append("/");
	axiomSetPath.append(axiomSetName);
	// ��������� ���������� � ������� ������ � aess
	axiomSetBase.readFromAS (axiomSetPath, aess);
	return initAxiomSetFromStructure (aess);
}

/****************************************************************************
*					AxiomExprSet::initAxiomSetFromFile
*
*	Description:	������� �������� ������� ������ ��� ����� ������ ������������ ������.
*					������� �� �������� � ����������, ������ ��������� ��������� ���� ������������
*	Parameters:		axiomBaseDir - ����������, ��� ��������� ������������� ���� �������� ������� ������
*					axiomSetName - ��� ������� ������
*					paramNames - ����� ������������
*	Returns:		0
*	Throws:			-
*	Author:			wictor
*	History:
*
****************************************************************************/
signed int AxiomExprSet::initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, const std::vector<std::string> &paramNames) {
	AxiomSetBase axiomSetBase;
	AxiomExprSetStructure aess;
	// ��������� ������ ���� � �����
	std::string axiomSetPath = axiomSetBaseDir;
	axiomSetPath.append("/");
	axiomSetPath.append(axiomSetName);
	
	// TODO: ������ ��� � �������� const � ��������������� �����
	std::vector<std::string> temp(paramNames);
	
	// ��������� ���������� � ������� ������ � aess
	axiomSetBase.readFromAS (axiomSetPath, aess, temp);
	return initAxiomSetFromStructure (aess);
}


/****************************************************************************
*					AxiomExprSet::initAxiomSetFromStructure
*
*	Description:	������� �������� ������� ������ �� ������������������ ��������� � ��������� ������� ������
*	Parameters:		aess - ��������� � ��������� ������� ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int AxiomExprSet::initAxiomSetFromStructure (AxiomExprSetStructure &aess) {
	this->clear();
	if (aess.axiomSetName.size() > 0)
		nameOfAxiomSet = aess.axiomSetName;
	if (aess.axiomBankDir.size() > 0)
		nameOfAxiomBank = aess.axiomBankDir;
	AxiomExpr* axiomExpr;
	this->axioms.resize(aess.axioms.size());
	for (unsigned int i = 0; i < aess.axioms.size(); i++) {
		axiomExpr = new AxiomExpr(aess.axioms[i].axiomName);
		axiomExpr->initAxiomFromStructure (aess.axioms[i]);			
		this->axioms[i] = axiomExpr;
	}	
	return 0;
}


/****************************************************************************
*					AxiomExprSet::initAxiomSetByRand
*
*	Description:	������� ������� ������� ������ ��������� �������
*	Parameters:		numOfAxioms  - ����� ������� ����������� ������
*					maxECinAxiom - ������������ ����� ������������ ������� ������� ����� ���������� ���� �������
*	Returns:		0 - ���� ��� ��������, � ��������� ������ ����� �� throw 
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int AxiomExprSet::initAxiomSetByRand (const int numOfAxioms, const int maxECinAxiom) {
	return initAxiomSetByRand (numOfAxioms, maxECinAxiom, -1, -1);
}


/****************************************************************************
*					AxiomExprSet::initAxiomSetByRand
*
*	Description:	������� ������� ������� ������ ��������� ������� � ��������� 
*					����������� ������ ������ ��� ������� ������� �� ��������� ���������
*	Parameters:		numOfAxioms  - ����� ������� ����������� ������
*					maxECinAxiom - ������������ ����� ������������ ������� ������� ����� ���������� ���� �������
*					dimensionMin - ����������� �������� ��� ����������� �������� ������
*					dimensionMax - ������������ �������� ��� ����������� �������� ������
*	Returns:		0 - ���� ��� ��������, � ��������� ������ ����� �� throw 
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int AxiomExprSet::initAxiomSetByRand (const int numOfAxioms, const int maxECinAxiom, const int dimensionMin, const int dimensionMax) {
	this->clear();
	int numOfEC;
	// �������� ����� ������ � ������ �������
	this->axioms.resize(numOfAxioms);
	for (int i = 0; i < numOfAxioms; i++) {
		this->axioms[i] = new AxiomExpr();
		// ��������� ������� ���������� ������� ������������ ������� ����� ������� � i-�� �������
		numOfEC = round(((double) rand() / (double) RAND_MAX) * (maxECinAxiom-1) + 1);
		// ���������� �������� ����� �������
		this->axioms[i]->initAxiomByRand(numOfEC, dimensionMin, dimensionMax);
	}
	return 0;
}


/****************************************************************************
*					AxiomExprSet::saveAxiomSetToFile
*
*	Description:	������� ���������� ������� ������ � ����
*	Parameters:		baseDir - ����������, ��� ��������� ������������� ���� �������� ������� ������
*					AxiomSetName - ���, ��� ������� ��������� ������ ������� ������
*					first - ����� ������ I ���� ������ ������� ������ �� ��������� �������
*					second - ����� ������ II ���� ������ ������� ������ �� ��������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int AxiomExprSet::saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, int first, int second) const {
	std::vector <std::string> dataSetParams;
	dataSetParams.clear();
	saveAxiomSetToFile (baseDir, axiomSetName, dataSetParams, first, second);
	return 0;
}


/****************************************************************************
*					AxiomExprSet::saveAxiomSetToFile
*
*	Description:	������� ���������� ������� ������ � ����
*					(� ��������� ���������� �������� ������ ��� ������� ������������� �������)
*	Parameters:		baseDir - ����������, ��� ��������� ������������� ���� �������� ������� ������
*					AxiomSetName - ���, ��� ������� ��������� ������ ������� ������
*					dataSetParams - ������ �������� ������������ �������� ������
*					first - ����� ������ I ���� ������ ������� ������ �� ��������� �������
*					second - ����� ������ II ���� ������ ������� ������ �� ��������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int AxiomExprSet::saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, std::vector <std::string> &dataSetParams, int first, int second) const {
	AxiomExprSetStructure aess;
	createAxiomExprSetStructure (aess);
	aess.axiomSetName = axiomSetName;
	AxiomSetBase axiomSetBase;
	axiomSetBase.saveToAS (baseDir, aess, dataSetParams, first, second);
	return 0;
}
