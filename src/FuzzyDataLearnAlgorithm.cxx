/****************************************************************************
*
*				������� ������ FuzzyDataLearnAlgorithm
*
****************************************************************************/
#include "FuzzyDataLearnAlgorithm.h"
#include "Defines.h"

using namespace AxiomLib;

#define numBestECs 10

#define eps 0.001

// ���������� ������
FuzzyDataLearnAlgorithm::FuzzyDataLearnAlgorithm (void) {
	// ���������� �� ��������� �� �������� ������� ������������
	comments = false;
	compareStatistic = NULL;
	goalStrategy = NULL;

	dataSetParams.clear();
	axiomSetBaseDir.clear();
	axiomName.clear();
	stage = 0;
	costEthIn = 1.0;
	costEthOut = 1.0;
	costNormalIn = 1.0;
	costNormalOut = 1.0;

	costStrongPoint = 1.0;
	costMaxMinLenRatio = 1.0;
	costLenRatio = 1.0;
	costHiAreasNum = 1.0;
	hiAreasNumEffect = 1.0;
	shiftErrIn = 10.0;
	extendMarkUpLensLevel = 0.0;

	maxAxiomPopSize = 30;
	maxIterNum = 100;
	maxAxiomRes = 1.0;
	percentBestAxioms = 1.0;
} 


// ���������� ������ - �������� ��� ��������� � ������ ������ ������������ �������
FuzzyDataLearnAlgorithm::~FuzzyDataLearnAlgorithm (void) {
	if (compareStatistic != NULL) delete compareStatistic;
	if (goalStrategy != NULL) delete goalStrategy;
}


/****************************************************************************
*				FuzzyDataLearnAlgorithm::initFromEnv
*
*	Description:	������� ������������� ��������� ������� �� ������ �� 
*					���������, ������� �������� ��� �������� �������
*	Parameters:	env - ������ ������ ���������, ������ ������� ��������� ������� ������
*	Returns:	0
*	Throws:		AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:		dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::initFromEnv (const Environment& env) {
	// �������������� dataSet
	std::string datasetDir, datasetName;
	if (env.getStringParamValue(datasetDir, "BaseDataSetDir") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : data set directory is undefined.");
	if (env.getStringParamValue(datasetName, "DataSet") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : data set is undefined.");
	// ��������� ����������� ��� ������� ������ ��������� � ������ ������
	EnvDataSet envDataSet;
	envDataSet.readConfigFile (datasetDir, datasetName);
	// ��������� ����������� ����������� NullStr - ����������� ��������� � ������ ����� ���� ������ ���� ���������� ���������
	fuzzyDataSet.setNullStr (envDataSet);
	fuzzyDataSet.setNormalStr (envDataSet);
	// ���������� ��������� ����� ������ - ������� ��� �� ���������� �������������
	fuzzyDataSet.readDataSet(datasetDir, datasetName);
	// ��������������� � ������ ������ ������ �������� ���������� � ������������ �������������� �� ������� ����� ����� �������� ��������
	fuzzyDataSet.getParamNums(dataSetParams, env, envDataSet);

	// ���������, �� ������� ��������� ������ �������
	if (env.getStringParamValue(axiomSetBaseDir, "AxiomSetBaseDir") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : axiomSetBaseDir directory is undefined.");
	if (env.getStringParamValue(axiomName, "saveTo") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : axiomName to save is undefined.");

	// �������� ������ �������� ����������
	std::string compClassName;
	CompareClassFactory ccf;
	if (env.getStringParamValue(compClassName, "compareClass") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::initFromEnv: compare-class is undefined.");
	compareStatistic = ccf.create(compClassName);
	compareStatistic->setParamsFromEnv(env);

	// �������������� ��������� ���������� ������� �������
	std::string goalStrategyName;
	GoalStrategyFactory gsf;
	if (env.getStringParamValue(goalStrategyName, "goalClass") < 0) 
		throw AxiomLibException("FuzzyDataLearnAlgorithm::initFromEnv: goal-class is undefined.");
	goalStrategy = gsf.create(goalStrategyName);
	goalStrategy->setParamsFromEnv(env);

	// ��������� ��������� ��������� ������������ �������
	if (env.getIntParamValue (leftLimit, "leftLimit") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : leftLimit is undefined.");
	if (env.getIntParamValue (rightLimit, "rightLimit") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : rightLimit is undefined.");
	if (env.getDoubleParamValue (reserve, "reserve") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : reserve is undefined.");
	if (env.getIntParamValue (numOfLevels, "numOfLevels") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : numOfLevels is undefined.");

	// ������ �������� ���������� ������� ������ � ������ �������� ���������� 0 (����� �� ���������)
	stage = 0;

	// ��������� ���������� ������������� ������� �� �������, ���������� ��������� ���������
	if (env.getDoubleParamValue (costEthIn, "costEthIn") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costEthIn is undefined.");
	// ��������� ������������ ������������� ������� �� �������, ���������� ��������� ���������
	if (env.getDoubleParamValue (costEthOut, "costEthOut") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costEthOut is undefined.");
	// ��������� ���������� ������������� ������� �� ������� ����������� ���������
	if (env.getDoubleParamValue (costNormalIn, "costNormalIn") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costNormalIn is undefined.");
	// ��������� ������������ ������������� ������� �� ������� ����������� ���������
	if (env.getDoubleParamValue (costNormalOut, "costNormalOut") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costNormalOut is undefined.");

	// ��������� ������ ������
	if (env.getIntParamValue (maxAxiomPopSize, "maxAxiomPopSize") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : maxAxiomPopSize is undefined.");
	if (env.getIntParamValue (maxIterNum, "axiomAlgIterNum") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : axiomAlgIterNum is undefined.");
	if (env.getDoubleParamValue (maxAxiomRes, "maxCostForAxiom") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : maxCostForAxiom is undefined.");
	if (env.getDoubleParamValue (percentBestAxioms, "percentBestAxioms") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : percentBestAxioms is undefined.");

	// ��������� ��� ���������� ����������� ������������� ������� �����
	if (env.getDoubleParamValue (costStrongPoint, "costStrongPoint") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costStrongPoint is undefined.");
	// ��������� �������� ��������� ����������� ����� � ������������ ��� �������� ��� ������� ����������� �� �������� �������
	if (env.getDoubleParamValue (costMaxMinLenRatio, "costMaxMinLenRatio") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costMaxMinLenRatio is undefined.");
	// ��������� �������� � 0 ��������� ����� �����, ��� ������� ����������� � ������ ����� �� �������� �������.
	if (env.getDoubleParamValue (costLenRatio, "costLenRatio") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costLenRatio is undefined.");
	// ��������� ���������� �������� ������������ ������� �� ����������� ����������� ���������
	if (env.getDoubleParamValue (costHiAreasNum, "costNormalInWhole") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : costNormalInWhole is undefined.");
	// ��������, � ������� ��������� costHiAreasNum
	if (env.getDoubleParamValue (hiAreasNumEffect, "normalInWholeEffect") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : normalInWholeEffect is undefined.");

	// ��������� ������, ���� ��� ������ �������� �������� ������� �������� ��� ������������ � ������ ��������
	if (env.getDoubleParamValue (shiftErrIn, "shiftErrIn") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : shiftErrIn is undefined.");

	// ������� ���������� ����� ��������� �������� (��� ������ �������� �� ����� �� �������) 
	if (env.getDoubleParamValue (extendMarkUpLensLevel, "extendMarkUpLensLevel") < 0)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setParamsFromEnv : extendMarkUpLensLevel is undefined.");

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::setComments
*
*	Description:	������������� ����� �������� ���������� ������ comments
*	Parameters:		newVal - ����� �������� ���������� comments
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::setComments (const bool newVal) {
	this->comments = newVal;
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::run
*
*	Description:	�������� ������� � ������ - ��������� �������� ������
*					������� ������ � �������� ���������� ����������� ���������.
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::run (void) {
	// �����!!! : ���������, ��� � ������� ������ ���� ������������� ��������, ��� ��� �������� �������� ��� ���������� ��������� �����
	if (dataSetParams.size() != 1)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::run : algorithm is not designed to work with multiple number of params.");
	int param = dataSetParams[0];

	// ��������� ������ �������� ���������� ������� ������ � ������ ��������
	stage = 0;

	// 1. ������ ���������� ������������ �������
	selectElemCond (param);
	// �������� ����� ������������ �������
	stage = 1;

	// 2. �������� ������ ������ �� ����������� �������
	formAxioms (param);
	// �������� ����� ������
	stage = 2;

	// 3. ������������ ������� ������ �� ����������� ������
	createAxiomSet (param);
	// ��������� ���������� ������� ������
	stage = 3;

	// 4. ���������� ����������� � ����
	//axiomSet->saveAxiomSetToFile(axiomSetBaseDir, axiomName);
	// ��������� ������� ������ ��������� � ����
	stage = 4;
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::storeBestECs
*
*	Description:	������� ���������� ������ ������������ ������� � ��������������� 
*					�������� ������� �������
*	Parameters:		ec - ����������� ������������ �������
*					curFun - �������� ������� �������
*					bestECs - ������ ������ ������� ���� �� ����
*					bestECres - ������ ������ �������� ������� �������
*	Returns:		1 - ���� ������� ���������
*					0 - ���� ������� �� ������ � ������ ������ 
*	Throws:			AxiomLibException - ���� ������� ������� �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::storeBestECs (ElemCondPlus &ec, double &curFun, std::vector <ElemCondPlus> &bestECs, std::vector <double> &bestECres) {
	// �������� ������� ����������
	if (bestECs.size() != bestECres.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::storeBestECs : incorrect input parameters.");
	// �������� - ������ �� ������ �� � ������ ������ �� �������
	int whereToSave = -1;
	for (int i = 0; i < (int) bestECs.size(); i++) {
		if (bestECs[i].elemCondition == NULL) {
			bestECs[i] = ec;
			bestECres[i] = curFun;
			return 1;
		} else {
			if (bestECres[i] < curFun) {
				whereToSave = i;
				break;
			}
		}
	}
	// ���� ������� �� ����� ������ - ������� � 0
	if (whereToSave < 0)
		return 0;
	// �������� ������ ��, ����� ��� ���� ��������� �� ������� �������� ������� �������
	for (unsigned int i = bestECs.size() - 1; i > (unsigned int) whereToSave; i--) {
		bestECs[i] = bestECs[i-1];
		bestECres[i] = bestECres[i-1];
	}
	// ��������� ��
	bestECs[whereToSave] = ec;
	bestECres[whereToSave] = curFun;
	return 1;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::numOfCarriedOutItems
*
*	Description:	������� ����� ����� � ����, �� ������� ������������ ������� �����������
*	Parameters:		ec - ����������� ������������ �������
*					row - ��� ��� �������� ������������� �������
*	Returns:		int - ����� �����, �� ������� ������������ ������� ���������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::numOfCarriedOutItems (ElemCondPlus &ec, std::vector <double> &row) const {
	int count = 0;
	for (unsigned long i = 0; i < row.size(); i++) {
		if (ec.check (i, row) > 0)
			count++;
	}
	return count;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::matterECFunc
*
*	Description:	������� ������� ������� ��� ������������� �������
*	Parameters:		ec - ������������ �������
*					param - �������� ������ ������, �� ������� ����������� ������������� �������
*	Returns:		double - �������� ������� �������
*	Throws:			AxiomLibException - ���� ������� �������� ��������������� ��������� �� ������ ������
*	Author:			dk
*	History:
*
****************************************************************************/
double FuzzyDataLearnAlgorithm::matterECFunc (ElemCondPlus &ec, const int param) {
	// ���������� ����������� ����������
	double toReturn = 0.0;
	int numOfClasses, numOfNormalMultiTS;
	std::vector <int> numOfMultiTS, numOfNormalTS;
	std::vector < std::vector <int> > numOfTS;
	std::vector <double> curTS;
	bool bres = false;
	double curItem, classCount;
	int curRes, ethNum, ethInc;
	// ��������� ���������� � ������ ������
	fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	// ���� �� ����������� �� ��������� �������
	for (int i = 0; i < (int) numOfMultiTS.size(); i++) {
		if (numOfMultiTS[i] != numOfTS[i].size())
			throw AxiomLibException("FuzzyDataLearnAlgorithm::matterECFunc : incorrect response from internal function.");
		classCount = 0.0;
		ethNum = 0;
		ethInc = 0;
		for (int j = 0; j < (int) numOfTS[i].size(); j++) {
			curTS.clear();
			bres = fuzzyDataSet.getTSByIndexFromClass (curTS, i, j, param);
			if (bres) {
				curRes = numOfCarriedOutItems (ec, curTS);
				// classCount += (double) curRes;
				ethNum++;
				if (curRes > 0)
					ethInc++;
			}
		}
		if (ethInc > 0)
			toReturn += costEthIn * (double) ethInc / (double) ethNum;
		else
			toReturn -= costEthOut;
	}
	// ���� �� ����������� ����������� ���������
	fuzzyDataSet.getNormalClassSize (numOfNormalMultiTS, numOfNormalTS);
	if (numOfNormalMultiTS != numOfNormalTS.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::matterECFunc : incorrect response from internal function getNormalClassSize");
	classCount = 0.0;
	int sumLen = 0;
	for (int j = 0; j < numOfNormalMultiTS; j++) {
		curTS.clear();
		bres = fuzzyDataSet.getNormalTS (curTS, j, param);
		if (bres) {
			curItem = (double) numOfCarriedOutItems (ec, curTS);
			classCount += curItem;
			sumLen += curTS.size();
		}
	}
	if (classCount < eps)
		toReturn += costNormalOut;
	else
		toReturn -= costNormalIn * classCount / (double) sumLen;

	return toReturn;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::selectElemCond
*
*	Description:	������� ������� � ��������� ������������ �������.
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::selectElemCond (const int param) {
	ElemConditionsFactory ecf;
	// ������� �������� ������ elemConds ���������� �� ���� ������������� ���������
	std::vector <ElemCondPlus> elemConds;
	elemConds.resize (2*N_EL_AXIOMS);
	for (unsigned int i = 0; i < N_EL_AXIOMS; i++) {
		elemConds[2*i].elemCondition = ecf.createAxiomByNum(i+1);
		elemConds[2*i].sign = true;
		elemConds[2*i+1].elemCondition = ecf.createAxiomByNum(i+1);
		elemConds[2*i+1].sign = false;
	}
	// ������� ����������� ��� �������� ���
	std::vector<double> teachRow;
	int numOfMultiTS;
	std::vector <int> numOfTS;
	fuzzyDataSet.getNormalClassSize (numOfMultiTS, numOfTS);
	if ((numOfTS.size() != numOfMultiTS) || (numOfMultiTS < 1))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::selectElemCond : not enough data for the algorithm.");
	for (int i = 0; i < numOfMultiTS; i++) {
		if (numOfTS[i] > param) {
			fuzzyDataSet.getNormalTS (teachRow, i, param);
			if (teachRow.size() > 0)
				break;
		}
	}
	if (teachRow.size() < 1)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::selectElemCond : incorrect normal data set.");
	// ������ ������� ���� ��������� ����� ������� ��������� ���������� �� ������������ � ����������� ��������� ���
	// ���������� ������ ���������� � ������������ ��������. ������� ������� ������� ������ �� �����, ����� ���������� 1000.
	if (teachRow.size() > 1000) {
		teachRow.resize (1000);
	}
	
	//double curFun;
	std::vector <double> curFun;
	curFun.resize (elemConds.size());
	bestECs.resize (elemConds.size());
	bestECres.resize (elemConds.size());
	// ������ � ����� ������� ������ ������������ �������
	if (this->comments)
		std::cout << "\n\tTeaching elementary conditions: \n\n";
	#pragma omp parallel for schedule(dynamic, 1)
	for (int i = 0; i < (int) elemConds.size(); i++) {
		if (this->comments) {
			printf ("\r\t Elementary condition: %d out of %d.", (i+1), elemConds.size());
			std::cout.flush();
		}
		bestECs[i].resize(numBestECs);
		bestECres[i].resize(numBestECs);
		elemConds[i].elemCondition->setLimits (leftLimit, rightLimit);
		elemConds[i].elemCondition->setLimits (teachRow, reserve, numOfLevels);
		elemConds[i].elemCondition->changeParams (0);
		curFun[i] = matterECFunc (elemConds[i], param);
		storeBestECs (elemConds[i], curFun[i], bestECs[i], bestECres[i]);
		while (elemConds[i].elemCondition->changeParams (1) >= 0) {
			curFun[i] = matterECFunc (elemConds[i], param);
			storeBestECs (elemConds[i], curFun[i], bestECs[i], bestECres[i]);
		}
	}
	if (this->comments) {
		std::cout << "\n\n\tChoosing parameters for elem conds done. First stage is over.\n";
		std::cout.flush();
	}

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::matterAxiomFunc
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
double FuzzyDataLearnAlgorithm::matterAxiomFunc (AxiomExpr &ax, const int param) {
	// ���������� ����������� ����������
	double toReturn = 0.0;
	int numOfClasses, numOfNormalMultiTS;
	std::vector <int> numOfMultiTS, numOfNormalTS;
	std::vector < std::vector <int> > numOfTS;
	std::vector <double> curTS;
	bool bres = false;
	double maxMinRelation, lenRelation;
	int ethNum, ethInc, len, num, minLen, maxLen, wholeNum, hiLen, sumLen;
	// ��������� ���������� � ������ ������
	fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	if ((numOfClasses != numOfMultiTS.size()) || (numOfClasses != numOfTS.size()))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function.");
	// ���� �� ������� ���������� ���������� ��������� ��������� �������
	for (int i = 0; i < numOfClasses; i++) {
		if (numOfMultiTS[i] != numOfTS[i].size())
			throw AxiomLibException("FuzzyDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function*.");
		ethNum = 0;
		ethInc = 0;
		wholeNum = 0;
		maxMinRelation = 1.0;
		lenRelation = 0.0;
		for (int j = 0; j < numOfMultiTS[i]; j++) {
			curTS.clear();
			bres = fuzzyDataSet.getTSByIndexFromClass (curTS, i, j, param);
			if (bres) {
				if (curTS.size() < 1)
					throw AxiomLibException("FuzzyDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal request to dataSet.");
				len = axiomMarkUpStat (ax, curTS, num, minLen, maxLen);
				wholeNum += num;
				if (num > 0) {
					maxMinRelation = min (maxMinRelation, (double) minLen / (double) maxLen);
				}
				lenRelation = max (lenRelation, (double) len / (double) curTS.size());
				ethNum++;
				if (len > 0)
					ethInc++;
			}
		}
		// ����� �� ethInc
		if (ethInc > 0)
			toReturn += costEthIn * (double) ethInc / (double) ethNum;
		else
			toReturn -= costEthOut;
		// ����� �� wholeNum
		toReturn += costStrongPoint * ((double) (wholeNum + ethNum - abs(wholeNum - ethNum)) / (double) (wholeNum + ethNum));
		// ����� �� maxMinRelation � lenRelation
		if (wholeNum > 0) {
			// ����� �� maxMinRelation
			toReturn += costMaxMinLenRatio * maxMinRelation;
			// ����� �� lenRelation
			toReturn += costLenRatio * pow4(1.0 - lenRelation);
		}
	}
	// ���� �� ����������� ����������� ���������
	fuzzyDataSet.getNormalClassSize (numOfNormalMultiTS, numOfNormalTS);
	if (numOfNormalMultiTS != numOfNormalTS.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal function getNormalClassSize");
	sumLen = 0;
	hiLen = 0;
	wholeNum = 0;
	for (int j = 0; j < numOfNormalMultiTS; j++) {
		curTS.clear();
		bres = fuzzyDataSet.getNormalTS (curTS, j, param);
		if (bres) {
			if (curTS.size() < 1)
				throw AxiomLibException("FuzzyDataLearnAlgorithm::matterAxiomFunc : incorrect response from internal request to dataSet*.");
			len = axiomMarkUpStat (ax, curTS, num, minLen, maxLen);
			wholeNum += num;
			hiLen += len;
			sumLen += curTS.size();
		}
	}
	// ��������� ����� ��������� � 0 - ���� hiLen
	if (hiLen < 1)
		toReturn += costNormalOut;
	else
		toReturn -= costNormalIn * (double) hiLen / (double) sumLen;
	// ����� wholeNum
	toReturn += costHiAreasNum * hiAreasNumEffect / (hiAreasNumEffect + (double) wholeNum);

	return toReturn;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::axiomMarkUpStat
*
*	Description:	������� �������� �������� axiomExpr ���� curTS � ���� ����������
*	Parameters:		axiomExpr - �������, ��� ������� ������������ ���� ����������
*					row - ��������� ���, �� ������� ����������� ������� axiomExpr
*					hiAreasNum - ����� �������� ��������� (����� ��� ������ ����������� ������� axiomExpr) 
*					hiAreasMinLen - ����������� ����� ����� ��������
*					hiAreasMaxLen - ������������ ����� ����� ��������
*	Returns:		int - ����� ����� ���� row, � ������� ������� axiomExpr �����������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::axiomMarkUpStat (AxiomExpr &axiomExpr, std::vector <double> &row, int &hiAreasNum, int &hiAreasMinLen, int &hiAreasMaxLen) const {
	bool pred = false;
	std::vector <long> hiLens;
	unsigned long predStart = 0;
	unsigned long wholeLen = 0;
	hiLens.clear();
	hiAreasNum = 0;
	for (unsigned long i = 0; i < row.size(); i++) {
		if (axiomExpr.check (i, row) > 0) {
			if (!pred) {
				pred = true;
				predStart = i;
			}
		} else {
			if (pred) {
				pred = false;
				hiLens.push_back ((long) (i - predStart));
				wholeLen += i - predStart;
			}
		}
	}
	if (pred) {
		hiLens.push_back ((long) (row.size() - predStart));
		wholeLen += row.size() - predStart;
	}
	hiAreasNum = hiLens.size();
	long curMin, curMax;
	if (hiAreasNum > 0) {
		curMin = hiLens[0];
		curMax = hiLens[0];
		for (unsigned int i = 1; i < hiLens.size(); i++) {
			if (hiLens[i] > curMax)
				curMax = hiLens[i];
			if (hiLens[i] < curMin)
				curMin = hiLens[i];
		}
		hiAreasMinLen = (int) curMin;
		hiAreasMaxLen = (int) curMax;
	} else {
		hiAreasMinLen = 0;
		hiAreasMaxLen = 0;
	}
	return (int) wholeLen;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::getPriority
*
*	Description:	��������������� ������� ���������� ������� �� �������� 
*					� ���������� ����������� � ��������� ������ ��������.
*					�������� ��������� ������������ ������� �� ���������� � �� 
*					������������.
*	Parameters:		vec - ����������� ������
*					pos - ������ �������� ��� �������� ����������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::getPriority (std::vector <double> &vec, std::vector <unsigned int> &pos) const {
	pos.resize(vec.size()); 
	for (unsigned int i = 0; i < vec.size(); i++) 
		pos[i] = i;
	unsigned int temp;
	for (unsigned int i = vec.size() - 1; i > 0; i--) {
		for (unsigned int j = 0; j < i; j++) {
			if (vec[pos[j]] < vec[pos[j+1]]) {
				temp = pos[j+1];
				pos[j+1] = pos[j];
				pos[j] = temp;
			}
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::cutDownBestAxioms
*
*	Description:	������� ��������� ��������� ������������ ������ �� ������������� � ���������� ��������
*	Parameters:		void
*	Returns:		0 - ���� �������� ��������� �� ����
*					1 - � ��������� ������
*	Throws:			AxiomLibException - ���� ���������� ���������� ������ �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::cutDownBestAxioms (void) {
	// �������� ������� ����������
	if ((maxAxiomPopSize < 1) || (bestAxioms.size() <= (unsigned int) maxAxiomPopSize))
		return 0;
	// �������� ��������������� ���������� ����������
	if (bestAxioms.size() != bestAxiomsRes.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::cutDownBestAxioms : incorrect internal variables.");
	
	// ���������� ������ �� �������� ������� �������
	std::vector <unsigned int> pos;
	getPriority (bestAxiomsRes, pos);
	
	// ����������� ����� ������ ������, ����������� � ���������
	unsigned int numBestToSave, numToChoose, numFrom;
	numBestToSave = (unsigned int) round (percentBestAxioms*maxAxiomPopSize);
	numToChoose = (unsigned int) maxAxiomPopSize - numBestToSave;
	
	numFrom = bestAxioms.size() - numBestToSave;
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
	// ������� ���������� ��������� ������� ������
	if ((toSave[0] + 1) < bestAxioms.size()) {
		for (unsigned int j = toSave[0] + 1; j < bestAxioms.size(); j++)
			bestAxioms[j].clear();
		bestAxioms.erase (bestAxioms.begin() + toSave[0] + 1, bestAxioms.end());
		bestAxiomsRes.erase (bestAxiomsRes.begin() + toSave[0] + 1, bestAxiomsRes.end());
	}
	for (int i = 1; i < maxAxiomPopSize; i++) {
		if ((toSave[i] + 1) < toSave[i-1]) {
			// ������� ���������� ��������� ������� ������
			for (unsigned int j = toSave[i] + 1; j < toSave[i-1]; j++)
				bestAxioms[j].clear();
			bestAxioms.erase (bestAxioms.begin() + toSave[i] + 1, bestAxioms.begin() + toSave[i-1]);
			bestAxiomsRes.erase (bestAxiomsRes.begin() + toSave[i] + 1, bestAxiomsRes.begin() + toSave[i-1]);
		}
	}
	// ������� ���������� ��������� ������� ������
	if (toSave[maxAxiomPopSize-1] > 0) {
		for (unsigned int j = 0; j < toSave[maxAxiomPopSize-1]; j++)
			bestAxioms[j].clear();
		bestAxioms.erase (bestAxioms.begin(), bestAxioms.begin() + toSave[maxAxiomPopSize-1]);
		bestAxiomsRes.erase (bestAxiomsRes.begin(), bestAxiomsRes.begin() + toSave[maxAxiomPopSize-1]);
	}
	
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::combineAxioms
*
*	Description:	����������� ���� ������ � ���������� ���������� ��� ���������� ����� ������.
*	Parameters:		i - ����� ����� �� ������, �� ������ ������� ����� ��������� ����� �������� ������
*					j - ����� ����� �� ������, �� ������ ������� ����� ��������� ����� �������� ������
*					axiomExpr - ����������� ���������� �������, � ������ ���� ������� ��������� ������ �������
*					axiomExprRes - ��������� ������� ���������� ��� ������ ��������� �������
*	Returns:		0 - ���� ����� �������, ��������� �� ���� ��������� ��������� �� �����
*					>0 - ���� ��������� ������� ��������� ����� ����������������
*					-1 - ���� �� ������� �������� �������������� ��������� ������
*	Throws:			AxiomLibException - ���� ��������� ������� ��������� �� ����������� � ����������� ���������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::combineAxioms (const unsigned int i, const unsigned int j, AxiomExpr &axiomExpr, double &axiomExprRes, const int param) {
	// �������� ������� ����������
	if ((i >= bestAxioms.size()) || (j >= bestAxioms.size()) || (bestAxioms.size() != bestAxiomsRes.size()))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::combineAxioms : input parameters out of range.");
	
	// �������� ����� ������
	AxiomExpr axiomExprAnd, axiomExprOr;
	double axiomExprAndRes, axiomExprOrRes;
	axiomExprAnd.andExpr(bestAxioms[i], bestAxioms[j]);
	axiomExprOr.orExpr(bestAxioms[i], bestAxioms[j]);
	
	// ���������� ������� �������
	axiomExprAndRes = matterAxiomFunc (axiomExprAnd, param);
	axiomExprOrRes = matterAxiomFunc (axiomExprOr, param);

	// ����������� ����� �� ���������� �������� ������� ������� � ����� ������
	if ((axiomExprAndRes > max (bestAxiomsRes[i], bestAxiomsRes[j])) && (axiomExprAndRes > axiomExprOrRes)) {
		axiomExpr = axiomExprAnd;
		axiomExprRes = axiomExprAndRes;
		return 1;
	}
	if (axiomExprOrRes > max (bestAxiomsRes[i], bestAxiomsRes[j])) {
		axiomExpr = axiomExprOr;
		axiomExprRes = axiomExprOrRes;
		return 2;
	}

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::formAxioms
*
*	Description:	������� ������������ ������ �� ������������ �������
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::formAxioms (const int param) {
	// �������� ����, ��� �� ����� � ������� ��� ����������� ������ �����������
	if ((bestAxioms.size () != 0) || (bestAxiomsRes.size() != 0))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::formAxioms : set of axioms is not empty.");
	if (bestECs.size() < 1)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::formAxioms : set of prepared elementary conditions is empty.");
	unsigned int totSize = 0;
	for (unsigned int i = 0; i < bestECs.size(); i++)
		totSize += bestECs[i].size();
	if (totSize < 1)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::formAxioms : set of prepared elementary conditions is indeed empty.");

	// �������� ��������� ��������� ������, �� ������� ���� ��������� ������� ������ � ����������� ���������
	bestAxioms.resize(totSize);
	unsigned int k = 0;
	for (unsigned int i = 0; i < bestECs.size(); i++) {
		for (unsigned int j = 0; j < bestECs[i].size(); j++, k++) {
			bestAxioms[k].expression.resize(1);
			bestAxioms[k].expression[0].resize(1);
			bestAxioms[k].expression[0][0] = bestECs[i][j];
		}
	}
	// ��������� ��������� ���������� �� ���������
	bestAxiomsRes.resize(totSize);
	for (unsigned int i = 0; i < totSize; i++)
		bestAxiomsRes[i] = matterAxiomFunc (bestAxioms[i], param);

	// ���������� ���������� ��� ������ ������
	AxiomExpr axiomExpr;
	double axiomExprRes;
	int iterNum = 0;
	bool criteriaToContinue = true;
	int curSize;
	// ����������� ��������� ��������� 
	while (criteriaToContinue) {
		iterNum++;
		// �������, ���� ����������, ������ ������ �� ����������� ���������� ��������
		cutDownBestAxioms ();
		// ������� ����� �������� � ��������� �� �������� ������� �������
		curSize = bestAxioms.size();
		std::vector <AxiomExpr> bestAxiomsNew;
		std::vector <double> bestAxiomsResNew;
		bestAxiomsNew.resize(round ((double)((curSize - 1) * curSize) / 2.0));
		bestAxiomsResNew.resize(round ((double)((curSize - 1) * curSize) / 2.0));
		int curBest = 0;
		for (unsigned int i = 0; i < (unsigned int) (curSize - 1); i++) {
			for (unsigned int j = i+1; j < (unsigned int) curSize; j++) {
				if (combineAxioms (i, j, axiomExpr, axiomExprRes, param) > 0) {
					/*bestAxioms.resize(bestAxioms.size() + 1);
					bestAxioms[bestAxioms.size() - 1] = axiomExpr;
					bestAxiomsRes.resize(bestAxiomsRes.size() + 1);
					bestAxiomsRes[bestAxiomsRes.size() - 1] = axiomExprRes;
					*/
					bestAxiomsNew[curBest] = axiomExpr;
					bestAxiomsResNew[curBest] = axiomExprRes;
					curBest++;
				}
			}
		}
		// ������� ����� �������� � ����� bestAxioms
		if (curBest > 0) {
			bestAxioms.resize(bestAxioms.size() + curBest); 
			bestAxiomsRes.resize(bestAxiomsRes.size() + curBest); 
			for (int i = 0; i < curBest; i++) {
				bestAxioms[bestAxioms.size() - i - 1] = bestAxiomsNew[i];
				bestAxiomsRes[bestAxiomsRes.size() - i - 1] = bestAxiomsResNew[i];
			}
		}

		// ��������� �������� ��������
		if (curSize == bestAxioms.size())
			criteriaToContinue = false;
		if (iterNum > maxIterNum)
			criteriaToContinue = false;
		for (unsigned int i = 0; i < bestAxiomsRes.size(); i++)
			if (bestAxiomsRes[i] > maxAxiomRes)
				criteriaToContinue = false;
	}

	cutDownBestAxioms ();

	if (this->comments) {
		std::cout << "\n	Forming of axioms is done. Second stage is over.\n";
		std::cout.flush();
	}
	
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::createMarkUp
*
*	Description:	�������� ���� curTS �������� axiom � ������� ���������� � resMarkUp.
*	Parameters:		resMarkUp - �������� ���������� curTS �������� axiom
*					curTS - ����������� ����������
*					axiom - �������, ������� ���������� ��������
*					axNum - ����� ������� � ������� bestAxioms ������� ������
*	Returns:		int - ������ ����������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::createMarkUp (std::vector <int> &resMarkUp, std::vector <double> &curTS, AxiomExpr &axiom, const int axNum) const {
	resMarkUp.resize (curTS.size());
	for (unsigned long i = 0; i < curTS.size(); i++)
		if (axiom.check (i, curTS) > 0)
			resMarkUp[i] = axNum;
		else
			resMarkUp[i] = 0;
	return resMarkUp.size();
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::shiftErrors
*
*	Description:	��������������� ������� �������� ���������� - �����
*					�������� � ���������. 
*	Parameters:		markUp - �������� ���������� ����������� ���������
*					from - ����� �������, � �������� �������� 
*					upTo - ����� �������, �� �������� �������
*	Returns:		double - ����� �������� � ���������, ���������� �� 
*							 ��������������� ���
*	Throws:			AxiomLibException - ���� ������� ������ ������ �������
*	Author:			dk
*	History:
*
****************************************************************************/
inline double FuzzyDataLearnAlgorithm::shiftErrors (std::vector <int> &markUp, const int from, const int upTo) const {
	double toRet = 0;
	if ((from < 0) || (from > upTo) || (upTo > (int) markUp.size()))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::shiftErrors : incorrect input parameters.");
	int t = from;
	while (t < upTo) {
		t++;
		if (markUp[t-1] > 0) {
			toRet += 1.0;
			while (t < upTo) {
				if (markUp[t] != markUp[t-1])
					break;
				t++;
			}
		}
	}
	return toRet*shiftErrIn;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::shiftErrors
*
*	Description:	��������������� ������� �������� ���������� - �����
*					������������ �������� �������� �� ���� ����������. 
*	Parameters:		row1 - �������� ����� ���������� ����������� ���������
*					from1 - ����� �������, � �������� �������� �� row1
*					row2 - �������� ������ ���������� ����������� ���������
*					from2 - ����� �������, � �������� �������� �� row2
*					len - ������� �������� ���������� ����������
*	Returns:		double - ����� ������������ �������� � ���������, 
*							 ���������� �� ��������������� ���
*	Throws:			AxiomLibException - ���� ������� ������ ������ �������
*	Author:			dk
*	History:
*
****************************************************************************/
inline double FuzzyDataLearnAlgorithm::shiftErrors (std::vector <int> &row1, const int from1, std::vector <int> &row2, const int from2, const int len) const {
	if ((from1 < 0) || (from2 < 0) || ((from1 + len) > (int) row1.size()) || ((from2 + len) > (int) row2.size()))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::shiftErrors for 2 rows : incorrect input parameters.");
	
	bool flag;
	double toRet = 0;
	int t = 0;
	
	// ����� �� ������� ���� � ������� �� �������, ������� �� ����� ��������� �� ������ ����
	while (t < len) {
		if (row1[from1 + t] > 0) {
			flag = false;
			while (t < len) {
				if (row1[from1 + t] == row2[from2 + t]) {
					flag = true;
					t++;
					while (t < len) {
						if (row1[from1 + t - 1] != row1[from1 + t])
							break;
						t++;
					}
					break;
				}
				t++;
				if (row1[from1+t-1] != row1[from1+t])
					break;
			}
			if (!flag)
				toRet += 1.0;
		}
		t++;
	}

	// ����� �� ������� ���� � ������� �� �������, ������� �� ����� ��������� � ������ ����
	t = 0;
	while (t < len) {
		if (row2[from2 + t] > 0) {
			flag = false;
			while (t < len) {
				if (row2[from2 + t] == row1[from1 + t]) {
					flag = true;
					t++;
					while (t < len) {
						if (row2[from2 + t - 1] != row2[from2 + t])
							break;
						t++;
					}
					break;
				}
				t++;
				if (row2[from2+t-1] != row2[from2+t])
					break;
			}
			if (!flag)
				toRet += 1.0;
		}
		t++;
	}

	return toRet*shiftErrIn;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::moveVal
*
*	Description:	��������������� ������� ���������� �������� newMarkUp � newMarkUpSens 
*					�� ������� markUp � ��������� ������ �������� ����������.
*	Parameters:		newMarkUp - ����� ���������� ���������� ��������
*					newMarkUpSens - ������ ������ ������������� �������� � newMarkUpSens
*					newFrom - ����� ������, � ������� �������� �� newMarkUp
*					markUp - �������� ���������� ����������� ���������
*					from - ����� �������, � �������� �������� 
*					upTo - ����� �������, �� �������� �������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������� ������ ������ �������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::moveVal (std::vector <int> &newMarkUp, std::vector <int> &newMarkUpSens, const int newFrom, std::vector <int> &markUp, const int from, const int upTo) const {
	if ((from < 0) || (from > upTo) || (upTo > (int) markUp.size())
		|| (newFrom < 0) || (newMarkUp.size() != newMarkUpSens.size()) || ((upTo - from + newFrom) > (int) newMarkUp.size()) )
		throw AxiomLibException("FuzzyDataLearnAlgorithm::moveVal I : incorrect input parameters.");
	for (int i = from, j = newFrom; i < upTo; i++, j++) {
		newMarkUp[j] = markUp[i];
		if (markUp[i] > 0) 
			newMarkUpSens[j] = 1;
		else
			newMarkUpSens[j] = 0;
	}
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::moveVal
*
*	Description:	��������������� ������� ���������� �������� newMarkUp � newMarkUpSens 
*					�� ������ base, sens � ��������� ������ �������� ����������.
*	Parameters:		newMarkUp - ����� ���������� ���������� ��������
*					newMarkUpSens - ������ ������ ������������� �������� � newMarkUpSens
*					newFrom - ����� ������, � ������� �������� �� newMarkUp
*					base - ������ ���������� ���������� ��������
*					sens - ������ ������ ������
*					from - ����� �������, � �������� �������� 
*					upTo - ����� �������, �� �������� �������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������� ������ ������ �������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::moveVal (std::vector <int> &newMarkUp, std::vector <int> &newMarkUpSens, const int newFrom, std::vector <int> &base, std::vector <int> &sens, const int from, const int upTo) const {
	if ((from < 0) || (from > upTo) || (base.size() != sens.size()) || (upTo > (int) base.size())
		|| (newFrom < 0) || (newMarkUp.size() != newMarkUpSens.size()) || ((upTo - from + newFrom) > (int) newMarkUp.size()) )
		throw AxiomLibException("FuzzyDataLearnAlgorithm::moveVal II : incorrect input parameters.");
	for (int i = from, j = newFrom; i < upTo; i++, j++) {
		newMarkUp[j] = base[i];
		newMarkUpSens[j] = sens[i];	
	}
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::moveVal
*
*	Description:	��������������� ������� ���������� �������� newMarkUp � newMarkUpSens 
*					�� ������� markUp � �������� base, sens � ��������� ������ �������� ����������.
*	Parameters:		newMarkUp - ����� ���������� ���������� ��������
*					newMarkUpSens - ������ ������ ������������� �������� � newMarkUpSens
*					newFrom - ����� ������, � ������� �������� �� newMarkUp
*					base - ������ ���������� ���������� ��������
*					sens - ������ ������ ������
*					fromBase - ����� �������, � �������� �������� �� base
*					markUp - �������� ���������� ����������� ���������
*					fromMarkUp - ����� �������, � �������� �������� 
*					len - ����� ��������������� �������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������� ������ ������ �������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::moveVal (std::vector <int> &newMarkUp, std::vector <int> &newMarkUpSens, const int newFrom, std::vector <int> &base, std::vector <int> &sens, const int fromBase, std::vector <int> &markUp, const int fromMarkUp, const int len) const {
	if ((fromBase < 0) || (base.size() != sens.size()) || ((fromBase + len) > (int) base.size())
		|| (fromMarkUp < 0) || ((fromMarkUp + len) > (int) markUp.size())
		|| (newFrom < 0) || (newMarkUp.size() != newMarkUpSens.size()) || ((newFrom + len) > (int) newMarkUp.size()) )
		throw AxiomLibException("FuzzyDataLearnAlgorithm::moveVal III : incorrect input parameters.");
	for (int i = fromBase, j = newFrom, k = fromMarkUp; i < (fromBase + len); i++, j++, k++) {
		if (markUp[k] > 0) {
			newMarkUp[j] = markUp[k];
			if (base[i] == markUp[k]) {
				newMarkUpSens[j] = sens[i] + 1;
			} else {
				newMarkUpSens[j] = 1;
			}
		} else { 
			newMarkUp[j] = base[i];
			newMarkUpSens[j] = sens[i];
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::createBaseMarkUp
*
*	Description:	C���� �������� �������� � ������������ ���������� ��������
*	Parameters:		sens - ����������� ���������� �������� (����� ���������� �������� � ������ �������)
*					markUps - �������� ���������� ����������� ��������� ������ 
*								���� � ��������� ����� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::createBaseMarkUp (std::vector <int> &sens, std::vector < std::vector <int> > &markUps) const {
	int  lastStart, newStart;
	double lastScore, newScore;
	bool flag = true;
	std::vector <int> base, newBase, newSens;
	base.clear();
	sens.clear();
	newBase.clear();
	newSens.clear();
	for (int i = 0; i < (int) markUps.size(); i++) {
		if (markUps[i].size() > 0) {
			if (flag) {
				base = markUps[i];
				sens.resize(base.size());
				for (unsigned int h = 0; h < base.size(); h++) {
					if (base[h] > 0) sens[h] = 1;
					else sens[h] = 0;
				}
				flag = false;
			} else {
				lastStart = 0;
				lastScore = -1.0;
				// ����� �� ����� � base
				for (int y = 0; y < (int) base.size(); y++) {
					if (base[y] > 0) {
						// ��� ������� ���� � base ����� �� ���� ����� markUps[i]
						for (int z = 0; z < (int) markUps[i].size(); z++) {
							// ��� ������� ���� � base � � markUps[i] - ������� ����� ����������
							if (markUps[i][z] > 0) {
								newStart = y - z;
								if (newStart < 0) {
									// �� 0 �� z - y ����� �� markUps[i]
									newScore = shiftErrors (markUps[i], 0, z-y);
									if ((markUps[i].size() + newStart) > base.size()) {
										// �� 0 �� base.size() ����� �� base � �� markUps[i]
										newScore += shiftErrors (base, 0, markUps[i], z-y, base.size());
										// �� base.size() - newStart �� markUps[i].size() �� markUps[i]
										newScore += shiftErrors (markUps[i], base.size() - newStart, markUps[i].size());
									} else {
										// �� 0 �� markUps[i].size() + newStart �� base � �� markUps[i]
										newScore += shiftErrors (base, 0, markUps[i], z-y, markUps[i].size() + newStart);
										// �� markUps[i].size() + newStart �� base.size() �� base
										newScore += shiftErrors (base, markUps[i].size() + newStart, base.size());
									}
								} else {
									// �� 0 �� y - z ����� �� base
									newScore = shiftErrors (base, 0, newStart);
									if ((markUps[i].size() + newStart) > base.size()) {
										// �� y - z �� base.size() ����� �� base � �� markUps[i] �� 0 �� base.size() - y + z
										newScore += shiftErrors (base, newStart, markUps[i], 0, base.size() - newStart);
										// �� base.size() - newStart �� markUps[i].size() �� markUps[i]
										newScore += shiftErrors (markUps[i], base.size() - newStart, markUps[i].size());
									} else {
										// �� y-z �� markUps[i].size() + newStart �� base � �� markUps[i]
										newScore += shiftErrors (base, newStart, markUps[i], 0, markUps[i].size());
										// �� markUps[i].size() + newStart �� base.size() �� base
										newScore += shiftErrors (base, markUps[i].size() + newStart, base.size());
									}
								}
								// ���������� ����� ���������� ��� ������� ������ (��� ������ ���� �����)
								// ����� ��������� ������ �������� �������������� ������
								if ((lastScore < 0) || (newScore < lastScore)) {
									lastScore = newScore;
									lastStart = newStart;
								}
							}
						}
					}
				}
				// ��������� �� ���� ����� � ��������� ��������� ������ - ������ ���������� ���������� base � ������������ � ������ ��������� ������
				if (lastStart < 0) {
					if ((markUps[i].size() + lastStart) > base.size()) {
						newBase.resize(markUps[i].size());
						newSens.resize(markUps[i].size());
						// �� 0 �� -lastStart ����� �� markUps[i]
						moveVal (newBase, newSens, 0, markUps[i], 0, -lastStart);
						// �� 0 �� base.size() ����� �� base � �� markUps[i]
						moveVal (newBase, newSens, -lastStart, base, sens, 0, markUps[i], -lastStart, base.size());
						// �� base.size() - lastStart �� markUps[i].size() �� markUps[i]
						moveVal (newBase, newSens, base.size() - lastStart, markUps[i], base.size() - lastStart, markUps[i].size());
					} else {
						newBase.resize(base.size() - lastStart);
						newSens.resize(base.size() - lastStart);
						// �� 0 �� -lastStart ����� �� markUps[i]
						moveVal (newBase, newSens, 0, markUps[i], 0, -lastStart);
						// �� 0 �� markUps[i].size() + lastStart �� base � �� markUps[i]
						moveVal (newBase, newSens, -lastStart, base, sens, 0, markUps[i], -lastStart, markUps[i].size() + lastStart);
						// �� markUps[i].size() + lastStart �� base.size() �� base
						moveVal (newBase, newSens, markUps[i].size(), base, sens, markUps[i].size() + lastStart, base.size());
					}
				} else {
					if ((markUps[i].size() + lastStart) > base.size()) {
						newBase.resize(markUps[i].size() + lastStart);
						newSens.resize(markUps[i].size() + lastStart);
						// �� 0 �� lastStart ����� �� base
						moveVal (newBase, newSens, 0, base, sens, 0, lastStart);
						// �� y - z �� base.size() ����� �� base � �� markUps[i] �� 0 �� base.size() - y + z
						moveVal (newBase, newSens, lastStart, base, sens, lastStart, markUps[i], 0, base.size() - lastStart);
						// �� base.size() - lastStart �� markUps[i].size() �� markUps[i]
						moveVal (newBase, newSens, base.size(), markUps[i], base.size() - lastStart, markUps[i].size());
					} else {
						newBase.resize(base.size());
						newSens.resize(base.size());
						// �� 0 �� lastStart ����� �� base
						moveVal (newBase, newSens, 0, base, sens, 0, lastStart);
						// �� y-z �� markUps[i].size() + lastStart �� base � �� markUps[i]
						moveVal (newBase, newSens, lastStart, base, sens, lastStart, markUps[i], 0, markUps[i].size());
						// �� markUps[i].size() + lastStart �� base.size() �� base
						moveVal (newBase, newSens, markUps[i].size() + lastStart, base, sens, markUps[i].size() + lastStart, base.size());
					}
				}
				base = newBase;
				sens = newSens;
			}
		}
	}

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::formGeneralizedMarkUps
*
*	Description:	������������ ���������� �������� ��� ��������� ���� 
*					����������� ��������� � �������� �������:
*					�������� �������� ���� ��������� ���������� ��� ��������� 
*					���� ����������� ��������� � ��� ���� ������. �������� ��� 
*					�������� �� ���������� ������������ � ��������� ���������� 
*					��������.
*	Parameters:		baseMarkUps - ���������� �������� �� ��������� ��������
*					resMarkUps - ������ ���� �������� ��� ������ �������
*					i - ����� ������ ����������� ��������� � ������ ������, 
*						������� ������������ ��� �������
*					axNum - ����� ������� � ������� bestAxioms, ������� ������������
*							� ������ ���������
*					param - ����������� ������ ������ �� ������� ����� ������
*	Returns:		0
*	Throws:			AxiomLibException - ��� ������������ ������ �� ������ ������ ��� ������������ ������� ����������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::formGeneralizedMarkUps (std::vector <int> &baseMarkUp, std::vector < std::vector <int> > &resMarkUps, int axNum, int i, int numOfMultiTS, const int param) {
	// �������� ������� ����������
	if (axNum >= (int) bestAxioms.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::formGeneralizedMarkUps : incorrect input parameters.");
	// �������� �������� ���� ���������� ����������� ��������� ���� i ��� ������� axNum
	bool bres;
	std::vector <double> curTS;
	resMarkUps.resize (numOfMultiTS);
	for (int j = 0; j < numOfMultiTS; j++) {
		bres = fuzzyDataSet.getTSByIndexFromClass (curTS, i, j, param);
		if (bres) {
			if (curTS.size() < 1)
				throw AxiomLibException("FuzzyDataLearnAlgorithm::formGeneralizedMarkUps : incorrect response from internal request to dataSet.");
			// �������� ���������� curTS
			createMarkUp (resMarkUps[j], curTS, bestAxioms[axNum], axNum+1);
		}
	}
	// �� ��������� ���������� �������� ���������� ����������� ��������� - ���������� �� ����� � ��������� ���������� ��������
	createBaseMarkUp (baseMarkUp, resMarkUps);
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::simplifyMarkUp
*
*	Description:	��������� �������� � ���������� �� � ����������� ��������
*	Parameters:		simp - ����������� � ���������� ������� ��������
*					len - ������ ���� ��������� ��������� �������� �� simp � 
*						  �������� �������� row
*					row - ������������� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::simplifyMarkUp (std::vector <int> &simp, std::vector <int> &len, std::vector <int> &row, const int axNum) const {
	simp.clear();
	len.clear();
	// ������� ��� 0 � ������ ������� - �� �� ���������
	unsigned int i = 0;
	while (i < row.size()) {
		if (row[i] > 0)
			break;
		i++;
	}
	// �� ��������� ��� 0 � ������ �������
	unsigned int upTo = row.size() - 1;
	while (upTo > i) {
		if (row[upTo] > 0)
			break;
		upTo--;
	}
	upTo++;
	// ���� ���� ������ 0 - �� ������� �� �������
	if (((i+1) == upTo) && (row[i] == 0))
		return -1;
	// ��������� �� ���� �������������� ��������� � ���������� ������������ �������� �������
	int maxVal = 0;
	for (unsigned int k = i; k < upTo; k++) {
		if (row[k] > maxVal)
			maxVal = row[k];
	}
	// ���� �� ���� �������������� ��������� ������ row
	unsigned int count = i;
	unsigned int j = 0;
	while ( i < upTo ) {
		if (row[i] == 0) {
			simp.push_back (axNum);
			len.push_back (i - count);
			count = i;
			while (i < upTo) {
				if (row[i] > 0)
					break;
				i++;
			}
			simp.push_back (0);
			len.push_back(i - count);
			count = i;
		}
		i++;
	}
	simp.push_back (axNum);
	len.push_back (i - count);
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::subcheck 
*
*	Description:	������� �������� ���������� ����� �� �������� � ������.
*	Parameters:		rowAxiomTypes - ������� ��� ������ (�������� ����)
*					rowAxiomLen - ��������� ��������
*					upTo - ����� ���� ������ � ������� ������ ������ ��������
*					curEnd - ������� ��������� � ��������� �������� ��� ���������
*	Returns:		0 - false
*					1 - true
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::subcheck (std::vector<int> &rowAxiomTypes, std::vector<int> &rowAxiomLen, int upTo, std::vector<int> &axiomTypes, std::vector<int> &axiomMinLen, std::vector<int> &axiomMaxLen, int curEnd) const {
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
			flag = this->subcheck (rowAxiomTypes, rowAxiomLen, j, axiomTypes, axiomMinLen, axiomMaxLen, curEnd - 1);
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


/****************************************************************************
*					FuzzyDataLearnAlgorithm::splitMarkUps
*
*	Description:	����������� ����������� ��������
*	Parameters:		markUp - ����� ������������ ��������
*					markUpMinLen - ����������� ����� ��������� ������ � ����� ��������
*					markUpMaxLen - ������������ ����� ��������� ������ � ����� ��������
*					set - ������� ��������, ������� ����������
*					markUps - ������ ��������, �� ������� �������� �������� ��� �����������
*					markUpsMinLen - ������ ����������� ��������� ������ ��������
*					markUpsMaxLen - ������ ������������ ��������� ������ ��������
*	Returns:		0
*	Throws:			AxiomLibException - 
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::splitMarkUps (std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <int> &set, std::vector < std::vector <int> > &markUps, std::vector < std::vector <int> > &markUpsMinLen, std::vector < std::vector <int> > &markUpsMaxLen) const {
	// ���� ������ ��������
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::countMarkUpEfficiency
*
*	Description:	������� ������������� �������� �� ����������� ��������� �������
*	Parameters:		markUp - ����������� ��������
*					markUpMinLen - ����������� ����� ��������� ������ � ��������
*					markUpMaxLen - ������������ ����� ��������� ������ � ��������
*					localMarkUps - �������� ���������� ��������� �������
*					localMarkUpsEngaged - ����������� �� �������� �������� 
*								          ���������� ��������� �������
*	Returns:		int - ����� ��������� �������� � �������� ��������� �������
*	Throws:			AxiomLibException - ���� ������� ��������� localMarkUpsEngaged � 
*					localMarkUps ������ �����
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::countMarkUpEfficiency (std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <std::vector <int> > &localMarkUps, std::vector <bool> &localMarkUpsEngaged) const {
	// �������� ������� ������
	if (localMarkUpsEngaged.size() != localMarkUps.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::countMarkUpEfficiency : incorrect input parameters.");
	// ����� �� ��������� ��������� ��������� ������� � �������� ��, ��� ����� ��������� ��������
	std::vector <int> row, rowLen;
	int rowSize;
	int toRet = 0;
	for (unsigned int tsNum = 0; tsNum < localMarkUps.size(); tsNum++) {
		if (!localMarkUpsEngaged[tsNum])
			continue;
		if (localMarkUps[tsNum].size() < 1) {
			localMarkUpsEngaged[tsNum] = false;
			continue;
		}
		row.resize(localMarkUps[tsNum].size());
		rowLen.resize(localMarkUps[tsNum].size());
		row[0] = localMarkUps[tsNum][0];
		rowLen[0] = 1;
		rowSize = 0; // ��������� ������ ��������� ������� � �������� row � rowLen
		for (unsigned int i = 1; i < localMarkUps[tsNum].size(); i++) {
			// �������� ��� ����������� ��������
			if (localMarkUps[tsNum][i] != localMarkUps[tsNum][i-1]) {
				rowSize++;
				row[rowSize] = localMarkUps[tsNum][i];
				rowLen[rowSize] = 1;
			}
			else {
				rowLen[rowSize]++;
			}
			// ���������� ���� �� ���������� � �������� ���������
			if (subcheck (row, rowLen, rowSize, markUp, markUpMinLen, markUpMaxLen, (int) markUp.size() - 1) > 0) {
				toRet++;
				break;
			}
		}
	}
	return toRet;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::setEngagedMarkUps
*
*	Description:	���������� ����������� �� �������� �������� ���������� 
*					��������� ������� � ������������ � ����������, ����������� 
*					�������� markUp.
*	Parameters:		markUp - ����������� ��������
*					markUpMinLen - ����������� ����� ��������� ������ � ��������
*					markUpMaxLen - ������������ ����� ��������� ������ � ��������
*					localMarkUps - �������� ���������� ��������� �������
*					localMarkUpsEngaged - ����������� �� �������� �������� 
*								          ���������� ��������� �������
*	Returns:		bool - �������� �� ��� �������� ���������� ��������� ������� 
*						�� �������� ������� ��������� ���������� ���������� ���������
*	Throws:			AxiomLibException - ���� ������� ��������� localMarkUpsEngaged � 
*					localMarkUps ������ �����
*	Author:			dk
*	History:
*
****************************************************************************/
bool FuzzyDataLearnAlgorithm::setEngagedMarkUps (std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <std::vector <int> > &localMarkUps, std::vector <bool> &localMarkUpsEngaged) const {
	// �������� ������� ������
	if (localMarkUpsEngaged.size() != localMarkUps.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::setEngagedMarkUps : incorrect input parameters.");
	// ����� �� ��������� ��������� ��������� ������� � �������� ��, ��� ����� ��������� ��������
	std::vector <int> row, rowLen;
	int rowSize;
	bool toRet = false;
	for (unsigned int tsNum = 0; tsNum < localMarkUps.size(); tsNum++) {
		if (!localMarkUpsEngaged[tsNum])
			continue;
		if (localMarkUps[tsNum].size() < 1) {
			localMarkUpsEngaged[tsNum] = false;
			continue;
		}
		row.resize(localMarkUps[tsNum].size());
		rowLen.resize(localMarkUps[tsNum].size());
		row[0] = localMarkUps[tsNum][0];
		rowLen[0] = 1;
		rowSize = 0; // ��������� ������ ��������� ������� � �������� row � rowLen
		for (unsigned int i = 1; i < localMarkUps[tsNum].size(); i++) {
			// �������� ��� ����������� ��������
			if (localMarkUps[tsNum][i] != localMarkUps[tsNum][i-1]) {
				rowSize++;
				row[rowSize] = localMarkUps[tsNum][i];
				rowLen[rowSize] = 1;
			}
			else {
				rowLen[rowSize]++;
			}
			// ���������� ���� �� ���������� � �������� ���������
			if (subcheck (row, rowLen, rowSize, markUp, markUpMinLen, markUpMaxLen, (int) markUp.size() - 1) > 0) {
				localMarkUpsEngaged[tsNum] = false;
				break;
			}
		}
		if (localMarkUpsEngaged[tsNum]) {
			toRet = true;
		}
	}
	return toRet;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::expandMarkUp
*
*	Description:	�������������� ����������� �������� � ������� ��������
*	Parameters:		simpleMarkUp - ��������� ��������������, ������� ��������
*					markUp - ����������� ��������
*					markUpMinLen - ����������� ����� ��������� ������ � ����������� ��������
*					markUpMaxLen - ������������ ����� ��������� ������ � ����������� ��������
*	Returns:		0
*	Throws:			AxiomLibException - 
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::expandMarkUp (std::vector <int> &simpleMarkUp, std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen) const {
	// ������ �������� �� ����� ������������
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::extendMarkUpLen
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
inline int FuzzyDataLearnAlgorithm::extendMarkUpLen (std::vector <int> &stdLen, std::vector <int> &minLen, std::vector <int> &maxLen) const {
	minLen.resize (stdLen.size());
	maxLen.resize (stdLen.size());
	for (unsigned int i = 0; i < stdLen.size(); i++) {
		minLen[i] = max (0, round (((double) stdLen[i]) * (1.0 - extendMarkUpLensLevel)));
		maxLen[i] = round (((double) stdLen[i]) * (1.0 + extendMarkUpLensLevel));
	}
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::addResults
*
*	Description:	������� ��������� ��� ����������� ������������� ��������� 
*					�������� � �������� �������������� ����
*	Parameters:		result - ����������� ��� ����������� �������������
*					markUp - ����������� ��������
*					markUpMinLen - ����������� ����� ��������� ������ � ��������
*					markUpMaxLen - ������������ ����� ��������� ������ � ��������
*					wholeRow - �������� �������������� ���� 
*					indicator - ������ ������ �������������, �� �������� ������� ������
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������� ��������� localMarkUpsEngaged � 
*					localMarkUps ������ �����
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::addResults (std::vector <int> &result, std::vector <int> &markUp, std::vector <int> &markUpMinLen, std::vector <int> &markUpMaxLen, std::vector <int> &wholeRow, int indicator) const {
	// �������� ������� ������
	if (result.size() != wholeRow.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::addResults : incorrect input parameters.");
	if (wholeRow.size() < 1)
		return -1;
	std::vector <int> row, rowLen;
	int rowPos;
	int markUpPos = markUp.size() - 1;
	row.resize(wholeRow.size());
	rowLen.resize(wholeRow.size());
	row[0] = wholeRow[0];
	rowLen[0] = 1;
	rowPos = 0; // ��������� ������ ��������� ������� � �������� row � rowLen
	for (unsigned int i = 1; i < wholeRow.size(); i++) {
		// �������� ��� ����������� ��������
		if (wholeRow[i] != wholeRow[i-1]) {
			rowPos++;
			row[rowPos] = wholeRow[i];
			rowLen[rowPos] = 1;
		} else {
			rowLen[rowPos]++;
		}
		// ���������� ���� �� ���������� � �������� ���������
		if (subcheck (row, rowLen, rowPos, markUp, markUpMinLen, markUpMaxLen, markUpPos) > 0) {
			result[i] = indicator;
		}
	}
	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::countExtraStat
*
*	Description:	������� �������� ���������� ��� ������� �������� �� ����������� 
*					������� � �� ��������� ���������� ����������� ���������.
*	Parameters:		curSet - ������� ��������, ������� ���������� � �������������
*					simpMarkUps - ������ ���� �������� ��� ���������� ����������� ���������
*					simpMarkUpsMinLens - ������ ����������-������������ ����� ��������� �������� ��������� �������� � ��������
*					simpMarkUpsMinLens - ������ ������������� ����� ��������� �������� ��������� �������� � ��������
*					extraMarkUps - �������� ���� ���������� ����������� ������� � 
*								   ���������� ����������� ��������� ����� ���������.
*					correctExtraMarkUps - ��������� ���������� ����������� ��������� 
*								   ��� extraMarkUps.
*					indicator - ������ ������ �������������, �� �������� ������� ������
*	Returns:		int - �������� ���������� �� ����� ������ ������� � ������� ����, ���������� ��� �������������
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::countExtraStat (std::vector <int> &curSet, std::vector < std::vector <int> > &simpMarkUps, std::vector < std::vector <int> > &simpMarkUpsMinLens, std::vector < std::vector <int> > &simpMarkUpsMaxLens, std::vector < std::vector <std::vector <int> > > &extraMarkUps, std::vector <std::vector <int> > &correctExtraMarkUps, const int indicator) const {
	int toRet = 0;
	int wholeNum, inpSize, first, second;
	double tmpFirst, tmpSecond;
	std::vector <int> result;

	// �������� ������� ����������
	if (extraMarkUps.size() < 1)
		throw AxiomLibException("FuzzyDataLearnAlgorithm::countExtraStat : input params are clear");
	
	inpSize = extraMarkUps[0].size();
	
	for (unsigned int setItr = 0; setItr < curSet.size(); setItr++)
		if (extraMarkUps[curSet[setItr]].size() != inpSize)
			throw AxiomLibException("FuzzyDataLearnAlgorithm::countExtraStat : input params are corrupted");

	first = 0;
	second = 0;
	// ���� �� ���� ����� ����������� �������
	for (int rowItr = 0; rowItr < inpSize; rowItr++) {
		result.resize (extraMarkUps[0][rowItr].size(), 0);
		for (unsigned int setItr = 0; setItr < curSet.size(); setItr++) {
			addResults (result, simpMarkUps[curSet[setItr]], simpMarkUpsMinLens[curSet[setItr]], simpMarkUpsMaxLens[curSet[setItr]], extraMarkUps[curSet[setItr]][rowItr], indicator);
		}
		wholeNum = compareStatistic->getStatistic(result, correctExtraMarkUps[rowItr], tmpFirst, tmpSecond, comments);
		first += (int) tmpFirst;
		second += (int) tmpSecond;
	}

	// ���������� �������� ������� �������
	toRet = round (goalStrategy->compute (first, second));

	return toRet;
}

/****************************************************************************
*					FuzzyDataLearnAlgorithm::createCandidates
*
*	Description:	������������ ���������� � �������� ��� ��������� ���� 
*					����������� ���������  
*	Parameters:		candMarkUps - ����������� ��������� � ��������
*					candMarkUpsMinLen - ����������� ����� ��������� ������ � ��������
*					candMarkUpsMaxLen - ������������ ����� ��������� ������ � ��������
*					baseMarkUps - ���������� �������� �� ��������� ��������
*					localMarkUps - �������� ���� ���������� ��������� �������
*								   ����� ���������.
*					extraMarkUps - �������� ���� ���������� ����������� ������� � 
*								   ���������� ����������� ��������� ����� ���������.
*					correctExtraMarkUps - ��������� ���������� ����������� ��������� 
*								   ��� extraMarkUps.
*					indicator - ������ ������ �������������, �� �������� ������� ������
*	Returns:		0
*	Throws:			AxiomLibException - ���� �� ������� ���������� ������ ����������
*	Author:			dk
*	History:
*
****************************************************************************/
inline int FuzzyDataLearnAlgorithm::createCandidates (std::vector < std::vector <int> > &candMarkUps, std::vector < std::vector <int> > &candMarkUpsMinLens, std::vector < std::vector <int> > &candMarkUpsMaxLens, std::vector < std::vector <int> > &baseMarkUps, std::vector < std::vector <std::vector <int> > > &localMarkUps, std::vector < std::vector <std::vector <int> > > &extraMarkUps, std::vector <std::vector <int> > &correctExtraMarkUps, const int indicator) const {
	std::vector < std::vector <int> > simpMarkUps;
	std::vector < std::vector <int> > simpMarkUpsLens, simpMarkUpsMinLens, simpMarkUpsMaxLens;
	// �������� ����� ��������
	if (baseMarkUps.size() < 1)
		return 0;		
	simpMarkUps.resize (baseMarkUps.size());
	simpMarkUpsLens.resize(baseMarkUps.size());
	simpMarkUpsMinLens.resize(baseMarkUps.size());
	simpMarkUpsMaxLens.resize(baseMarkUps.size());
	// ��������� ���� ���������� �������� � ���������� �� � ����������� �����
	for (int i = 0; i < (int) baseMarkUps.size(); i++) {
		simplifyMarkUp (simpMarkUps[i], simpMarkUpsLens[i], baseMarkUps[i], i+1);
		extendMarkUpLen (simpMarkUpsLens[i], simpMarkUpsMinLens[i], simpMarkUpsMaxLens[i]);
	}
	// �� ������ ����������� ���������� ���������� �������� - ��������� ��������� � �������� ��� ������� ���� ����������� ���������
	// ��������� ������� ���������
	for (unsigned int axNum = 1; axNum < localMarkUps.size(); axNum++) {
		if (localMarkUps[axNum].size() != localMarkUps[axNum-1].size()) 
			throw AxiomLibException("FuzzyDataLearnAlgorithm::createCandidates : different number of TimeSeries per different axioms - incompatible condition");
	}
	if (localMarkUps.size() < 1) 
		throw AxiomLibException("FuzzyDataLearnAlgorithm::createCandidates : localMarkUps variable is undefined");
	// ������� ����������� ����������
	std::vector < bool > localMarkUpsEngaged;
	localMarkUpsEngaged.resize (localMarkUps[0].size(), true);
	std::vector <int> curMarkUp, newMarkUp, toSave; 
	std::vector <int> curMarkUpMinLen, curMarkUpMaxLen, newMarkUpMinLen, newMarkUpMaxLen, toSaveMinLen, toSaveMaxLen;
	curMarkUp.clear();
	curMarkUpMinLen.clear();
	curMarkUpMaxLen.clear();
	int curRes, newRes, bestNum, curStart, lastRes;
	bool flag;
	std::vector < std::vector <int> *> curMarkUps, curMarkUpsMinLens, curMarkUpsMaxLens;
	curMarkUps.clear();
	curMarkUpsMinLens.clear();
	curMarkUpsMaxLens.clear();
	std::vector <int> curSet, lastSet;
	lastRes = -1;
	lastSet.clear();
	// ���� �� ���� ��������� - � ����� �������� ����� �������� ����������� �������� ��������� ��������
	for (int base = 0; base < (int) simpMarkUps.size(); base++) {
		// �������� � ������ �������� �������� � �������� base
		localMarkUpsEngaged.resize (localMarkUps[0].size(), true);
		curSet.resize (1, base);
		flag = setEngagedMarkUps (simpMarkUps[base], simpMarkUpsMinLens[base], simpMarkUpsMaxLens[base], localMarkUps[base], localMarkUpsEngaged);
		// ���������� ���������� �������� �� �������������
		curStart = base;
		while (flag) {
			bestNum = -1;
			curRes = 0;
			for (int i = curStart + 1; i < (int) simpMarkUps.size(); i++) {
				newRes = countMarkUpEfficiency (simpMarkUps[i], simpMarkUpsMinLens[i], simpMarkUpsMaxLens[i], localMarkUps[i], localMarkUpsEngaged);
				if (newRes > curRes) {
					curRes = newRes;
					bestNum = i;
				}
			}
			// �������� - ���� �� ������� �������� ����� �������
			if (bestNum < 0) {
				// ���� �������� ����� �� ���� ������� - �� ������� �� �����
				flag = false;
				break;
			}
			// ���� ����� �������� ����� - �� ��������� ��
			curSet.push_back (bestNum);
			// ������ false �� ��� �������� ��������� - ��� ������� ������� ��������
			flag = setEngagedMarkUps (simpMarkUps[bestNum], simpMarkUpsMinLens[bestNum], simpMarkUpsMaxLens[bestNum], localMarkUps[bestNum], localMarkUpsEngaged);
		}
		// ��������� �������� ������� ������� ��� ����������� ������ �������� �� ����������� �������
		curRes = countExtraStat (curSet, simpMarkUps, simpMarkUpsMinLens, simpMarkUpsMaxLens, extraMarkUps, correctExtraMarkUps, indicator);
		if ((lastRes < 0) || (curRes < lastRes)) {
			lastRes = curRes;
			lastSet = curSet;
			if (curRes == 0)
				break;
		}
	}
	// ���������� ����� �������� ������� � ����
	//splitMarkUps (curMarkUp, curMarkUpMinLen, curMarkUpMaxLen, lastSet, simpMarkUps, simpMarkUpsMinLens, simpMarkUpsMaxLens);
	// ������ �������� ��������� � ������� ���
	candMarkUps.resize (lastSet.size());
	candMarkUpsMinLens.resize (lastSet.size());
	candMarkUpsMaxLens.resize (lastSet.size());
	for (unsigned int mu = 0; mu < lastSet.size(); mu++) {
		candMarkUps[mu] = simpMarkUps[lastSet[mu]];
		candMarkUpsMinLens[mu] = simpMarkUpsMinLens [lastSet[mu]];
		candMarkUpsMaxLens[mu] = simpMarkUpsMaxLens [lastSet[mu]];
	}

	// Output
	std::cout << "\n		Best Result is " << lastRes << "\n";
	for (unsigned int mu = 0; mu < lastSet.size(); mu++) {
		std::cout << "\n		";
		for (unsigned int s = 0; s < simpMarkUps[lastSet[mu]].size(); s++)
			std::cout << simpMarkUps[lastSet[mu]][s] << "[" << simpMarkUpsMinLens [lastSet[mu]][s] << "," << simpMarkUpsMaxLens [lastSet[mu]][s] << "] ";
	}
	std::cout << "\n";
	std::cout.flush();

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::sortCandidateMarkUps
*
*	Description:	����� �������� ��� ��������� ���� ����������� ��������� �� 
*					�������������� ����������
*	Parameters:		markUps - ���������� �������� ���������� ����������� ���������
*					candMarkUps - ��������� � ��������
*					param - ��������� ���������� ������ ������ � ������� �������� 
*					� ������ ���������
*	Returns:		0
*	Throws:			AxiomLibException - 
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::sortCandidateMarkUps (std::vector < std::vector <int> > &markUps, std::vector < std::vector <int> > &candMarkUps, const int  param) const {
	// ���� ������ ��������

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::formAxiomSets
*
*	Description:	�������� ������ ������ �� ������ �������� � ����� ������ ������
*	Parameters:		markUps - ������ �������� ���������� ����������� ���������
*	Returns:		0
*	Throws:			AxiomLibException - 
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::formAxiomSets (std::vector < std::vector <std::vector <int> > > &markUps, std::vector < std::vector <std::vector <int> > > &markUpsMinLens, std::vector < std::vector <std::vector <int> > > &markUpsMaxLens) {
	// ���� ������ ��������

	return 0;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::saveResults
*
*	Description:	���������� �������������� ������ ������ � �������� ���������� 
*					����������� ���������
*	Parameters:		void
*	Returns:		int
*	Throws:			AxiomLibException - 
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::saveResults (void) {
	// ���� ������ ��������

	return -1;
}


/****************************************************************************
*					FuzzyDataLearnAlgorithm::getExtraMarkUps
*
*	Description:	������� ������� �������� ���� ���������� ����������� ������� � ���������� ������������
*					��������� �������� ��������.
*	Parameters:		markUps - �������� ���������� ����������� ������� � ���������� ����������� ���������
*					correctMarkUps - ���������� ��������� ���������� ��������� ������� � markUps
*					i - ����� ������ ����������� ��������� � ������ ������, 
*						������� ������������ ��� �������
*					axNum - ����� ������� � ������� bestAxioms, ������� ������������
*							� ������ ���������
*					param - ����������� ������ ������ �� ������� ����� ������
*	Returns:		0
*	Throws:			AxiomLibException - ��� ������������ ������ �� ������ ������ ��� ������������ ������� ����������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::getExtraMarkUps (std::vector < std::vector <int> > &markUps, std::vector < std::vector <int> > &correctMarkUps, int axNum, int i, const int param) const {
	// �������� ������� ����������
	if (axNum >= (int) bestAxioms.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::getExtraMarkUps : incorrect input parameters.");
	// �������� �������� ���� ���������� ������������ ������� ��� ������� axNum
	std::vector <double> row;
	bool bres;
	int curRes;
	int numOfTests = 0;
	int numOfNormalMultiTS;
	std::vector <int> numOfTS, numOfNormalTS;

	// ������ ����� �����
	fuzzyDataSet.getTestSize (numOfTests, numOfTS);
	// ������ ����� ����� ����������� ���������
	fuzzyDataSet.getNormalClassSize (numOfNormalMultiTS, numOfNormalTS);
	if (numOfNormalMultiTS != numOfNormalTS.size())
		throw AxiomLibException("FuzzyDataLearnAlgorithm::getExtraMarkUps : incorrect response from internal function getNormalClassSize");

	// �������� ���� - ������� �� ��� ���������� ��������
	bool correctEmpty = true;
	if (correctMarkUps.size () > 0)
		correctEmpty = false;
	else {
		correctMarkUps.resize (numOfTests + numOfNormalMultiTS);
	}
	// ��������� ����� �������� ����������
	markUps.resize(numOfTests + numOfNormalMultiTS);
	
	// ���� �� ���� ������ - ��������� �������� �� ����������
	for (int testNum = 0; testNum < numOfTests; testNum++) {
		// ��������� ���������� ��������
		if (correctEmpty) {
			fuzzyDataSet.getTSByIndexFromTests (correctMarkUps[testNum], testNum, -1);
			// ������� �� �������� ���������� ����������� ������� ��� ���������� � ��������� ��������� ���������� �������� �� i-�� ������ ����������� ���������.
			for (unsigned int j = 0; j < correctMarkUps[testNum].size(); j++)
				if ((correctMarkUps[testNum][j] > 0) && (correctMarkUps[testNum][j] != i))
					correctMarkUps[testNum][j] = 0;
		}
		// ��������� ���������� �������� 
		bres = fuzzyDataSet.getTSByIndexFromTests (row, testNum, param);
		if (bres) {
			markUps[testNum].resize(row.size(), 0);
			for (unsigned int k = 0; k < row.size(); k++) {
				curRes = bestAxioms[axNum].check(k, row);
				if (curRes > 0) {
					markUps[testNum][k] = i;
				}
			}
		} else {
			throw AxiomLibException("Error in RecognizerExtend::getExtraMarkUps: some params are missing in Tests in DataSets.");
		}
	}

	// �������� ���������� ����������� ���������
	for (int j = 0; j < numOfNormalMultiTS; j++) {
		bres = fuzzyDataSet.getNormalTS (row, j, param);
		if (bres) {
			markUps[numOfTests + j].resize(row.size(), 0);
			for (unsigned int k = 0; k < row.size(); k++) {
				curRes = bestAxioms[axNum].check(k, row);
				if (curRes > 0) {
					markUps[numOfTests + j][k] = i;
				}
			}
			if (correctEmpty)
				correctMarkUps[numOfTests + j].resize(row.size(), 0);
		} else {
			throw AxiomLibException("Error in RecognizerExtend::getExtraMarkUps: some params are missing in NormalRows in DataSets.");
		}
	}

	return 0;
}



/****************************************************************************
*					FuzzyDataLearnAlgorithm::createAxiomSet
*
*	Description:	������� ������������ ������� ������ �� ������
*	Parameters:		param - ����������� ���������� ������ ������ � ������� �������� 
*					� ������ ���������
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int FuzzyDataLearnAlgorithm::createAxiomSet (const int param) {
	// ���������� ����������� ����������
	int numOfClasses;
	std::vector <int> numOfMultiTS;
	std::vector < std::vector <int> > numOfTS;
	std::vector < std::vector <int> > baseMarkUps;
	std::vector < std::vector <int> > candMarkUps;
	std::vector < std::vector <std::vector <int> > > markUps, markUpsMinLen, markUpsMaxLen; // ������ ���������� - �� ����� ����������� ���������, ��������� - ����� ��������
	std::vector < std::vector <std::vector <int> > > localMarkUps; // ��� ������� ���� ����������� ���������, ��� ������ ������� - �������� ���� ���������� ��������� �������, ���������� ���������� ����������� ���������
	std::vector < std::vector <std::vector <int> > > extraMarkUps; // ��� ������� ���� ����������� ���������, ��� ������ ������� - �������� ���� ���������� ����������� �������, ���������� ���������� ����������� ���������
	std::vector < std::vector <int> > correctExtraMarkUps; // ���������� ������ ��� ���������� ������������ �������

	// ��������� ���������� �� ��������� �������	
	fuzzyDataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	if ((numOfClasses != numOfMultiTS.size()) || (numOfClasses != numOfTS.size()))
		throw AxiomLibException("FuzzyDataLearnAlgorithm::createAxiomSet : incorrect response from internal function.");

	// ���� �� ���� ����� ����������� ���������
	markUps.resize (numOfClasses);
	markUpsMinLen.resize (numOfClasses);
	markUpsMaxLen.resize (numOfClasses);
	for (int i = 0; i < numOfClasses; i++) {
		// 0. �������� ������ ������
		if (numOfMultiTS[i] != numOfTS[i].size())
			throw AxiomLibException("FuzzyDataLearnAlgorithm::createAxiomSet : incorrect response from internal function*.");
		
		// 1. �������� �������� ���� ��������� ���������� ��� ������� ���� ����������� ��������� � ��� ���� ������
		//    �������� ��� �������� �� ���������� ������������ � ��������� ���������� ��������
		baseMarkUps.clear();
		baseMarkUps.resize (bestAxioms.size());
		//    ���� �� ���� ������������� ����� ��������
		localMarkUps.resize(bestAxioms.size());
		extraMarkUps.resize(bestAxioms.size());
		correctExtraMarkUps.clear();
		for (unsigned int axNum = 0; axNum < bestAxioms.size(); axNum++) {	
			formGeneralizedMarkUps (baseMarkUps[axNum], localMarkUps[axNum], axNum, i, numOfMultiTS[i], param);
			getExtraMarkUps (extraMarkUps[axNum], correctExtraMarkUps, axNum, i+1, param);
		}

		// 2. �� ���������� ��������� ��� ������ ������ ��������� ��������� �� �������� ��� ������� ���� ����������� ���������
		createCandidates (markUps[i], markUpsMinLen[i], markUpsMaxLen[i], baseMarkUps, localMarkUps, extraMarkUps, correctExtraMarkUps, i+1);
		
		// 3. ����� �������� ��� ������� ���� ����������� ��������� ����� �������������� ����������
		//sortCandidateMarkUps (markUps[i], candMarkUps, param);
	}

	// �������� ������ ������ �� ������ ������� �������������� ��������
	formAxiomSets (markUps, markUpsMinLen, markUpsMaxLen);

	// ���������� �������������� ������ ������ � �������� ���������� ����������� ���������
	saveResults ();

	if (this->comments) {
		std::cout << "\n	Creating of AxiomSets is done. Third stage is over.\n";
		std::cout.flush();
	}

	return 0;
}
