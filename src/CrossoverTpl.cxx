/****************************************************************************
*					CrossoverTpl.cxx
*
*	Description:	������� ������ CrossoverTpl - ����������
*	Author:			dk
*	History:		
*
****************************************************************************/
#include "CrossoverTpl.h"

using namespace AxiomLib;

// ���������, ������� �� �������������� ������
#define levelOfSub 0.55  // ������� ����������� ������, �� �������� ��� ����������� ������ ������� ������
#define minProfit 0.5 // ������� �������� ��� ��������� profit ��������� �������
#define epsilon 0.001 // ����� �������� ������� ��� ��������� double'��
#define ecAverageValue 0.75 // ����� ���������� �� � ����� double, ������� ����� ����������� ��� ���������� ����������� �� ������ ��

// ����������� ������ � �������� �������� �� ���������
CrossoverTpl::CrossoverTpl (void) {
	axioms.clear();
	axiomsProfit.clear();
	axiomsMask.clear();
	axiomsMaskProb.clear();
}

// ���������� ������ - ������
CrossoverTpl::~CrossoverTpl (void) {

}


/****************************************************************************
*					CrossoverTpl::name
*
*	Description:	���������� ��� ������� ������
*	Parameters:		-
*	Returns:		std::string - ��� ������� ������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
std::string CrossoverTpl::name (void) const {
	std::string toReturn("CrossoverTpl");
	return toReturn;
}


/****************************************************************************
*                       CrossovercTry::setParamsFromEnv
*
*       Description:	���������������� ��������� ������� �� env
*       Parameters:		const Environment& env - �����
*       Returns:		0
*       Throws:			AxiomLibException - ���� ��� ������-�� ���������
*       Author:			dk
*       History:
*
****************************************************************************/
int CrossoverTpl::setParamsFromEnv (const Environment& env) {	

	return 0;
}


/****************************************************************************
*					CrossoverTpl::choosePairs
*
*	Description:	������� ������ ��� ������ ������ ��� ����������� �� 
*					������ ������ ������. ����� ���������� �� ������� 
*					�� ����� ������ ������ ������. ����� ��������� ��� 
*					��������������� ��������� ����������� axiomSetCrossLevel.
*	Parameters:		asPairs - ������ ��� ������ ������ ��� ����������� (����������� � ������ �������)
*	Returns:		int - ����� �������������� ���
*	Throws:			- 
*	Author:			dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::choosePairs (std::vector <Pair> &asPairs, const std::vector <AxiomSetPlus> &population) {
	// �������� �� ���������� ����� ������ ������
	if (population.size() < 2) return 0;
	// ���������� ���������� ��������������� ����������
	Pair tmpPair;
	unsigned short sec;
	int flag = 0;
	// ��������� �� ����� ������ ������ ������ � ���������� ����� �� ������ ������ ����� ����������� � ����������
	for (unsigned short i = 0; i < (int) population.size(); i++) {
		// ��� ������ ������� ������ ��������� ������� � �������� ���������� ��������� ���������� ����� �� ��� ������������ � �����������
		if (( (double) rand() / (double) RAND_MAX ) < population[i].asCrossProb) {
			sec = (unsigned short) ( ( (double) rand() / (double) RAND_MAX ) * ((double) population.size() - 2.0) );
			// ������� ������� ������ - ��������� ��������� ���� � ��������� �� � asPairs
			if (sec == i) sec++;
			tmpPair.first = i;
			tmpPair.second = sec;
			asPairs.push_back(tmpPair);
			flag++;
		}
	}
	return flag;
}


/****************************************************************************
*				CrossoverTpl::crossAxiomSets
*
*	Description:	������� ����������� ���� ��������� ������ ������ �� 
*				������ ������ ������. ����������� ���������� ����� 
*				�������� ����� ������� ������ �� ������ ������������
*				��� �������, ��������� ��� �����������.
*	Parameters:	as1 - ����� ������ ������������ ������� ������
*				as1 - ����� ������ ������������ ������� ������
*				axiomSetPlus - ����� ���������, ������� ����� ���� ������� ��� �����������
*	Returns:		1 - ���� ���� ������� ����� �����
*					0 - ���� ����� ����� �� ���� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::crossAxiomSets (unsigned short as1, unsigned short as2, AxiomSetPlus &axiomSetPlus) {
	// ���������� ���������� ���������� �������
	int newSet1, newSet2; 
	std::vector <Axiom *> toInclude;
	std::vector <bool> mask;
	unsigned short curNum;
	// ����������� ����� ������ �� ������ �������, ������� ����� ������� �� ����� ��������� ������� ������
	// (� ����� ������� ����� ������� �� 1/3 �� 2/3 ������ �� ������ �� ������)
	newSet1 = (int) ( ( ( (double) rand() / (double) RAND_MAX ) * ( (double) axioms[as1]->size() - 1.0) / 3 ) + ( (double) axioms[as1]->size() - 1.0) / 3);
	newSet2 = (int) ( ( ( (double) rand() / (double) RAND_MAX ) * ( (double) axioms[as2]->size() - 1.0) / 3 ) + ( (double) axioms[as2]->size() - 1.0) / 3);
	if ( (newSet1 <= 0) && (newSet2 <= 0) ) return 0;
	// ����� ������, ������� ����� ��������������
	toInclude.resize( newSet1 + newSet2 );
	// ����������� - ����� ������� �� ������ ������� ������ ����� ������� �� ����� ����������� ������� ������
	// ��������� ������� ����� - ������������ ��� ����������� ��������� � ������� ������ - ���������� ������
	mask.resize (axioms[as1]->size());
	for (unsigned int t = 0; t < mask.size(); t++) {
		mask[t] = false;
	}
	double curProb;
	double curSum = 0.0;
	for (int i = 0; i < newSet1; i++) {
		curProb = ( (double) rand() / (double) RAND_MAX ) * (axiomsMaskProb[as1][axiomsMaskProb[as1].size() - 1] - curSum - epsilon);
		curNum = 0;
		while (curProb > axiomsMaskProb[as1][curNum])
			curNum++;
		while (mask[curNum])
			curNum++;
		if (curNum >= axioms[as1]->size())
			throw AxiomLibException("CrossoverTpl::crossAxiomSets : internal error - wrong logic in choosing axioms for new AxiomSet.");
		mask[curNum] = true;
		curSum += minProfit + axiomsProfit[as1][axiomsMask[as1][curNum]];
		toInclude[i] = (*axioms[as1])[axiomsMask[as1][curNum]];
	}
	// ����������� - ����� ������� �� ������ ������� ������ ����� ������� �� ����� ����������� ������� ������
	// ��������� ������� ����� - ������������ ��� ����������� ��������� � ������� ������ - ���������� ������
	mask.resize (axioms[as2]->size());
	for (unsigned int t = 0; t < mask.size(); t++) {
		mask[t] = false;
	}
	curSum = 0.0;
	for (int i = 0; i < newSet2; i++) {
		curProb = ( (double) rand() / (double) RAND_MAX ) * (axiomsMaskProb[as2][axiomsMaskProb[as2].size() - 1] - curSum - epsilon);
		curNum = 0;
		while (curProb > axiomsMaskProb[as2][curNum])
			curNum++;
		while (mask[curNum])
			curNum++;
		if (curNum >= axioms[as2]->size())
			throw AxiomLibException("CrossoverTpl::crossAxiomSets : internal error - wrong logic in choosing axioms for new AxiomSet.");
		mask[curNum] = true;
		curSum += minProfit + axiomsProfit[as2][axiomsMask[as2][curNum]];
		toInclude[newSet1 + i] = (*axioms[as2])[axiomsMask[as2][curNum]];
	}
	// ������������� ������� ������ ����� ����������� ��������� ������
	if (axiomSetPlus.axiomSet == NULL)
		axiomSetPlus.axiomSet = new AxiomSet;
	axiomSetPlus.axiomSet->initAxiomSetByCopy (toInclude, levelOfSub);
	return 1;
}


/****************************************************************************
*				CrossoverTpl::chooseAxQuads
*
*	Description:	������� ������ ��� ������ ��� ����������� �� ������ ������.
*					����� ���������� �� ������� �� ����� ������ ������. ��� ���� ��
*					������ ������� ������ ����� ���� ������� � ������� 1-3 �������
*					��� �����������, ��� ���� ����������� ����������� axCrossProb
*					������� ������ ������ ������� � �����������.
*	Parameters:		axQuads - ������ �� �������� Quad - ����� ������� ����������
*	Returns:		int - ����� �������������� ���
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::chooseAxQuads (std::vector <Quad> &axQuads, const std::vector <AxiomSetPlus> &population) {
	// ���������� ��������� ���������� �������
	int tmpDiv;
	int fax, second, sax;
	double average;
	Quad tmpQuad;
	int flag = 0;

	// ������� �������� ����� ������ � ������� ������ � ���������
	average = 0.0;
	for (unsigned int j = 0; j < axioms.size(); j++) {
		average += (double) (axioms[j])->size();
	}
	if (axioms.size() < 1) return 0;
	average = average / (double) axioms.size();

	// ��������� �� ���� �������� � ��������� � �������� ���������� ����� �� �������������� ������� � �����������
	for (int i = 0; i < (int) axioms.size(); i++) {
		tmpDiv = (int) ( 1.0 + (double) (axioms[i])->size() / average );
		for (int y = 0; y < tmpDiv; y++) {
			if (( (double) rand() / (double) RAND_MAX ) < population[i].axCrossProb) {
				fax = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (axioms[i])->size() - 1.0) );
				second = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) axioms.size() - 1.0) );
				sax = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (axioms[second])->size() - 1.0) );
				// ���������� ���, ��� ������������ � ����������� - ��������� ���������������� ��������� � ��������� ������ axQuads
				tmpQuad.first = i;
				tmpQuad.fax = fax;
				tmpQuad.second = second;
				tmpQuad.sax = sax;
				axQuads.push_back(tmpQuad);
				flag++;
			}
		}
	}
	
	return flag;
}


/****************************************************************************
*					CrossoverTpl::crossAxioms
*
*	Description:	������� ����������� ���� ��������� ������ 
*					�� ������ ������. � ������ ���������� ����������� 
*					������������ ����� ������������� ��������� �� ������.
*					����� ������������ ������� ��� ������ ������������ �� 
*					������������� � ������������� ��������� �� ����� 
*					���������� ��������.
*	Parameters:		as1 - ����� ������ ������������ ������� ������
*					ax1 - ����� ������� � ������ ������� ������
*					as1 - ����� ������ ������������ ������� ������
*					ax1 - ����� ������� �� ������ ������� ������
*					axiomSetPlus - ����� ���������, ������� ����� ���� ������� ��� �����������
*	Returns:		1 - ���� ���� ������� ����� �����
*					0 - ���� ����� ����� �� ���� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::crossAxioms (unsigned short as1, unsigned short ax1, unsigned short as2, unsigned short ax2, AxiomSetPlus &axiomSetPlus) {
	// ������� ����� ������ � ������� � ��� ����� ������ �� ������ ��������� ������� as1
	std::vector < Axiom* > newSet;
	newSet.resize (axioms[as1]->size());
	for (unsigned int i = 0; i < newSet.size(); i++) {
		newSet[i] = new Axiom;
		*(newSet[i]) = *(*axioms[as1])[i];
	}
	// ������� ����� ������ �������, ��������� ��� �����������
	Axiom* secondAxiom;
	secondAxiom = new Axiom;
	*secondAxiom = *(*axioms[as2])[ax2];
	// ��� �������� ������� ������ ��� �� � �� ������ �������
	Axiom* firstAxiom;
	firstAxiom = newSet[ax1];

	// ����������� ����� ������������ ������� � ������ �� ������
	int numOfEc1, numOfEc2;
	numOfEc1 = firstAxiom->getNumOfEC();
	numOfEc2 = secondAxiom->getNumOfEC();

	// ���������� ������������ ����� ��, �������� ����� ���������� �������
	int numToChange;
	if (numOfEc1 > 2*numOfEc2)
		numToChange = numOfEc2;
	else
		numToChange = round ( ((double) numOfEc1) / 2.0 );

	// ���������� ���������� ���������� ������
	Check checker;	// ����� ��� �������� ������������� ��
	int chgDone = 0;	// ����� ��������� ����� ��
	std::vector < std::vector <bool> > compatMatrix;	// ������� ������������� �� ��������������� ������
	compatMatrix.resize (numOfEc1);
	for (unsigned int k = 0; k < (unsigned int) numOfEc1; k++)
		compatMatrix[k].resize (numOfEc2);
	int numOfFalse;	// ����� ������������� �� � ����� ������� ������� compatMatrix
	unsigned short lastFalse;	// ������ ���������� �������������� �� � ������� ������� compatMatrix
	std::vector <Pair> ableToChg;	// ������ ��� �������� �� � ��������, ������� ����� ��������
	int able;	// ������� � ������� ableToChg, ��������� ��������
	unsigned short curSize;	// ���������� ��� �������� �������� ������� �������
	// ���� �� ����� ��, �������� ����� ���������� �������
	for (int t = 0; t < numToChange; t++) {
		// ����� ����������� �� ������ ������������� ������� - �� ���� �����, ��� ������ �������� ���������� ����������� ������� (������� �� ����������� �� and ������������ �������)
		able = 0;
		for (int i = 0; i < numOfEc1; i++) {
			for (int j = 0; j < numOfEc2; j++) {
				compatMatrix[i][j] = checker.checkAndCompatibility (firstAxiom, i, secondAxiom, j) != 0;
			}
		}
		// ���������� ����� �� �� ����� ����� �������� � ������� ��������������� ������� � ������ ��� ableToChg
		ableToChg.clear();
		for (unsigned short j = 0; j < numOfEc2; j++) {
			numOfFalse = 0;
			for (unsigned short i = 0; i < numOfEc1; i++) {
				if (!compatMatrix[i][j]) {
					lastFalse = i;
					numOfFalse++;
					if (numOfFalse > 1)
						break;
				}
			}
			if (numOfFalse == 0) {
				curSize = ableToChg.size();
				ableToChg.resize (curSize + numOfEc1);
				for (unsigned int i = 0; i < (unsigned int) numOfEc1; i++) {
					ableToChg[curSize + i].first = i;
					ableToChg[curSize + i].second = j;
				}
				continue;
			}
			if (numOfFalse == 1) {
				curSize = ableToChg.size();
				ableToChg.resize (curSize + 1);
				ableToChg[curSize].first = lastFalse;
				ableToChg[curSize].second = j;
				continue;
			}
		}
		// ������ � ableToChg �������� ���� �������� ��, �������� ����� ���������� ��������������� �������
		if (ableToChg.size() < 1) continue;
		// �������� ����� ������ ����� ����������� - ��������
		able = round ( ((double) ableToChg.size() - 1.0) * ((double) rand()) / ((double) RAND_MAX) );
		// ���������� ����� ������������� ���������
		checker.change (firstAxiom, ableToChg[able].first, secondAxiom, ableToChg[able].second);
		chgDone++;
	}

	// � ������, ���� ����������� ��������� - ������� ����� ����� �������� ������� �����������
	if (chgDone > 0) {
		// ������������� ������� ������ ����� ����������� ������ �� ��������������� �������
		if (axiomSetPlus.axiomSet == NULL)
			axiomSetPlus.axiomSet = new AxiomSet;
		axiomSetPlus.axiomSet->initAxiomSetByCopy (newSet, levelOfSub);
	}
	
	// �������� ��������� ������ 
	for (unsigned int i = 0; i < newSet.size(); i++) {
		newSet[i]->clear();
		delete newSet[i];
	}
	secondAxiom->clear();
	delete secondAxiom;
	
	// ����� �� ��������� � ����������� �� ���� - ���� �� ������� ����� �����
	if (chgDone > 0)
		return 1;
	return 0;
}


/****************************************************************************
*					CrossoverTpl::chooseEcQuads
*
*	Description:	������� ������ ��� ������������ ������� ��� ����������� 
*					�� ������ ���������� ������������ ������.
*					����� ���������� �� ������� �� ����� ������ ������������ �������.
*	Parameters:		ecQuads - ������ �������� QuadPlus - �������� ����� ������������ ������� 
*					���� �������� ��� �����������
*	Returns:		int - ����� �������������� ���
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::chooseECQuads (std::vector <QuadPlus> &ecQuads, const std::vector <AxiomSetPlus> &population) {
	// ���������� ���������� ���������� ������ �������
	std::map <std::string, std::vector<Pair> > ecMap;
	std::map <std::string, std::vector<Pair> >::iterator iter;
	std::vector<std::string> ecNames;
	std::vector<Pair> tmpVec;
	Pair tmpPair;
	ecMap.clear();
	tmpVec.resize(1);
	int overall = 0;
	double average = 0.0;
	int tmpDiv;
	int ec1, ec2;
	int flag;
	QuadPlus tmpQuad;
	// ��������������� ���� - ���� ���������� � ������� ������������ ������� � �������� - ���������� ��������� ecMap
	for (int i = 0; i < (int) axioms.size(); i++) {
		for (int j = 0; j < (int) (axioms[i])->size(); j++) {
			ecNames.clear();
			(*(axioms[i]))[j]->getECNames(ecNames);
			overall += (int) ecNames.size();
			for (int k = 0; k < (int) ecNames.size(); k++) {
				tmpPair.first = i;
				tmpPair.second = j;
				if (ecMap.count (ecNames[k]) > 0) {
					// �� ���� ������ � ������������ �������� � ��� �� ������ �� ������� ������ ������ ������ ��� ������� �� ���������
					ecMap[ecNames[k]].push_back(tmpPair);
				}
				else {
					// �� ���� ������ � ����� ������������ ������� ��� ��� - ������� �� � ecMap
					tmpVec[0] = tmpPair;
					ecMap.insert(make_pair (ecNames[k], tmpVec));
				}
			}
		}
	}
	// �������������� ������������� - ����� ������������ ������� ��������� - ��������� ��������� ecQuad
	ecQuads.clear();
	flag = 0;
	if (ecMap.size() < 1) return 0;
	average = (double) overall / (double) ecMap.size() ; // ������� ����� ���������� ������������� ������� �� ���� �������� ������
	for (iter = ecMap.begin(); iter != ecMap.end(); iter++) {
		if ((iter->second).size() < 2) continue; // �� ���� ��� ���� ������������ ������� ������� ���� - ������ ������������
		tmpDiv = (int) ( 1.0 + (double) (iter->second).size() / average );
		for (int h = 0; h < tmpDiv; h++) {
			// ����� ��
			ec1 = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (iter->second).size() - 1.0) );
			// ���������� - � ����� �� ����������� ��� ���������� �� - ���������� �������� ecCrossProb 
			if (( (double) rand() / (double) RAND_MAX ) < population[((iter->second)[ec1]).first].ecCrossProb) {
				// ����� ������� �� ���� �� ���� ��� �����������				
				ec2 = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (iter->second).size() - 2.0) );
				if (ec2 == ec1) ec2++;
				// ���������� ��������� tmpQuad
				tmpQuad.first = ((iter->second)[ec1]).first;
				tmpQuad.fax = ((iter->second)[ec1]).second;
				tmpQuad.second = ((iter->second)[ec2]).first;
				tmpQuad.sax = ((iter->second)[ec2]).second;
				tmpQuad.ecName = iter->first;
				// ������ ����������� ��������� � ������ ������� ��������
				ecQuads.push_back(tmpQuad);
				flag++;
			}
		}
	}
	
	return flag;
}


/****************************************************************************
*				CrossoverTpl::crossEcs
*
*	Description:	����������� ���� ��������� ��� ������������ ������� 
*				�� ������ ���������� ������������� �������.
*				����������� ���������� ����� ���������� ����������
*				���������� ������������ ������� �� ������ ������.
*	Parameters:	as1 - ����� ������ ������������ ������� ������
*				ax1 - ����� ������� � ������ ������� ������
*				as1 - ����� ������ ������������ ������� ������
*				ax1 - ����� ������� �� ������ ������� ������
*				ecName - ��� ������������� �������, ������� ���� �������� ��� �����������
*				axiomSetPlus - ����� ���������, ������� ����� ���� ������� ��� �����������
*	Returns:		1 - ���� ���� ������� ����� �����
*					0 - ���� ����� ����� �� ���� �������
*	Throws:		AxiomLibExeption - ���� ��� ����������� ���� �������� ������������� ������������ ������� (������� ����)
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::crossECs (unsigned short as1, unsigned short ax1, unsigned short as2, unsigned short ax2, std::string &ecName, AxiomSetPlus &axiomSetPlus) {
	// ���������� ����������� ������� ��� �� ������������ ��������������
	Axiom saveAxiom;
	saveAxiom = *(*axioms[as1])[ax1];
	// ���������� ���������� ����������
	int ecNum1, ecNum2;
	int curRes = -1;
	double param = 0.0;
	double param1 = 0.0;
	double param2 = 0.0;
	int chgDone = 0;
	std::vector<std::string> ecParamNames;
	// ��������� ����� ������������ ������� � ������ �� ����� - ��������� ����� �� ����� ��� �� ���������� � ����� � ��� �� ������� � ������� ������ �������
	ecNum1 = (*axioms[as1])[ax1]->getECNumByName (ecName);
	ecNum2 = (*axioms[as2])[ax2]->getECNumByName (ecName);
	// �������� �� ������������ ������
	if ( (ecNum1 < 0) || (ecNum2 < 0) )
		throw AxiomLibException("CrossoverTpl::crossECs : there is an error in generating pairs of elementary conditions to cross. ");
	// �������� ����������� �������� ���������� ���������� ������������� �������
	(*axioms[as1])[ax1]->getECParamNames(ecNum1, ecParamNames);
	for (int j = 0; j < (int) ecParamNames.size(); j++) {
		curRes = (*axioms[as1])[ax1]->getParamValue (param1, ecNum1, ecParamNames[j]);
		if ( (curRes == 0) && ( ( (double) rand() / (double) RAND_MAX ) < ecAverageValue ) ) {
			// ���� 0 - �� ��� ��������� ��������� (� double) - �������� ��������� ���������� � ���������� �������
			// (��� ����� ��������� ������ ��������)
			curRes = (*axioms[as2])[ax2]->getParamValue (param2, ecNum2, ecParamNames[j]);
			if (curRes != 0)
				throw AxiomLibException("CrossoverTpl::crossEcs : there is an error in generating pairs of elementary conditions to cross, ECs are not of the same Class. ");
			param = (param1 + param2) / 2.0; // �������� ���������� ���������� - �������� �������� �� ������������ �������
			// ���������� ������� ���������� �������� ��������
			(*axioms[as1])[ax1]->setParamValue (param, ecNum1, ecParamNames[j]);
			chgDone++;
		}
		else{
			// ���� �������� �� double - � � ���� �� ���������� ���������������� - ����� ������ ����� �������� - ���� ��� ����������			
		}
	}

	// � ������, ���� ����������� ��������� - ������� ����� ����� �������� ������� �����������
	if (chgDone > 0) {
		// ������������� ������� ������ ����� ����������� ������ �� ��������������� �������
		if (axiomSetPlus.axiomSet == NULL)
			axiomSetPlus.axiomSet = new AxiomSet;
		axiomSetPlus.axiomSet->initAxiomSetByCopy (*axioms[as1], levelOfSub);
		// �������������� ���������� ��������
		*(*axioms[as1])[ax1] = saveAxiom;
		saveAxiom.clear();
		return 1;
	}

	return 0;
}


/****************************************************************************
*					CrossoverTpl::initCrossover
*
*	Description:	������� ������������� ������ ��������� �����������. 
*					��������� ������� ������ �� ������ ������
*	Parameters:		population - ��������� ������ ������ �� �����������
*	Returns:		-
*	Throws:			AxiomLibException - ���� ���� �� ���� ������� ������ 
*					� ��������� �� ����������
*	Author:			dk
*	History:
*
****************************************************************************/ 
void CrossoverTpl::initCrossover (const std::vector <AxiomSetPlus> &population) {
	axioms.resize (population.size());
	axiomsProfit.resize (population.size());
	axiomsMask.resize (population.size());
	axiomsMaskProb.resize (population.size());
	for (unsigned int i = 0; i < population.size(); i++) {
		// ������������� ��������� �� �������
		if (population[i].axiomSet == NULL) 
			throw AxiomLibException("CrossoverTpl::initCrossover : population is not set or empry.");
		axioms[i] = &(population[i].axiomSet->axioms);
		// ������������� ���������� �� ������ ������� ������
		axiomsProfit[i] = population[i].axiomProfit;
		if (axioms[i]->size() != axiomsProfit[i].size())
			throw AxiomLibException("CrossoverTpl::initCrossover : axioms profit is not set.");
		// ������������� �������� �����, ������� �������� ������ ������ � ������� � ������� ���������� ������� ����������
		sort (axiomsProfit[i], axiomsMask[i]);
		// ������������� �������� ������� ������������ �������� ������� ������
		axiomsMaskProb[i].resize(axiomsProfit[i].size());
		if (axiomsProfit[i].size() > 0) {
			axiomsMaskProb[i][0] = minProfit + axiomsProfit[i][axiomsMask[i][0]];
			for (unsigned short j = 1; j < axiomsProfit[i].size(); j++)
				axiomsMaskProb[i][j] = axiomsMaskProb[i][j-1] + minProfit + axiomsProfit[i][axiomsMask[i][j]];
		}
	}
}

 
/****************************************************************************
*					CrossoverTpl::sort
*
*	Description:	��������� �������� ������� vec � ������� ��������������� 
*					������������������ �������� � mask
*	Parameters:		vec - ����������� ������
*					mask - ������������������ ��������
*	Returns:		-
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/ 
inline void CrossoverTpl::sort (std::vector <double> &vec, std::vector <unsigned short> &mask) {
	mask.resize(vec.size()); 
	for (unsigned short i = 0; i < vec.size(); i++) 
		mask[i] = i;
	unsigned short temp;
	for (unsigned short i = vec.size() - 1; i > 0; i-- ) {
		for (unsigned short j = 0; j < i; j++) {
			if (vec[mask[j]] < vec[mask[j+1]]) {
				temp = mask[j+1];
				mask[j+1] = mask[j];
				mask[j] = temp;
			}
		}
	}
}


/****************************************************************************
*					CrossoverTpl::addSets
*
*	Description:	������� ���������� � ��������� ��������� ������ ������
*	Parameters:		population - ��������� ������ ������ �� �����������
*					newSets - ����� ������ ������
*					setsMask - ����� �� ���������� ����� ������
*	Returns:		-
*	Throws:			AxiomLibException - ���� ������� ��������� �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/ 
inline void CrossoverTpl::addSets (std::vector <AxiomSetPlus> &newPop, const std::vector <AxiomSetPlus> &population, std::vector < std::vector <AxiomSetPlus> > &newSets, std::vector < std::vector <int> > &setsMask) const {
	// �������� ������� ������
	if (setsMask.size() != newSets.size())
		throw AxiomLibException("CrossoverTpl::addSets : internal error - vector and its mask of different size.");
	
	// ������� ����� ����������� ���������
	unsigned short toAdd = 0;
	for (unsigned int j = 0; j < setsMask.size(); j++) {
		if (setsMask[j].size() != newSets[j].size())
			throw AxiomLibException("CrossoverTpl::addSets : internal error - vector and its mask of different size.");
		for (unsigned short i = 0; i < setsMask[j].size(); i++) {
			if (setsMask[j][i] > 0)
				toAdd++;
		}
	}

	if (newPop.size() > 0)
		for (unsigned int i = 0; i < newPop.size(); i++)
			newPop[i].clear();

	unsigned short popSize = (unsigned short) population.size();
	newPop.resize(popSize + toAdd);
	for (unsigned short i = 0; i < popSize; i++)
		newPop[i] = population[i];
	toAdd = popSize;
	for (unsigned int j = 0; j < setsMask.size(); j++) {
		for (unsigned short i = 0; i < setsMask[j].size(); i++) {
			if (setsMask[j][i] > 0) {
				newPop[toAdd] = newSets[j][i];
				newPop[toAdd].clearStat();
				toAdd++;
			}
		}
	}
}

/****************************************************************************
*					CrossoverTpl::crossover
*
*	Description:	������� ����������� ��������� ������ ������ �� ������� ������.
*					����� ������������ �������, ������ � ������ ������ ��� 
*					����������� ���������� � ���������� ��������������� 
*					��� ����� �������� ������� ������.
*	Parameters:		axiomSetPop - ��������� ������ ������ - ����� ������ ������, ��� ������� ��������� ��������� �����������
*	Returns:		0 - ���� ��� ��������� �������
*					-1 - ���� ����������� �� ������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/ 
int CrossoverTpl::crossover (const std::vector <AxiomSetPlus> &population, std::vector <AxiomSetPlus> &toRet) {
	// ������������� ������ ����������� - ��� ���� ��������� ������ ������ �� ������� � �������� ������
	initCrossover (population);

	// ����������� ���� �� ���������� �������:
	// 1) ����������� �� ������ ������ ������
	// 2) ����������� �� ������ ������
	// 3) ����������� �� ������ ������������ �������

	// ������� ������ ��� �������� ����� ������ ���������� ����� ��� ����
	std::vector < std::vector <AxiomSetPlus> > newSets;
	std::vector < std::vector <int> > setsMask;

	// ��� ��� 3 ������ ����������� - ��� ������� ���� ������ ����� ������
	newSets.resize(3); 
	setsMask.resize(3);
	
	// 1) ����������� �� ������ ������ ������
	// ����� ��� ������ ������ ��� �����������
	std::vector <Pair> asPairs;
	//if (false) {
	if (choosePairs (asPairs, population) > 0) {
		// ����������� ��������� ���
		newSets[0].resize(asPairs.size());
		setsMask[0].resize(asPairs.size());
		for (unsigned int i = 0; i < asPairs.size(); i++) {
			setsMask[0][i] = crossAxiomSets (asPairs[i].first, asPairs[i].second, newSets[0][i]);
		}
	} else {
		// ���� �� ���� ���� ��� ������������ �� ������ ������ ������ �� �������
		newSets[0].clear();
		setsMask[0].clear();
	}
	
	// 2) ����������� �� ������ ������
	// ����� ������ ��� �����������
	std::vector <Quad> axQuads;
	//if (false) {
	if (chooseAxQuads (axQuads, population) > 0) {
		newSets[1].resize(axQuads.size());
		setsMask[1].resize(axQuads.size());
		// ����������� ��������� ������
		for (unsigned int j = 0; j < axQuads.size(); j++) {
			setsMask[1][j] = crossAxioms ( axQuads[j].first, axQuads[j].fax, axQuads[j].second, axQuads[j].sax, newSets[1][j]);
		}
	}
	else {
		// ���� �� ���� ���� ��� ������������ �� ������ ������ �� �������
		newSets[1].clear();
		setsMask[1].clear();
	}

	// 3) ����������� �� ������ ������������ �������
	// ����� ������, ������������ ������� �� ������� ����� ��������
	std::vector <QuadPlus> ecQuads;
	//if (false) {
	if (chooseECQuads (ecQuads, population) > 0) {
		newSets[2].resize(ecQuads.size());
		setsMask[2].resize(ecQuads.size());
		// ����������� �� ������ ����������� ������� ��������� ������
		for (unsigned int k = 0; k < ecQuads.size(); k++) {
			setsMask[2][k] = crossECs ( (ecQuads[k]).first, (ecQuads[k]).fax, (ecQuads[k]).second, (ecQuads[k]).sax, (ecQuads[k]).ecName, newSets[2][k]);
		}
	}
	else {
		// ���� �� ���� ���� ��� ������������ �� ������ ������������ ������� �� �������
		newSets[2].clear();
		setsMask[2].clear();
	}
	
	// ���������� ���� ��������� ��������� � ���������
	addSets (toRet, population, newSets, setsMask);
	
	/*toRet.resize (population.size() + 1);
	for (unsigned int i = 0; i < population.size(); i++)
		toRet[i] = population[i];
	toRet[population.size()] = population[0];
	*/
	return 0;
}
