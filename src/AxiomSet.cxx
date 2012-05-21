/****************************************************************************
*			������� ������ AxiomSet.h
*
*	Description:	����� ������ - ����������
*	Author:		dk,gevor
*	History:	
*
****************************************************************************/
#include "AxiomSet.h"

// ��������� �������� ��� ��������� ������
#define str_default_axiomSet_name	"no_name_AxiomSet"
#define str_default_axiomSet_bank	"no_name_AxiomBank"

using namespace AxiomLib;
using namespace std;

// ������ ����������� � �������� ����� ������� ������ �� default'�
AxiomSet::AxiomSet () {
	nameOfAxiomSet.assign (str_default_axiomSet_name);
	nameOfAxiomBank.assign (str_default_axiomSet_bank);
	axioms.resize(0);
	crossoverControlBit = 0;
}


// ������ ����������� � �������� ����� ������� ������ � ���������
AxiomSet::AxiomSet (const std::string sName) {
	nameOfAxiomSet.assign (sName);
	nameOfAxiomBank.assign (str_default_axiomSet_bank);
	axioms.resize(0);
	crossoverControlBit = 0;
}


// ���������� - ������� ��� ��������� ������������ ������ � ������� ������
AxiomSet::~AxiomSet(void) {
	for (unsigned int i = 0; i < this->axioms.size(); i++) {
		(axioms[i])->clear();
		delete (axioms[i]);
	}
}


// ���������� - ������� ��� ��������� ������������ ������ � ������� ������
void AxiomSet::clear(void) {
	for (unsigned int i = 0; i < this->axioms.size(); i++) {
		(axioms[i])->clear();
		delete (axioms[i]);
	}
	axioms.resize(0);
}


// �������� ������� (���������� ���������� �� ����������, ������������ ��� ������� ��������� ������)
std::string AxiomSet::name (void) const {
	//std::cout << "name() : \n";// << nameOfAxiomSet << "\n";
	return nameOfAxiomSet;
}


// ������� ���������� ����� ������ � ������ �������
int AxiomSet::size (void) const {
	return axioms.size();
}


// ������� ��������� �������� ������������ ���� �����������
int AxiomSet::setCrossoverControlBit (const int newCrossoverControlBit) {
	crossoverControlBit = newCrossoverControlBit;
	return newCrossoverControlBit;
}


// ������� ���������� ������� �������� ������������ ���� �����������
int AxiomSet::getCrossoverControlBit (void) const {
	return crossoverControlBit;
}


// ������� ���������� ����� ��������� � ������� axioms ������� ������ ������
signed int AxiomSet::getNumOfAxioms(void) const {
	return (int) axioms.size();
}


/****************************************************************************
*				AxiomSet::getAxiom
*
*	Description:	������� ���������� ������� � �������� ������� � ������� ������ ������ �������
*	Parameters:	int i - ����� ������������� ������� � ������� ������
*	Returns:		������� ���������� ������������� ������� ��� ����� ���, ��� � ������� ������ ������ ������� ������
*	Throws:		AxiomLibExeption - ���� �������� ������� �� ������������� ������� ��������� ���������
*	Author:		dk
*	History:
*
****************************************************************************/
Axiom& AxiomSet::getAxiom (int i) const {
	// �������� �������� ������� ������������� �������
	if ( ( i <  0) || ( i  >= (int) axioms.size() ) ) {
		throw AxiomLibException("Error in AxiomSet::getAxiom : parameter value out of range.");
	}
	return (*axioms[i]);
}


/****************************************************************************
*				AxiomSet::setAxiom
*
*	Description:	������� �������� ������� � �������� �������  ������� ������
*	Parameters:	axiom - ������� - �� ������� �������� i-�� � ������� ������
*				i - ����� ������������� ������� � ������� ������
*	Returns:		0 - ���� ��� ���������
*	Throws:		AxiomLibExeption - ���� �������� ������� �� ������������� ������� ��������� ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::setAxiom (Axiom& axiom, const int i) {
	if ( ( i <  0) || ( i  > (int) axioms.size() ) ) {
		throw AxiomLibException("Error in AxiomSet::setAxiom : parameter value out of range.");
	}
	
	if ( i  < (int) axioms.size() ) {
		*(axioms[i]) = axiom;
        }
	else { // this mean: i == axioms.size()
		Axiom* addAxiom;
		addAxiom = new Axiom;
		*addAxiom = axiom;
		axioms.push_back (addAxiom);
	}
	return 0;
}


/****************************************************************************
*				AxiomSet::slide
*
*	Description:	������� ������������ ������ ������ � ������� �� ������� �������
*	Parameters:	i - ������ ������
*				j - ����� ������ 
*	Returns:		0 - ���� ��� ���������
*	Throws:		AxiomLibExeption - ���� �������� ������� �� ������������� ������� ��������� ���������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::slide (const int i, const int j) {
	if ( ( i <  0) || ( i  > (int) axioms.size() ) || ( j <  0) || (j  > (int) axioms.size() ) ) {
		throw AxiomLibException("Error in AxiomSet::setAxiom : parameter value out of range.");
	}
	int start, finish;
	if ( j < i ) {
		start = j;
		finish = i;
	} else {
		start = i;
		finish = j;
	}
	Axiom* axiomTemp;
	axiomTemp = axioms[finish];
	for (int k = finish - 1; k >= start; k--) {
		axioms[k+1] = axioms[k];
	}
	axioms[start] = axiomTemp;
	return 0;
}


/****************************************************************************
*				AxiomSet::enter
*
*	Description:	������� �������� ���� row ������� � ������� begin �� ������� end
*	Parameters:	marked - ������, ���� ������ ��������
*				row    - ��� ��� ��������
*				begin - ������ � �������� �������� ��������
*				end    - ������ �� �������� ����� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*				-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:		AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:		dk
*	History:
*
****************************************************************************/
int AxiomSet::enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end) {
	if ((end <= begin) || (row.size() < end)) {
		marked.resize(0);
		throw AxiomLibException("Error in AxiomSet::enter : wrong parameters.");
	}
	else {
		int curRes;
		marked.resize(end - begin);
		//std::cout << "\n New markuped row: \n	";
		for (unsigned long i = begin; i < end; i++) {
			marked[i - begin] = 0;
			curRes = 0;
			for (unsigned int j = 0; j < axioms.size(); j++ ) {
				curRes = (axioms[j])->check(i, row);
				if (curRes == 1) {
					marked[i - begin] = j + 1;
					break;
				}
			}
		}
	}
 	return 0;
}


/****************************************************************************
*				AxiomSet::enter
*
*	Description:	������� �������� ���� row ������� � ������� begin �� ������� end
*	Parameters:	marked - ������, ���� ������ ��������
*				row    - ��� ��� ��������
*				begin - ������ � �������� �������� ��������
*				end    - ������ �� �������� ����� ��������
*				stat - ���������� �� �������� �������, ����� ���� ������������ ��� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*				-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:		AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:		dk
*	History:
*
****************************************************************************/
int AxiomSet::enter (std::vector<int>& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat) {
	if ((end <= begin) || (row.size() < end)) {
		marked.resize(0);
		throw AxiomLibException("Error in AxiomSet::enter : wrong parameters.");
	}
	else {
		int curRes;
		marked.resize(end - begin);
		stat.resize(axioms.size());
		for (unsigned int i = 0; i < axioms.size(); i++)
			stat[i] = false;
		//std::cout << "\n New markuped row: \n	";
		for (unsigned long i = begin; i < end; i++) {
			marked[i - begin] = 0;
			curRes = 0;
			for (unsigned int j = 0; j < axioms.size(); j++ ) {
				curRes = (axioms[j])->check(i, row);
				if (curRes == 1) {
					marked[i - begin] = j + 1;
					if (!stat[j]) stat[j] = true;
					break;
				}
			}
		}
	}
 	return 0;
}

/****************************************************************************
*				AxiomSet::enter
*
*	Description:	������� �������� ���� ��� ������ �������� ���������� ������
*                       row ������� � ������� begin �� ������� end
*	Parameters:	marked -���� ������ ��������
*				row    - ��� ��� ��������
*				begin - ������ � �������� �������� ��������
*				end    - ������ �� �������� ����� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*				-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:		AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:		armkor
*	History:
*
****************************************************************************/

int AxiomSet::enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end) {
        if ((end <= begin) || (row.size() < end)) {
                marked.resize(0);
                throw AxiomLibException("Error in AxiomSet::enter : wrong parameters.");
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

                for (unsigned long i = begin; i < end; i++) {
                        curRes = 0;
                        for (unsigned int j = 0; j < axioms.size(); j++ ) {
                                curRes = (axioms[j])->check(i, row);
                                if (curRes == 1) {
                                        marked[i - begin][j] = true;
                                }
                        }
                }
        }
        return 0;
}

/****************************************************************************
*				AxiomSet::enter
*
*	Description:	������� �������� ���� ��� ������ �������� ���������� ������
*                       row ������� � ������� begin �� ������� end
*	Parameters:	marked -���� ������ ��������
*				row    - ��� ��� ��������
*				begin - ������ � �������� �������� ��������
*				end    - ������ �� �������� ����� ��������
*				stat - ���������� �� �������� �������, ����� ���� ������������ ��� ��������
*	Returns:		������� ���������� ����������� ��� - ��� � ������� ������� ����������� �� ������ �� �������� ��������� ����
*				-1 - �������� ��� �� ���� ������� �� ��������� - �� ���� ������� ������� ������������ ��� ���� ���������
*	Throws:		AxiomLibExeption - ���� begin � end ������ �� ���������
*	Author:		armkor
*	History:
*
****************************************************************************/


int AxiomSet::enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat){
  //  std::cout <<"AxiomSet::enter";
    if ((end <= begin) || (row.size() < end)) {
            marked.resize(0);
            throw AxiomLibException("Error in AxiomSet::enter : wrong parameters.");
    }
    else {
            int curRes;
            marked.resize(end - begin);
            stat.resize(axioms.size());
            for (unsigned int i = 0; i < axioms.size(); i++)
                                    stat[i] = false;
            for (unsigned long i=begin;i<end;i++){
                marked[i-begin].resize(axioms.size());

                for (int j=0;j<axioms.size();j++){
                    marked[i-begin][j]=false;
                }
            }

            for (unsigned long i = begin; i < end; i++) {
                //std::cout << "S";
                    curRes = 0;
                    for (unsigned int j = 0; j < axioms.size(); j++ ) {
                            curRes = (axioms[j])->check(i, row);
         //                   std::cout << "!"<< curRes << "!";
                            if (curRes != 0) {
                  //              std::cout << "E";
                                    marked[i - begin][j] = true;
       //                             std::cout << "@"<< marked[i - begin][j] << "@";
                                    if (!stat[j]) stat[j] = true;
                            }
                    }
            }
    }/*
    for(int k=0;k<marked.size();k++){
        for (int t=0;t<marked[k].size();t++){
            std::cout << marked[k][t] << "\n";
        }
    }*/
    return 0;
}


/****************************************************************************
*				AxiomSet::initAxiomSetByRand
*
*	Description:	������� ������� ������� ������ ��������� �������
*	Parameters:	numOfAxioms  - ����� ������� ����������� ������
*				maxECinAxiom - ������������ ����� ������������ ������� ������� ����� ���������� ���� �������
*	Returns:		0 - ���� ��� ��������, � ��������� ������ ����� �� throw 
*	Throws:		-
*	Author:		dk,gevor
*	History:
*
****************************************************************************/
signed int AxiomSet::initAxiomSetByRand (const int numOfAxioms, const int maxECinAxiom) {
	int numOfEC;
	Axiom* axiom;
	axioms.resize(numOfAxioms);
	for (int i = 0; i < numOfAxioms; i++) {
		axiom = new Axiom();
		numOfEC = round(((double) rand() / (double) RAND_MAX) * (maxECinAxiom-1) + 1);
		axiom->initAxiomByRand(numOfEC);
		while (axiom->size() == 0) {
		  numOfEC--;
		  cout << "numOfEC too large, setting '" << numOfEC << "'\n";
		  axiom->initAxiomByRand(numOfEC);
		}
//                cout << "numOfEC = '" << numOfEC << "', num of ecs in axiom = '" << axiom->size() << "'\n";
		axioms[i] = axiom;
	}
	
	return 0;
}


/****************************************************************************
*				AxiomSet::initAxiomSetByNames
*
*	Description:	������� �������� ������� ������ ��� ����� ������ ������������ ������
*	Parameters:	axiomEC - ������ "�����������"
*				"�����������" - ������ ����� ������������ ������� ��� ����� ������������ ������� � ������� ��������� � �������
*				axiomNames - ������ ���� ����������� ������ (� ������� ���������� ����������)
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:		AxiomLibExeption - ���� ������ �������� ������������ ������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::initAxiomSetByNames (const std::vector < std::vector < std::string > >& axiomEC, const std::vector <std::string>& axiomNames) {
	if (axiomEC.size() < 1) {
		throw AxiomLibException("Error in AxiomSet::initAxiomSetByNames : wrong first input parameter - it is empty.");
		return -1;
	}
	else {
		Axiom* axiom;
		for (unsigned int i = 0; i < axiomEC.size(); i++) {
			if (i < axiomNames.size()) {
				axiom = new Axiom(axiomNames[i]);
			}
			else {
				axiom = new Axiom();
			}
			axiom->initAxiomByNames(axiomEC[i]);
			axioms.push_back(axiom);
		}
	}
	return 0;
}


/****************************************************************************
*				AxiomSet::initAxiomSetByNums
*
*	Description:	������� �������� ������� ������ ��� ����� ������ ������������ ������
*	Parameters:	axiomEC - ������ "�����������"
*				"�����������" - ������ ������� ������������ ������� ��� ����� ������������ ������� � �������� ��������� � �������
*				axiomNames - ������ ���� ����������� ������ (� ������� ���������� ����������)
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:		AxiomLibExeption  - ���� ������ �������� ������������ ������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::initAxiomSetByNums (const std::vector < std::vector < int > >& axiomEC, const std::vector <std::string>& axiomNames) {
	if (axiomEC.size() < 1) {
		throw AxiomLibException("Error in AxiomSet::initAxiomSetByNums : wrong first input parameter - it is empty.");
		return -1;
	}
	else {
		Axiom* axiom;
		for (unsigned int i = 0; i < axiomEC.size(); i++) {
			if (i < axiomNames.size()) {
				axiom = new Axiom(axiomNames[i]);
			}
			else {
				axiom = new Axiom();
			}
			axiom->initAxiomByNums(axiomEC[i]);
			axioms.push_back(axiom);
		}
	}
	return 0;
}


/****************************************************************************
*				AxiomSet::initAxiomSetByCopy - ���������� ������ - ��� �������������� - ����� ������������ ��� �������� ����
*
*	Description:	������� �������� ������� ������ ����� ����������� ��������� ������� ������ �� �������� ���������
*	Parameters:	axiomVec - ����� ������, ������� ������� ������� � ����������� axiomSet'�
*				levelOfSub - ������� �� �������� ������� ���������� ����������� ����� ������ � ������
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:		AxiomLibExeption - ���� ������ �������� ������������ ������� 
*	Author:		dk
*	History:
*
****************************************************************************/
/*signed int AxiomSet::initAxiomSetByCopy (const std::vector <Axiom*> &axiomVec, double levelOfSub) {
	this->axioms.clear();
	this->axioms.resize(axiomVec.size());
//std::cout << "\n 			Copying \n";
	for (int i = 0; i < axiomVec.size(); i++) {
		this->axioms[i] = new Axiom;
		*(this->axioms[i]) = *(axiomVec[i]);
//std::cout << "\n 			this->axioms[i]->name() is " << this->axioms[i]->name();
//std::cout << "\n 			this->axioms[i]->size() is " << this->axioms[i]->size() << "\n";
	}
//std::cout << "\n";
	return 0;
}*/


/****************************************************************************
*				AxiomSet::initAxiomSetByCopy
*
*	Description:	������� �������� ������� ������ ����� ����������� ��������� ������� ������ �� �������� ���������
*	Parameters:	axiomVec - ����� ������, ������� ������� ������� � ����������� axiomSet'�
*				levelOfSub - ������� �� �������� ������� ���������� ����������� ����� ������ � ������
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:		AxiomLibExeption - ���� ������ �������� ������������ ������� 
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::initAxiomSetByCopy (const std::vector <Axiom*> &axiomVec, double levelOfSub) {
	// �������� ������������ �������� ��������� - �� ���� �� ������
	if (axiomVec.size() < 1) {
		throw AxiomLibException("Error in AxiomSet::initAxiomSetByCopy : wrong first input vector of axioms- it is empty.");
	}
	if ( (levelOfSub > 1) || (levelOfSub < 0) ) {
		throw AxiomLibException("Error in AxiomSet::initAxiomSetByCopy : wrong first input level Of Sub- it is out of range.");
	}
	// ���������� ������ ��� ����, ����� � ������� ������ ������� ����� ���������� ���������
	double temp;
	unsigned int cur;
	std::vector<int> mask;
	mask.resize(axiomVec.size());
	for (unsigned int i = 0; i < axiomVec.size(); i++) {
		cur = i;
		for (unsigned int j = 0; j < i; j++) {
			temp = (*(axiomVec[i])) < (*(axiomVec[mask[j]]));
			if (temp > levelOfSub) {
				cur = j;
				break;
			}
		}
		// ������� ���� ���������� ����� ������� 
		for (unsigned int h = i; h > cur; h--) {
			mask[h] = mask[h-1];
		}
		mask[cur] = i;
	}
	// ������ ������ � �������
	this->axioms.clear();
	this->axioms.resize(axiomVec.size());
	//std::cout << "\n 		Init::Maks is \n		";
	for (unsigned int i = 0; i < axiomVec.size(); i++) {
		this->axioms[i] = new Axiom;
		*(this->axioms[i]) = *(axiomVec[mask[i]]);
		//std::cout << "  " << mask[i];
	}
	//std::cout << "\n";
	return 0;
}


/****************************************************************************
*				AxiomSet::initAxiomSetFromFile
*
*	Description:	������� �������� ������� ������ ��� ����� ������ ������������ ������
*	Parameters:	axiomBaseDir - ����������, ��� ��������� ������������� ���� �������� ������� ������
*				axiomSetName - ��� ������� ������
*				axiomBase - ������������ ���� ������
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:		AxiomLibExeption - ���� ����� �� �������
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, AxiomBase &axiomBase) {
	AxiomSetBase axiomSetBase;
	AxiomSetStructure axiomSetStructure;
	// ��������� ������ ���� � �����
	std::string axiomSetPath = axiomSetBaseDir;
	axiomSetPath.append("/");
	axiomSetPath.append(axiomSetName);
	// ��������� ���������� � ������� ������ � axiomSetStructure
	axiomSetBase.readFromAS (axiomSetPath, axiomSetStructure);
	// ��������� ���� ������ � ������������ �� ���������� ����������
	if (axiomSetStructure.axiomSetName.size() > 0)
		nameOfAxiomSet = axiomSetStructure.axiomSetName;
	if (axiomSetStructure.axiomBankDir.size() > 0)
		nameOfAxiomBank = axiomSetStructure.axiomBankDir;
	std::vector<std::string> axiomECs;
	axiomECs.resize(0);
	Axiom* axiom;
	for (unsigned int i = 0; i < axiomSetStructure.axioms.size(); i++) {
		axiom = new Axiom(axiomSetStructure.axioms[i].axiomName);
		if (axiomSetStructure.axioms[i].axiomName == "no_name") {
			axiomSetStructure.axioms[i].getVectorOfEC (axiomECs);
		}
		else {
			axiomBase.getVectorOfEC(axiomECs, axiomSetStructure.axioms[i].axiomName);
		}
		if (axiomECs.size() > 0) {
			axiom->initAxiomByNames(axiomECs);
			if (axiomSetStructure.axioms[i].axiomParameters.size() != 0)
				axiom->setECParameters (axiomSetStructure.axioms[i].axiomParameters);
			axioms.push_back(axiom);
		}
		else {
			delete (axiom);
			//std::cout << "\n 	i is " << i << "\n";
			//std::cout << "	axiomSetStructure.axioms[i].axiomName is " << axiomSetStructure.axioms[i].axiomName << "\n";
			throw AxiomLibException ("Error in AxiomSet::initAxiomSetFromFile: in Axiom Set found axiom with a name that is not in the AxiomBank.");
		}
	}
	
	return 0;
}


/****************************************************************************
*					AxiomSet::createAxiomSetStructure
*
*	Description:	������� ��������� ��������� ������ ������� ������ � AxiomSetStructures
*	Parameters:		ass - ���������, � ������� ���������� �������� ������� ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int AxiomSet::createAxiomSetStructure (AxiomSetStructure &ass) const {
	// ��������� ���� ��������
	ass.axiomSetName = this->nameOfAxiomSet;
	ass.axiomBankDir = this->nameOfAxiomBank;
	// �������� ���������� ����� � ��������� ������
	ass.axioms.resize (this->axioms.size());
	for (unsigned int i = 0; i < this->axioms.size(); i++) {
		this->axioms[i]->createAxiomStructure (ass.axioms[i]);		
	}
	return 0;
}


/****************************************************************************
*				AxiomSet::saveAxiomSetToFile
*
*	Description:	������� ���������� ������� ������ � ����
*	Parameters:		baseDir - ����������, ��� ��������� ������������� ���� �������� ������� ������
*					axiomSetName - ��� ������� ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
signed int AxiomSet::saveAxiomSetToFile (std::string baseDir, std::string axiomSetName, int first, int second) {
	AxiomSetStructure ass;
	createAxiomSetStructure (ass);
	ass.axiomSetName = axiomSetName;
	AxiomSetBase axiomSetBase;
	axiomSetBase.saveToAS (baseDir, ass, first, second);
	return 0;
}


/****************************************************************************
*				AxiomSet::saveAxiomSetToFile
*
*	Description:	������� ���������� ������� ������ � ����
*	Parameters:	env - ����� � ������� �� ��������� �������
*				envDataSet - ����� � ������� �� ������� ������ ������
*	Returns:		0 - ���� ��� ���������, � ��������� ������ ����� �� throw
*	Throws:		AxiomLibExeption - ���� ����� �� �������
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int AxiomSet::saveAxiomSetToFile (Environment &env, EnvDataSet &envDataSet, int first, int second){
  // ������ ������ ������� ������ � ������ �������� ����� ����� � ����� ������� 
  // (����� ��� ������������� �������� � ������� ������ ������)

  // �������� ������� ������� axiomset-��
  std::string axiomSetBaseDir;
  if (env.getStringParamValue(axiomSetBaseDir, "AxiomSetBaseDir") < 0) {
    throw AxiomLibException("AxiomSet::saveAxiomSetToFile : can not get axiom base dir");
  }

  // �������� �� ��������� ����� ��� - ���� ���������
  std::string whereToSave;
  if (env.getStringParamValue(whereToSave, "saveTo") < 0) {
    // ���, � �������� �� ������, ���������, ��� � dataset-�
    if (envDataSet.getWhereToSave (whereToSave) < 0) {
      throw AxiomLibException("AxiomSet::saveAxiomSetToFile : can not get axiom set name to save to");
    }
  }

  // ���������
  saveAxiomSetToFile (axiomSetBaseDir, whereToSave, first, second);

  return 0;
}


/****************************************************************************
*				AxiomSet::setAxiomParameters
*
*	Description:	������� ������� ���������� ������� ������
*	Parameters:	�������� ��������� ����������� �� ���� ������ �� "���������"
*				��� "���������" - ��� ����������� �� ���� ����������� ������� � "������ ���������"
*				��� "������ ���������" ���������� ���������� �� ���� ���������� � ������� ����������
*	Returns:		0 - ���� ���� �� ���� �������� ��� �������
*				-1 - ���� ���-�� ��������� �� ���, ������������ ���������� �� ��������
*				-2 - ���� �� ���� �� ���������� �� ��� �������
*	Throws:		AxiomLibExeption - ���� ������� �������� ����
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::setAxiomParameters (const std::map < std::string , std::map <std::string, std::map <std::string, std::string> > >& axiomParams) {
	signed int toReturn=-2;

	// ���������, ������ �� �����?
	if (axiomParams.size() == 0) {
		toReturn = -1;
		throw AxiomLibException("Error in AxiomSet::setAxiomParameters : zero parameter map.");
	}

	// ����� �����: ����� � �����, ��������� ���������
	// ��� ����, �� �����������, ��� �� ��� ��������� ����� �������� � ���� �����, ��� ���
	for (std::map <std::string, std::map <std::string, std::map <std::string, std::string> > >::const_iterator i = axiomParams.begin(); i != axiomParams.end(); i++) {
		std::string axiomName = i->first;
		std::map <std::string, std::map <std::string, std::string> > paramMap = i->second;
		for (unsigned int j = 0; j < this->axioms.size(); j++) {
			// ����, ������ ��������� ������������� i
			if (axiomName == this->axioms[j]->name()) {
				// ok, �����, ������� value
				toReturn = 0;
				this->axioms[j]->setECParameters(paramMap);
			}
		}
	}

  	return toReturn;
}
  

/****************************************************************************
*				AxiomSet::getAxiomParameters
*
*	Description:	������� ���������� �������� ������������� ���������� ������� ������
*	Parameters:	axiomParams - ����������� ��������
*				�� ��������� ����������� �� ���� ������ �� "���������"
*				��� "���������" - ��� ����������� �� ���� ����������� ������� � "������ ���������"
*				��� "������ ���������" ���������� ���������� �� ���� ���������� � ������� ����������
*	Returns:		0 
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::getAxiomParameters (std::map < std::string , std::map <std::string, std::map <std::string, std::string> > > &axiomParams) {
	std::string curName;
	axiomParams.clear();
	for (unsigned int j = 0; j < this->axioms.size(); j++) {
		std::map <std::string, std::map <std::string, std::string> > newMap;
		(this->axioms[j])->getECParameters(newMap);
		curName = this->axioms[j]->name();
		axiomParams[curName] = newMap;
	}
	return 0;
}


/****************************************************************************
*				AxiomSet::setParamValue
*
*	Description:	������� ������������� �������� ��������� �� ������ ������� � ������� ������, 
*				�� ������ ������������� ������� � ������ ������, �� ����� ���������
*	Parameters:	param - ��������������� �������� ���������
*				axiomNum - ����� ������� � ������� ������ 
*				ecNum - ����� ������������� ������� � �������
*				paramName - ������ � ������ ���������
*	Returns:		0 - ���� �������� ��������� ���������
*				-1 - ���� �������� �� �����������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::setParamValue (double param, int axiomNum, int ecNum, std::string &paramName) {
	// �������� ������������ ������� ����������
	if ( (axiomNum >= 0) && (axiomNum < (int) axioms.size()) ) {
		return ( (axioms[axiomNum])->setParamValue(param, ecNum, paramName) );
	}
	return -1;
}
    

/****************************************************************************
*				AxiomSet::getParamValue
*
*	Description:	������� ���������� �������� ��������� �� ������ ������� � ������� ������, 
*				�� ������ ������������� ������� � ������ ������, �� ����� ���������
*				���� �������� ������������� ���� - �� ������������ �������������� �������� 
*				������������� ������� �������� � ������������ 1.
*				���� �������� �� �������� � �� ������������� ���� - ������������ 2. 
*				(�������� �������� ������ ���� ���������� ��������� ��������� ������� ����������� - 
*				� ������ �������� �������� � �������� �������������� ���������� - 
*				��� ��������� ��������� ������������ ��� �����������)
*	Parameters:	param - ����������� �������� ���������
*				axiomNum - ����� ������� � ������� ������
*				ecNum - ����� ������������� ������� � �������
*				paramName - ����� � ������ ���������, �������� �������� ��������� ��������
*	Returns:		0 - ���� ������� ��������� ��������
*				-1 - ���� �� ������� ������� �������� (�������� ��� ������ ��� � ������ ������������ �������)
*				1 - ���� �������� ������������� ����
*				2 - ���� �������� �� ��������� � �� ������������ ����
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
signed int AxiomSet::getParamValue (double &param, int axiomNum, int ecNum, std::string &paramName) {
	// �������� ������������ ������� ����������
	if ( (axiomNum >= 0) && (axiomNum < (int) axioms.size()) ) {
		return ( (axioms[axiomNum])->getParamValue(param, ecNum, paramName) );
	}
	return -1;
}


/****************************************************************************
*				AxiomSet::operator=
*
*	Description:	�������� ������������ ������� ������
*	Parameters:	second - ������������� ������� ������
*	Returns:		*this
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
AxiomSet & AxiomSet::operator= (const AxiomSet &second) {
	// �������� �������� ������������ ����
	crossoverControlBit = second.getCrossoverControlBit ();
	// �������� ��� ������� ������
	nameOfAxiomSet = second.name ();
	// �������� �������� � ������� �������
	for (unsigned int i = 0; i < axioms.size(); i++) {
		(axioms[i])->clear();
		delete (axioms[i]);
	}
	int num;
	num = second.getNumOfAxioms ();
	axioms.resize(num);
	for (int i = 0; i < num; i++) {
		axioms[i] = new Axiom;
		*axioms[i] = second.getAxiom (i);
	}
	return *this;
}

/****************************************************************************
*                               AxiomSet::checkWeakEquivalence
*
*       Description:    �������� (�������) ����������� ������������ ���� ������ ������:
*                       ��������� ������ ���������.
*       Parameters:     const AxiomSet& second - ����� ������, � ������� ���� ��������� ���������
*       Returns:        0 ���� ����������� ��������������� ���
*                       1 ���� ����������� ��������������� ����
*       Throws:         -
*       Author:         gevor
*       History:
*
****************************************************************************/
signed int AxiomSet::checkWeakEquivalence(const AxiomSet& second) const
{
  // ��������������� ����������
  for(vector<Axiom*>::const_iterator ax = axioms.begin(); ax != axioms.end(); ax++) {
  }
  return 0;
}


/****************************************************************************
*                               AxiomSet::transmuteAxiom
*
*       Description:    Transmute axiom in the axiomSet with given number
*       Parameters:     axiomNum - axiom number to transmute
*       Returns:        result of transmute operation
*       Throws:         AxiomLibException - if number of axiom is wrong or params are out of boundaries
*       Author:         dk
*       History:
*
****************************************************************************/
signed int AxiomSet::transmuteAxiom (const int axiomNum, double ectl, double atl) {
	if ((axiomNum < 0) || (axiomNum >= (int) axioms.size())) 
		throw AxiomLibException("Error in AxiomSet::transmuteAxiom: wrong parameter value.");
	int res = axioms[axiomNum]->transmute(ectl, atl);
	return res;
}
