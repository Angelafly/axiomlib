#include <algorithm>
#include <utility>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/variance.hpp>

#include <omp.h>

#include "../AxiomStatistics.h"
#include "../FuzzyMultiDataExtAlgorithm.h"

#include "ECStage.h"
#include "../Logger.h"
#include "Common.h"

#include "AXStage.h"

#define FUZZYMULTIDATA_AXIOMGOAL_EXPERIMENTAL

#define eps						0.0000001 // ������������ � ��������� ��������, ����� ��������� ������ ��������� ����������
#define max_goal_val			100000 // ������������ �������� ������� ������� ��� ������� (������ ���������� ������� �����)

using namespace AxiomLib;
using namespace AxiomLib::FuzzyMultiDataExt;

AXStage::AXStage(FuzzyDataSet *fuzzyDataSet, ECStage *stage1)
	: parent(stage1->getParent()), fuzzyDataSet(fuzzyDataSet), stage1(stage1), logger(Logger::getInstance())
{
	//TODO: ������������� ���������� ����������
	goalOccurenceWeight = 0.0;
	
	axiomNameTemplate = "axiom";
}

AXSelection axSelectionCreator(const AxiomExprPlus& ax) { return AXSelection(ax); }

void AXStage::setAxioms(const std::vector<std::vector<AxiomExprPlus> > &initial) {
	TransformMultiVectorHelper<2>::transform(initial, this->axioms, 
											 axSelectionCreator
	);	
}

void AXStage::initFromEnv(const Environment &env) {
	// ��������� ������ ������
	if (env.getIntParamValue (maxAxiomPopSize, "maxAxiomPopSize") < 0)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::setParamsFromEnv : maxAxiomPopSize is undefined.");
	if (env.getIntParamValue (maxIterNum, "axiomAlgIterNum") < 0)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::setParamsFromEnv : axiomAlgIterNum is undefined.");
	if (env.getDoubleParamValue (maxAxiomRes, "maxCostForAxiom") < 0)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::setParamsFromEnv : maxCostForAxiom is undefined.");
	if (env.getDoubleParamValue (percentBestAxioms, "percentBestAxioms") < 0)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::setParamsFromEnv : percentBestAxioms is undefined.");	
	
	env.getParamValue(goalOccurenceWeight, "axiomGoalOccurenceWeight");
	
	env.getStringParamValue(axiomNameTemplate, "AxiomNameTemplate");
	
}


const AxiomExprPlus &AXStage::getAX(int aType, int n) const {
	return axioms[aType][n].element();
}

bool AXStage::isAXSelected(int aType, int n) const {
	return axioms[aType][n].selected();
}

void AXStage::setAXSelected(int aType, int n, bool value) {
	axioms[aType][n].setSelected(value);
}

void AXStage::getAXSize(std::vector<int> &result) const {
	result.clear();
	result.reserve(axioms.size());
	
	for(auto it = axioms.begin(); it != axioms.end(); ++it)
	{
		const std::vector<AXSelection>& v = *it;
		result.push_back(v.size());
	}
}

int AXStage::getAXSize() const {
	return axioms.size();
}

int AXStage::getAXSize(int aType) const {
	return axioms.at(aType).size();
}

void AXStage::recalculateMatterAxiomFunc(AxiomExprPlus &ax, int abType) {
	setSatPointsSize(fuzzyDataSet->getClassCount());
	
	int totalSize = 0;
	for(auto it = ax.expression.begin(); it != ax.expression.end(); ++it)
	{
		totalSize += it->size();
	}

	setSatPointsSize(abType, totalSize);
	for(auto it = ax.expression.begin(); it != ax.expression.end(); ++it)
	{
		for(auto it2 = it->begin(); it2 != it->end(); ++it2)
		{
			this->addToEcSatPoints(abType, *it2);
		}
	}
//	TransformMultiVectorHelper<2>::forEach(ax.expression,
//	                                       [abType, this](ElemCondPlus& ec) {  }
//	                                       );
	this->matterAxiomFunc(ax, abType);
}

void axSelector(AXSelection &ax){ ax.setSelected(true); }

void AXStage::run() {

	
	int classCount = fuzzyDataSet->getClassCount();
	
	this->axioms.resize(classCount);
	
	setSatPointsSize(classCount);
	
	#pragma omp parallel for schedule(dynamic, 1)
	for(int classNo = 0; classNo < classCount; classNo++) {
		logger->writeComment("Selecting axioms for abnormal type " + str(classNo));
		formAxioms(classNo);
		logger->writeComment("Finished selecting axioms for abnormal type " + str(classNo));
	}
	
	TransformMultiVectorHelper<2>::forEach(this->axioms, axSelector);

	setNames();
	
//	for(int abType = 0; abType < getAXSize(); abType++) {
//		for(int n = 0; n < getAXSize(abType); n++) {
//			AxiomExprPlus ax = getAX(abType, n);
//			AxiomExprPlus ax2(ax);
//			recalculateMatterAxiomFunc(ax2, abType);
//			if(!(AxiomStatistics(ax) == AxiomStatistics(ax2))) {
//				logger->writeDebug("Warning: statistics don't match");
//				break;
//			} else {
//				logger->writeDebug("Stat ok for "+str(abType)+" "+str(n));
//			}
//		}
//	}
}

void AXStage::formAxioms(int abType) {
	std::vector<std::vector<std::vector<int> > > ecSizeVector;
	std::vector<AXSelection> &axioms = this->axioms[abType];
	
	stage1->getECSize(ecSizeVector);
	
	// ������� ����� ��������� ������������ �������
	unsigned int totSize = 0;
	for (unsigned int i = 0; i < ecSizeVector[abType].size(); i++)
		for (unsigned int j = 0; j < ecSizeVector[abType][i].size(); j++)
			for(size_t k = 0; k < ecSizeVector[abType][i][j]; k++) {
				if(stage1->isECSelected(abType, i, j, k)) {
					++totSize;
				}
			}
	
	if (totSize < 1)
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::formAxioms : set of prepared elementary conditions is indeed empty.");
	
	// �������� ��������� ��������� ������, �� ������� ���� ��������� ������� ������ � ����������� ���������
	axioms.clear();
	axioms.reserve(totSize);
	setSatPointsSize(abType, totSize);
	
	logger->writeComment("Creating intial population and sat points ...");
	//std::vector <std::string> dataSetParamNames;
	//dataSetParamNames = fuzzyDataSet.getParamNames();
	for (unsigned int i = 0; i < ecSizeVector[abType].size(); i++) {
//		logger->writeComment("Processing dimension "+str(i));
		for (unsigned int j = 0; j < ecSizeVector[abType][i].size(); j++) {
//			logger->writeComment("Processing ecType "+str(j));
			for (unsigned int k = 0; k < ecSizeVector[abType][i][j]; k++) {
//				logger->writeComment("Processing ec "+str(k));
				if(stage1->isECSelected(abType, i, j, k)) {
//					logger->writeComment("EC is selected");
					ElemCondPlus ec = stage1->getEC(abType, i, j, k);
//					logger->writeComment("EC retrieved from stage1");
					addToEcSatPoints(abType, ec);
//					logger->writeComment("SatPoints created");
					// �������� ������� �� ��
//					if(t >= axioms.size()) {
//						throw AxiomLibException("t >= axioms.size() : t="+str(t)+", axioms.size()="+str(axioms.size()));
//					}
					axioms.push_back(AxiomExprPlus(ec));
//					logger->writeComment("EC added to initial axioms");
				}
			}
		}
	}
	if(totSize != axioms.size()) {
		logger->writeComment("Warning: totSize != axioms.size(): totSize="+str(totSize)+", axioms.size()="+str(axioms.size()));
		totSize = axioms.size();
	}
	// ��������� ��������� ���������� �� ���������
	logger->writeComment("Counting initial statistics");
	
	for (int i = 0; i < (int) totSize; i++) {
		{
			char buf[128];
			sprintf (buf, "\r\t\taxiomNum=%d\tby thread %d.\t", i, omp_get_thread_num());
			logger->writeComment(buf);
			//std::cout.flush();
		}
		matterAxiomFunc (axioms[i].element(), abType);
	}
	// ���������� ���������� ��� ������ ������
	AxiomExprPlus axiomExprPlus;
	int iterNum = 0; // ������� �������� ��������� ���������� ������
	bool criteriaToContinue = true; // ���� ����������� ��������� ���������� ������
	int curSize, curBest;
	std::vector <AxiomExprPlus> bestAxiomsNew;
	// �������� ������������ �������� ������������� ������� ���������
	if (maxAxiomPopSize < 2) 
		throw AxiomLibException("FuzzyMultiDataLearnAlgorithm::formAxioms : incorrect value of parameter 'maxAxiomPopValue'.");
	// ������ ������ ��������� ������, ������� ����� ���� �������� �� ������ ���� ��������� - ������ ������ ����������� ��� ������� ������
	bestAxiomsNew.resize(maxAxiomPopSize + round ((double)((maxAxiomPopSize - 1) * maxAxiomPopSize) / 2.0));
	// ����������� ��������� ��������� 
	while (criteriaToContinue) {
		iterNum++;
		// ���������� �� ����� ������ ��������
		
		//cutDownByOccurrence(axioms, iterNum - 1, maxIterNum);
		
		{
			char buf[128];
			sprintf (buf, "\r\tAbType: %d\tIteration: %d\tNumber of axioms: %d\tby thread %d.\t", abType, iterNum, axioms.size(), omp_get_thread_num());
			logger->writeComment(buf);
			//std::cout.flush();
		}
		// �������, ���� ����������, ������ ������ �� ����������� ���������� ��������
		cutDownBestAxioms (axioms);
		// ��������� - ����������� �� ����� ��������� �������
		curSize = axioms.size();
		if (curSize < 2) {
			criteriaToContinue = false;
			{
				logger->writeComment("Stopping because axioms vector size < 2");
			}
			break;
		}
		
		logger->writeComment("Cut down size : "+str(curSize) + "; Creating sat points for axioms...");
		
		createAxiomSatPoints(abType, axioms);
		
		logger->writeComment("Finished creating sat points for axioms");
		
		// ��������� ������������ ������� � ��������� �� ������ ����		
		for (int i = 0; i < curSize; i++) {
			bestAxiomsNew[i] = axioms[i].element();
		}
		curBest = curSize;
		// ������� ����� �������� � ��������� �� �������� ������� �������
		logger->writeDebug("Creating new axioms");
		for (unsigned int i = 0; i < (unsigned int) (curSize - 1); i++) {
			for (unsigned int j = i+1; j < (unsigned int) curSize; j++) {
				if (combineAxioms (axioms[i].element(), axioms[j].element(), axiomExprPlus, abType) > 0) {
					bestAxiomsNew[curBest] = axiomExprPlus;
					curBest++;
				}
			}
			axioms[i].element().clear();
		}
		logger->writeDebug("Finished creating new axioms");
		axioms[curSize - 1].element().clear();
		axioms.clear();
		// ���������� ���������� �������� � ������ ��� ��������� ��������
		axioms.resize(curBest);
		for (int i = 0; i < curBest; i++) {
			axioms[i] = bestAxiomsNew[i];
			bestAxiomsNew[i].clear();
		}

		// ��������� �������� ��������
		if (curBest == curSize) {
			criteriaToContinue = false;
			{
				logger->writeComment("Stopping because no new axioms created");
			}
		}
		if (iterNum > maxIterNum) {
			criteriaToContinue = false;
			{
				logger->writeComment("Stopping because maximum number of iterations exceeded");
			}
			
		}
		for (int i = 0; i < curBest; i++) {
			if (axioms[i].element().goal > maxAxiomRes) {
				criteriaToContinue = false;
				{
					logger->writeComment("Stopping because goal value "+
							str(axioms[i].element().goal)+" exceeds maximum goal value " +
							str(maxAxiomRes));
				}
				break;
			}			
		}
	}

	// ������� �������������� ������������� �������
	bestAxiomsNew.clear();

	// ��������� ������ ��������� ������
	cutDownBestAxioms (axioms);
	
	logger->writeComment("Abnormal type " + str(abType) + ", final number of axioms : " + str(axioms.size()));
}

class AxiomSort
{
    const std::vector <AXSelection> &axioms;
public:
    AxiomSort(const std::vector <AXSelection> &axioms): axioms(axioms) {}
    
    bool operator()(unsigned int i1, unsigned int i2)
    { return axioms[i1].element().goal > axioms[i2].element().goal; }
    
};

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::cutDownBestAxioms
*
*	Description:	������� ��������� ��������� ������������ ������ 
*					�� ������������� � ���������� (�� ������-�����) ��������
*	Parameters:		axioms - ������ ������ ������
*	Returns:		0 - ���� �������� ��������� �� ����
*					1 - � ��������� ������
*	Throws:			AxiomLibException - ���� ���������� ���������� ������ �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
int AXStage::cutDownBestAxioms (std::vector <AXSelection> &axioms) const {
	// �������� ������� ����������
	if ((maxAxiomPopSize < 1) || (axioms.size() <= (unsigned int) maxAxiomPopSize))
		return 0;
	
	////
	
	std::vector<unsigned int> sortedPositions(axioms.size());
	std::vector<unsigned int> finalPositions(maxAxiomPopSize);
	
	
	
	int i = 0;
	
	for(auto it = sortedPositions.begin(); it != sortedPositions.end(); ++it) {
	    *it = i;
	    i++;
	}
	
	//std::generate(sortedPositions.begin(), sortedPositions.end(), [&i](){ return i++; });
	
	std::sort(sortedPositions.begin(), sortedPositions.end(), AxiomSort(axioms));
	

	// ����������� ����� ������ ������, ����������� � ���������
	unsigned int numBestToSave;//, numToChoose, numFrom;
	numBestToSave = (unsigned int) round (percentBestAxioms*maxAxiomPopSize);
	//numToChoose = (unsigned int) maxAxiomPopSize - numBestToSave;
	//numFrom = axioms.size() - numBestToSave;
	
	for(int i = 0; i < numBestToSave; ++i) {
		finalPositions[i] = sortedPositions[i];
	}
	
	for(int i = numBestToSave; i < maxAxiomPopSize; ++i) {
		int curChoice = i + round (((double) rand() / (double) RAND_MAX) * (double) (axioms.size() - i - 1));
		finalPositions[i] = sortedPositions[curChoice];
		std::swap(sortedPositions[i], sortedPositions[curChoice]);
	}
	
	for(int i = 0; i < maxAxiomPopSize; ++i) {
		if(i != finalPositions[i]) {
			std::swap(axioms[i], axioms[finalPositions[i]]);
		}
	}
	
	axioms.resize(maxAxiomPopSize);

	return 0;
	
	////

/*	
	// ���������� ������ �� �������� ������� �������
	std::vector <unsigned int> pos;
	getPriority (axioms, pos);
	
	// ����������� ����� ������ ������, ����������� � ���������
	unsigned int numBestToSave, numToChoose, numFrom;
	numBestToSave = (unsigned int) round (percentBestAxioms*maxAxiomPopSize);
	numToChoose = (unsigned int) maxAxiomPopSize - numBestToSave;
	
	numFrom = axioms.size() - numBestToSave;
	std::vector <bool> chosen;
	chosen.resize (numFrom, false);
	int curChoice;
	std::vector <unsigned int> toSave;
	toSave.resize (maxAxiomPopSize);
	// ������� � ������ ����������� ��������� - �������� ����� ������
	for (unsigned int i = 0; i < numBestToSave; i++) {
		toSave[i] = pos[i];
	}
	// ��������� ����� ����������
	for (unsigned int i = 0; i < numToChoose; i++) {
		curChoice = round (((double) rand() / (double) RAND_MAX) * (double) (numFrom - i - 1));
		for (unsigned int t = (unsigned int) curChoice; t < numFrom; t++)
			if (!chosen[t]) {
				chosen[t] = true;
				toSave[numBestToSave + i] = pos[numBestToSave + t];
				break;
			}
	}

	// ���������� ������� ��������� ��� ���������� ��������� �� ��������
	unsigned int tmpInt;
	for (unsigned int i = maxAxiomPopSize - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			if (toSave[j] < toSave[j+1]) {
				tmpInt = toSave[j+1];
				toSave[j+1] = toSave[j];
				toSave[j] = tmpInt;
			}
		}
	}
	
	// �������� ����������� ������
	// ������� ��� �������� ����� ���������� �� ������� � ������
	if ((toSave[0] + 1) < axioms.size()) {
		for (unsigned int j = toSave[0] + 1; j < axioms.size(); j++)
			axioms[j].element().clear();
		axioms.erase (axioms.begin() + toSave[0] + 1, axioms.end());
	}
	for (int i = 1; i < maxAxiomPopSize; i++) {
		if ((toSave[i] + 1) < toSave[i-1]) {
			// ������� ���������� ��������� ������� ������
			for (unsigned int j = toSave[i] + 1; j < toSave[i-1]; j++)
				axioms[j].element().clear();
			// ������� �������� ��������
			axioms.erase (axioms.begin() + toSave[i] + 1, axioms.begin() + toSave[i-1]);
		}
	}
	// ������� ��� �������� �� ������� �� ������� � ������������ ������
	if (toSave[maxAxiomPopSize-1] > 0) {
		for (unsigned int j = 0; j < toSave[maxAxiomPopSize-1]; j++) {
			axioms[j].element().clear();
		}
		axioms.erase (axioms.begin(), axioms.begin() + toSave[maxAxiomPopSize-1]);
	}

	return 0;
	*/
}

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::getPriority
*
*	Description:	��������������� ������� ���������� ������� �� �������� 
*					� ���������� ����������� � ��������� ������ ��������.
*					�������� ��������� ������������ ������� �� ���������� � �� 
*					������������.
*	Parameters:		vec - ����������� ������ ������ �� �������� ������� �������
*					pos - ������ �������� ��� �������� ����������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int AXStage::getPriority (std::vector <AXSelection> &vec, std::vector <unsigned int> &pos) const {
	pos.resize(vec.size()); 
	for (unsigned int i = 0; i < vec.size(); i++) 
		pos[i] = i;
	unsigned int temp;
	for (unsigned int i = vec.size() - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			if (vec[pos[j]].element().goal < vec[pos[j+1]].element().goal) {
				temp = pos[j+1];
				pos[j+1] = pos[j];
				pos[j] = temp;
			}
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::combineAxioms
*
*	Description:	����������� ���� ������ � ���������� ���������� ��� ���������� ����� ������.
*	Parameters:		axiomFirst - ������ �� ������, �� ������ ������� ����� ��������� ����� �������� ������
*					axiomSec - ������ �� ������, �� ������ ������� ����� ��������� ����� �������� ������
*					axiomExpr - ����������� ���������� �������, � ������ ���� ������� ��������� ������ �������
*					abType - ��� ���������� ��������� ��� �������� ������������ ���������� �������
*	Returns:		0 - ���� ����� �������, ��������� �� ���� ��������� ��������� �� �����
*					>0 - ���� ��������� ������� ��������� ����� ����������������
*					-1 - ���� �� ������� �������� �������������� ��������� ������
*	Throws:			AxiomLibException - ���� ��������� ������� ��������� �� ����������� � ����������� ���������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int AXStage::combineAxioms (AxiomExprPlus &axiomFirst, AxiomExprPlus &axiomSecond, AxiomExprPlus &axiomExprPlus, const int abType) const {
	// �������� ����� ������
	//logger->writeDebug("Creating Or and And axioms");
	AxiomExprPlus axiomExprAnd, axiomExprOr;
	axiomExprAnd.andExpr(axiomFirst, axiomSecond);
	axiomExprOr.orExpr (axiomFirst, axiomSecond);
	
	// ���������� ������� ����� ������������
	//logger->writeDebug("Calculating SatPointSets");
	ReducedSatPointSet orSet = getOr(abType, axiomFirst, axiomSecond, m_temporary1[abType]);
	ReducedSatPointSet andSet = getAnd(abType, axiomFirst, axiomSecond, m_temporary2[abType]);
	
	//Debug
/*	ReducedSatPointSet orSet2 = createAxiomSatPoints(abType, axiomExprOr, 
	                                                 ReducedSatPointSet(), ReducedSatPointSet());
	ReducedSatPointSet andSet2 = createAxiomSatPoints(abType, axiomExprAnd, 
	                                                 ReducedSatPointSet(), ReducedSatPointSet());
	
	if(!(orSet == orSet2) ) {
		logger->writeDebug("Warning: orSet != orSet2");
	} else {
		logger->writeDebug("Or ok");
	}
	if(!(andSet == andSet2)) {
		logger->writeDebug("Warning: andSet != andSet2");
	} else {
		logger->writeDebug("And ok");
	}
*/	
	// ���������� ������� �������
	//logger->writeDebug("Calculating matterAxiomFuncs");
	matterAxiomFunc (axiomExprOr, abType, orSet);
	matterAxiomFunc (axiomExprAnd, abType, andSet);

	// ����� ���������� ����������
	//std::cout << "\n\t andRes = " << axiomExprAnd.goal << "\torRes = " << axiomExprOr.goal << "\taxF = " << fi << "\taxS = " << se << "\tand.size() = " << axiomExprAnd.expression.size() << "\tand[0].size() = " << axiomExprAnd.expression[0].size() << "\tor.size() = " << axiomExprOr.expression.size() << "\tor[0].size() = " << axiomExprOr.expression[0].size();
	//logger->writeDebug("Analyzing result");
	// ����������� ����� �� ���������� �������� ������� ������� � ����� ������
	if ((axiomExprAnd.goal >= axiomExprOr.goal) || (((axiomExprOr.goal - axiomExprAnd.goal) <= eps) && (axiomExprAnd.statAbnorm >= axiomExprOr.statAbnorm))) {
		// ������ axiomExprAnd - ����� axiomExprOr
		if ((axiomFirst.goal >= axiomSecond.goal) || (((axiomSecond.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomSecond.statAbnorm))) {
			// ������ axiomFirst - ����� axiomSec
			if ((axiomFirst.goal >= axiomExprAnd.goal) || (((axiomExprAnd.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomExprAnd.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprAnd;
				return 1;
			}
		} else {
			// ������ axiomSec - ����� axiomFirst
			if ((axiomSecond.goal >= axiomExprAnd.goal) || (((axiomExprAnd.goal - axiomSecond.goal) <= eps) && (axiomSecond.statAbnorm >= axiomExprAnd.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprAnd;
				return 1;
			}			
		}
	} else {
		// ������ axiomExprOr - ����� axiomExprAnd
		if ((axiomFirst.goal >= axiomSecond.goal) || (((axiomSecond.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomSecond.statAbnorm))) {
			// ������ axiomFirst - ����� axiomSec
			if ((axiomFirst.goal >= axiomExprOr.goal) || (((axiomExprOr.goal - axiomFirst.goal) <= eps) && (axiomFirst.statAbnorm >= axiomExprOr.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprOr;
				return 2;
			}
		} else {
			// ������ axiomSec - ����� axiomFirst
			if ((axiomSecond.goal >= axiomExprOr.goal) || (((axiomExprOr.goal - axiomSecond.goal) <= eps) && (axiomSecond.statAbnorm >= axiomExprOr.statAbnorm))) {
				// ������ ������ �� axiomExprAnd, axiomExprOr - ����, ��� ���� �� axiomFirst, axiomSec
				return 0;
			} else {
				// ������ ������ �� axiomExprAnd, axiomExprOr - �����, ��� ������ �� axiomFirst, axiomSec
				axiomExprPlus = axiomExprOr;
				return 2;
			}			
		}

	}

	// ���� ������ ��� �� ������ 
	return -1;
}

double AXStage::matterAxiomFunc(AxiomExprPlus &ax, const int abType) const {
	
	ReducedSatPointSet axSatPointSet = createAxiomSatPoints(abType, ax, m_temporary1[abType], m_temporary2[abType]);
	return matterAxiomFunc(ax, abType, axSatPointSet);
}

/****************************************************************************
*					FuzzyMultiDataLearnAlgorithm::matterAxiomFunc
*
*	Description:	������� ������� ������� ��� �������
*	Parameters:		ax - �������, ��� ������� ����������� �������� ������� �������
*					param - �������� ������ ������, �� ������� ����������� �������
*	Returns:		double - �������� ������� �������
*	Throws:			AxiomLibException - ���� ������� �������� ��������������� ��������� �� ������ ������
*	Author:			dk
*	History:
*
****************************************************************************/
double AXStage::matterAxiomFunc (AxiomExprPlus &ax, const int abType, const ReducedSatPointSet& axSatPointSet) const {
		
	const SatPointSet& normalSatPointSet = axSatPointSet.normal();
	const SatPointSet& abnormalSatPointSet = axSatPointSet.abnormal();
	
	// ���� �� ���� ����������� �� ��������� ������� ��� ������� ������ ���������� ���������
	int classCount = 0;
	int classLen = 0;
		
	boost::accumulators::accumulator_set<double, 
		boost::accumulators::features<boost::accumulators::tag::variance> > varianceAccumulator;
	
	// ����� ����������, ��������������� ���������� ���������, �� ������� ������� ����������
	int numOccured = 0;
	
	// ����� ����� ����������, ��������������� ���������� ���������
	int numTraj = abnormalSatPointSet.size();
	
	for (int j = 0; j < numTraj; j++) {
		// ��������� ��� ����������� ������� �� ��������� ������������ ��������� ���������� � ������� ���������� ��� �������� �������� ������� �������
		int currNumOfCarriedOutItems = abnormalSatPointSet.numSat(j);
		
		if(currNumOfCarriedOutItems > 0) {
			++numOccured;
		}
		
		int classLenCur = abnormalSatPointSet.size(j);
		
		// ��������� ������� ������������ �� ������� ����������
		double currFreq = (double) currNumOfCarriedOutItems / (double) classLenCur;
		
		// ������������ �������� ������� ��� ����������� �������� ��������
		varianceAccumulator(currFreq);
		
		classCount += currNumOfCarriedOutItems;
		classLen += classLenCur;
	}
	// ��������� ���������� �� ���������� ���������
	
	if (classLen > 0) {
		ax.statAbnorm = (double) classCount/ (double) classLen;
		ax.statVariation = std::sqrt(
				boost::accumulators::variance(varianceAccumulator)
				);
		ax.statOccurence = (double) numOccured / (double) numTraj;
	} else {
		ax.statAbnorm = -1.0; // - ������ ������ �� ����������
		ax.statVariation = -1.0;
		ax.statOccurence = -1.0;
		std::cout << "\nWarning in  FuzzyMultiDataLearnAlgorithm::matterAxiomFunc : incorrect dataSet request for abnormal type.\n";
	}	

	classCount = 0;
	classLen = 0;
	numTraj = normalSatPointSet.size();
	for (int j = 0; j < numTraj; j++) {
		classCount += normalSatPointSet.numSat(j);
		classLen += normalSatPointSet.size(j);
	}
	// ��������� ���������� �� ���������� ���������
	if (classLen > 0)
		ax.statNormal = (double) classCount/ (double) classLen;
	else {
		ax.statNormal = -1.0; // - ������ ������ �� ����������
		std::cout << "\nWarning in FuzzyMultiDataLearnAlgorithm::matterAxiomFunc: incorrect dstaSet request for normal data.\n";
	}

#ifndef FUZZYMULTIDATA_AXIOMGOAL_EXPERIMENTAL
	// ����������� �������� ������� �������
	if (ax.statAbnorm < eps)
		ax.goal = 0.0;
	else
		if (ax.statNormal < eps)
			ax.goal = max_goal_val;
		else
			ax.goal = min (ax.statAbnorm / ax.statNormal, max_goal_val);
#else
	if (ax.statAbnorm < eps)
		ax.goal = 0.0;
	else {
		const double delta = 0.000005;
		double v1 = std::min ((double) ax.statAbnorm / (ax.statNormal + delta), (double)max_goal_val);
		double v2 = ax.statOccurence;
		ax.goal = v1 + goalOccurenceWeight * v2;
	}
#endif
	return ax.goal;
}

ReducedSatPointSet AXStage::conjunction(int aType, const std::vector<ElemCondPlus> &ecs, ReducedSatPointSet& storage) const {
	ReducedSatPointSet& initial = storage;
	ReducedSatPointSet ecSatPointSet = getEcSatPointSet(aType, ecs[0]);
	initial.assign(ecSatPointSet);
		
	for(int i = 1; i < ecs.size(); i++) {
		const ReducedSatPointSet& current = getEcSatPointSet(aType, ecs[i]);
		initial &= current;
	}
	
	return initial;
}

ReducedSatPointSet& AXStage::disjunction(ReducedSatPointSet &s1, const ReducedSatPointSet &s2) {
	s1 |= s2;
	
	return s1;
}

void AXStage::setSatPointsSize(int size) {
	ecSatPoints.resize(size);
	axSatPoints.resize(size);
	m_temporary1.resize(size);
	m_temporary2.resize(size);
}

void AXStage::setSatPointsSize(int aType, int size) {
	ecSatPoints[aType].clear();
	ecSatPoints[aType].reserve(size);
}

void AXStage::addToEcSatPoints(int aType, ElemCondPlus &ecPlus) {
	int index = ecSatPoints[aType].size();
	ecPlus.index = index;
	
	PSatPointSet normal;

	if(parent->oneVsAllMode()) {
		normal = PSatPointSet(
					new SatPointSet(ecPlus, *fuzzyDataSet, FuzzyDataSet::Reference, aType, SatPointSet::ComplementarySatPoints)
					);
	} else {
		normal = PSatPointSet(
				new SatPointSet(ecPlus, *fuzzyDataSet, FuzzyDataSet::Reference, -1, SatPointSet::ClassSatPoints)
				);
	}
	
	PSatPointSet abnorm = PSatPointSet(
			new SatPointSet(ecPlus, *fuzzyDataSet, FuzzyDataSet::Reference, aType, SatPointSet::ClassSatPoints)
			);
			
	ecSatPoints[aType].push_back(ReducedSatPointSet(normal, abnorm));
}

const ReducedSatPointSet& AXStage::getEcSatPointSet(int aType, const ElemCondPlus &ecPlus) const {
	return ecSatPoints[aType][ecPlus.index];
}

void AXStage::createAxiomSatPoints(int abType, std::vector<AXSelection> &axVector) {
	std::vector<ReducedSatPointSet>& satPointVector = axSatPoints[abType];
	
	satPointVector.clear();
	satPointVector.reserve(axVector.size());
	satPointVector.resize(axVector.size());
	
	for(int i = 0; i < axVector.size(); i++) {
		createAxiomSatPoints(abType, axVector[i].element(), satPointVector[i], m_temporary1[abType]);
		if(satPointVector[i].isNull()) {
			logger->writeComment("Warning: failed to create SatPointSet for axiom");
			throw AxiomLibException("Failed to create SatPointSet for axiom");
		}
		axVector[i].element().index = i;
	}
}

// ���������� ��������� � storage
ReducedSatPointSet AXStage::createAxiomSatPoints(int abType, const AxiomExprPlus &ax,
	ReducedSatPointSet& storage, ReducedSatPointSet& storageAux) const {
	// ������ ����� ����� ���������� �������
	ReducedSatPointSet axSatPointSet = conjunction(abType, ax.expression[0], storage);
	for(int disjunctIndex = 1; disjunctIndex < ax.expression.size(); ++disjunctIndex) {
		ReducedSatPointSet current = conjunction(abType, ax.expression[disjunctIndex], storageAux);
		disjunction(axSatPointSet, current);
	}
	
	return axSatPointSet;	
}

const ReducedSatPointSet& AXStage::getAxSatPointSet(int aType, const AxiomExprPlus &ax) const {
	return axSatPoints[aType][ax.index];
}

ReducedSatPointSet AXStage::getAnd(int abType, const AxiomExprPlus &first, const AxiomExprPlus &second, ReducedSatPointSet& storage) const {
	const ReducedSatPointSet& firstSatPoints = getAxSatPointSet(abType, first);
	const ReducedSatPointSet& secondSatPoints = getAxSatPointSet(abType, second);
	
	storage.assign(firstSatPoints);
	storage &= secondSatPoints;
	
	return storage;
}

ReducedSatPointSet AXStage::getOr(int abType, const AxiomExprPlus &first, const AxiomExprPlus &second, ReducedSatPointSet& storage) const {
	const ReducedSatPointSet& firstSatPoints = getAxSatPointSet(abType, first);
	const ReducedSatPointSet& secondSatPoints = getAxSatPointSet(abType, second);
	
	storage.assign(firstSatPoints);
	storage |= secondSatPoints;
	
	return storage;
}

void AXStage::setNames() {
	for(int abType = 0; abType < getAXSize(); abType++) {
		for(int n = 0; n < getAXSize(abType); n++) {
			axioms[abType][n].element().nameOfAxiomExpr = getAXName(abType, n);
		}
	}
}

std::string AXStage::getAXName(int abType, int n) const {
	return axiomNameTemplate + "_at_" + str(abType+1) + "_" + str(n+1);
}

class CheckThreshold {
    double threshold;
public:
    CheckThreshold(double threshold) : threshold(threshold) {}
    bool operator()(const AXSelection& selection) { return selection.element().statOccurence < threshold; }
};

void AxiomLib::FuzzyMultiDataExt::AXStage::cutDownByOccurrence(std::vector<AxiomLib::FuzzyMultiDataExt::AXSelection> &axioms, int iterNum, int maxIterNum) const {
	double threshold = std::pow(((double) iterNum)  / ((double) maxIterNum) - eps, 2);
	
	auto iter = std::remove_if(axioms.begin(), axioms.end(), 
                                   CheckThreshold(threshold) );

	int newSize = std::distance(axioms.begin(), iter);
	
	Logger::getInstance()->writeDebug("Shrinking axioms by occurrence. Threshold : " + str(threshold) + ", " + str(newSize) + " / " + str(axioms.size()));
	
	if(newSize < 2*maxAxiomPopSize) {
		Logger::getInstance()->writeDebug("New size less than 2*pop size, leaving 2*pop size axioms");
		newSize = 2*maxAxiomPopSize;
	}
	
	axioms.resize(newSize);
}
