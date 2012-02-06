/****************************************************************************
*				CrossoverTry.cxx
*
*	Description:	������� ������ CrossoverTry - ����������
*	Author:		dk
*	History:		
*
****************************************************************************/
#include "CrossoverTry.h"

using namespace AxiomLib;


/****************************************************************************
*					CrossoverTry::CrossoverTry
*
*	Description:	����������� ����������
*	Parameters:	-
*	Returns:		-
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
CrossoverTry::CrossoverTry (void) {
	// ������� �������� ���������� ����������� �� ��������� (���������� �����������)
	elemCondCrossLevel = 0.0;
	axiomCrossLevel = 0.0;
	axiomSetCrossLevel = 0.0;
}


/****************************************************************************
*					CrossoverTry::name
*
*	Description:	���������� ��� ������� ������
*	Parameters:	-
*	Returns:		std::string - ��� ������� ������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
std::string CrossoverTry::name (void) const {
	std::string toReturn("CrossoverTry");
	return toReturn;
}


/****************************************************************************
*                       CrossovercTry::setParamsFromEnv
*
*       Description:	���������������� ��������� ������� �� env
*       Parameters:	const Environment& env - �����
*       Returns:		0
*       Throws:		AxiomLibException - ���� ��� ������-�����������
*       Author:		dk
*       History:
*
****************************************************************************/
int CrossoverTry::setParamsFromEnv (const Environment& env) {
	double allCrossLevel;
	if (env.getDoubleParamValue(allCrossLevel, "crossLevel") < 0.0) {
		env.getDoubleParamValue(elemCondCrossLevel, "elemCondCrossLevel");
		if ( (elemCondCrossLevel < 0.0) || (elemCondCrossLevel > 1.0) ) 
			throw AxiomLibException("CrossoverTry::setParamsFromEnv : elemCondCrossLevel isn't set in config file or elemCondCrossLevel is out of range.");
			
		env.getDoubleParamValue(axiomCrossLevel, "axiomCrossLevel");
		if ( (axiomCrossLevel < 0.0) || (axiomCrossLevel > 1.0) ) 
			throw AxiomLibException("CrossoverTry::setParamsFromEnv : axiomCrossLevel isn't set in config file or axiomCrossLevel is out of range.");
			
		env.getDoubleParamValue(axiomSetCrossLevel, "axiomSetCrossLevel");
		if ( (axiomSetCrossLevel < 0.0) || (axiomSetCrossLevel > 1.0) ) 
			throw AxiomLibException("CrossoverTry::setParamsFromEnv : axiomSetCrossLevel isn't set in config file or axiomSetCrossLevel is out of range.");
	}
	else {
		if ( (allCrossLevel < 0.0) || (allCrossLevel > 1.0) ) 
			throw AxiomLibException("CrossoverTry::setParamsFromEnv : crossLevel isn't set in config file or crossLevel is out of range.");
		// ���� �� ������ ������� ������� �� ����������� ��� ������� ������ �������, � ����� ������ ������� - �� ��� � ����������� ������� �� �������
		elemCondCrossLevel = allCrossLevel;
		axiomCrossLevel = allCrossLevel;
		axiomSetCrossLevel = allCrossLevel;
	}
	return 0;
}


/****************************************************************************
*				CrossoverTry::setParams
*
*	Description:	������� ������� �������� ���������� 
*	Parameters:	params - ����������� �� ������ � ��������� ��������� ������ � ��������������� �������� ����� ��������� � ���� ������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� �������� ���������� ������������
*	Author:		dk
*	History:
*
****************************************************************************/
int CrossoverTry::setParams (std::map<std::string, std::string> &params) {
	std::map<std::string, std::string>::iterator iter;
	double value;
	for (iter = params.begin(); iter != params.end(); ++iter) {
		if ((*iter).first == "crossLevel") {
			value = atof ( (*iter).second.c_str() );
			if ( (value < 0.0) || (value > 1.0) ) 
				throw AxiomLibException("CrossoverTry::setParams : crossLevel is out of range.");
			// ���� �� ������ ������� ������� �� ����������� ��� ������� ������ �������, � ����� ������ ������� - �� ��� � ����������� ������� �� �������
			elemCondCrossLevel = value;
			axiomCrossLevel = value;
			axiomSetCrossLevel = value;
			break;
		}
		
		if ((*iter).first == "elemCondCrossLevel") {
			value = atof ( (*iter).second.c_str() );
			if ( (value < 0.0) || (value > 1.0) ) 
				throw AxiomLibException("CrossoverTry::setParams : elemCondCrossLevel is out of range.");
			elemCondCrossLevel = value;
			continue;
		}
		
		if ((*iter).first == "axiomCrossLevel") {
			value = atof ( (*iter).second.c_str() );
			if ( (value < 0.0) || (value > 1.0) ) 
				throw AxiomLibException("CrossoverTry::setParams : axiomCrossLevel is out of range.");
			axiomCrossLevel = value;
			continue;
		}
		
		if ((*iter).first == "axiomSetCrossLevel") {
			value = atof ( (*iter).second.c_str() );
			if ( (value < 0.0) || (value > 1.0) ) 
				throw AxiomLibException("CrossoverTry::setParams : axiomSetCrossLevel is out of range.");
			axiomSetCrossLevel = value;
			continue;
		}
			
		// ������ � ����������� ������ ����� - �� ����������� �������� - ������ ���������� ��� ��� ��� ����� ���� �����
		throw AxiomLibException ("Error in CrossoverTry::setParams: in config - too many params for CrossoverTry Class.");
	}
	return 0;
}


/****************************************************************************
*				CrossoverTry::setParams
*
*	Description:	������� ������� �������� ���������� 
*	Parameters:	����� ��������� �����������:
*				newElemCondCrossLevel - �������� ����������� �� ������ ������������ ������� 
*				newAxiomCrossLevel - �������� ����������� �� ������ ������
*				newAxiomSetCrossLevel - �������� ����������� �� ������ ������ ������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� �������� ���������� ������������
*	Author:		dk
*	History:
*
****************************************************************************/
int CrossoverTry::setParams (const double newElemCondCrossLevel, const double newAxiomCrossLevel, const double newAxiomSetCrossLevel) {
	// �������� �� ������������ ��������� ��������� ����������
	if (	(newElemCondCrossLevel < 0.0) || (newElemCondCrossLevel > 1.0) ||
		(newAxiomCrossLevel < 0.0) || (newAxiomCrossLevel > 1.0) ||
		(newAxiomSetCrossLevel < 0.0) || (newAxiomSetCrossLevel > 1.0) 	) 
		throw AxiomLibException ("Error in CrossoverTry::setParams: some of crossLevel Parameters are out of range.");
	// ��������� ����� �������� ����������
	elemCondCrossLevel = newElemCondCrossLevel;
	axiomCrossLevel = newAxiomCrossLevel;
	axiomSetCrossLevel = newAxiomSetCrossLevel;	
	return 0;
}


/****************************************************************************
*				CrossoverTry::setParams
*
*	Description:	������� ������� �������� ���������� 
*	Parameters:	����� ��������� �����������:
*				newCrossLevel - �������� ����������� �� ���� �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� �������� ���������� ������������
*	Author:		dk
*	History:
*
****************************************************************************/
int CrossoverTry::setParams (const double newCrossLevel) {
	// �������� �� ������������ ��������� ��������� ����������
	if ( (newCrossLevel < 0.0) || (newCrossLevel > 1.0) ) 
		throw AxiomLibException ("Error in CrossoverTry::setParams: given crossLevel is out of range.");
	// ��������� ����� �������� ����������
	elemCondCrossLevel = newCrossLevel;
	axiomCrossLevel = newCrossLevel;
	axiomSetCrossLevel = newCrossLevel;
	return 0;
}


/****************************************************************************
*				CrossoverTry::choosePairs
*
*	Description:	������� ������ ��� ������ ������ ��� ����������� �� 
*				������ ������ ������. ����� ���������� �� ������� 
*				�� ����� ������ ������ ������. ����� ��������� ��� 
*				��������������� ��������� ����������� axiomSetCrossLevel.
*	Parameters:	asPairs - ������ ��� ������ ������ ��� ����������� (����������� � ������ �������)
*	Returns:		0 - ���� �� ����� ���� ������ ������ �� ���� ��������
*				1 - ���� ���� �������� ���� �� ���� ���� ������ ������ ��� �����������
*	Throws:		- 
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::choosePairs (std::vector <Pair> &asPairs) {

	// �������� �� ���������� ����� ������ ������
	if (asPop->size() < 2) return 0;
	
	// ����� ��� ��� �����������
	// ���������� ���������� ��������������� ����������
	Pair tmpPair;
	int sec;
	int flag = 0;
	// ��������� �� ����� ������ ������ ������ � ���������� ����� �� ������ ������ ����� ����������� � ����������
	for (int i = 0; i < (int) asPop->size(); i++) {
		// ��� ������ ������� ������ ��������� ������� � �������� ���������� ��������� ���������� ����� �� ��� ������������ � �����������
		if (( (double) rand() / (double) RAND_MAX ) < axiomSetCrossLevel) {
			sec = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) asPop->size() - 2.0) );
			// ������� ������� ������ - ��������� ��������� ���� � ��������� �� � asPairs
			if (sec >= i) sec++;
			tmpPair.first = i;
			tmpPair.second = sec;
			asPairs.push_back(tmpPair);
			flag = 1;
		}
	}
	return flag;
}


/****************************************************************************
*				CrossoverTry::crossAxiomSets
*
*	Description:	������� ����������� ���� ��������� ������ ������ �� 
*				������ ������ ������. ����������� ���������� ����� 
*				�������� ����� ������� ������ �� ������ ������������
*				��� �������, ��������� ��� �����������.
*	Parameters:	axSet1 - ��������� �� ������ ������ �� ������ ������� ������, ��������� ��� �����������
*				axSet2 - ��������� �� ������ ������ �� ������ ������� ������, ��������� ��� �����������
*				axiomSet - ������� ������ , ������� ����� ���� ������� ��� �����������
*	Returns:		1 - ���� ��� ��������� �������, ���� ������� ����� ������� ������
*				0 - ���� ��� ��������� �������, �� ����� ������� ������ �� ���� �������
*				-1 - ���� ����������� �� ������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::crossAxiomSets (std::vector<Axiom *>* axSet1, std::vector<Axiom *>* axSet2, AxiomSetWithStats &axiomSet) {
	// ����� - ��������� ������� ������� ��������� � environment - ������� - �� �������� 
	// ����������� - ������������������ ������ � �������:
	// �� ���� ����� ������� ������� ������� ����� ���������� ����� ������� ������ �� ���������� ������� �� ���������� "<" � ������� ��������� � levelOfSub
	double  levelOfSub = 0.55;
	// ���������� ���������� ���������� �������
	int newSet1, newSet2; 
	std::vector<Axiom *> toInclude;
	//AxiomSetWithStats axiomSet;
	std::vector<int> mask;
	int curNum;
	// ����������� ����� ������ �� ������ �������, ������� ����� ������� �� ����� ��������� ������� ������
	// (� ����� ������� ����� ������� �� 1/3 �� 2/3 ������ �� ������ �� ������)
	newSet1 = (int) ( ( ( (double) rand() / (double) RAND_MAX ) * ( (double) axSet1->size() - 1.0) / 3 ) + ( (double) axSet1->size() - 1.0) / 3);
	newSet2 = (int) ( ( ( (double) rand() / (double) RAND_MAX ) * ( (double) axSet2->size() - 1.0) / 3 ) + ( (double) axSet2->size() - 1.0) / 3);
	if ( (newSet1 <= 0) && (newSet2 <= 0) ) return 0;
	// ����� ������, ������� ����� ��������������
	toInclude.resize( newSet1 + newSet2 );
	// ����������� - ����� ������� �� ������ ������� ������ ����� ������� �� ����� ����������� ������� ������
	// ��������� ������� ����� - ������������ ��� ����������� ��������� � ������� ������ - ���������� ������
	mask.resize (axSet1->size());
	for (unsigned int t = 0; t < mask.size(); t++) {
		mask[t] = 0;
	}
	for (int i = 0; i < newSet1; i++) {
		curNum = (int) ( ( (double) rand() / (double) RAND_MAX ) * (double) (axSet1->size() - i - 1) );
		while (mask[curNum] == 1) {
			curNum++;
		}
		mask[curNum] = 1;
		toInclude[i] = (*axSet1)[curNum];
	}
	// ����������� - ����� ������� �� ������ ������� ������ ����� ������� �� ����� ����������� ������� ������
	// ��������� ������� ����� - ������������ ��� ����������� ��������� � ������� ������ - ���������� ������
	mask.resize ( axSet2->size() );
	for (unsigned int t = 0; t < mask.size(); t++) {
		mask[t] = 0;
	}
	for (int i = 0; i < newSet2; i++) {
		curNum = (int) ( ( (double) rand() / (double) RAND_MAX ) * (double) (axSet2->size() - i - 1) );
		while (mask[curNum] == 1) {
			curNum++;
		}
		mask[curNum] = 1;
		toInclude[newSet1 + i] = (*axSet2)[curNum];
	}
	// ������������� ������� ������ ����� ����������� ��������� ������
	axiomSet.initAxiomSetByCopy (toInclude, levelOfSub);
	return 1;
}


/****************************************************************************
*				CrossoverTry::chooseAxQuads
*
*	Description:	������� ������ ��� ������ ��� ����������� �� ������ ������.
*				����� ���������� �� ������� �� ����� ������ ������. ����� 
*				��������� ��� ��������������� ��������� ����������� axiomCrossLevel.
*	Parameters:	axQuads - ������ �� �������� Quad - ����� ������� ����������
*	Returns:		0 - ���� �� ������� ����� ������� ����������
*				1 - ���� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::chooseAxQuads (std::vector <Quad> &axQuads) {
	// ���������� ��������� ���������� �������
	int tmpDiv;
	int fax, second, sax;
	double average;
	Quad tmpQuad;
	int flag = 0;
	// ������� �������� ����� ������ � ������� ������ � ���������
	average = 0.0;
	for (unsigned int j = 0; j < axiomSets.size(); j++) {
		average += (double) (axiomSets[j])->size();
	}
	if (axiomSets.size() < 1) return 0;
	average = average / (double) axiomSets.size();
	// ��������� �� ���� �������� � ��������� � �������� ���������� ����� �� �������������� ������� � �����������
	for (int i = 0; i < (int) axiomSets.size(); i++) {
		tmpDiv = (int) ( 1.0 + (double) (axiomSets[i])->size() / average );
		for (int y = 0; y < tmpDiv; y++) {
			if (( (double) rand() / (double) RAND_MAX ) < axiomCrossLevel) {
				fax = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (axiomSets[i])->size() - 1.0) );
				second = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) axiomSets.size() - 1.0) );
				sax = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (axiomSets[second])->size() - 1.0) );
				// ���������� ���, ��� ������������ � ����������� - ��������� ���������������� ��������� � ��������� ������ axQuads
				tmpQuad.first = i;
				tmpQuad.fax = fax;
				tmpQuad.second = second;
				tmpQuad.sax = sax;
				axQuads.push_back(tmpQuad);
				flag = 1;
			}
		}
	}
	
	return flag;
}


/****************************************************************************
*				CrossoverTry::crossAxioms
*
*	Description:	������� ����������� ���� ��������� ������ 
*				�� ������ ������. � ������ ���������� ����������� 
*				������������ ����� ������������� ��������� �� ������.
*				����� ������������ ������� ��� ������ ������������ �� 
*				������������� � ������������� ��������� �� ����� 
*				���������� ��������.
*	Parameters:	ax1 - ��������� �� ������ ��������� ��� ����������� ������� ������
*				ax2 - ��������� �� ������ ��������� ��� ����������� ������� ������
*				newAxiomSet - ������� ������ , ������� ����� ���� ������� ��� �����������
*	Returns:		1 - ���� ��� ��������� �������, ���� ������� ����� ������� ������
*				0 - ���� ��� ��������� �������, �� �� ���� ������� ����� ������� ������
*				-1 - ���� ����������� �� ������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::crossAxioms (Axiom* ax1, Axiom* ax2, AxiomSetWithStats &newAxiomSet) {
	// ���������� ���������� ���������� ������
	Check checker;
	int numOfEc1, numOfEc2;
	int ch;
	Axiom* tmp;
	bool flag;
	int t;
	// ����������� ����� ������������ ������� � ������ �� ������
	numOfEc1 = ax1->getNumOfEC();
	numOfEc2 = ax2->getNumOfEC();
	// ���������� � �������� ��������������� - ���������� ��� ����, ����� ����������� ���� - �������������� �������, �� ������ � ����������� �����
	if (numOfEc1 <= numOfEc2) {
		t = numOfEc2; 
		numOfEc2 = numOfEc1;
		numOfEc1 = t;
		tmp = ax2;
		ax2 = ax1;
		ax1 = tmp;
	}
	// ����� ����������� �� ������ ������������� ������� - �� ���� �����, ��� ������ �������� ���������� ����������� ������� (������� �� ����������� �� and ������������ �������)
	for (int i = 0; i < numOfEc1; i++) {
		flag = true;
		ch = (int) (( (double) rand() / (double) RAND_MAX ) * ( (double) numOfEc2 - 1.0) );
		for (int j = 0; j < numOfEc2; j++) {
			if (j == ch) continue;
			if (checker.checkAndCompatibility (ax1, i, ax2, j) == 0 ) {
				// ������ ����������� ������� �� ���������� 
				flag = false;
				break;
			}
		}
		if (flag) {
			for (int k = 0; k < numOfEc1; k++) {
				if (k == i) continue;
				if (checker.checkAndCompatibility (ax1, k, ax2, ch) == 0 ) {
					// ������ ����������� ������� �� ���������� �� ������
					flag = false;
					break;
				}
			}
		}
		if (flag) {
			// ����� ��������� �������� - ���������� ��� ��� ����� �������������� �������
			if ( ( (double) rand() / (double) RAND_MAX ) < axiomCrossLevel) {
				checker.change (ax1, i, ax2, ch);
			}
		}
	}
	return 0;
}


/****************************************************************************
*				CrossoverTry::chooseEcQuads
*
*	Description:	������� ������ ��� ������������ ������� ��� ����������� 
*				�� ������ ���������� ������������ ������.
*				����� ���������� �� ������� �� ����� ������ ������������ �������. ����� 
*				��������� ��� ��������������� ��������� ����������� elemCondCrossLevel.
*	Parameters:	ecQuads - ������ �������� QuadPlus - �������� ����� ������������ ������� ���� �������� ��� �����������
*	Returns:		0 - ���� �� ������� ����� ������������ ������� ����������
*				1 - ���� �������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::chooseEcQuads (std::vector <QuadPlus> &ecQuads) {
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
	//int first;
	//int fax, second, sax;
	int ec1, ec2;
	int flag;
	QuadPlus tmpQuad;
	// ��������������� ���� - ���� ���������� � ������� ������������ ������� � �������� - ���������� ��������� ecMap
	for (int i = 0; i < (int) axiomSets.size(); i++) {
		for (int j = 0; j < (int) (axiomSets[i])->size(); j++) {
			ecNames.clear();
			(*(axiomSets[i]))[j]->getECNames(ecNames);
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
			// ���������� - � ����� �� ����������� - ���������� �������� elemCondCrossLevel 
			if (( (double) rand() / (double) RAND_MAX ) < elemCondCrossLevel) {
				// ����� ������������ ������� ������ ���� �� ������ ������
				ec1 = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (iter->second).size() - 1.0) );
				ec2 = (int) ( ( (double) rand() / (double) RAND_MAX ) * ((double) (iter->second).size() - 2.0) );
				if (ec2 >= ec1) ec2++;
				// ���������� ��������� tmpQuad
				tmpQuad.first = ((iter->second)[ec1]).first;
				tmpQuad.fax = ((iter->second)[ec1]).second;
				tmpQuad.second = ((iter->second)[ec2]).first;
				tmpQuad.sax = ((iter->second)[ec2]).second;
				tmpQuad.ecName = iter->first;
				// ������ ����������� ��������� � ������ ������� ��������
				ecQuads.push_back(tmpQuad);
				flag = 1;
			}
		}
	}
	
	return flag;
}


/****************************************************************************
*				CrossoverTry::crossEcs
*
*	Description:	����������� ���� ��������� ��� ������������ ������� 
*				�� ������ ���������� ������������� �������.
*				����������� ���������� ����� ���������� ����������
*				���������� ������������ ������� �� ������ ������.
*	Parameters:	ax1 - ��������� �� ������ �������, �� ������� ���� �������� ������������ ������� ��� �����������
*				ax2 - ��������� �� ������ �������, �� ������� ���� �������� ������������ ������� ��� �����������
*				ecName - ��� ������������� �������, ������� ���� �������� ��� �����������
*				newAxiomSet - ������� ������ , ������� ����� ���� ������� ��� �����������
*	Returns:		1 - ���� ��� ��������� �������, ���� ������� ����� ������� ������
*				0 - ���� ��� ��������� �������, ������� ����� ������ ������ �� ���� �������
*				-1 - ���� ����������� �� ������
*	Throws:		AxiomLibExeption - ���� ��� ����������� ���� �������� ������������� ������������ ������� (������� ����)
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::crossEcs (Axiom* ax1, Axiom* ax2, std::string &ecName, AxiomSetWithStats &newAxiomSet) {
	// ���������� ���������� ����������
	int ecNum1, ecNum2;
	int curRes = -1;
	double param = 0.0;
	double param1 = 0.0;
	double param2 = 0.0;
	std::vector<std::string> ecParamNames;
	// ��������� ����� ������������ ������� � ������ �� ����� - ��������� ����� �� ����� ��� �� ���������� � ����� � ��� �� ������� � ������� ������ �������
	ecNum1 = ax1->getECNumByName (ecName);
	ecNum2 = ax2->getECNumByName (ecName);
	// �������� �� ������������ ������
	if ( (ecNum1 < 0) || (ecNum2 <0) )
		throw AxiomLibException("CrossoverTry::crossEcs : there is an error in generating pairs of elementary conditions to cross. ");
	// �������� ����������� �������� ���������� ���������� ������������� �������
	ax1->getECParamNames(ecNum1, ecParamNames);
	for (int j = 0; j < (int) ecParamNames.size(); j++) {
		curRes = ax1->getParamValue (param1, ecNum1, ecParamNames[j]);
		if ( (curRes == 0) && ( ( (double) rand() / (double) RAND_MAX ) < elemCondCrossLevel ) ) {
			// ���� 0 - �� ��� ��������� ��������� (� double) - �������� ��������� ���������� � ���������� �������
			// (��� ����� ��������� ������ ��������)
			curRes = ax2->getParamValue (param2, ecNum2, ecParamNames[j]);
			if (curRes != 0)
				throw AxiomLibException("CrossoverTry::crossEcs : there is an error in generating pairs of elementary conditions to cross, ECs are not of the same Class. ");
			param = (param1 + param2) / 2.0; // �������� ���������� ���������� - �������� �������� �� ������������ �������
			// ���������� ������� ���������� �������� ��������
			ax1->setParamValue (param, ecNum1, ecParamNames[j]);
			ax2->setParamValue (param, ecNum2, ecParamNames[j]);
		}
		else{
			// ���� �������� �� double - � � ���� �� ���������� ���������������� - ����� ������ ����� �������� - ���� ��� ����������
			
		}
	}
	return 0;
}


/****************************************************************************
*				CrossoverTry::crossover
*
*	Description:	������� ����������� ��������� ������ ������ �� ������� ������.
*				����� ������������ �������, ������ � ������ ������ ��� 
*				����������� ���������� � ���������� ��������������� 
*				��� ����� �������� ������� ������.
*	Parameters:	axiomSetPop - ��������� ������ ������ - ����� ������ ������, ��� ������� ��������� ��������� �����������
*	Returns:		0 - ���� ��� ��������� �������
*				-1 - ���� ����������� �� ������
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/ 
int CrossoverTry::crossover (AxiomSetPop &axiomSetPop) {
	// ������������� ������ ����������� - ��� ���� ��������� ������ ������ �� ������� � �������� ������
	initCrossover (axiomSetPop);
	// ����������� ���� �� ���������� �������:
	// 1) ����������� �� ������ ������������ �������
	// 2) ����������� �� ������ ������
	// 3) ����������� �� ������ ������ ������

	std::vector < std::vector <AxiomSetWithStats> > newSets;
	std::vector < std::vector <int> > setsMask;
	newSets.clear();
	setsMask.clear();
	newSets.resize(3);
	setsMask.resize(3);
	// 1) ����������� �� ������ ������ ������
	if (axiomSetCrossLevel > 0) {
		// ����� ��� ������ ������ ��� �����������
		std::vector <Pair> asPairs;
		if (choosePairs (asPairs)) {
			// ����������� ��������� ���
			newSets[0].resize(asPairs.size());
			setsMask[0].resize(asPairs.size());
			for (unsigned int i = 0; i < asPairs.size(); i++) {
				setsMask[0][i] = crossAxiomSets (axiomSets[asPairs[i].first], axiomSets[asPairs[i].second], newSets[0][i]);
			}
		}
		else {
			newSets[0].clear();
			setsMask[0].clear();
		}
	}
	
	// 2) ����������� �� ������ ������
	if (axiomCrossLevel > 0) {
		// ����� ������ ��� �����������
		std::vector <Quad> axQuads;
		if (chooseAxQuads (axQuads)) {
			newSets[1].resize(axQuads.size());
			setsMask[1].resize(axQuads.size());
			// ����������� ��������� ������
			for (unsigned int j = 0; j < axQuads.size(); j++) {
				setsMask[1][j] = crossAxioms ( (*(axiomSets [ (axQuads[j]).first ] ))[(axQuads[j]).fax], (*(axiomSets[(axQuads[j]).second]))[(axQuads[j]).sax], newSets[1][j]);
			}
		}
		else {
			newSets[1].clear();
			setsMask[1].clear();
		}
	}
	// 3) ����������� �� ������ ������������ �������
	if (elemCondCrossLevel > 0) {
		// ����� ������ ������������ ������� �� ������� ����� ��������
		std::vector <QuadPlus> ecQuads;
		if (chooseEcQuads (ecQuads)) {
			newSets[2].resize(ecQuads.size());
			setsMask[2].resize(ecQuads.size());
			// ����������� �� ������ ����������� ������� ��������� ������
			for (unsigned int k = 0; k < ecQuads.size(); k++) {
				setsMask[2][k] = crossEcs ( (*(axiomSets[(ecQuads[k]).first]))[(ecQuads[k]).fax], (*(axiomSets[(ecQuads[k]).second]))[(ecQuads[k]).sax], (ecQuads[k]).ecName, newSets[2][k]);
			}
		}
		else {
			newSets[2].clear();
			setsMask[2].clear();
		}
	}
	
	axiomSetPop.addSets(newSets, setsMask);
	//std::cout << "\n 	NewSets were succesfully added. Press Enter, please.";
	//std::cin.get();
	
	return 0;
}
