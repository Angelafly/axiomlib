/****************************************************************************
*
*			������� ������ AxiomSetPop
*
****************************************************************************/

#include "AxiomSetPop.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

/****************************************************************************
*			AxiomSetPop::initFromEnv
*
*	Description:	�������������� ��������� ��������� ������ ������,
*				��������� ������ �� ����������������� �����.
*				� ������:
*				    - ���������� ���������� ������ � ���������
*				    - ����������, ��� ��������� ������� ������� ���
*				      ��������� ���������
*				    - ��� �������������, ������ ����� ������ ������ ��
*				      ������
*				    - ��� �������������, �������������� ����� ������
*				      ������ ��������� �������
*	Parameters:	const Environment& env - ���������, ���������:
*				    * popSize - ������ ���������
*				    * popPreset - ������� ������, ������� ���� �������
*				      �� �������� ��������� ���������
*	Returns:		0
*	Throws:		AxiomLibException
*	Author:		gevor
*	History:
*
****************************************************************************/
int AxiomSetPop::initFromEnv(const Environment& env)
{
  // ��� ������, ���������� ������ �������
  if (env.getIntParamValue(popSize, "popSize") < 0)
    throw AxiomLibException("AxiomSetPop::initFromEnv : population size is not set.");
  //asPop = new vector<AxiomSetWithStats>;
  //asPop.resize(popSize);
  asPop.clear();

  // �������, ���� �� ����� �����-�� ���������� ������ �� ��������
  // ��������� ���������
  double pPresetAS=0;
  env.getDoubleParamValue(pPresetAS, "popPreset");
  int nPresetAS = (int)((pPresetAS) * (double)(popSize)); // ���-�� ������, ������� ���� ������������� �� ��������
  if (nPresetAS < 0)
    throw AxiomLibException("AxiomSetPop::initFromEnv : nPresetAS < 0 (maybe, popPreset < 0).");
  
  // ���������, ���� pPresetAS != 0, ������� ������ ������� �� ��������� etc.
  if (nPresetAS != 0) {
    // ����� ������ ������ ������ �� �����
    // 1. ���� AxiomBase - ��� ������ ���� ������, ������ ��
    std::string axiomBaseDir;
    if (env.getStringParamValue(axiomBaseDir, "AxiomBaseDir") < 0)
      throw AxiomLibException("AxiomSetPop::initFromEnv : popPreset > 0 but axiom base dir is not set.");
    AxiomBase axiomBase;
    axiomBase.readAxioms(axiomBaseDir.c_str());
    
    // 2. ���� ������� ���������� � �������� ������ ������
    std::string axiomSetDir;
    if (env.getStringParamValue(axiomSetDir, "AxiomSetBaseDir") < 0)
      throw AxiomLibException("AxiomSetPop::initFromEnv : popPreset > 0 but axiom set base dir is not set.");

    // 3. �������� ��������� ������� ������
    std::set<std::string> axiomSetNames;
    if (env.getStringSetParamValue(axiomSetNames, "AxiomSet") < 0)
      throw AxiomLibException("AxiomSetPop::initFromEnv : popPreset > 0 but no axiom set is given.");
    int nGivenAxiomSets = (int) axiomSetNames.size();

    // 4. �������, ����� �� �� ������ �� ��� ���������
	if (nGivenAxiomSets > nPresetAS) {
	  //throw AxiomLibException("AxiomSetPop::initFromEnv : number of given axiom sets > given number of slots in the population.");
		nPresetAS = nGivenAxiomSets;
	}

    // 5. �������� ��������� ����� ���������, ���� ������� random-�, ������ ���������������
    std::set<std::string>::const_iterator curAx = axiomSetNames.begin();
    for (int i = 0; i < nPresetAS; i++) {
	  std::cout << "AxiomSetPop::initFromEnv. Filling slot " << i+1 << " with axiom '" << *curAx << "'" << std::endl;
      
      // �� �����, �������� ��� axiomSet
	  //(asPop)[i].initAxiomSetFromFile(axiomSetDir, *curAx, axiomBase);
	  asPop.push_back(AxiomSetWithStats());
	  asPop.back().initAxiomSetFromFile(axiomSetDir, *curAx, axiomBase);
    
      // �������, ����� ��������� ��� �� ������
      curAx++;
      if (curAx == axiomSetNames.end())
        curAx = axiomSetNames.begin();
    }
  }
  
  // Ok, ������ ���� ���� �����-�� ��������� ��������� ������� - ������� ��
  for (int i = nPresetAS; i < popSize; i++) {
    int nOfAxioms = 10;
    int maxECs = 5;
    std::cout << "AxiomSetPop::initFromEnv. Filling slot " << i+1 << " with random axiom, nOfAxioms = " << nOfAxioms << ", maxECs = " << maxECs << ".\n";
	asPop.push_back(AxiomSetWithStats());
	(asPop).back().initAxiomSetByRand(nOfAxioms, maxECs);
	(asPop).back().saveAxiomSetToFile (".", "x");
  }

  popSize = asPop.size();

  return 0;
}


/****************************************************************************
*				AxiomSetPop::addSets
*
*	Description:	��������� ��������������� ������ ������ � ��������
*	Parameters:	newSets - ������� � ������ ������������� ��������� ������ 
*				setsMask - ������� ����� - ��������� ����� ������ ������ �� ������� newSets ������������� ��������� � ���������
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void AxiomSetPop::addSets (std::vector < std::vector<AxiomSetWithStats> > &newSets, std::vector < std::vector <int> > &setsMask) {
	// ��������� ������ ��� ����������� �����������
	std::vector<AxiomSetWithStats> temp;
	temp.clear();
	// ������� ����� ������� ����������� ������ ������
	int count = 0;
	//std::cout << "\n addSets::setsMask is\n ";
	for (unsigned int i = 0; i < setsMask.size(); i++) {
		//std::cout << "	";
		for (unsigned int j = 0; j < (setsMask[i]).size(); j++) {
			if (setsMask[i][j] > 0) count++;
			//std::cout << "  " << setsMask[i][j];
		}
		//std::cout << "\n";
	}
	// ���������� ���������� ������� - ��� ����������� ������ � ����� ������ ������
	temp.resize(asPop.size() + count);
	for (unsigned int h = 0; h < asPop.size(); h++)
		temp[h] = asPop[h];
	int t = (int) asPop.size();
	for (unsigned int g = 0; g < newSets.size(); g++) 
		for (unsigned int f = 0; f < (newSets[g]).size(); f++)
			if (setsMask[g][f] > 0) {
				temp[t] = newSets[g][f];
				t++;
			}
	// �������� ���������� ��� ���������� ������������� �������
	asPop.clear();
	asPop.resize(temp.size());
	for (unsigned int h = 0; h < temp.size(); h++) {
		asPop[h] = temp[h];
	}

//	 �������� ����� ������ � ����� ������ � �������� ������
//	 std::cout << "\n		AxiomSetPop::num of axioms in axiom-sets\n		";
//	 for (int i = 0; i < asPop.size(); i++) 
//		std::cout << "  " << (asPop[i]).getNumOfAxioms();
//	std::cout << "\n";

}


/****************************************************************************
*				AxiomSetPop::addSets
*
*	Description:	��������� ��������������� ������ ������ � ��������
*	Parameters:	newSets - ������� � ������ ������������� ��������� ������ 
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
void AxiomSetPop::addSets (std::vector<AxiomSetWithStats>  &newSets) {
	// ��������� ������ ��� ����������� �����������
	std::vector<AxiomSetWithStats> temp;
	temp.clear();
	// ���������� ���������� ������� - ��� ����������� ������ � ����� ������ ������
	temp.resize(asPop.size() + newSets.size());
	for (unsigned int h = 0; h < asPop.size(); h++) {
		temp[h] = asPop[h];
	}
	unsigned int t = (unsigned int) asPop.size();
	for (unsigned int g = 0; g < newSets.size(); g++)  {
		temp[t] = newSets[g];
		t++;
	}
	// �������� ���������� ��� ���������� ������������� �������
	asPop.resize(temp.size());
	for (unsigned int h = 0; h < temp.size(); h++) {
		asPop[h] = temp[h];
	}
	
	/*for (unsigned int i = 0; i < newSets.size(); i++) {
		asPop.push_back (newSets[i]);
	}
	return 0;*/
}


// ������� �������� ���������� ������
void AxiomSetPop::clear () {
	for (unsigned int i = 0; i < asPop.size(); i++) {
		(asPop[i]).clear();
	}
	asPop.clear();
}


/****************************************************************************
*				AxiomSetPop::exit
*
*	Description:	������� ��������� - ���������� �� ������� ������ � ������� �������� ���� ��������� ��������
*	Parameters:	exitCond - ��������, ���� �������� ���� ����� �������� ������� �������
*				bestAxiomSet - ���������� ������ ������� ������
*	Returns:		true - ���� ������� ������� ������ �������
*				false - ���� �� ������� ������� ������ � ������� �������� ���� ��������� ���������
*	Throws:		AxiomLibException - ������� ������� �� ���������� ���� �� � ����� �� ������ ������ (��� ����� ���� �����)
*	Author:		dk
*	History:
*
****************************************************************************/
bool AxiomSetPop::exit (double exitCond, AxiomSetWithStats &bestAxiomSet) {
	int tmpFirst, tmpSecond;
	double gBest, gCurBest, gCur;
	unsigned int indCurBest;
	// �������� ����� ��������� - �� ����� ���� ������ 1
	if (asPop.size() <= 0) {
		throw AxiomLibException("AxiomSetPop::exit - population of zero size.\n");
		return true;
	}
	// ��������� ���������� �� ������ ������� ������
	(asPop[0]).unsafeGetStats(tmpFirst, tmpSecond, gCurBest);
	indCurBest = 0;
	// ��������� � ������ ���������� �� ���������� ����� ������������� ���������
	for (unsigned int i = 1; i < asPop.size(); i++) {
		// ��������� ���������� �� ������� ������� ������
		(asPop[i]).unsafeGetStats(tmpFirst, tmpSecond, gCur);
		// �������� - ���� �� ����� ����������������
		if (gCur < 0)
			throw AxiomLibException("AxiomSetPop::exit - goal stat was not initialized.\n");
		// ����� ������ ������� ������ �� �������� ������� �������
		if (gCur < gCurBest) {
			gCurBest = gCur;
			indCurBest = i;
		}
	}
	// ������ ���������� � ���������� ������ ������� ������ �� ���������� ����� ��������� ������
	bestAxiomSet.unsafeGetStats(tmpFirst, tmpSecond, gBest);
	// ��������� ��������� ���������� �� ��������� � ������ �������� ������ �� ��� ������� - ������, ���� ��������� �������
	//std::cout << "\n AxiomSetPop::exit : indCurBest is " << indCurBest << "     gCurBest is " << gCurBest << "     gBest is " << gBest << "\n";
	if ( (gCurBest < gBest) || (gBest < 0) ) {
		bestAxiomSet = asPop[indCurBest];
	}
	
	// ����������� ������� ���������� ������
	if (gCurBest <= exitCond) 
		return true; 
	return false;
}


/****************************************************************************
*			AxiomSetPop::enter
*
*	Description:	��������� ��� row ������� � ������� begin ���������� 
*				�������� end �������� ������ ����� asNum �� ������� asPop 
*				� ���������� ���������� �������� � ��� markup
*	Parameters:	markup - ������ ��� ������ ����������� ��������
*				row - �������� �������� ��� ��� �������� �������� ������
*				begin - ����� �������� ���� row ������ ����� ��������
*				end - ����� ������� ���� row ������ ����� ��������
*				asNum - ����� ������� ����� � asPop ������� ��������� �������� ����
*	Returns:		0
*	Throws:		AxiomLibException - ���� asNum - out of range
*	Author:		dk
*	History:
*
****************************************************************************/
int AxiomSetPop::enter (std::vector<int>& markup, const std::vector<double>& row, const unsigned long begin, const unsigned long end, int asNum) {
	if ( (asNum < 0) || (asNum >= (int) asPop.size()) ) {
		throw AxiomLibException("Error in AxiomSetPop::enter : wrong parameter asNum - out of range.");
	}
	((asPop)[asNum]).enter (markup, row, begin, end);
	return 0;
}


/****************************************************************************
*			AxiomSetPop::getBasePopSize
*
*	Description:	���������� "�������" (��������� � �������) ������ ���������
*	Parameters:	-
*	Returns:		popSize
*	Throws:		AxiomLibException - ���� axiomPop �� �������������������
*	Author:		gevor
*	History:
*
****************************************************************************/
int AxiomSetPop::getBasePopSize(void) const
{
  if (popSize < 0)
    throw AxiomLibException("AxiomSetPop::getBasePopSize - population has not been initialized.");
  return popSize;
}


/****************************************************************************
*			AxiomSetPop::size
*
*	Description:	���������� ������� ������ ���������
*	Parameters:	-
*	Returns:	pop.size()
*	Throws:		AxiomLibException - ���� axiomPop �� �������������������
*	Author:		gevor
*	History:
*
****************************************************************************/
int AxiomSetPop::size(void) const
{
  //if (popSize < 0)
  //  throw AxiomLibException("AxiomSetPop::size - population has not been initialized.");
  return (int) asPop.size();
}


/****************************************************************************
*			AxiomSetPop::name
*
*	Description:	������� ������������ ��� ������� ������ �� ������ � ���������
*	Parameters:		index - ����� ������� ������ � ���������
*	Returns:		std::string - �������� ������� ������ ��� ������ ������ ��� ������������� �������� index
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
std::string AxiomSetPop::name (int index) const {
	if ((index >=0) && (index < (int) asPop.size()))
		return asPop[index].name();
	return (std::string) "";
}


/****************************************************************************
*				AxiomSetPop::voidStats
*	
*	Description:	����� ���������� ������� ������ ��� ������� asNum � ������� asPop
*	Parameters:	asNum - ����� ������� ������ � ������� asPop � ������� ���� ���������
*	Returns:		-
*	Throws:		AxiomLibException - ���� asNum ��������� �� ���������� ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
void AxiomSetPop::voidStats (const int asNum) {
	if ( (asNum < 0) || (asNum >= (int) asPop.size()) )
		throw AxiomLibException("Error in AxiomSetPop::voidStats: input number of axiomSet is out of range.");
	(asPop)[asNum].voidStats();
}
 

/****************************************************************************
*				AxiomSetPop::setStats
*	
*	Description:	��������� ���������� ������� ������ ��� ������� asNum � ������� asPop
*	Parameters:	asNum - ����� ������� ������ � ������� asPop � ������� ���� ���������
*				nf - ��������������� �������� ����� ������ ������� ����
*				ns - ��������������� �������� ����� ������ ������� ����
*				g - ��������������� �������� ������� �������
*	Returns:		-
*	Throws:		AxiomLibException - ���� asNum ��������� �� ���������� ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
void AxiomSetPop::setStats (const int asNum, signed int nf, signed int ns, double g) {
	if ( (asNum < 0) || (asNum >= (int) asPop.size()) )
		throw AxiomLibException("Error in AxiomSetPop::setStats: input number of axiomSet is out of range.");
	(asPop)[asNum].setStats (nf, ns, g);
}


/****************************************************************************
*				AxiomSetPop::getStats
*	
*	Description:	��������� ����������, ��� ��������, ��� ������� ������ ��� ������� asNum � ������� asPop
*	Parameters:	asNum - ����� ������� ������ � ������� asPop � ������� ���� ���������
*				nf - ����������� �������� ����� ������ ������� ����
*				ns - ����������� �������� ����� ������ ������� ����
*				g - ����������� �������� ������� �������
*	Returns:		-
*	Throws:		AxiomLibException - ���� asNum ��������� �� ���������� ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
void AxiomSetPop::getStats (const int asNum, signed int& nf, signed int& ns, double& g) const {
	if ( (asNum < 0) || (asNum >= (int) asPop.size()) )
		throw AxiomLibException("Error in AxiomSetPop::getStats: input number of axiomSet is out of range.");
	(asPop)[asNum].unsafeGetStats (nf, ns, g);
}


/****************************************************************************
*				AxiomSetPop::cutDown
*	
*	Description:	������� ������������ ���������: �������� �� ��������� ��� ����� (������� ������) 
*				- �� ������� ������� �� ������� ������� ����� false.
*	Parameters:	stayAlive - ������ �������� - ����� � ����� �������� �������� � ���������
*						(�� ���� ���� stayAlive[i] = false - �� i-�� ����� ����� ������� �� ���������)
*	Returns:		0
*	Throws:		AxiomLibException - ���� ����� �������� ������� �� ������������� ����� ������ � ���������
*	Author:		dk
*	History:
*	
****************************************************************************/
int AxiomSetPop::cutDown (std::vector <bool> &stayAlive) {
	if ( stayAlive.size() != asPop.size() )
		throw AxiomLibException("Error in AxiomSetPop::cutDown: input vector's length is incorrect.");
	std::vector<AxiomSetWithStats>::iterator iter;
	std::vector <bool>::iterator witer;
	int flag = 0;
	while (flag == 0) {
		flag = 1;
		witer = stayAlive.begin();
		for (iter = asPop.begin(); iter != asPop.end(); iter++) {
			if (*witer == false) { 
				asPop.erase(iter);
				stayAlive.erase(witer);
				flag = 0;
				break;
			}
			witer++;
		}
	}
	return 0;
}


/****************************************************************************
*				AxiomSetPop::copyAxiomSet
*	
*	Description:	Copy axiom set with statistic by number asNum from this class
*	Parameters:		asNum - number of copiyng axiomSet in asPop
*	Returns:		copy of axiom Set
*	Throws:			AxiomLibException - if asNum is out of range
*	Author:			dk
*	History:
*	
****************************************************************************/
AxiomSetWithStats& AxiomSetPop::axiomSet (const int asNum) {
	if ( (asNum < 0) || (asNum >= (int) asPop.size()) )
		throw AxiomLibException("Error in AxiomSetPop::copyAxiomSet: input paramenters out of range.");
	return asPop[asNum];
}


/****************************************************************************
*				AxiomSetPop::saveAxiomSetPopToFiles
*	
*	Description:	���������� ������ ������ � �����
*	Parameters:		env - ����� ���������, �� �������� ����������� ����������� ���������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*	
****************************************************************************/
int AxiomSetPop::saveAxiomSetPopToFiles (Environment &env) {
	std::string saveName, saveDir, saveNameCur;
	if (env.getStringParamValue (saveDir, "PathToSaveResults") < 0)
		return -1;
	if (env.getStringParamValue (saveName, "saveTo") < 0)
		saveName.assign ("axiomSet_");
	char tmps[10];
	int first, second;
	double goal;
	for (int asNum = 0; asNum < (int) asPop.size(); asNum++) {
		tmps[0]='\0';
		sprintf(tmps,"%d",asNum + 1);
		saveNameCur = saveName;
		saveNameCur.append(tmps);
		asPop[asNum].unsafeGetStats (first, second, goal);
		asPop[asNum].saveAxiomSetToFile (saveDir, saveNameCur, first, second);
	}
	return 0;
}


/****************************************************************************
*				AxiomSetPop::operator=
*	
*	Description:	�������� ����������� ���������
*	Parameters:	second - ���������� ���������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*	
****************************************************************************/
/*AxiomSetPop& AxiomSetPop::operator= (AxiomSetPop& second) {
	// �������� �������� ���������� ������ - ���������� popSize
	this->popSize = second.popSize;
	// ���������� asPop - ������ AxiomSetWithStat - ��� ��������� ���������� �������� �����������
	this->asPop.resize(second.asPop.size());
	for (int i = 0; i < second.asPop.size(); i++) {
		(this->asPop)[i] = second.asPop[i];
	}
	return *this;
}*/
