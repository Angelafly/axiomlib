#include "ASStageSimple.h"

#include <algorithm>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <omp.h>

#include "../ReducedRecognizerFactory.h"
#include "../GoalStrategyFactory.h"
#include "../Logger.h"

#include "AXStage.h"

#include "ASStage.h"

// �������� ���������� ������������ ������ ������� � ������� ��������� �������, ������� ������� ��� ���� ���������
//  (������ ���� ���������� �������, ��� ��� �� ����������� ��� ����� ����� ���������� ������)
#define int_max_size_of_vector_axiomExprSet		100

// ������������ ����� ������ ������, ������� ����� ���� ������� �� ������ ���� ��������� ��� ����� �������� ������� ������
#define int_max_number_of_renew_axiom_set		10

// ����� �������� ����� �������� � ��������������� �������
#define eps			0.0000000001

using namespace AxiomLib;
using namespace AxiomLib::FuzzyMultiDataExt;

ASStageSimple::ASStageSimple(FuzzyDataSet *fuzzyDataSet, AXStage *stage2) 
    : fuzzyDataSet(fuzzyDataSet), stage2(stage2), logger(Logger::getInstance())
{
	//TODO default values
}

void ASStageSimple::setAxiomSets(const std::vector<AxiomExprSetPlus> &initialAS) {
	bestAxiomSets = initialAS;
}

void ASStageSimple::initFromEnv(const Environment &env) {
	if (env.getIntParamValue (ccNumPoints, "ccNumPoints") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : ccNumPoints is undefined.");
	
	env.getParamValue(singleAxiomMarkupPenalty, "singleAxiomMarkupPenalty", 0.0);

	// �������� ������ ������������� �������� �������� � ���� ��������
	std::string recogClassName;
	ReducedRecognizerFactory rrf;
	if (env.getStringParamValue(recogClassName, "ReducedRecognizer") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::initFromEnv: ReducedRecognizer class is undefined.");
	recognizer = PReducedRecognizer(rrf.create(recogClassName));
	recognizer->setParamsFromEnv (env);

	// �������������� ��������� ���������� ������� �������
	std::string goalStrategyName;
	GoalStrategyFactory gsf;
	if (env.getStringParamValue(goalStrategyName, "goalClass") < 0) 
		throw AxiomLibException("FuzzyMultiDataExt_AS::initFromEnv: goal-class is undefined.");
	goalStrategy = PGoalStrategy(gsf.create(goalStrategyName));
	goalStrategy->setParamsFromEnv(env);

	// ��������� ������, ���� ��� ������ �������� �������� ������� �������� ��� ������������ � ������ ��������
	if (env.getDoubleParamValue (shiftErrIn, "shiftErrIn") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : shiftErrIn is undefined.");
	// ������� ���������� ����� ��������� �������� (��� ������ �������� �� ����� �� �������)
	if (env.getDoubleParamValue (extendMarkUpLensLevel, "extendMarkUpLensLevel") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : extendMarkUpLensLevel is undefined.");
	if (env.getIntParamValue (maxAxiomSetPopSize, "maxAxiomSetPopSize") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : maxAxiomSetPopSize is undefined.");
	if (env.getIntParamValue (maxNumberOfSteps, "maxNumberOfSteps") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : maxNumberOfSteps is undefined.");
	if (env.getDoubleParamValue (bestAxiomSetGoal, "bestAxiomSetGoal") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : bestAxiomSetGoal is undefined.");
	if (env.getIntParamValue (numberOfBestAxiomSets, "numberOfBestAxiomSets") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : numberOfBestAxiomSets is undefined.");
	if (env.getDoubleParamValue (percentBestAxiomSets, "percentBestAxiomSets") < 0)
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : percentBestAxiomSets is undefined.");
	if ((percentBestAxiomSets <= 0.0) || (percentBestAxiomSets > 1.0))
		throw AxiomLibException("FuzzyMultiDataExt_AS::setParamsFromEnv : percentBestAxiomSets is out of range.");	
}

int ASStageSimple::getASSize() const {
	return bestAxiomSets.size();
}

AxiomExprSetPlus &ASStageSimple::getAS(int n) {
	return bestAxiomSets[n];
}

const AxiomExprSetPlus &ASStageSimple::getAS(int n) const {
	return bestAxiomSets[n];
}

void ASStageSimple::recalculateMatterASFunc(AxiomExprSetPlus &as) const {
	this->setSatPointsSize(as.axioms.size());
	
	for(int i = 0; i < as.axioms.size(); i++) {
		this->addToSatPoints(*as.axioms[i]);
	}
	
	if(as.markUps.size() == 0) {
		// ���� �������� ��� -- ���� �� ���������
		matterAxiomSetFunc(as);
	} else {
		// ����� -- ������������� ��� ��������� ��������
		matterAxiomSetFunc(as, as.markUps);
	}
}

void ASStageSimple::run() {
	if(stage2 == 0) {
		throw AxiomLibException("ASStageSimple::run(): axiom stage not set, cannot run training");
	}

	std::vector<int> sizeVector;
	stage2->getAXSize(sizeVector);
	
	//  ������� ������ ���� ������ - ����� ��� ����� ���� ������������ ��� ������������ ������ ������
	int numOfAxioms = 0;
	for (unsigned int i = 0; i < sizeVector.size(); i++) {
		for(int j = 0; j < sizeVector[i]; j++) {
			if(stage2->isAXSelected(i, j)) {
				++numOfAxioms;
			}
		}
	}
	
	// ������ ������, ����������� �� ���� ������������ ������ �� ������������ �������
	std::vector <AxiomExpr> bestAxioms;
	bestAxioms.reserve(numOfAxioms);
	setSatPointsSize(numOfAxioms);
	
	for (unsigned int i = 0; i < sizeVector.size(); i++) {
		for(int j = 0; j < sizeVector[i]; j++) {
			if(stage2->isAXSelected(i, j)) {
				AxiomExpr ax = stage2->getAX(i, j);
				addToSatPoints(ax);
				bestAxioms.push_back(ax);
			}
		}
	}
	
	// ������� ���������� ���������� ��� �������� ������ ������ ������ ������
	bestAxiomSets.clear();
	{ // �������� ���������� ����� �������, ����� �� ����������� ���������� � ��������� ���������
		// �������� ��������� ��������� - ������� ������� ������ ������ ��� ���������� ��������� ������
		// ������� ����� ������ ������, ������ �� ������� ����� ��������� ������ ���� �������
		std::vector <AxiomExprSetPlus> axiomSets, newAxiomSets;
		std::vector < std::vector <AxiomExprSetPlus> > nextStepAxiomSets;
		axiomSets.resize(numOfAxioms);
		int as = 0;
		#pragma omp parallel for schedule(dynamic, 1)
		for (int i = 0; i < numOfAxioms; i++) {
			// ��������� ������� ������ �� ����� �������
			axiomSets[i].addAxiom (bestAxioms[i]);
			// ������������� ����������
			axiomSets[i].axiomsIndex.push_back(i);
			// ��������� �������� ������� ������� ��� ����� ������� ������
			matterAxiomSetFunc (axiomSets[i]);
			// ����� ������� ����������, ���� ��� ����������
			{
				char buf[128];
				sprintf (buf, "\r\tCurrent axiomSet: %d out of %d. Goal value is %.2f (%d, %d).\tThread %d.\t", i + 1, numOfAxioms, axiomSets[i].goal, axiomSets[i].errFirst, axiomSets[i].errSecond, omp_get_thread_num());
				logger->writeComment(buf);

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
				{
					char buf[128];
					sprintf (buf, "\r\tProc start point: %d - %d\tIteration: %d\tCurrent axiomSet: %d out of %d.\t Thread %d.\t", 1, 1, iterNum+1, axSet+1, axiomSets.size(), omp_get_thread_num());
					logger->writeComment(buf);
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
						
						tryAddAxiom(newAxiomSets[ax], bestAxioms[ax], ax);
						
//						newAxiomSets[ax].addAxiom (bestAxioms[ax]);
//						newAxiomSets[ax].axiomsIndex.push_back(ax);
//						// ������� �������� ������� ������� ��� ���������� ������� ������ � ��������� ���
//						matterAxiomSetFunc (newAxiomSets[ax]);
					} else {
						// ���� ������� ��� ������ � ������� - �� �� ������� ����� �������. ��������� �������� ������� ������� �� ��������� ��������� ����������� ������ axiomSets[axSet].goal .
						newAxiomSets[ax].goal = defMaxGoal;
					}
				}
				// �������� �������� ����� ������ ������ ������ � ��������� �� � ��������� �� ��������� ���� ���������
				indicesOfBestSets.clear();
				chooseIndicesOfBestAxiomSets (newAxiomSets, indicesOfBestSets, axiomSets[axSet].goal);
				const AxiomExprSetPlus& bestAS = newAxiomSets[indicesOfBestSets[0]];
				{
					char buf[256];
					sprintf (buf, "\r\tProc start point: %d - %d\tIteration: %d\tCurrent axiomSet: %d out of %d.\t Thread %d.\n\t Best objective value: %d (%d, %d)", 1, 1, iterNum+1, axSet+1, axiomSets.size(), omp_get_thread_num(),
							 int(bestAS.goal), bestAS.errFirst, bestAS.errSecond);
					logger->writeComment(buf);
				}

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
				logger->writeComment("Stopping because no new axiom sets created");
				condition = false; 
				break;
			}
			// - �������, ���� ����� �������� ������ ��������� �������
			if ((iterNum >= maxNumberOfSteps) || ((curBestVal - bestAxiomSetGoal) < eps)) {
				if(iterNum >= maxNumberOfSteps) {
					logger->writeComment("Stopping because maximum number of iterations reached");
				} else {
					logger->writeComment("Stopping because best axiom set goal value reached");
				}
				// ��������� �������� ������� ������ � ����������� ������ ����������� � �� ���� ���� ������� �� ����������
				addToBestAxiomSets (axiomSets);
				condition = false;
				break;
			}
		}

		// ���������� ������ ������ ������ �� ��������� ����� ���������
		sortBestAxiomSets ();
	}

	std::sort(bestAxiomSets.begin(), bestAxiomSets.end());
}

int ASStageSimple::tryAddAxiom(AxiomExprSetPlus &as, const AxiomExpr &ax, int axIndex) const {
	std::vector<std::vector<std::vector<int> > > markUpVariants;
	as.addAxiom(ax);
	as.axiomsIndex.push_back(axIndex);
        // �������� ���� �������� ���������� ���������� ���������
	createAllMarkUpVariants(as, markUpVariants);
	
	matterAxiomSetFunc(as, markUpVariants);
	//matterAxiomSetFunc(as);
	
	return -1;
//	// ������������� ������� ������� ������ ���� ����� ������� ���������� � ���������
//	if(markupsContain(markUpVariants, as.axioms.size())) {
		
//		return -1;
//	} else {
//		logger->writeComment("Optimization for matterAxiomSetFunc recalculation worked!");
//	}
//	return 0;
}

// ������, � ��� �������� ��� ������� ������ ���������� ��������� ���������� ���


void ASStageSimple::createAllMarkUpVariants(const AxiomExprSetPlus &as, std::vector<std::vector<std::vector<int> > > &markUpVariants) const {
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
	// ��������� ���������� �� ��������� ������ ������
	fuzzyDataSet->getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	// ��� ������� ������ ���������� ��������� ��������� ����������� ���������� ��������
	std::vector < std::vector <int> > resMarkUps;
	markUpVariants.resize(numOfClasses);
	for (int abType = 0; abType < numOfClasses; abType++) {
		markUpVariants[abType].clear();
		resMarkUps.resize (numOfMultiTS[abType]);
		for (int multiTS = 0; multiTS < numOfMultiTS[abType]; multiTS++) {
			createRefMarkUp (resMarkUps[multiTS], as, abType, multiTS, numOfTS);
		}
		// �������� �������� - ������� 0 � ������ � � ����� �������� - ����� ����� ����� ���� ������ �����
		simplifyMarkUps (resMarkUps);
		// �� ��������� ���������� �������� ���������� ����������� ��������� - ���������� �� ����� � ��������� �������� ���������� ��������
		createMarkUpVariants (markUpVariants[abType], resMarkUps);
		// �������� ������� �������� - ������� 0 � ������ � � �����. ���� �������� �������� ��������� ��������� � ���� ��������
		simplifyMarkUps (markUpVariants[abType]);
		// ��������� ����������� �������� �������� - ���� ����� ����� �������� �� �������
		if (markUpVariants[abType].size() < 1) {
			//logger->writeDebug("Warning: couldn't find common subsequence, creating simple markup");
			createSimpleMarkUpVariants (markUpVariants[abType], (int) as.axioms.size());
		}
	}
}

bool ASStageSimple::markupsContain(const std::vector<std::vector<std::vector<int> > > &markUpVariants, int axiomNumber) {
	for(int i = 0; i < markUpVariants.size(); i++) {
		for(int j = 0; j < markUpVariants[i].size(); j++) {
			for(int k = 0; k < markUpVariants[i][j].size(); k++) {
				int currentAxiomNumber = markUpVariants[i][j][k];
				if(axiomNumber == currentAxiomNumber || 0 == currentAxiomNumber) {
					return true;
				}
			}
		}
	}
	return false;
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::cutDownAxiomSets
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
int ASStageSimple::cutDownAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) const {
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
inline int ASStageSimple::createSimpleMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, const int numOfAxioms) const {
	genMarkUps.resize(numOfAxioms);
	for (int i = 0; i < numOfAxioms; i++) {
		genMarkUps[i].resize(1);
		genMarkUps[i][0] = i+1;
	}
	return numOfAxioms;
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::chooseIndicesOfBestAxiomSets
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
int ASStageSimple::chooseIndicesOfBestAxiomSets (std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets, double &goal) const {
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
*					FuzzyMultiDataExt_AS::addAxiomSets
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
int ASStageSimple::addAxiomSets(std::vector <AxiomExprSetPlus> &nextStepAxiomSets, std::vector <AxiomExprSetPlus> &newAxiomSets, std::vector <int> &indicesOfBestSets) const {
	if (nextStepAxiomSets.size() > 0)
		throw AxiomLibException("Error in FuzzyMultiDataExt_AS::addAxiomSets: input arguments out of consistency.");
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
*					FuzzyMultiDataExt_AS::sortBestAxiomSets
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
int ASStageSimple::sortBestAxiomSets (void) {
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
*					FuzzyMultiDataExt_AS::addToBestAxiomSets
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
int ASStageSimple::addToBestAxiomSets (std::vector <AxiomExprSetPlus> &axiomSets) {
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
*					FuzzyMultiDataExt_AS::sortAxiomSets
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
int ASStageSimple::sortAxiomSets (const std::vector <AxiomExprSetPlus> &axiomSets, std::vector <int> &indecies) const {
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
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
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
double ASStageSimple::matterAxiomSetFunc (AxiomExprSetPlus &as) const {
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
//	std::vector <double> curTS;
//	std::vector <int> tempLen, corrMarkUp, curMarkUp, result;
	// ��������� ���������� �� ��������� ������ ������
	fuzzyDataSet->getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	// ��� ������� ������ ���������� ��������� ��������� ����������� ���������� ��������
	std::vector < std::vector <int> > resMarkUps, genMarkUps;
	int errFirstVal, errSecondVal;
	as.errFirst = 0;
	as.errSecond = 0;
	as.markUps.resize(numOfClasses);
	as.errors.resize(numOfClasses);
	for (int abType = 0; abType < numOfClasses; abType++) {
		genMarkUps.clear();
		resMarkUps.resize (numOfMultiTS[abType]);
		for (int multiTS = 0; multiTS < numOfMultiTS[abType]; multiTS++) {
			createRefMarkUp (resMarkUps[multiTS], as, abType, multiTS, numOfTS);
		}
		// �������� �������� - ������� 0 � ������ � � ����� �������� - ����� ����� ����� ���� ������ �����
		simplifyMarkUps (resMarkUps);
		// �� ��������� ���������� �������� ���������� ����������� ��������� - ���������� �� ����� � ��������� �������� ���������� ��������
		createMarkUpVariants (genMarkUps, resMarkUps);
		// �������� ������� �������� - ������� 0 � ������ � � �����. ���� �������� �������� ��������� ��������� � ���� ��������
		simplifyMarkUps (genMarkUps);
		// ��������� ����������� �������� �������� - ���� ����� ����� �������� �� �������
		if (genMarkUps.size() < 1) {
			logger->writeComment("Warinig: couldn't find common subsequence, creating simple markup");
			createSimpleMarkUpVariants (genMarkUps, (int) as.axioms.size());
		}
		// �������� �� ���� �������� ������ ��� ������� ������ ���������� ��������� � ��������� ��
		chooseBestMarkUp (as, abType, as.markUps[abType], genMarkUps, errFirstVal, errSecondVal);
		
		as.errors[abType].first = errFirstVal;
		as.errors[abType].second = errSecondVal;
		
		as.errFirst += errFirstVal;
		as.errSecond += errSecondVal;
	}
	// ���������� ������ �������� ������� �������� ��� ������ ������� ������ � ������ ��������� �������� ��������� ����������
	//matterAxiomSetFunc (as, as.markUps);
	as.goal = goalStrategy->compute(as.errFirst, as.errSecond);

	return as.goal;
}

double ASStageSimple::matterAxiomSetFunc(AxiomExprSetPlus &as, const std::vector<std::vector<std::vector<int> > >& markupVariants) const {
	int numOfClasses = fuzzyDataSet->getClassCount();
	int errFirstVal, errSecondVal;
	as.errFirst = 0;
	as.errSecond = 0;
	as.markUps.resize(numOfClasses);
	as.errors.resize(numOfClasses);
	for (int abType = 0; abType < numOfClasses; abType++) {
		// �������� �� ���� �������� ������ ��� ������� ������ ���������� ��������� � ��������� ��
		chooseBestMarkUp (as, abType, as.markUps[abType], markupVariants[abType], errFirstVal, errSecondVal);
		
		as.errors[abType].first = errFirstVal;
		as.errors[abType].second = errSecondVal;
		
		as.errFirst += errFirstVal;
		as.errSecond += errSecondVal;
	}
	// ���������� ������ �������� ������� �������� ��� ������ ������� ������ � ������ ��������� �������� ��������� ����������
	//matterAxiomSetFunc (as, as.markUps);
	as.goal = goalStrategy->compute(as.errFirst, as.errSecond);

	// penalize axiom sets with single axiom markup
	if(as.axioms.size() > 1) {
		for(int i = 0; i < as.markUps.size(); ++i) {
			if(as.markUps[i].size() <= 1) {
				as.goal += singleAxiomMarkupPenalty;
			}
		}
	}

	return as.goal;	
}

/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
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
double ASStageSimple::matterAxiomSetFunc (AxiomExprSetPlus &as, std::vector <std::vector <int> > &markUps) const {
	int tmpFirst, tmpSecond;
	double tmpGoal;
	as.errFirst = 0;
	as.errSecond = 0;
	as.errors.resize(markUps.size());
	for (int abType = 0; abType < (int) markUps.size(); abType++) {
                matterAxiomSetFunc (as, abType, markUps[abType], tmpGoal, tmpFirst, tmpSecond);
		as.errors[abType] = std::make_pair(tmpFirst, tmpSecond);
		as.errFirst += tmpFirst;
		as.errSecond += tmpSecond;
	}
	// ���������� �������� ������� ������� ��� ����������� ����� ������ I � II ����
	as.goal = goalStrategy->compute(as.errFirst, as.errSecond);

	// penalize axiom sets with single axiom markup
	if(as.axioms.size() > 1) {
		for(int i = 0; i < as.markUps.size(); ++i) {
			if(as.markUps[i].size() <= 1) {
				as.goal += singleAxiomMarkupPenalty;
			}
		}
	}

	return as.goal;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
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
double ASStageSimple::matterAxiomSetFunc (AxiomExprSetPlus &as, int abType, const std::vector <int> &genMarkUp, double &goalVal, int &errFirstVal, int &errSecondVal) const {
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
	// �������� ���������� � ����� ���������� ����������� �������
	fuzzyDataSet->getTestSize (numOfClasses, numOfMultiTS, numOfTS);
	if ((abType < 0) || (abType >= numOfClasses))
		throw AxiomLibException("FuzzyMultiDataExt_AS::matterAxiomSetFunc: incorrect input number of abnormal type.");
	// �������������� ������ - ����� ��������� ����� ������������� �������� ������
	std::vector <bool> dims;
	dims.resize (fuzzyDataSet->paramNamesSize(), false);
	for (unsigned int axNum = 0; axNum < as.axioms.size(); axNum++) {
		for (unsigned int ecNumI = 0; ecNumI < as.axioms[axNum]->expression.size(); ecNumI++) {
			for (unsigned int ecNumII = 0; ecNumII < as.axioms[axNum]->expression[ecNumI].size(); ecNumII++) {
				if ((as.axioms[axNum]->expression[ecNumI][ecNumII].dimension < 0) || (as.axioms[axNum]->expression[ecNumI][ecNumII].dimension >= (int) dims.size()))
					throw AxiomLibException("FuzzyMultiDataExt_AS::matterAxiomSetFunc : incomplete definition of elementary condition, it's data set dimension is out of range.");
				dims[as.axioms[axNum]->expression[ecNumI][ecNumII].dimension] = true;
			}
		}
	}
	// �������������� ��������� ��� �������� ����� ������
	errFirstVal = 0;
	errSecondVal = 0;
	// �������� ��������� ���������� ��� ��������� ���� ���������� ��������� � ��������� ��������������
	std::vector <int> curMarkUp, curLabeling;
	int num;
        for (int t = 0; t < (int) fuzzyDataSet->getMutiTSCount(FuzzyDataSet::Testing, abType); t++) {
		int currentFirstKindErrors = 0;
		int currentSecondKindErrors = 0;
		// �������� ���������� ����������� ������� �������� ������ as
		createTestMarkUp (curMarkUp, as, dims, abType, t, numOfTS[abType][t]);

		// ������������� ���������� ��������� � �������� ����
		recognizer->run (curMarkUp, genMarkUp, curLabeling);
		
		// ���������� ����� ������ ������� � ������� ����
		num = getStatistic (curLabeling);
		
		// ������������ ����� ������
		if (num == 0) {
			currentSecondKindErrors = 1;
		}
		else {
			currentFirstKindErrors = num - 1;
		}

		as.setErrorsForTraj(abType, t, currentFirstKindErrors, currentSecondKindErrors);

		errSecondVal+= currentSecondKindErrors;
		errFirstVal += currentFirstKindErrors;
	}
	// ��������� �� ���� ����� ����������� ���������
	int numNormalMultiTS;
	std::vector <int> numNormalTS;
	fuzzyDataSet->getNormalTestSize (numNormalMultiTS, numNormalTS);
        for (int t = 0; t < (int) fuzzyDataSet->getMutiTSCount(FuzzyDataSet::Testing, -1); t++) {

		// ��������� ���������� ����������� ���������
		createTestMarkUp (curMarkUp, as, dims, t, numNormalTS[t]);

		// ������������� ���������� ��������� � �������� ����
		recognizer->run (curMarkUp, genMarkUp, curLabeling);
		
		// ���������� ����� ������ ������� � ������� ����
		num = getStatistic (curLabeling);
		
		// ������������ ����� ������
		errFirstVal += num;

		int oldTypeIErrors = as.getErrorsForTraj(-1, t).first;
		if(oldTypeIErrors < 0) oldTypeIErrors = 0;

		as.setErrorsForTraj(-1, t, num + oldTypeIErrors, 0);
	}

	// ���������� �������� ������� ������� ��� ����������� ����� ������ I � II ����
	goalVal = goalStrategy->compute(errFirstVal, errSecondVal);

	return goalVal;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::matterAxiomSetFunc
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
double ASStageSimple::matterAxiomSetFunc (const std::string baseDir, const std::string fileName, int &errFirst, int &errSecond) const {
	// ������������� ������� ������ �� ���������� �����
	AxiomExprSetPlus aesp;
	AxiomSetBase axiomSetBase;
	AxiomExprSetStructure aess;
	std::string axiomSetPath = baseDir;
	axiomSetPath.append("/");
	axiomSetPath.append(fileName);
	// ��������� ���������� � ������� ������ � aess
	std::vector <std::string> dataSetParams;
	dataSetParams = fuzzyDataSet->getParamNames();
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
*					FuzzyMultiDataExt_AS::getStatistic
*
*	Description:	������� ������� ����� ��������� ��������� ��������
*	Parameters:		row - ���, � ������� ������ ��������� ������
*	Returns:		int - ����� ��������� ��������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int ASStageSimple::getStatistic (std::vector <int> &row) const {
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
*					FuzzyMultiDataExt_AS::chooseBestMarkUp
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
int ASStageSimple::chooseBestMarkUp (AxiomExprSetPlus &as, int abType, std::vector <int> &markUp, const std::vector <std::vector <int> > &genMarkUps, int &errFirstVal, int &errSecondVal) const {
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
*					FuzzyMultiDataExt_AS::createMarkUpVariants
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
inline int ASStageSimple::createMarkUpVariants (std::vector < std::vector <int> > &genMarkUps, std::vector < std::vector <int> > &resMarkUps) const {
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
*					FuzzyMultiDataExt_AS::createRefMarkUp
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
inline int ASStageSimple::createRefMarkUp (std::vector <int> &markUp, const AxiomExprSetPlus &as, const int abType, const int multiTSNumber, std::vector < std::vector <int> > &numOfTS) const {
	performMarkUp(as, FuzzyDataSet::Reference, abType, multiTSNumber, markUp);
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::createTestMarkUp
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
inline int ASStageSimple::createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &abType, int &indexTest, int &numOfTestTS) const {
	performMarkUp(as, FuzzyDataSet::Testing, abType, indexTest, markUp);
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::createTestMarkUp
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
inline int ASStageSimple::createTestMarkUp (std::vector <int> &markUp, AxiomExprSetPlus &as, std::vector <bool> &dims, int &indexTest, int &numOfTestTS) const {
	performMarkUp(as, FuzzyDataSet::Testing, -1, indexTest, markUp);
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataExt_AS::simplifyMarkUps
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
inline int ASStageSimple::simplifyMarkUps (std::vector <std::vector <int> > &markUps) const {
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
*					FuzzyMultiDataExt_AS::getSubsequences
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
int ASStageSimple::getSubsequences (const std::vector <int> &str1, const std::vector <int> &str2, int i, int j, const std::vector < std::vector <int> > &square, std::vector < std::vector <int> > &res) const {
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
*					FuzzyMultiDataExt_AS::getSquare
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
int ASStageSimple::getSquare (const std::vector <int> &str1, const std::vector <int> &str2, std::vector < std::vector <int> > &square) const {
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

void ASStageSimple::setSatPointsSize(int size) const {
	satPoints.clear();
	satPoints.reserve(size);
}

void ASStageSimple::addToSatPoints(AxiomExpr &ax) const {
	int index = satPoints.size();
	ax.index = index;
	
	satPoints.push_back(
			AXSatPointSet(ax, *fuzzyDataSet)
			);
}

const SatPointSet &ASStageSimple::getSatPoints(const AxiomExpr &ax, FuzzyDataSet::DataSetDivisionType type, int classNo) const {
	switch(type) {
	case FuzzyDataSet::Reference: return satPoints[ax.index].getRefSatPointSet(classNo); break;
	case FuzzyDataSet::Testing: return satPoints[ax.index].getTestSatPointSet(classNo); break;
	default: throw AxiomLibException("Unexpected dataset division value: " + boost::lexical_cast<std::string>(type));
	}
}

void ASStageSimple::performMarkUp(const AxiomExprSetPlus &as, FuzzyDataSet::DataSetDivisionType division, int classNo, int multiTSNo, std::vector<int>& result) const {
	int multiTSLen = fuzzyDataSet->getMultiTSLength(division, classNo, multiTSNo);
	
	//Logger::getInstance()->writeComment("MultiTSLen: " + str(multiTSLen));
	
	result.resize(multiTSLen);
	std::fill(result.begin(), result.end(), 0);
	
	for(unsigned int i = 0; i < as.axioms.size(); i++) {
		const SatPointSet& currentSatPoints = getSatPoints(*as.axioms[i], division, classNo);
		for(int j = 0; j < multiTSLen; j++) {
			if(currentSatPoints.isSat(multiTSNo, j) && result[j] == 0) {
				result[j] = i + 1;
			}
		}
	}
}
