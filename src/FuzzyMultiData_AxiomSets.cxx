/****************************************************************************
*
*				������� ������ FuzzyMultiDataLearnAlgorithm
*
****************************************************************************/

#include "FuzzyMultiDataLearnAlgorithm.h"
#include "Defines.h"

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"
#include <string.h>

using namespace AxiomLib;

// ������������ ����� �������� ����� � ��������� ������� ������ (���������� ������� �����)
#define int_AxiomSet_Name_Max_Size				200

// ����� ������� ��������� ��� ���������� ����������� �������� - 
//  ���� �������� ��������� ����� ��� �������� =< ���� �����
//  - �� ������ ������� �� ������ � ����������� ��������� ��������
// ����� ������� ������� �� ���� ���������: ����� � �������� ������� ����� ������������ ������� � ��������
//#define double_weight_markUps_length	0.05
//  ����� � �������� ����� ���������� � �������� ��������� �������
//#define double_weight_markUps_sens	0.5
// �������� ��������� - ���� �������� ���������� ����� �� ����� � ������������ ������������� ������, ��� ������������ �������� ���������� ����� * �� ������ �����������, �� ������� �� ������ � �������
//#define double_threshold_func	0.33

// [0.0; 1.0] - ������� (� ����� �������; �������� �� 0 - ��� ������� �� 1 - ������������ ��������) � �������� ��������� � ������� ��������� ���������� ��������
//#define double_simplify_coeff	0.3


// �������� ���������� ������������ ������ ������� � ������� ��������� �������, ������� ������� ��� ���� ���������
//  (������ ���� ���������� �������, ��� ��� �� ����������� ��� ����� ����� ���������� ������)
#define int_max_size_of_vector_axiomExprSet		100

// ������������ ����� ������ ������, ������� ����� ���� ������� �� ������ ���� ��������� ��� ����� �������� ������� ������
#define int_max_number_of_renew_axiom_set		10

// ����� �������� ����� �������� � ��������������� �������
#define eps			0.0000000001


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::createAxiomSet
*
*	Description:	������� ������������ ������� ������ �� ������
*	Parameters:		param - ����������� ���������� ������ ������ � ������� �������� 
*					� ������ ���������
*	Returns:		0 - ���� �������� ��������� ���������
*					1 - ���� �������� �� ��� �������, ��� ��� ��� ������ �������� �� � ������������ �������
*						(���� ������ ������� ��� ������ �� �� ��������)
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::createAxiomSet (void) {
	// �������������� ����������� ����������
	std::vector <std::string> dataSetParams;
	dataSetParams = fuzzyDataSet.getParamNames();
	
	//  ������� ������ ���� ������ - ����� ��� ����� ���� ������������ ��� ������������ ������ ������
	int numOfAxioms = 0;
	for (unsigned int i = 0; i < bestAxiomsFileNames.size(); i++) {
		numOfAxioms += bestAxiomsFileNames[i].size();
	}

	// ������ ������, ����������� �� ���� ������������ ������ �� ������������ �������
	std::vector <AxiomExpr> bestAxioms;

	/* // ��� ������ - ���� ����������� bestAxioms �������� � ����������� ������
	// ������� ���������� ����������, ���� ��� ����������
	for (unsigned int i = 0; i < bestAxioms.size(); i++)
		bestAxioms[i].clear();
	bestAxioms.clear();
	*/

	// ������� ����� ���� ������
	bestAxioms.resize(numOfAxioms);
	int ax = 0;
	for (unsigned int i = 0; i < bestAxiomsFileNames.size(); i++) {
		for (unsigned int j = 0; j < bestAxiomsFileNames[i].size(); j++, ax++) {
			// ������ ������� �� �����
			bestAxioms[ax].initAxiomFromFile (axiomBaseDir, bestAxiomsFileNames[i][j], dataSetParams);
		}
	}

	//	����������� ����� �������� ��������
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// ����������������� ���������� �� ��������� - �������� �� ����� ������ ������ ���������� ���������� ��������� �������� �� ������ �����
	int from, upTo, rankFrom, rankUpTo;
	splitRange (rank, size, 0, numOfAxioms, from, upTo, rankFrom, rankUpTo);
	// ��� ���� ��������� - ������� ���������� ���������� ��� �������� ������ ������ ������ ������
	bestAxiomSets.clear();
	if (rank == rankFrom) { // �������� ���������� ����� �������, ����� �� ����������� ���������� � ��������� ���������
		// �������� ��������� ��������� - ������� ������� ������ ������ ��� ���������� ��������� ������
		// ������� ����� ������ ������, ������ �� ������� ����� ��������� ������ ���� �������
		std::vector <AxiomExprSetPlus> axiomSets, newAxiomSets;
		std::vector < std::vector <AxiomExprSetPlus> > nextStepAxiomSets;
		axiomSets.resize(upTo - from);
		int as = 0;
		#pragma omp parallel for schedule(dynamic, 1)
		for (int i = 0; i < (upTo - from); i++) {
			// ��������� ������� ������ �� ����� �������
			axiomSets[i].addAxiom (bestAxioms[i + from]);
			// ������������� ����������
			axiomSets[i].axiomsIndex.push_back(i + from);
			// ��������� �������� ������� ������� ��� ����� ������� ������
			matterAxiomSetFunc (axiomSets[i]);
			/*
			AxiomExprPlus aep;
			AxiomExprStructure aes;
			bestAxioms[i + from].createAxiomExprStructure (aes); 
			aep.initAxiomFromStructure (aes);
			matterAxiomFunc (aep, 0);
			matterAxiomFunc (aep, 1);
			aes.clear();
			aep.clear();
			*/
			// ����� ������� ����������, ���� ��� ����������
			if (this->comments) {
				char buf[128];
				sprintf (buf, "\r\tCurrent axiomSet: %d out of %d. Goal value is %.2f (%d, %d).\tThread %d.\t", from + i + 1, numOfAxioms, axiomSets[i].goal, axiomSets[i].errFirst, axiomSets[i].errSecond, omp_get_thread_num());
				//printf ("\n\tCurrent axiomSet: %d out of %d. Goal value is %.2f (%d, %d).\tThread %d.\t", from + i + 1, numOfAxioms, axiomSets[i].goal, axiomSets[i].errFirst, axiomSets[i].errSecond, omp_get_thread_num());
				std::cout<<buf<<std::endl;
				//std::cout.flush();
			}
		}

		// ������������ �������� ���������� ������� ������
		bool condition = true;
		std::vector <int> indicesOfBestSets;
		nextStepAxiomSets.clear();
		int iterNum = 0;
		double defMaxGoal;
		defMaxGoal = -1.0; // (axiomSets[axSet].goal + 1)*2.0; // ������ defMaxGoal ���������� ������� (�������� ������ axiomSets[axSet].goal - ���� � ������ �������������� �����������)
		// ������������� ������ ������� �� ����� ������
		unsigned int wholeNum = 0;
		newAxiomSets.resize (bestAxioms.size());
		while (condition) {
			// �������������� ���������� ��� ������� ������� ������ ������
			wholeNum = 0;
			nextStepAxiomSets.resize (axiomSets.size());
			for (unsigned int axSet = 0; axSet < axiomSets.size(); axSet++) {
				// ���������� �� ����� ������ ��������
				if (this->comments) {
					char buf[128];
					sprintf (buf, "\r\tProc start point: %d - %d\tIteration: %d\tCurrent axiomSet: %d out of %d.\t Thread %d.\t", from + 1, upTo, iterNum+1, axSet+1, axiomSets.size(), omp_get_thread_num());
					std::cout<<buf<<std::endl;
					//std::cout.flush();
				}
				// ������� ���������� ������� � �������������� ������������
				for (unsigned int u = 0; u < newAxiomSets.size(); u++)
					newAxiomSets[u].clear();
				// ��� ������ ������� - ������� ����� ������� �� ������ ������ ������� � ������� axiomSets[axSet]
				#pragma omp parallel for schedule(dynamic, 1)
				for (int ax = 0; ax < (int) bestAxioms.size(); ax++) {
					// ��� ���� ������, ��� �� �������� � ��������������� ������� ������ ������� ����� �������
					if (isIn ((unsigned int) ax, axiomSets[axSet].axiomsIndex) < 0) {
						// ������� ����� ������� ������ �� ������ ��������� - �������� � ��� ��� ����������� �������
						newAxiomSets[ax] = axiomSets[axSet];
						newAxiomSets[ax].addAxiom (bestAxioms[ax]);
						newAxiomSets[ax].axiomsIndex.push_back(ax);
						// ������� �������� ������� ������� ��� ���������� ������� ������ � ��������� ���
						matterAxiomSetFunc (newAxiomSets[ax]);
					} else {
						// ���� ������� ��� ������ � ������� - �� �� ������� ����� �������. ��������� �������� ������� ������� �� ��������� ��������� ����������� ������ axiomSets[axSet].goal .
						newAxiomSets[ax].goal = defMaxGoal;
					}
				}
				// �������� �������� ����� ������ ������ ������ � ��������� �� � ��������� �� ��������� ���� ���������
				indicesOfBestSets.clear();
				chooseIndicesOfBestAxiomSets (newAxiomSets, indicesOfBestSets, axiomSets[axSet].goal);
				wholeNum += indicesOfBestSets.size();
				addAxiomSets(nextStepAxiomSets[axSet], newAxiomSets, indicesOfBestSets);
			}
			// ��������� �������� ������� ������ � ����������� ������ ����������� � �� ���� ���� ������� �� ����������
			addToBestAxiomSets (axiomSets);
			// ��������� ������� ������ ��� ���������� ���� � axiomSets
			axiomSets.resize(wholeNum);
			unsigned int cur = 0;
			double curBestVal = -1.0;
			for (unsigned int y = 0; y < nextStepAxiomSets.size(); y++) {
				for (unsigned int u = 0; u < nextStepAxiomSets[y].size(); u++, cur++) {
					axiomSets[cur] = nextStepAxiomSets[y][u];
					nextStepAxiomSets[y][u].clear();
					if ((curBestVal < -eps) || ((curBestVal - axiomSets[cur].goal) > eps))
						curBestVal = axiomSets[cur].goal;
				}
				nextStepAxiomSets[y].clear();
			}
			nextStepAxiomSets.clear();
			
			// - ������, ��� ��� ���� ����� ������ ������ ��� �������������� � ������� chooseIndicesOfBestAxiomSets
			// ������������ ����� ������ ������, ����� ��� �� ����� ������������ �� ��������� �������������� ����������
			cutDownAxiomSets (axiomSets);

			// ����������� ����� ��������, ����������� ����������
			iterNum++;

			// ��������� �������� ��������:
			// - �������, ���� �� ���� ������� �� ����� ����� ������� ������ � ������ ��������� ������� �������
			if (axiomSets.size() < 1) {
				condition = false; 
				break;
			}
			// - �������, ���� ����� �������� ������ ��������� �������
			if ((iterNum >= maxNumberOfSteps) || ((curBestVal - bestAxiomSetGoal) < eps)) {
				// ��������� �������� ������� ������ � ����������� ������ ����������� � �� ���� ���� ������� �� ����������
				addToBestAxiomSets (axiomSets);
				condition = false;
				break;
			}
		}

		// ���������� ������ ������ ������ �� ��������� ����� ���������
		sortBestAxiomSets ();

		// ���������� �������������� ������ ������ � �����
		saveBestAxiomSets (rank, dataSetParams);
	}

	// ����������� ������ ����������� ������ ������ � ���������� �� ���
	gatherBestAxiomsSets (rank, size);

	// ������������� ���������
	MPI_Barrier (MPI_COMM_WORLD);

	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::cutDownAxiomSets
*
*	Description:	������� ��������� ����� ������ ������ �� ������� ������� �� 
*					��������� � ���������� ������ ��������.
*	Parameters:		axiomSets - ������ ������ ������, ������� �������� ���������� 
*					�� ��������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::cutDownAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) const {
	// �������� ������� ����������
	if ((maxAxiomSetPopSize < 1) || ( (int) axiomSets.size() <= maxAxiomSetPopSize))
		return 0;
	
	// ���������� ������ �� �������� ������� �������
	std::vector <int> indecies;
	sortAxiomSets (axiomSets, indecies);
	
	// ����������� ����� ������ ������, ����������� � ���������
	unsigned int numBestToSave, numToChoose, numFrom;
	numBestToSave = (unsigned int) round (percentBestAxiomSets*maxAxiomSetPopSize);
	numToChoose = (unsigned int) maxAxiomSetPopSize - numBestToSave;
	
	numFrom = axiomSets.size() - numBestToSave;
	std::vector <bool> chosen;
	chosen.resize (numFrom, false);
	int curChoice;
	std::vector <int> toSave;
	toSave.resize (maxAxiomSetPopSize);
	// ������� � ������ ����������� ��������� - �������� ����� ������
	for (unsigned int i = 0; i < numBestToSave; i++) {
		toSave[i] = indecies[i];
	}
	// ��������� ����� ����������
	for (unsigned int i = 0; i < numToChoose; i++) {
		curChoice = round (((double) rand() / (double) RAND_MAX) * (double) (numFrom - i - 1));
		for (unsigned int t = (unsigned int) curChoice; t < numFrom; t++)
			if (!chosen[t]) {
				chosen[t] = true;
				toSave[numBestToSave + i] = indecies[numBestToSave + t];
				break;
			}
	}

	// ���������� ������� ��������� ��� ���������� ��������� �� ��������
	unsigned int tmpInt;
	for (unsigned int i = maxAxiomSetPopSize - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			if (toSave[j] < toSave[j+1]) {
				tmpInt = toSave[j+1];
				toSave[j+1] = toSave[j];
				toSave[j] = tmpInt;
			}
		}
	}
	
	// �������� ����������� ������ ������
	// ������� ���������� ��������� ������� ������
	if ((toSave[0] + 1) < (int) axiomSets.size()) {
		for (unsigned int j = toSave[0] + 1; j < axiomSets.size(); j++)
			axiomSets[j].clear();
		axiomSets.erase (axiomSets.begin() + toSave[0] + 1, axiomSets.end());
	}
	for (int i = 1; i < maxAxiomSetPopSize; i++) {
		if ((toSave[i] + 1) < toSave[i-1]) {
			// ������� ���������� ��������� ������� ������
			for (int j = toSave[i] + 1; j < toSave[i-1]; j++)
				axiomSets[j].clear();
			// ������� �������� ��������
			axiomSets.erase (axiomSets.begin() + toSave[i] + 1, axiomSets.begin() + toSave[i-1]);
		}
	}
	// ������� ���������� ��������� ������� ������
	if (toSave[maxAxiomSetPopSize-1] > 0) {
		for (int j = 0; j < toSave[maxAxiomSetPopSize-1]; j++) {
			axiomSets[j].clear();
		}
		axiomSets.erase (axiomSets.begin(), axiomSets.begin() + toSave[maxAxiomSetPopSize-1]);
	}

	return 0;
}	


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::chooseIndicesOfBestAxiomSets
*
*	Description:	������� �������� �� ������� ������ ������ �������� �����
*					������, ������ �� �������� ������� ������� ������ ����
*					������ ��������� ��������.
*	Parameters:		newAxiomSets - ������ ������ ������, �� ������� ���������� ������
*					indicesOfBestSets - ����������� ������ �������� ������ ������ ������
*					goal - �������� ������� �������, ��� �������� ������ ������ ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::chooseIndicesOfBestAxiomSets (std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets, double &goal) const {
	// ��������� ������ ������ ������ �� �������� ������� �������
	std::vector <int> indecies;
	sortAxiomSets (newAxiomSets, indecies);
	// ���� ������ �������������� ������� ������
	int first = 0; 
	while (first < (int) indecies.size()) {
		if (newAxiomSets[indecies[first]].goal > -eps)
			break;
		first++;
	}
	// ���� ������� ������, ������� � �������� ��� ������� ������ ����� �������� ������� ������� ������ ��������� goal
	int last = first;
	while (last < (int) indecies.size()) {
		if ((newAxiomSets[indecies[last]].goal - goal) > eps)
			break;
		last++;
	}
	// ���� ����� �� ������� - �� �������
	if ((first == (int) indecies.size()) || (first == last)) {
		indicesOfBestSets.clear();
		return 0;
	}
	
	// ���������� ������������ ����� ��������� � �������������� �������
	//  �������� ����� ������ ������ ������ � �������� ���������
	int numBest, numRand, num;
	numBest = round ((double) int_max_number_of_renew_axiom_set * percentBestAxiomSets);
	if (numBest > (last - first)) {
		numBest = last - first;
		numRand = round ((double) numBest * (1.0 - percentBestAxiomSets) / percentBestAxiomSets);
		num = numBest + numRand;
	} else {
		numRand = int_max_number_of_renew_axiom_set - numBest;
		num = int_max_number_of_renew_axiom_set;
	}

	// ���� �������� ������� ����� ������ ������
	if (((int) newAxiomSets.size() - first) <= num) {
		num = (int) newAxiomSets.size() - first;
		indicesOfBestSets.resize (num);
		for (int i = 0; i < num; i++, first++)
			indicesOfBestSets[i] = indecies[first];
		return 0;
	}

	// �������� ��������� ������� ������
	int chooseDelta;
	chooseDelta = indecies.size() - first - numBest;
	std::vector <int> jRand;
	jRand.resize (numRand);
	int cur;
	int condition;
	for (int t = 0; t < numRand; t++) {
		// �������� �������� ����� �� ������� [0, chooseDelta-1]
		cur = first + numBest + (int) (((double) rand () / ((double) RAND_MAX + 1.0)) * (double) chooseDelta);
		// ��������� - �� �������� �� ��� ������ - ���� ��������, �� ��������
		do {
			condition = false;
			for (int s = 0; s < t; s++) {
				if (cur == jRand[s]) {
					cur++;
					condition = true;
					break;
				}
			}
		} while (condition);
		// ��������� ��������� �����
		jRand[t] = cur;
		// ��������� �������� ������
		chooseDelta--;
	}
	// ��������� ��� ��������� �������
	indicesOfBestSets.resize (num);
	cur = first;
	for (int i = 0; i < numBest; i++, cur++)
		indicesOfBestSets[i] = indecies[cur];
	cur = 0;
	for (int i = numBest; i < num; i++, cur++)
		indicesOfBestSets[i] = indecies[jRand[cur]];
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::addAxiomSets
*
*	Description:	������� ��������� ������� ������ �� ������ �������, ������� 
*					������� ����������� �� ������� �������, � ������ ������.
*	Parameters:		nextStepAxiomSets - ������ ������ ������, � ������� ����������� ����� �������
*					newAxiomSets - ������ ������, �� �������� ����������� ������� ������
*					indicesOfBestSets - ������ �������� ����������� ������ ������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������, � ������� ��������� ������� ������ �� ����
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::addAxiomSets(std::vector <AxiomExprSetPlus> &nextStepAxiomSets, std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets) const {
	if (nextStepAxiomSets.size() > 0)
		throw AxiomLibException("Error in FuzzyMultiDataLearnAlgorithm::addAxiomSets: input arguments out of consistency.");
	// ���������� ������� ������ ������ ���������
	unsigned int toAdd = 0;
	for (unsigned int i = 0; i < indicesOfBestSets.size(); i++) {
		if ((indicesOfBestSets[i] >= 0 ) && (indicesOfBestSets[i] < (int) newAxiomSets.size()))
			toAdd++;
	}
	// ������� ��� ����� ������� ������ � ������ ���� ������ � ������������ � ��������� ��������
	unsigned int curSize = 0; // nextStepAxiomSets.size();
	nextStepAxiomSets.resize (toAdd);
	for (unsigned int i = 0; i < indicesOfBestSets.size(); i++) {
		if ((indicesOfBestSets[i] >= 0 ) && (indicesOfBestSets[i] < (int) newAxiomSets.size())) {
			nextStepAxiomSets[curSize] = newAxiomSets[indicesOfBestSets[i]];
			curSize++;
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::sortBestAxiomSets
*
*	Description:	������� ��������� ����� ��������� ������� ������ ������ ������
*					�� ��������� �� ��������� ��� � ���������������� ����� ��������
*	Parameters:		-
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::sortBestAxiomSets (void) {
	// ���� ����� ��������� � ������� ������ ������ ������ ������ ��������� ��������� - �� ������ �������
	if ((int) bestAxiomSets.size() <= numberOfBestAxiomSets)
		return 0;
	// ���� ����� ���, ������� ������ ������ - ������ 1, �� ������ ������� ��� ��������
	if (numberOfBestAxiomSets < 1) {
		for (unsigned int i = 0; i < bestAxiomSets.size(); i++) 
			bestAxiomSets[i].clear();
		bestAxiomSets.clear();
		return 0;
	}
	// �������� �� ������� �������� ����� ������ ������ ������ � ������� ��
	int numToDel;
	numToDel = (int) bestAxiomSets.size() - numberOfBestAxiomSets;
	std::vector <int> indeciesToDel;
	indeciesToDel.resize(numToDel);
	for (int i = 0; i < numToDel; i++) {
		indeciesToDel[i] = i;
	}
	int curIndex; 
	double curMin;
	for (int i = numToDel; i < (int) bestAxiomSets.size(); i++) {
		curMin = bestAxiomSets[i].goal;
		curIndex = -1;
		for (int j = 0; j < numToDel; j++) {
			if (curMin > bestAxiomSets[indeciesToDel[j]].goal) {
				curMin = bestAxiomSets[indeciesToDel[j]].goal;
				curIndex = j;
			}
		}
		if (curIndex >= 0) {
			indeciesToDel[curIndex] = i;
		}
	}
	// ������ �������� ��������� - ��������� ��� �� ��������
	for (int i = numToDel - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (indeciesToDel[j] < indeciesToDel[j+1]) {
				curIndex = indeciesToDel[j+1];
				indeciesToDel[j+1] = indeciesToDel[j];
				indeciesToDel[j] = curIndex;
			}
		}
	}
	// ������� ������� ������ � ������������ � ��������� �������� ��������
	for (int i = 0; i < numToDel; i++) {
		bestAxiomSets[indeciesToDel[i]].clear();
		bestAxiomSets.erase (bestAxiomSets.begin() + indeciesToDel[i]);
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::saveBestAxiomSets
*
*	Description:	���������� ������ �������������� ������ ������
*	Parameters:		dataSetParams - �������� ������������ ������ ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::saveBestAxiomSets (const int rank, std::vector <std::string> &dataSetParams) {
	// ���������� ����������� ������ ������ � �����
	bestAxiomSetsNamesPlus.resize (bestAxiomSets.size());
	std::string curAxiomSetName;
	char tmss[20];
	for (unsigned int asNum = 0; asNum < bestAxiomSets.size(); asNum++) {
		curAxiomSetName = axiomSetName;
		tmss[0]='\0';
		sprintf(tmss, "%d_%d", rank, asNum + 1);
		curAxiomSetName.append (tmss);
		bestAxiomSets[asNum].saveAxiomSetToFile (axiomSetBaseDir, curAxiomSetName, dataSetParams);
		// ��������� �������� ����� � ���������� �� ������ ������� ������ � ����������� ���������� ����������
		bestAxiomSetsNamesPlus[asNum].str = curAxiomSetName;
		bestAxiomSetsNamesPlus[asNum].doubleVal = bestAxiomSets[asNum].goal;
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::gatherBestAxiomsSets
*
*	Description:	������� �������� ����� ������ � ���������� ������ ������ 
*					������ � ������ ���������, �� ������� ��� ������������� 
*					�������� ��������.
*	Parameters:		rank - ���� �������� ��������
*					size - ����� ����� ���������, �� ������� ��� ������������� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::gatherBestAxiomsSets (const int rank, const int size) {
	if (size > 1) { // ������ ��������� ������� ��� ������ ����� ���������� ��������� ����������� ���������� - ���������� ���������� ������
		// ��� ����� - �������� ������������ ���� MPI ��� �������� �������� ������ �� �� ������, � ������ ���������
		struct FileDescrPlus {
			char name[int_AxiomSet_Name_Max_Size]; // file name
			double goal[1]; // value of goal-function for axiomSet
		};
		struct FileDescrPlus fDescr[2];
		/* build datatype describing structure */
		MPI_Datatype FileDescrPlusType;
		MPI_Aint base;
		MPI_Datatype type[3] = {MPI_CHAR, MPI_DOUBLE, MPI_UB};
		int blocklen[3] = {int_AxiomSet_Name_Max_Size, 1, 1};
		MPI_Aint disp[3];
		/* compute displacements of structure components */
		MPI_Address(fDescr, disp);
		MPI_Address(fDescr[0].goal, disp+1);
		MPI_Address(fDescr+1, disp+2);
		base = disp[0];
		for (int i = 0; i < 3; i++) disp[i] -= base;
		/* build datatype describing structure */
		MPI_Type_struct( 3, blocklen, disp, type, &FileDescrPlusType);
		// ����������� ���� � MPI
		MPI_Type_commit(&FileDescrPlusType);

		// �������� ��� ����� ������ � ���������� ������ ��������������
		unsigned int buffer[1];
		if (rank == 0) { // ���� ������� �������� ������
			struct FileDescrPlus *fDescrips;
			unsigned int st, vecSize;
			MPI_Status status;
			for (int procNum = 1; procNum < size; procNum++) {
				// �������� ����� ��������, ���������� ������� �� �������� - ������ ����������� ���� ���������
				MPI_Recv( buffer, 1, MPI_UNSIGNED, procNum, 400 + procNum, MPI_COMM_WORLD, &status);
				vecSize = buffer[0];
				st = bestAxiomSetsNamesPlus.size();
				bestAxiomSetsNamesPlus.resize (st + vecSize);
				fDescrips = new struct FileDescrPlus[vecSize];
				MPI_Recv( fDescrips, vecSize, FileDescrPlusType, procNum, 401 + procNum, MPI_COMM_WORLD, &status);
				for (unsigned int i = 0; i < vecSize; i++, st++) {
					// ��������� ���������� �������� - ������ ���� ������ � ��������� ������������ �������
					bestAxiomSetsNamesPlus[st].str.assign (fDescrips[i].name);
					bestAxiomSetsNamesPlus[st].doubleVal = fDescrips[i].goal[0];
				}
				delete fDescrips;
			}
		} else { // ���� ������� �������� ������
			struct FileDescrPlus *fDescrips;
			// �������� ����� ������������, �� ������� � ���� �������� ����� ����� ���������� ������������ �������
			buffer[0] = bestAxiomSetsNamesPlus.size();
			MPI_Send( buffer, 1, MPI_UNSIGNED, 0, 400 + rank, MPI_COMM_WORLD);
			// ���� ����� ����� � ������� ������ > 0, �� �������� ��
			if (bestAxiomSetsNamesPlus.size() > 0) {
				// �������� ������ � ������� ������ � ��������� ��������� ��� �������� ���������� MPI
				fDescrips = new struct FileDescrPlus[bestAxiomSetsNamesPlus.size()];
				for (unsigned int t = 0; t < bestAxiomSetsNamesPlus.size(); t++) {
					strncpy(fDescrips[t].name, bestAxiomSetsNamesPlus[t].str.c_str(), int_AxiomSet_Name_Max_Size - 1);
					fDescrips[t].goal[0] = bestAxiomSetsNamesPlus[t].doubleVal;
				}
				// �������� ���������� ������ � ������� ������
				MPI_Send( fDescrips, bestAxiomSetsNamesPlus.size(), FileDescrPlusType, 0, 401 + rank, MPI_COMM_WORLD);
				delete fDescrips;
			}
		}
	}

	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::isIn
*
*	Description:	������� ���������� ������ ������ ��������� �������� � ������, 
*					��� -1 ���� ������ �������� � ������� ���
*	Parameters:		elem - ������� �������
*					vec - ������, � ������� ������� �����
*	Returns:		-1 - ���� ������� �� ������ � ������
*					>=0  - ������ ������� ��������� �������� elem � ������ vec
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::isIn (const unsigned int elem, const std::vector <unsigned int> &vec) const {
	for (int i = 0; i < (int) vec.size(); i++)
		if (elem == vec[i]) {
			return i;
		}
	return -1;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::addToBestAxiomSets
*
*	Description:	������� ��������� �������� ����� ������ ������ � ������� ������ ������, 
*					���� ��� ���� ��� ����� ��� �������� ������� ������ �� ������� �� ������
*					�������.
*	Parameters:		axiomExprSetPlus - ����������� ������� ������
*	Returns:		-1 - ���� � ���� ���������� �������� ������
*					0  - ���� ������� ������ ���� ���������
*					1  - ���� ������� ������� � ������ ������� ������ ������, ��� � ������� 
*						 � ������� ������ ������, ������� ������ ������� �� ���� ���������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::addToBestAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) {
	// �������� ���������� ������� bestAxiomSets �� ��������� ����������
	std::vector <AxiomExprSetPlus> tmpBest;
	tmpBest.resize (bestAxiomSets.size());
	for (unsigned int i = 0; i < bestAxiomSets.size(); i++) {
		tmpBest[i] = bestAxiomSets[i];
		bestAxiomSets[i].clear();
	}
	// ����������� ������� tmpBest � axiomSets �� �����������
	std::vector <int> indeciesSets, indeciesBest;
	sortAxiomSets (axiomSets, indeciesSets);
	sortAxiomSets (tmpBest, indeciesBest);
	// �������� ����� ������� ������ ���������
	unsigned int s = 0;
	unsigned int b = 0;
	unsigned int sMax = axiomSets.size();
	unsigned int bMax = tmpBest.size();
	unsigned int upTo = int_max_size_of_vector_axiomExprSet;
	if ((sMax + bMax) < upTo) {
		upTo = sMax + bMax;
		s = sMax;
		b = bMax;
	}
	while ((s + b) < upTo) {
		if (b >= bMax) {
			s++;
			continue;
		}
		if (s >= sMax) {
			b++;
			continue;
		}
		if (tmpBest[indeciesBest[b]].goal < axiomSets[indeciesSets[s]].goal)
			b++;
		else
			s++;
	}

	// ��������� ��������� ������� ������ � ������ ������
	bestAxiomSets.resize (upTo);
	for (unsigned int i = 0; i < b; i++)
		bestAxiomSets[i] = tmpBest[i];
	for (unsigned int i = 0; i < s; i++, b++)
		bestAxiomSets[b] = axiomSets[i];

	// ������� �������������� �������
	for (unsigned int i = 0; i < axiomSets.size(); i++)
		axiomSets[i].clear();
	axiomSets.clear();
	for (unsigned int i = 0; i < tmpBest.size(); i++)
		tmpBest[i].clear();
	tmpBest.clear();

	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::sortAxiomSets
*
*	Description:	������� ��������� ������ ������ ������ �� ���������� ������� ��������
*	Parameters:		axiomSets - ������ ����������� ������ ������
*					indecies - ������ �������� ������ ������ � ������� ���������� �������� ������� �������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::sortAxiomSets (const std::vector <AxiomExprSetPlus> &axiomSets, std::vector <int> &indecies) const {
	indecies.resize (axiomSets.size());
	for (int i = 0; i < (int) axiomSets.size(); i++)
		indecies[i] = i;
	int tmp;
	for (int i = (int) axiomSets.size() - 1; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			if (axiomSets[indecies[j]].goal > axiomSets[indecies[j+1]].goal) {
				tmp = indecies[j+1];
				indecies[j+1] = indecies[j];
				indecies[j] = tmp;
			}
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������:
*					1. �������� ���������� ��������� ������� � ������������
*					   ����������� ���������� ��������
*					2. �������� ���������� ����������� ������� � ������� 
*					   �������� ������� �������
*	Parameters:		as - ������� ������, ��� ������� ��������� �������� ������� �������
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc (AxiomExprSetPlus &as) const {
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
	std::vector <double> curTS;
	std::vector <int> tempLen, corrMarkUp, curMarkUp, result;
	// ��������� ���������� �� ��������� ������ ������
	fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	int dataSetParamsSize = fuzzyDataSet.paramNamesSize();
	// ������ �������� ������������ ���������� ������
	/* // ��� �������� ��� ���� ������� - ��� ������ ��������� � ������� �� ������ �� �����������
	if ((numOfClasses != numOfMultiTS.size()) || (numOfClasses != numOfTS.size()))
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc : incorrect response from internal function of dataSet.");
	for (int u = 0; u < numOfClasses; u++) {
		if (numOfMultiTS[u] != (int) numOfTS[u].size()) 
			throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc : incorrect response from internal function of dataSet (level2).");
		for (int v = 0; v < (int) numOfTS[u].size(); v++) {
			if (dataSetParamsSize != numOfTS[u][v])
				throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc: incorrect response from some of dataSet function (level3).");
		}
	}
	*/
	// ��� ������� ������ ���������� ��������� ��������� ����������� ���������� ��������
	std::vector < std::vector <int> > resMarkUps, genMarkUps;
	int upToNumTS;
	int errFirstVal, errSecondVal;
	as.errFirst = 0;
	as.errSecond = 0;
	as.markUps.resize(numOfClasses);
	for (int abType = 0; abType < numOfClasses; abType++) {
		// ����������� ����� ������������ ���������� ��� ������������ ��������
		if ((numberOfUsedClassTraj > 0) && (numberOfUsedClassTraj < numOfMultiTS[abType]))
			upToNumTS = numberOfUsedClassTraj;
		else
			upToNumTS = numOfMultiTS[abType];
		// �������������� ���������� ��� ������ �������� ����������, ���������� ������� ���������� ���������
		genMarkUps.clear();
		resMarkUps.resize (upToNumTS);
		for (int multiTS = 0; multiTS < upToNumTS; multiTS++) {
			createRefMarkUp (resMarkUps[multiTS], as, abType, multiTS, numOfTS);
		}
		// �������� �������� - ������� 0 � ������ � � ����� �������� - ����� ����� ����� ���� ������ �����
		simplifyMarkUps (resMarkUps);
		// �� ��������� ���������� �������� ���������� ����������� ��������� - ���������� �� ����� � ��������� �������� ���������� ��������
		createMarkUpVariants (genMarkUps, resMarkUps);
		// �������� ������� �������� - ������� 0 � ������ � � �����. ���� �������� �������� ��������� ��������� � ���� ��������
		simplifyMarkUps (genMarkUps);
		// ��������� ����������� �������� �������� - ���� ����� ����� �������� �� �������
		if (genMarkUps.size() < 1)
			createSimpleMarkUpVariants (genMarkUps, (int) as.axioms.size());
		// �������� �� ���� �������� ������ ��� ������� ������ ���������� ��������� � ��������� ��
		chooseBestMarkUp (as, abType, as.markUps[abType], genMarkUps, errFirstVal, errSecondVal);
		as.errFirst += errFirstVal;
		as.errSecond += errSecondVal;
	}
	// ���������� ������ �������� ������� �������� ��� ������ ������� ������ � ������ ��������� �������� ��������� ����������
	//matterAxiomSetFunc (as, as.markUps);
	as.goal = goalStrategy->compute(as.errFirst, as.errSecond);

	return as.goal;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������ � �������� ��������� 
*					�������� ��������� ����������
*	Parameters:		as - ������� ������, ��� ������� ��������� �������� ������� �������
*					markUps - �������� ��������� ����������
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc (AxiomExprSetPlus &as, std::vector <std::vector <int> > &markUps) const {
	int tmpFirst, tmpSecond;
	double tmpGoal;
	as.errFirst = 0;
	as.errSecond = 0;
	for (int abType = 0; abType < (int) markUps.size(); abType++) {
		matterAxiomSetFunc (as, abType, markUps[abType], tmpGoal, tmpFirst, tmpSecond);
		as.errFirst += tmpFirst;
		as.errSecond += tmpSecond;
	}
	// ���������� �������� ������� ������� ��� ����������� ����� ������ I � II ����
	as.goal = goalStrategy->compute(as.errFirst, as.errSecond);
	return as.goal;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������ ��� ��������� 
*					���� ���������� ��������� � ��������� �������� �������� 
*					��������� ���������� ����� ����
*	Parameters:		as - ������� ������, ��� ������� ��������� �������� ������� �������
*					abType - ��� ���������� ���������, ��� �������� ��������� �������� ������� �������
*					genMarkUp - ������� �������� ��������� ���������� ��������� ���� ���������� ���������
*					goalVal - ����������� �������� ������� �������
*					errFirstVal - ����������� �������� ����� ������ I ����
*					errSecondVal - ����������� �������� ����� ������ II ����
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc (AxiomExprSetPlus &as, int abType, std::vector <int> &genMarkUp, double &goalVal, int &errFirstVal, int &errSecondVal) const {
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
	// �������� ���������� � ����� ���������� ����������� �������
	fuzzyDataSet.getTestSize (numOfClasses, numOfMultiTS, numOfTS);
	if ((abType < 0) || (abType >= numOfClasses))
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc: incorrect input number of abnormal type.");
	// �������������� ������ - ����� ��������� ����� ������������� �������� ������
	std::vector <bool> dims;
	dims.resize (fuzzyDataSet.paramNamesSize(), false);
	for (unsigned int axNum = 0; axNum < as.axioms.size(); axNum++) {
		for (unsigned int ecNumI = 0; ecNumI < as.axioms[axNum]->expression.size(); ecNumI++) {
			for (unsigned int ecNumII = 0; ecNumII < as.axioms[axNum]->expression[ecNumI].size(); ecNumII++) {
				if ((as.axioms[axNum]->expression[ecNumI][ecNumII].dimension < 0) || (as.axioms[axNum]->expression[ecNumI][ecNumII].dimension >= (int) dims.size()))
					throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc : incomplete definition of elementary condition, it's data set dimension is out of range.");
				dims[as.axioms[axNum]->expression[ecNumI][ecNumII].dimension] = true;
			}
		}
	}
	// �������������� ��������� ��� �������� ����� ������
	errFirstVal = 0;
	errSecondVal = 0;
	// �������� ��������� ���������� ��� ��������� ���� ���������� ��������� � ��������� ��������������
	std::vector <int> curMarkUp, result;
	int num;
	for (int t = 0; t < (int) numOfTS[abType].size(); t++) {
		// �������� ���������� ����������� ������� �������� ������ as
		createTestMarkUp (curMarkUp, as, dims, abType, t, numOfTS[abType][t]);

		// ������������� ���������� ��������� � �������� ����
		recognizer->run (curMarkUp, genMarkUp, result);
		
		// ���������� ����� ������ ������� � ������� ����
		num = getStatistic (result);
		
		// ������������ ����� ������
		if (num == 0)
			errSecondVal++;
		else
			errFirstVal += num - 1;
	}
	// ��������� �� ���� ����� ����������� ���������
	int numNormalMultiTS;
	std::vector <int> numNormalTS;
	fuzzyDataSet.getNormalTestSize (numNormalMultiTS, numNormalTS);
	for (int t = 0; t < (int) numNormalTS.size(); t++) {
		// ��������� ���������� ����������� ���������
		createTestMarkUp (curMarkUp, as, dims, t, numNormalTS[t]);

		// ������������� ���������� ��������� � �������� ����
		recognizer->run (curMarkUp, genMarkUp, result);
		
		// ���������� ����� ������ ������� � ������� ����
		num = getStatistic (result);
		
		// ������������ ����� ������
		errFirstVal += num;
	}

	// ���������� �������� ������� ������� ��� ����������� ����� ������ I � II ����
	goalVal = goalStrategy->compute(errFirstVal, errSecondVal);

	return goalVal;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc
*
*	Description:	������� ������� ������� ��� ������� ������ ��������� � �����
*	Parameters:		baseDir - �������� ��������, ��� ����� ��������� �������� ������� ������
*					fileName - ���� � ��������� ������� ������, ��� ������� ��������� �������� ������� �������
*					errFirst - ����� ������ I ����
*					errSecond - ����� ������ II ����
*	Returns:		double - �������� ������� �������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyMultiDataLearnAlgorithm::matterAxiomSetFunc (const std::string baseDir, const std::string fileName, int &errFirst, int &errSecond) const {
	// ������������� ������� ������ �� ���������� �����
	AxiomExprSetPlus aesp;
	AxiomSetBase axiomSetBase;
	AxiomExprSetStructure aess;
	std::string axiomSetPath = baseDir;
	axiomSetPath.append("/");
	axiomSetPath.append(fileName);
	// ��������� ���������� � ������� ������ � aess
	std::vector <std::string> dataSetParams;
	dataSetParams = fuzzyDataSet.getParamNames();
	axiomSetBase.readFromAS (axiomSetPath, aess, dataSetParams);
	aesp.initAxiomSetFromStructure (aess);
	// ���������� �������� ������� �������
	matterAxiomSetFunc (aesp);
	// ���������� ����������� � ����� �� �������
	errFirst = aesp.errFirst;
	errSecond = aesp.errSecond;
	double toReturn = aesp.goal;
	aesp.clear();
	return toReturn;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::getStatistic
*
*	Description:	������� ������� ����� ��������� ��������� ��������
*	Parameters:		row - ���, � ������� ������ ��������� ������
*	Returns:		int - ����� ��������� ��������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::getStatistic (std::vector <int> &row) const {
	int num = 0;
	int i = 0;
	int j;
	int len = (int) row.size();
	while (i < len) {
		if (row[i] > 0) {
			num++;
			j = i+1;
			while (j < len) {
				if (row[j]!=row[j-1])
					break;
				j++;
			}
			if ((j - i) > ccNumPoints)
				num += j - i - ccNumPoints;
			i = j;
		} else {
			i++;
		}
	}
	return num;
}

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::chooseBestMarkUp
*
*	Description:	������� �������� �� ���� ��������� ��� �������� ������
*	Parameters:		as - ������� ������, ��� ������� ���������� �������� ���������� ���������� ���� 
*					abType - ��� ���������� ���������, ��� �������� ���������� �������� 
*					markUp - ��������� ������ �������, �.�. ��������� ������� �������� ��������� ���������� ������� ����
*					genMarkUps - �������������� ����� ��������� � �������� ���������� ���������� ���������
*					errFirstVal - ����������� ����� ������ I ���� ��� ������ �� �������� ������� ������� ��������� ��������
*					errSecondVal - ����������� ����� ������ II ���� ��� ������ �� �������� ������� ������� ��������� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::chooseBestMarkUp (AxiomExprSetPlus &as, int abType, std::vector <int> &markUp, std::vector <std::vector <int> > &genMarkUps, int &errFirstVal, int &errSecondVal) const {
	// ��������� ������ ������� ���������� � �������� ��������� ���������� ��������� ����
	if (genMarkUps.size() < 1) { 
		markUp.clear();
		errFirstVal = 0;
		errSecondVal = 0;
		return 1;
	}
	if (genMarkUps.size() == 1) {
		markUp = genMarkUps[0];
		double tmpGoalVal;
		matterAxiomSetFunc (as, abType, genMarkUps[0], tmpGoalVal, errFirstVal, errSecondVal);
		return 0;
	}
	// �������������� ����������� ���������
	double goalVal, goalTmp;
	int errFirstTmp, errSecondTmp;
	int curBestIndex = 0;
	// ��������� ����� ������ I � II ���� ��� ��������� �������� �������� ��������� ���������� ��������� ���� � �������� ������ �������� ������� �������
	matterAxiomSetFunc (as, abType, genMarkUps[0], goalVal, errFirstVal, errSecondVal);
	for (unsigned int i = 1; i < genMarkUps.size(); i++) {
		matterAxiomSetFunc (as, abType, genMarkUps[i], goalTmp, errFirstTmp, errSecondTmp);
		if (goalTmp < goalVal) {
			goalVal = goalTmp;
			errFirstVal = errFirstTmp;
			errSecondVal = errSecondTmp;
			curBestIndex = i;
		}
	}
	// ��������� �������������� ������ ��������
	markUp = genMarkUps[curBestIndex];
	return 0;
}

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::createMarkUpVariants
*
*	Description:	������� ������� ������� �������� ���������� ���������� ���������
*					�� ��������� ����������, ���������� ������� ���������� ���������.
*	Parameters:		genMarkUps - ����������� ������ ��������� ��������� ��������� �������� 
*						��� ���������� ���� ���������� ���������
*					resMarkUps - �������� �������� ����������, ���������� ������� 
*						���������� ���������.
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::createMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, std::vector < std::vector <int> > &resMarkUps) const {
	// ���� ����� ���������� ������ 2, �� ���������� ������ - ������� �� �������
	if (resMarkUps.size() < 2) {
		genMarkUps = resMarkUps;
		return 0;
	}
	// ���� ����� ���������� ���� �� 2, �� �������� �� ��� ���������� ����� ���������������������
	std::vector < std::vector <int> > square;
	getSquare (resMarkUps[0], resMarkUps[1], square);
	getSubsequences (resMarkUps[0], resMarkUps[1], resMarkUps[0].size() - 1, resMarkUps[1].size() - 1, square, genMarkUps);
	// ��� ���� ��������� ���������� - ����� �������� ����� ��������������������� � ��� ���������� �������������������
	std::vector <bool> toSave;
	std::vector <std::vector <int> > curRes, toAdd;
	unsigned int cRes;
	for (unsigned int i = 2; i < resMarkUps.size(); i++) {
		toAdd.clear();
		toSave.resize (genMarkUps.size(), true);
		for (unsigned int j = 0; j < genMarkUps.size(); j++) {
			// ������� ������������ ���������
			curRes.clear();
			square.clear();
			// ��������� ���������� ����� ��������
			getSquare (genMarkUps[j], resMarkUps[i], square);
			getSubsequences (genMarkUps[j], resMarkUps[i], genMarkUps[j].size() - 1, resMarkUps[i].size() - 1, square, curRes);
			// ����������� ���������� ��������
			cRes = curRes.size();
			if (cRes == 1) {
				// ������ ������ �������� ������ ������ ����� ���������� 
				genMarkUps[j].clear();
				genMarkUps[j] = curRes[0];
			} else {
				// ���� ����� ���������� �������� �� 1, �� ��������� �� � ���������� ���������� toAdd
				toSave[j] = false;
				toAdd.insert (toAdd.begin(), curRes.begin(), curRes.end());
			}
		}
		// ������ ���������, ���������� ����� ������ ����� ���������������������� � ��� ����������� �����������
		for (int m = (int) toSave.size() - 1; m >= 0; m--) {
			if (!toSave[m]) {
				genMarkUps.erase (genMarkUps.begin() + m);
				toSave[m] = true;
			}
		}
		genMarkUps.insert (genMarkUps.begin(), toAdd.begin(), toAdd.end());
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::createSimpleMarkUpVariants
*
*	Description:	������� ������� ����� ������� ������� �������� ���������� ���������� ���������
*					����� �������� ��, �������� �� ����� ������.
*	Parameters:		genMarkUps - ����������� ������ ��������� ��������� ��������� �������� 
*						��� ���������� ���� ���������� ���������
*					numOfAxioms - ����� ������ � ������� ������, ��� ������� ��������� �������� ��������
*	Returns:		int - ����� ��������� ��������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::createSimpleMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, const int numOfAxioms) const {
	genMarkUps.resize(numOfAxioms);
	for (int i = 0; i < numOfAxioms; i++) {
		genMarkUps[i].resize(1);
		genMarkUps[i][0] = i+1;
	}
	return numOfAxioms;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::createRefMarkUp
*
*	Description:	�������� ������������ ���� multiTSNumber �������� 
*					������ as � ������� ���������� � markUp.
*	Parameters:		resMarkUp - �������� ����������
*					as - ������� ������, ������� ���������� ��������
*					abType - ����� ������ ���������� ���������
*					multiTSNumber - ����� ������������ ���� ��� ������� 
*					������ ���������� ���������, �������� �������� ����������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::createRefMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, const int abType, const int multiTSNumber, std::vector < std::vector <int> > &numOfTS) const {
	// ����� ������� ��������� �� ����������� ������� ������ � ���������� ������ ����������� ����������
	std::vector <bool> dims;
	dims.resize (fuzzyDataSet.paramNamesSize(), false);
	for (unsigned int axNum = 0; axNum < as.axioms.size(); axNum++) {
		for (unsigned int ecNumI = 0; ecNumI < as.axioms[axNum]->expression.size(); ecNumI++) {
			for (unsigned int ecNumII = 0; ecNumII < as.axioms[axNum]->expression[ecNumI].size(); ecNumII++) {
				if ((as.axioms[axNum]->expression[ecNumI][ecNumII].dimension < 0) || (as.axioms[axNum]->expression[ecNumI][ecNumII].dimension >= (int) dims.size()))
					throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::createRefMarkUp : incomplete definition of elementary condition, it's data set dimension is out of range.");
				dims[as.axioms[axNum]->expression[ecNumI][ecNumII].dimension] = true;
			}
		}
	}
	// �������������� ����������� ����������
	std::vector < std::vector<double> > multiTS;
	multiTS.resize (fuzzyDataSet.paramNamesSize());
	// ����������� ���������� �� ��������� ������� ��� ������� ������ ���������� ��������� (����������� ������ ������ �����������)
	int rowLen = 0;
	for (int t = 0; t < numOfTS[abType][multiTSNumber]; t++) {
		if (dims[t]) {
			if (!fuzzyDataSet.getTSByIndexFromClass (multiTS[t], abType, multiTSNumber, t))
				throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::createRefMarkUp : unable to get concrete time series from dataSet.");
			rowLen = multiTS[t].size();
		} else {
			multiTS[t].clear();
		}
	}
	// �������� ������ ����������� ����, ������ ���������� ��� ��������
	as.enter (markUp, multiTS, 0, rowLen);
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::createTestMarkUp
*
*	Description:	�������� ������������ ���� �� ����������� ������� �������� 
*					������ as � ������� ���������� � markUp.
*	Parameters:		markUp - �������� ����������
*					as - ������� ������, ������� ���������� ��������
*					dims - ������ � ������� ������: ����� ����������� ����� �������������
*					abType - ��� ���������� ��������� ��� �������� ���������� ��� ��� �������
*					indexTest - ����� ���� ��� ���������� ������ ���������� ���������
*					numOfTestTS - ����� ������� ������� ������������ ����
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &abType, int &indexTest, int &numOfTestTS) const {
	// �������������� ����������� ����������
	std::vector < std::vector<double> > multiTS;
	multiTS.resize (fuzzyDataSet.paramNamesSize());
	// ����������� ���������� �� ��������� ������� ��� ������� ������ ���������� ��������� (����������� ������ ������ �����������)
	int rowLen = 0;
	for (int t = 0; t < numOfTestTS; t++) {
		if (dims[t]) {
			if (!fuzzyDataSet.getTSByIndexFromTest (multiTS[t], abType, indexTest, t))
				throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::createTestMarkUp : unable to get concrete time series from dataSet.");
			rowLen = multiTS[t].size();
		} else {
			multiTS[t].clear();
		}
	}
	// �������� ������ ����������� ����, ������ ���������� ��� ��������
	return as.enter (markUp, multiTS, 0, rowLen);
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::createTestMarkUp
*
*	Description:	�������� ������������ ���� ����������� ��������� �� ����������� 
*					������� �������� ������ as � ������� ���������� � markUp.
*	Parameters:		markUp - �������� ����������
*					as - ������� ������, ������� ���������� ��������
*					dims - ������ � ������� ������: ����� ����������� ����� �������������
*					indexTest - ����� ���� ��� ���������� ������ ���������� ���������
*					numOfTestTS - ����� ������� ������� ������������ ����
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &indexTest, int &numOfTestTS) const {
	// �������������� ����������� ����������
	std::vector < std::vector<double> > multiTS;
	multiTS.resize (fuzzyDataSet.paramNamesSize());
	// ����������� ���������� �� ��������� ������� ��� ������� ������ ���������� ��������� (����������� ������ ������ �����������)
	int rowLen = 0;
	for (int t = 0; t < numOfTestTS; t++) {
		if (dims[t]) {
			if (!fuzzyDataSet.getNormalTSFromTest (multiTS[t], indexTest, t))
				throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::createTestMarkUp : unable to get concrete time series from dataSet.");
			rowLen = multiTS[t].size();
		} else {
			multiTS[t].clear();
		}
	}
	// �������� ������ ����������� ����, ������ ���������� ��� ��������
	return as.enter (markUp, multiTS, 0, rowLen);
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::simplifyMarkUps
*
*	Description:	��������� �������� ����� �������� 0-�� � ������ � � �����
*					������� ��������
*	Parameters:		genMarkUp� - ������������� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyMultiDataLearnAlgorithm::simplifyMarkUps (std::vector <std::vector <int> > &markUps) const {
	int k, l; 
	for (int i = (int) markUps.size() - 1; i >= 0; i--) {
		// ������� ��� 0 � ������ ������� - �� �� ���������
		k = 0;
		while (k < (int) markUps[i].size()) {
			if (markUps[i][k] > 0)
				break;
			k++;
		}
		// �� ��������� ��� 0 � ����� ������� ��������
		l = (int) markUps[i].size() - 1;
		while (l >= k) {
			if (markUps[i][l] > 0)
				break;
			l--;
		}
		if (l < k) {
			// ���� ���� ������ 0 - �� ������� �������� �� ������
			markUps.erase(markUps.begin() + i);
		} else {
			// ������� 0 � ������ � � ����� ������� ��������
			markUps[i].erase(markUps[i].begin() + l + 1, markUps[i].end());
			markUps[i].erase(markUps[i].begin(), markUps[i].begin() + k);
		}
	}	
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::simplifyMarkUp
*
*	Description:	��������� �������� � ���������� �� � ����������� ��������
*	Parameters:		markUp - ����������� � ���������� ������� ��������
*					markUpLen - ������ ���� ��������� ��������� �������� �� simp � 
*						�������� �������� row
*					genMarkUp - ������������� ��������
*					genMarkUpSens - ������� ������������� ��������� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
/*int FuzzyMultiDataLearnAlgorithm::simplifyMarkUp (std::vector <int> &markUp, std::vector <int> &markUpLen, std::vector <int> &genMarkUp, std::vector <int> &genMarkUpSens) const {
	markUp.clear();
	markUpLen.clear();
	// ������� ��� 0 � ������ ������� - �� �� ���������
	unsigned int i = 0;
	while (i < genMarkUp.size()) {
		if (genMarkUp[i] > 0)
			break;
		i++;
	}
	// �� ��������� ��� 0 � ������ �������
	unsigned int upTo = genMarkUp.size() - 1;
	while (upTo >= i) {
		if (genMarkUp[upTo] > 0)
			break;
		upTo--;
	}
	upTo++;
	// ���� ���� ������ 0 - �� ������� �� �������
	if (((i+1) == upTo) && (genMarkUp[i] == 0))
		return -1;
	// ��������� �� ���� �������������� ��������� � ���������� ������������ �������� ������� + ����� ������� ������������� �������� ��������
	int curMaxVal = genMarkUpSens[i];
	int curLen = 1;
	double cutCriteriaMax = countCutCriteria (curLen, curMaxVal);
	for (unsigned int k = i+1; k < upTo; k++) {
		if (genMarkUp[k] == genMarkUp[k-1]) {
			curLen++;
			if (genMarkUpSens[k] > curMaxVal)
				curMaxVal = genMarkUpSens[k];
		} else {
			if (countCutCriteria (curLen, curMaxVal) > cutCriteriaMax)
				cutCriteriaMax = countCutCriteria (curLen, curMaxVal);
			curMaxVal = genMarkUpSens[k];
			curLen = 1;
		}
	}
	// ������������� �������� �������� ��������� - �� ������������� �������� ��������
	double cutCriteria = cutCriteriaMax * double_threshold_func;
	// ���� �� ���� �������������� ��������� ������ row
	curMaxVal = genMarkUpSens[i];
	curLen = 1;
	for (unsigned int k = i+1; k < upTo; k++) {
		if (genMarkUp[k] == genMarkUp[k-1]) {
			curLen++;
			if (genMarkUpSens[k] > curMaxVal)
				curMaxVal = genMarkUpSens[k];
		} else {
			if (countCutCriteria (curLen, curMaxVal) > cutCriteria) {
				markUp.push_back (genMarkUp[k-1]);
				markUpLen.push_back (curLen);
			}
			curMaxVal = genMarkUpSens[k];
			curLen = 1;
		}
	}
	if (countCutCriteria (curLen, curMaxVal) > cutCriteria) {
		markUp.push_back (genMarkUp[upTo-1]);
		markUpLen.push_back (curLen);
	}
	return 0;
}


// ������� ������ ������� ���������� ��� �� ������� ��������� �������� (�� ������� �������� ��������)
int FuzzyMultiDataLearnAlgorithm::simplifyMarkUp2 (std::vector <int> &markUp, std::vector <int> &markUpLen, std::vector <int> &genMarkUp, std::vector <int> &genMarkUpSens) const {
	markUp.clear();
	markUpLen.clear();
	// ������� ��� 0 � ������ ������� - �� �� ���������
	unsigned int i = 0;
	while (i < genMarkUp.size()) {
		if (genMarkUp[i] > 0)
			break;
		i++;
	}
	// �� ��������� ��� 0 � ������ �������
	unsigned int upTo = genMarkUp.size() - 1;
	while (upTo >= i) {
		if (genMarkUp[upTo] > 0)
			break;
		upTo--;
	}
	upTo++;
	// ���� ���� ������ 0 - �� ������� �� �������
	if (((i+1) == upTo) && (genMarkUp[i] == 0))
		return -1;

	// ��������� �� ���� �������������� ��������� � ���������� ������������ �������� ������� + ����� ������� ������������� �������� ��������
	std::vector <int> entrance, repeat, maxSens;
	entrance.resize(2,0);
	repeat.resize (2,0);
	maxSens.resize (2,0);
	for (unsigned int k = i; k < upTo; k++) {
		if (genMarkUp[k] >= (int) entrance.size()) {
			entrance.resize (genMarkUp[k] + 1, 0);
			repeat.resize (genMarkUp[k] + 1, 0);
			maxSens.resize (genMarkUp[k] + 1, 0);
		}
		entrance[genMarkUp[k]]++;
		repeat[genMarkUp[k]] += genMarkUpSens[k];
		// �� ���� ���� ��������� ������������ �������� ��� sens
		if (genMarkUpSens[k] > maxSens[genMarkUp[k]])
			maxSens[genMarkUp[k]] = genMarkUpSens[k];
	}
	// ������� ������� �������� ��� ��������� ���������� ����� ���������� � ���������� ����� ���������� � ���������� ��������
	std::vector <double> average;
	average.resize (entrance.size(), 0);
	for (unsigned int i = 0; i < entrance.size(); i++) {
		if (entrance[i] > 0)
			average[i] = ((double) repeat[i]) / ((double) entrance[i]);
	}
	// average - ����� ��������� ���� �������� ��������� - ������� ��������� ������� ������� - � �������� �������� �� ��������� �����������
	for (unsigned int i = 0; i < average.size(); i++) {
		if (average[i] > 0)
			average[i] += ((double) maxSens[i] - average[i]) * double_simplify_coeff;
	}
	
	// ���� �� ���� �������������� ��������� ������ genMarkUp
	std::vector <int> tmpMarkUp; 
	tmpMarkUp.resize (genMarkUp.size());
	int cur = 0;
	for (unsigned int k = i; k < upTo; k++) {
		if ((double) genMarkUpSens[k] >= average[genMarkUp[k]]) {
			tmpMarkUp[cur] = genMarkUp[k];
			cur++;
		}
	}
	// ��������� �������� �� tmpMarkUp � markUp
	if (cur > 0) {
		markUp.resize (1, tmpMarkUp[0]);
		markUpLen.resize (1, 1);
		int loc = 0;
		for (int j = 1; j < cur; j++) {
			if (tmpMarkUp[j] == markUp[loc]) {
				markUpLen[loc]++;
			} else {
				markUp.push_back (tmpMarkUp[j]);
				markUpLen.push_back (1);
				loc++;
			}
		}
	}
	return 0;
}*/


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::countCutCriteria
*
*	Description:	������� ��������� ���������� ����� ���� ���������. 
*					������������ ��� ���������� �������� ������� ��������� 
*					� ������� ���������� �������� ��� �������� ������� ������
*					�� ������� �������� �������.
*	Parameters:		len - ������ ���������� (����� ������� ��������)
*					val - ������������ ������� ������������� ������� � ��������
*	Returns:		double - ���������� ����� ���� ���������. ������������ ���������� �������� ��� ������� define'��.
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
/*inline double FuzzyMultiDataLearnAlgorithm::countCutCriteria (int &len, int &val) const {
	return ((double) len * double_weight_markUps_length + (double) val * double_weight_markUps_sens);
}*/


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::extendMarkUpLen
*
*	Description:	����������� �������� ������� � �������� �� ������������ � 
*					������������� �������� � �������� �����������
*	Parameters:		stdLen - ������������� ��������
*					minLen - ����������� ������ ����������� ��������
*					maxLen - ����������� ������ ������������ ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
/*inline int FuzzyMultiDataLearnAlgorithm::extendMarkUpLen (std::vector <int> &row, std::vector <int> &minLen, std::vector <int> &maxLen, std::vector <int> &stdLen) const {
	// ��������� ������� ������
	if (row.size() != stdLen.size())
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::extendMarkUpLen : incorrect input parameters.");
	if (row.size() < 1)
		return 0;
	// ��������� ����������� � ������������ ����� ��������� �������
	std::vector <int> lenghtMin, lenghtMax;
	lenghtMin.resize (stdLen.size());
	lenghtMax.resize (stdLen.size());
	int maxVal = 0;
	for (unsigned int i = 0; i < stdLen.size(); i++) {
		lenghtMin[i] = max (0, round (((double) stdLen[i]) * (1.0 - extendMarkUpLensLevel)));
		lenghtMax[i] = round (((double) stdLen[i]) * (1.0 + extendMarkUpLensLevel));
		// �� ���� ������� ������������ �������� ��� �������� ����� ����������
		if (lenghtMax[i] > maxVal)
			maxVal = lenghtMax[i];
	}
	// ��������� �������� ����� ���������� ��� 0
	int zLen = round ((double) maxVal * 0.5);
	if (zLen < 1) 
		zLen = 1;
	// �������� ������ ��� ��� ����������
	std::vector <int> preRow;
	preRow = row;
	// �������� ������ ��������
	row.clear();
	row.push_back(preRow[0]);
	minLen.clear();
	minLen.push_back(lenghtMin[0]);
	maxLen.clear();
	maxLen.push_back(lenghtMax[0]);
	int cur = 0;
	// ��������� � ��������� ������� �������� � ���������� �� 0-��
	for (unsigned int i = 1; i < preRow.size (); i++) {
		if ((row[cur] != 0) && (preRow[i] != 0)) {
			row.push_back (0);
			minLen.push_back (0);
			maxLen.push_back (zLen);
			cur++;
		}
		row.push_back (preRow[i]);
		minLen.push_back (lenghtMin[i]);
		maxLen.push_back (lenghtMax[i]);
		cur++;
	}
	return 0;
}*/


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::getSubsequences
*
*	Description:	������� ������� ��� ������������ ����� ��������������������� ���� �����. 
*					(����� ���� ��������� ��������� ���������������������� ������������ �����)
*	Parameters:		str1 - ������ ������, ��� ������� ������ ���������������������
*					str2 - ������ ������, ��� ������� ������ ���������������������
*					i - ������ �������� � ������ ������, �� ������� ����������� ����� ����������������������
*					j - ������ �������� �� ������ ������, �� ������� ����������� ����� ����������������������
*					square - ������� ������������ ��������� ����� str1 � str2
*					res - ����������� ������ ���������� ����� ����������������������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::getSubsequences (const std::vector <int> &str1, const std::vector <int> &str2, int i, int j, const std::vector < std::vector <int> > &square, std::vector < std::vector <int> > &res) const {
	if (square[i][j] == 0) {
		return 0;
	}

	if (i == 0) {
		res.resize(1);
		res[0].push_back (str1[0]);
		return 0;
	}

	if (j == 0) {
		res.resize(1);
		res[0].push_back (str2[0]);
		return 0;
	}

	// ���� �� ���������
	if ((square[i][j] > square[i-1][j]) && (square[i][j] > square[i][j-1])) {
		if (square[i][j] == 1) {
			res.resize(1);
			res[0].push_back (str1[i]);
		} else {
			getSubsequences (str1, str2, i-1, j-1, square, res);
			for (unsigned int k = 0; k < res.size(); k++)
				res[k].push_back (str1[i]);
		}
		return 0;
	} else {
		// ���� �� ���������
		if (square[i][j] == square[i-1][j-1]) {
			getSubsequences (str1, str2, i-1, j-1, square, res);
		}
		std::vector < std::vector <int> > resNew;
		// ���� �����
		int i2 = i;
		while (i2 > 0) {
			if (square[i2][j] > square[i2-1][j])
				break;
			i2--;
		}
		if (square[i2][j] > square[i2][j-1]) {
			if (i2 > 0) {
				if (square[i2][j] == 1) {
					resNew.resize(1);
					resNew[0].push_back (str1[i2]);
				} else {
					getSubsequences (str1, str2, i2-1, j-1, square, resNew);
					for (unsigned int k = 0; k < resNew.size(); k++)
						resNew[k].push_back (str1[i2]);
				}
				for (unsigned int k = 0; k < resNew.size(); k++)
					res.push_back (resNew[k]);
				resNew.clear();
			} else {
				res.resize(res.size() + 1);
				res[res.size() - 1].push_back (str1[0]);
			}
		}
		// ���� �����
		while (j > 0) {
			if (square[i][j] > square[i][j-1])
				break;
			j--;
		}
		if (square[i][j] > square[i-1][j]) {
			if (j > 0) {
				if (square[i][j] == 1) {
					resNew.resize(1);
					resNew[0].push_back (str1[i]);
				} else {
					getSubsequences (str1, str2, i-1, j-1, square, resNew);
					for (unsigned int k = 0; k < resNew.size(); k++)
						resNew[k].push_back (str1[i]);
				}
				for (unsigned int k = 0; k < resNew.size(); k++)
					res.push_back (resNew[k]);
				resNew.clear();
			} else {
				res.resize(res.size() + 1);
				res[res.size() - 1].push_back (str2[0]);
			}
		}
		// ������� �� �������
		return 0;
	}
	// �� ���� - ���� ��� ����� �� �����
	return -1;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::getSquare
*
*	Description:	������� ��������� ������� ������������ ��������� ���� �����.
*					��� ������� ����� �������������� ��� ���������� ���������� 
*					����� ��������������������� ���� �����.
*	Parameters:		str1 - ������ ������, ��� ������� �������� �������
*					str2 - ������ ������, ��� ������� �������� �������
*					square - ������� ������������ ��� ����� str1 � str2
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyMultiDataLearnAlgorithm::getSquare (const std::vector <int> &str1, const std::vector <int> &str2, std::vector < std::vector <int> > &square) const {
	int s1 = (int) str1.size();
	int s2 = (int) str2.size();
	// �������� ������� ������
	if ((s1 < 1) || (s2 < 1)) {
		square.clear ();
		return 0;
	}
	
	int preVal = 0;
	// ��������� ������ ������� �������
	square.resize (s1);
	square[0].resize(s2, 0);
	if (str1[0] == str2[0]) {
		preVal = 1;
		square[0][0] = 1;
	}
	// ��������� ����� �������
	int i, j;
	for (i = 1; i < s1; i++) {
		square[i].resize(s2, 0);
		if (preVal > 0)
			square[i][0] = 1;
		else 
			if (str1[i] == str2[0]) {
				preVal = 1;
				square[i][0] = 1;
			}
	}
	// ��������� ������� ������ �������
	preVal = square[0][0];
	for (j = 1; j < s2; j++) {
		if (preVal > 0)
			square[0][j] = 1;
		else 
			if (str1[0] == str2[j]) {
				preVal = 1;
				square[0][j] = 1;
			}
	}

	// ��������� ��� �������
	for (i = 1; i < s1; i++) {
		for (j = 1; j < s2; j++) {
			if ((square[i-1][j-1] >= square[i][j-1]) && (square[i-1][j-1] >= square[i-1][j])) {
				square[i][j] = square[i-1][j-1];
				if (str1[i] == str2[j])
					square[i][j]++;
			} else 
				if (square[i-1][j] >= square[i][j-1])
					square[i][j] = square[i-1][j];
				else
					square[i][j] = square[i][j-1];		
		}
	}

	return 0;
}
