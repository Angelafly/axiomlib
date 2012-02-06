/****************************************************************************
*				RecognizerExtend.cxx
*
*	Description:	������� ������ RecognizerExtend - ����������
*	Author:			gevor, dk
*	History:		
*
****************************************************************************/
#include <limits.h>

#include "RecognizerExtend.h"
#include "TemplateRecognizer.h"

using namespace AxiomLib;

#define noValue -1.0
#define precMinValue 0.001
#define precMaxValue 0.999
#define precCentralValue 0.3 // ���� �������� ���������� ������ � ��������� �� precDelta �������� �����
#define precDelta 0.25
#define precMinDelta 0.005
#define epsilon 0.01

// ����������� ������ � �������� �������� �� ���������
RecognizerExtend::RecognizerExtend (void) { 
	comments = false;
	outputToFile = false;
	whereToSave = "./";
	saveDirName = "axiomSet_";
	curDir.clear();
	curFile.clear();
	precision = 0.0; 
	compareStatistic = NULL; 
	goalStrategy = NULL;
}

// ���������� ������ � ������������ ���� ��������� ������������ ��������
RecognizerExtend::~RecognizerExtend (void) { 
	if (compareStatistic != NULL) delete compareStatistic; 
	if (goalStrategy != NULL) delete goalStrategy;
}

/****************************************************************************
*                       RecognizerExtend::setParamsFromEnv
*
*       Description:    ���������������� ��������� ������� �� env
*       Parameters:     const Environment& env - �����
*       Returns:        0
*       Throws:         AxiomLibException - ���� ��� ������-�����������
*       Author:         gevor
*       History:
*
****************************************************************************/
int RecognizerExtend::setParamsFromEnv (const Environment& env) {
	// �������������� dataset
	std::string datasetDir, datasetName;
	if (env.getStringParamValue(datasetDir, "BaseDataSetDir") < 0)
		throw AxiomLibException("RecognizerExtend::setParamsFromEnv : data set directory is undefined.");
	if (env.getStringParamValue(datasetName, "DataSet") < 0)
		throw AxiomLibException("RecognizerExtend::setParamsFromEnv : data set is undefined.");

	// ��������� ����������� ��� ������� ������ ��������� � ������ ������
	EnvDataSet envDataSet;
	envDataSet.readConfigFile (datasetDir, datasetName);
	// ��������� ����������� ����������� NullStr - ����������� ��������� � ������ ����� ���� ������ ���� ���������� ���������
	dataSet.setNullStr (envDataSet);
	// ���������� ��������� ����� ������ - ������� ��� �� ���������� �������������
	dataSet.readDataSet(datasetDir, datasetName);
	// ��������������� � ������ ������ ������ �������� ���������� � ������������ �������������� �� ������� ����� ����� �������� ��������
	dataSet.getParamNums(params, env, envDataSet);

	// ��������� ���������� ����������� � MarkUp
	if (env.getDoubleParamValue(precision, "precision") < 0)
		throw AxiomLibException("RecognizerExtend::setParamsFromEnv : precision is undefined.");
 	//std::cout << "\n Precision set to " << precision << " !\n";
	// �������� ������ �������� ����������
	std::string compClassName;
	CompareClassFactory ccf;
	if (env.getStringParamValue(compClassName, "compareClass") < 0)
		throw AxiomLibException("RecognizerExtend::setParamsFromEnv : compare-class is undefined.");
	compareStatistic = ccf.create(compClassName);
	compareStatistic->setParamsFromEnv(env);

	// �������������� ��������� ���������� ������� �������
	std::string goalStrategyName;
	GoalStrategyFactory gsf;
	if (env.getStringParamValue(goalStrategyName, "goalClass") >= 0) {
		goalStrategy = gsf.create(goalStrategyName);
		goalStrategy->setParamsFromEnv(env);
	}

	// ���������� � ���������� ������
	env.getStringParamValue(whereToSave, "PathToSaveResults");
	env.getStringParamValue (saveDirName, "saveTo");

	std::string str;
	env.getStringParamValue (str, "outputMarkUpToFile");
	if ((str == "true") || (str == "True") || (str == "TRUE"))
		outputToFile = true;	

	return 0;
}


/****************************************************************************
*                       RecognizerExtend::setNominalParamsFromEnv
*
*       Description:	���������������� ������ �������� ��������� ������� �� env
*       Parameters:	const Environment& env - �����
*       Returns:		0
*       Throws:		AxiomLibException - ���� ��� ������-�� ���������
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerExtend::setNominalParamsFromEnv (const Environment& env) {
	// ��������� ���������� ����������� � ��������
	if (env.getDoubleParamValue(precision, "precision") < 0)
		throw AxiomLibException("RecognizerExtend::setParamsFromEnv : precision is undefined.");
 
	// �������� ������ �������� ����������
	std::string compClassName;
	CompareClassFactory ccf;
	if (env.getStringParamValue(compClassName, "compareClass") < 0)
		throw AxiomLibException("RecognizerExtend::setParamsFromEnv : compare-class is undefined.");
	compareStatistic = ccf.create(compClassName);
	compareStatistic->setParamsFromEnv(env);

	// ���������� � ���������� ������
	env.getStringParamValue(whereToSave, "PathToSaveResults");
	env.getStringParamValue (saveDirName, "saveTo");

	std::string str;
	env.getStringParamValue (str, "outputMarkUpToFile");
	if ((str == "true") || (str == "True") || (str == "TRUE"))
		outputToFile = true;

	return 0;
}


/****************************************************************************
*                       RecognizerExtend::setDataSet
*
*       Description:	������� ��������� ������ ������ ������ � ������� ����� ��������������
*       Parameters:	newDataSet - ����� ����� ������
*       Returns:		0
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerExtend::setDataSet (const DataSet &newDataSet) {
	dataSet = newDataSet;
	return 0;
}


/****************************************************************************
*                       RecognizerExtend::setParamsOfDataSet
*
*       Description:	������� ��������� ������ ��������� ��������� ������ params
*       Parameters:	newParams - ����� �������� �������
*       Returns:		0
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerExtend::setParamsOfDataSet (std::vector<int> &newParams) {
	params = newParams;
	return 0;
}


/****************************************************************************
*                       RecognizerExtend::setPrecision
*
*       Description:	������� ��������� ������ �������� ��������� precision
*       Parameters:	newPrecision - ����� �������� ���������
*       Returns:		0 - param was set
*				-1 - param wasn't set
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerExtend::setPrecision (const double newPrecision) {
	if ( (newPrecision > 0.0) && (newPrecision < 1.0) ) {
		precision = newPrecision;
		return 0;
	}
	return -1;
}


/****************************************************************************
*                       RecognizerExtend::setCompareStatistic
*
*       Description:	������� ��������� ������ ������ �������� ����������
*       Parameters:		newCompareStatistic - ����� ��������, ����� �������� 
*						��������������� ��� ����� ����� ����������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int RecognizerExtend::setCompareStatistic (CompareStatistic *newCompareStatistic) {
	if (compareStatistic != NULL) {
		delete compareStatistic;
		compareStatistic = NULL;
	}
	if (newCompareStatistic != NULL) {
		compareStatistic = newCompareStatistic->copy();
	}
	return 0;
}


/****************************************************************************
*                       RecognizerExtend::copy
*
*       Description:	������� ���������� ����� ������� ��������������
*       Parameters:		-
*       Returns:		Recognizer* - ������ �� ����� ������� ��������������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
Recognizer* RecognizerExtend::copy (void) {
	RecognizerExtend* toReturn;
	toReturn = new RecognizerExtend;
	toReturn->setDataSet (dataSet);
	toReturn->setParamsOfDataSet (params);
	toReturn->setPrecision (precision);
	toReturn->setCompareStatistic (compareStatistic);
	toReturn->setOutputToFile(outputToFile, whereToSave, saveDirName);
	return (Recognizer*) toReturn;
}


/****************************************************************************
*						RecognizerExtend::learn
*
*       Description:	������� �������� ������� ��������� �������������
*       Parameters:		templateRecognizer - ������ ��������������, �� �������
*						��������� �������� �������� ������� ��������� �������������
*       Returns:		double - �������� ������� ������� ��� ������� ������ ����������
*       Throws:			AxiomLibException - ���� ��������� ������ ������ �������
*       Author:			dk
*       History:
*
****************************************************************************/
double RecognizerExtend::learn (TemplateRecognizer &templateRecognizer) {
	// � ������ ������� ����� ������������� ��������� ��������� �������� ������� ��������� �������� prcision. 
	// ��� ��� � ������ ������ - ��� ������������ ��������.
	// � ������� ����� �������� ������� learn � ������ compareStatistic.
	// � ������ ������� ������� ����� �������� ������� ��� ��������� precision

	// 1. �������� ��������� ��������
	if (goalStrategy == NULL)
		throw AxiomLibException("RecognizerExtend::learn: goalClass is not set in RecognizerExtend, but needed for learn function.");

	// 2. ���������� ���������� ��� ������
	// 2.1. ��������� �������� ���������, �� ������� ����� ����� ������������ ��������
	std::vector <double> curPrecision;
	curPrecision.resize(5);
	// ������� ��������� �������� ��������� ������� curPrecision
	if ((precision > precMinValue) && (precision < precMaxValue)) {
		// ������ �� ����������� ����� ����� ������� �������� ��������� precision
		curPrecision[2] = precision;
	} else {
		// ������ ��������� �������� ��������� �� ������ - ���������� ��� �������� � �������� ���������
		do {
			curPrecision[2] = precCentralValue + precDelta*(2.0*((double) rand ()) / ((double) RAND_MAX) - 1.0);
		} while ((curPrecision[2] <= precMinValue) || (curPrecision[2] >= precMaxValue));
	}
	// ���������� ����������� ��������� ���������
	if (curPrecision[2] > (precMinValue + precDelta))
		curPrecision[0] = curPrecision[2] - precDelta;
	else 
		curPrecision[0] = precMinValue;
	// ���������� ������������ ��������� ���������
	if (curPrecision[2] < (precMaxValue - precDelta))
		curPrecision[4] = curPrecision[2] + precDelta;
	else 
		curPrecision[4] = precMaxValue;
	// ���������� ��������� ������������� �����
	curPrecision[1] = 0.5*(curPrecision[0] + curPrecision[2]);		
	curPrecision[3] = 0.5*(curPrecision[2] + curPrecision[4]);

	// 2.2. ������ ��� �������� ������� �������, ��������������� ��������� �� curPrecision
	std::vector <double> curRes; 
	curRes.resize(5);
	for (unsigned int i=0; i < 5; i++) {
		curRes[i] = noValue;
	}
	
	// 2.3 ������� ������������� ����������� ��� ������� curRes
	std::vector < std::vector <signed short> > comparisonTable; 
	comparisonTable.resize(5);
	for (unsigned int i=0; i < 5; i++) {
		comparisonTable[i].resize(5);
		comparisonTable[i][i] = 0;
	}

	// 3. ������ ������������ ��������� ������ 
	templateRecognizer.runPreprocessor();
	return sublearn (templateRecognizer, curRes, curPrecision, comparisonTable);
}


/****************************************************************************
*						RecognizerExtend::sublearn
*
*       Description:	��������������� ������� ��� ��������� ��������. 
*						���������� ���������� ����� ���������� �������� ���������
*						precision. ��� ���� �������� ������ � ������ ������ 
*						������������ ����� � 2 ����. �������� ������ - ������
*						���������� ������ ��������� precMinDelta.
*       Parameters:		templateRecognizer - ������, �� ������ ����������� ���������� ������� �������
*						curRes - ������ �������� ������� ������� ��� �������� precision �� ������� curPrecision
*						curPrecision - ������ �������� precision �� ������� ������� ����� ������� ��������
*						comparisonTable - ������� ��������� ������������ �������� ������� curRes
*       Returns:		double - 
*       Throws:			AxiomLibExceprion - ���� ������� ������� � ��������������� �����������
*       Author:			dk
*       History:
*
*****************************************************************************/
double RecognizerExtend::sublearn (TemplateRecognizer &templateRecognizer, std::vector <double> &curRes, std::vector <double> &curPrecision, std::vector < std::vector <signed short> > &comparisonTable) {
	// ����������� �������� ������� ������� ��� �������� ��������� precision �� ������� curPrecision
	int resFirst, resSecond;
	for (unsigned int i = 0; i < 5; i++) {
		if (curRes[i] == noValue) {
			precision = curPrecision[i];
			templateRecognizer.runRecognizer(resFirst, resSecond);
			curRes[i] = goalStrategy->compute (resFirst, resSecond);
		}
	}

	// ����������� ��������� ������������ ����������� ��� ��������� �������� ��������� precision
	for (unsigned int i=0; i < 4; i++) {
		for (unsigned int j = i+1; j < 5; j++) {
			if (curRes[i] > (curRes[j] + epsilon))
				comparisonTable[i][j] = 1;
			else 
				if (curRes[i] < (curRes[j] - epsilon))
					comparisonTable[i][j] = -1;
				else
					comparisonTable[i][j] = 0;
			comparisonTable[j][i] = - comparisonTable[i][j];
		}
	}

	// ����������� �� ������� �������� ����������� - ���� ������� ������
	unsigned int direction;
	direction = chooseDirection (comparisonTable);
	
	// ������� ������ �� ������������ �����
	if ( ( (direction < 2) && ((curPrecision[2] - curPrecision[1]) < precMinDelta) ) || 
		 ( (direction >= 2) && ((curPrecision[3] - curPrecision[2]) < precMinDelta) ) ) {
		precision = curPrecision[direction];
		return curRes[direction];
	}
	
	// ��������� �������� � ������������ � ��������� ������������ ��������
	manageSets (direction, curRes, curPrecision);
	
	// ����������� ��������� ��������
	return sublearn (templateRecognizer, curRes, curPrecision, comparisonTable);
}


/****************************************************************************
*						RecognizerExtend::chooseDirection
*
*       Description:	��������������� ������� ��� �������� ������ �� �������� precision.
*						� ������ ������� ���������� ������ �������� ��������� precision ��
*						��� ������������ ������� ���������� ����������� compTable.
*       Parameters:		compTable - ������������� ������� ����������� ��� ������ �������� 
*						��������� precision.
*       Returns:		unsigned int - ����� ����� �� [0,4] - ������ ����������� ���������
*       Throws:			AxiomLibException - � ������ ����������� ������������ �������
*       Author:			dk
*       History:
*
*****************************************************************************/
inline unsigned int RecognizerExtend::chooseDirection (std::vector < std::vector <signed short> > &compTable) const {
	// �������������� ����������� ����������
	std::vector <bool> vec; // ���������� � ���, ����� �� ����������� �������� �����������
	vec.resize(5);
	// ���������� ���������� ��� ����������� ��������� ��������� precision
	int numOfCandidates = 5;
	for (unsigned int i = 0; i < 5; i++) {
		vec[i] = true;
		for (unsigned int j = 0; j < 5; j++) {
			if (compTable[i][j] > 0) {
				vec[i] = false;
				numOfCandidates--;
				break;
			}
		}
	}
	// ����� ������� ����������� ��������� �� ������� ����������
	switch (numOfCandidates) {
		case 1:
			for (int i = 0; i < 5; i++)
				if (vec[i]) return i;
			break;
		case 2:
			if (vec[2]) return 2;
			if (vec[0]) 
				if (!vec[4]) return 0;
				else
					if (compTable[1][3] >= 0)  return 0;
					else return 4;
			if (vec[4]) return 4;
			if (compTable[0][4] >= 0) return 1;
			else return 3;
			break;
		case 3:
			if (vec[2]) return 2;
			if (vec[0] && vec[3]) return 0;
			if (vec[1] && vec[4]) return 4;
			break;
		case 4:
			if (vec[2]) return 2;
			else return 0;
			break;
		case 5:
			return 2;
			break;
		default:
			throw AxiomLibException("RecognizerExtend::chooseDirection: internal error - irrelevant number of candidates.");
			break;
	}
	throw AxiomLibException("RecognizerExtend::chooseDirection: internal error - should be returned before this point.");
	return 2;
}


/****************************************************************************
*						RecognizerExtend::manageSets
*
*       Description:	��������������� ������� ��� �������� ������ �� �������� precision
*						� ������ ������� ���������� ��������� ������ �������� precision 
*						�������� ������������ �������� ��������� pos.
*       Parameters:		pos - ����������� �������� ����� ��������� ����� �������� [-2,2]
*						curRes - ������ �� ���������� ������� ������� ��������������� 
*							��������� precision �� ������� curPrecision
*						curPrecision - ������ �������� precision, ������������ � ��������
*       Returns:		-
*       Throws:			AxiomLibException - ���� ������� ����� ����������� ��������
*       Author:			dk
*       History:
*
*****************************************************************************/
inline void RecognizerExtend::manageSets (unsigned int direction, std::vector <double> &curRes, std::vector <double> &curPrecision) const {
	switch (direction) {
		case 0:
			curPrecision[2] = curPrecision[0];
			curPrecision[3] = 0.5*(curPrecision[0] + curPrecision[1]);
			curPrecision[4] = curPrecision[1];
			if ( (2*curPrecision[2]) > (curPrecision[4] + precMinValue) )
				curPrecision[0] = 2*curPrecision[2] - curPrecision[4];	
			else
				curPrecision[0] = precMinValue;
			curPrecision[1] = 0.5*(curPrecision[0] + curPrecision[2]);
			curRes[2] = curRes[0];
			curRes[4] = curRes[1];
			curRes[0] = noValue;
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 1:
			curPrecision[3] = 0.5*(curPrecision[1] + curPrecision[2]);
			curPrecision[4] = curPrecision[2];
			curPrecision[2] = curPrecision[1];
			curPrecision[1] = 0.5*(curPrecision[0] + curPrecision[1]);
			curRes[4] = curRes[2];
			curRes[2] = curRes[1];
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 2:
			curPrecision[0] = curPrecision[1];
			curPrecision[4] = curPrecision[3];
			curPrecision[1] = 0.5*(curPrecision[1] + curPrecision[2]);
			curPrecision[3] = 0.5*(curPrecision[2] + curPrecision[3]);
			curRes[0] = curRes[1];
			curRes[4] = curRes[3];
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 3:
			curPrecision[0] = curPrecision[2];
			curPrecision[1] = 0.5*(curPrecision[2] + curPrecision[3]);
			curPrecision[2] = curPrecision[3];
			curPrecision[3] = 0.5*(curPrecision[3] + curPrecision[4]);
			curRes[0] = curRes[2];
			curRes[2] = curRes[3];
			curRes[1] = noValue;
			curRes[3] = noValue;
			break;
		case 4:
			curPrecision[0] = curPrecision[3];
			curPrecision[1] = 0.5*(curPrecision[3] + curPrecision[4]);
			curPrecision[2] = curPrecision[4];
			if ( (2*curPrecision[2]) < (curPrecision[0] + precMaxValue) )
				curPrecision[4] = 2*curPrecision[2] - curPrecision[0];
			else
				curPrecision[4] = precMaxValue;
			curPrecision[3] = 0.5*(curPrecision[2] + curPrecision[4]);
			curRes[0] = curRes[3];
			curRes[2] = curRes[4];
			curRes[1] = noValue;
			curRes[3] = noValue;
			curRes[4] = noValue;
			break;
		default:
			throw AxiomLibException("RecognizerExtend::manageSets: unrecgnized value of managment input param 'direction'.");
			break;
	}
}


/****************************************************************************
*				RecognizerExtend::joinResults
*
*	Description:	������� �����������. ���������� ����� ���� ���������. 
*				� ������ ������: ��������� ���������� �� ������ �� ������������ ����,
*				���� ���� �� �� ����� ����������� ���� ������� ��������� �������� - 
*				�� ���������� ����������� - � ��������� ������ ��������� �������� � ������� - 
*				������� ���������� �� ����������� ������������. �� ���� ��������� ������������
*				�� �������� ����������� �������. 
*	Parameters:	result - ����������� ������ - ��������� �����������
*				prelimResult - ������� ��������� � ������������ ������ ��������� ��������� �� ������ ��������������� ����������� ���������� �������� ��������� �������
*				numOfClasses - ����� ������� ��������������, ������� ����� ���� ������� ��� ������� ��������� �����
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� ������ �� ����������� ��� ����� ������� �������������� < 1
*	Author:		dk
*	History:
*
****************************************************************************/
inline signed int RecognizerExtend::joinResults (std::vector <int> &result, std::vector < std::vector <int> > &prelimResult, const int numOfClasses) const {
	// ���� ���������� �� � ������ - �� ������ �������� ��������� � �������
	if (prelimResult.size() == 1) {
		result = prelimResult[0];
		return 0;
	}
	// �������� ������������ ������� ������
	bool flag = 0;
	for (unsigned int i = 1; i < prelimResult.size(); i++) {
		if (prelimResult[i].size() != prelimResult[i-1].size()) {
			flag = 1;
			break;
		}
	}
	if ( (prelimResult.size() < 2) || (flag != 0) || (numOfClasses < 1) ){
		throw AxiomLibException("RecognizerExtend::joinResults: incorrect results in recognition process.");
	}
	// ���������� ���������� �������
	std::vector <int> bulletin;
	result.resize(prelimResult[0].size());
	bulletin.resize(numOfClasses);
	int curBest;
	int curScore;
	// � ����� - ��������� �� ��� ������������� ���������������� ���������� � ���������� � ������ ��������� �� �������� - ����������� 
	for (long int j = 0; j < (long int) prelimResult[0].size(); j++) {
		// ��������� ����������� �����������
		for (int t = 0; t < (int) bulletin.size(); t++) {
			bulletin[t] = 0;
		}
		// �����������
		for (int k = 0; k < (int) prelimResult.size(); k++) {
			if (prelimResult[k][j] > 0) {
				(bulletin[ prelimResult[k][j] - 1 ])++;
			}
		}
		// ������� ����������� - ��������� ����������
		curScore = 0;
		curBest = -1;
		for (int t = 0; t < (int) bulletin.size(); t++) {
			if (bulletin[t] > curScore) {
				curScore = bulletin[t];
				curBest = t;
			}
		}
		// ���������� ����������� ���������
		if (curBest < 0) {
			result[j] = 0;
		}
		else {
			result[j] = curBest + 1; // ��������������� 1-��, ������� ���� ������ ��� ���������� ���������
		}
	}
	return 0;
}



/****************************************************************************
*				RecognizerExtend::recognition
*
*	Description:	������� ������������� ��������� �������� �� ������ ����� �� ���������� �
*				������� ��������� �������� ���� ��������� ����� ��������� �������� � ��������
*				��������� ������������ ������� ������.
*	Parameters:	first - ����������� �������� - ����� ������ ������������� ������� ����
*				second - ����������� �������� - ����� ������ ������������� ������� ����
*				markUps - �������� �������� ����� ������ �������� ����� - ������� �� �������� - �������� �� ������� �� ������������ ��������� ����������
*				corrMarkUp - ���������� �������� �������� �����, ���������� ��� �������� ���������� �� ������� �����
*				refs - �������� ���� ��������� ���������� ���� ������� ��������������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ����� ��������� ���������� ���������, ��� ����� ���� ����������� ��������� �������� - �������������
*	Author:		dk
*	History:
*
****************************************************************************/
inline signed int RecognizerExtend::recognition (int &first, int &second,  
												 std::vector <std::vector <int> > &markUps,
												 std::vector <int> &corrMarkUp, 
												 std::vector < std::vector <RefMarkupSet> > &refs,
												 int numOfTest) const
{
	// ��������� �������� � �������� ���������� ����� ���� ���������
	// � ������ ������� ����� ����������� �������� ��������:
	//	1. � ����� �� ���� ���������� (�.�. ������������ ���������� ����) ��������� � ���� ���������� 
	//	����� ��������� �������� ���� ������� �� ������ �� ������������.
	//	2. ����� ���������� ���������� � ������ �� ������������ �������� �����������, �.�. ������� ������ ��� �������� 
	//	3. ����� ���������� ������ ��� �������� ������������ � ����� ���������� �������� � ������������ ����������
	//	4. ���������� ���������� �������� � ��������������� ���� ����������� � ������� ����� � ����������
	// ���������� ���������� ���������� �������
	long int cur = 0;
	int flag = 0;
	double tmpFirst, tmpSecond; // ��������� ��� �������� ����� ������ �� ������ �� ������
	int wholeNum; // ����� ������� �� ������ �� ������
	std::vector <int> newMarkup;
	std::vector <int> newMarkupLen;
	std::vector < std::vector <int> > prelimResult;
	prelimResult.resize(markUps.size());
	std::vector <int> result;
	first = 0;
	second = 0;
	// 1. ���� �� ���� ��������������� ������������ ��������� �����
	//#pragma omp for
	for (int j = 0; j < (int) markUps.size(); j++) {
		newMarkup.resize((markUps[j]).size());
		newMarkupLen.resize((markUps[j]).size());
		(prelimResult[j]).resize((markUps[j]).size());
		newMarkup[0] = markUps[j][0];
		newMarkupLen[0] = 1;
		prelimResult[j][0] = 0;
		// 1. ����� ��������� �������� ��������������� �� markUps[j]
		for (long int i = 1; i < (long int) markUps[j].size(); i++) {
			prelimResult[j][i] = 0;
			// �������� ��� ����������� ��������
			if (markUps[j][i] != markUps[j][i-1]) {
				cur++;
				newMarkup[cur] = markUps[j][i];
				newMarkupLen[cur] = 1;
			}
			else {
				(newMarkupLen[cur])++;
			}
			flag = 0;
			// ���������� ���� �� ���������� � ���������� ������ - �.�. ���� ���� �� 1 ����� �������������
			for (int k = 0; k < (int) refs[j].size(); k++) {
				flag = (refs[j][k]).check (newMarkup, newMarkupLen, cur);
				if (flag != 0) {
					prelimResult[j][i] = k+1;
					break;
				}
			}
		}
	}
	
	// 2. ����������� ����������� ��� ��������������� ������� ������ �� ������ ����� �� ������ �� ������������
	joinResults (result, prelimResult, (int) refs[0].size());
	// ����� ����������� � ����, ���� ��� ����������
	if (outputToFile) updateRows (corrMarkUp, result);
	
	// 3. C������������� ���������� ����������� � ����������� - ������� ����������
	wholeNum = compareStatistic->getStatistic(result, corrMarkUp, tmpFirst, tmpSecond, comments);
	if (wholeNum < 0) 
		throw AxiomLibException("Error in RecognizerExtend::recognition: incorrect results in process of getting statistic.");
	
	// 4. ���������� ����� �� ���������� � ������� �� �������
	first = (int) tmpFirst;
	second = (int) tmpSecond;
	
	return 0;
}


/****************************************************************************
*				RecognizerExtend::initRefs
*
*	Description:	������� ���������� ����������� ��������� �������� 
*				� ������� ��������. ���������� ���������� ���� ��������� 
*				�������� �� �������� ����������.
*	Parameters:	pop - ��������� ������ ������ 
*				asNum - ����� ������� ������ � ���������
*				param - ��� ����������� �������������� ���� - ��������� �������� �� ������� ����������
*				refer - ��������� �������� �� ������ ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
inline signed int RecognizerExtend::initRefs (AxiomSetPop& pop, const int asNum, const int param, std::vector <RefMarkupSet> &refer) const {
	// ���������� ���������� ���������� �������
	int numOfClasses;
	std::vector<int> numOfMultiTS;
	std::vector<std::vector<int> > numOfTS;
	bool b = dataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	std::vector<int> resRef;
	// ���������� ������ ��� ������� ����
	refer.resize (numOfClasses);
	std::vector<double> vec; // ������ ������������ � ����� - ��� ���������
	vec.clear();
	bool bres = false;
	// ���������� ������� ������ ��������������
	for (int i = 0; i < numOfClasses; i++) {
		RefMarkupSet rms;
		rms.setPrecision(precision);
		for (int j = 0; j < numOfMultiTS[i]; j++) {
			bres = dataSet.getTSByIndexFromClass (vec, i, j, param);
			if (bres) {
				pop.enter (resRef, vec, 0, (unsigned long) vec.size(), asNum);
				rms.addRefMarkup(resRef, comments);
			}
		}
		refer[i] = rms;
	}
	return 0;
}


/****************************************************************************
*				RecognizerExtend::initRefs
*
*	Description:	������� ���������� ����������� ��������� �������� 
*				� ������� ��������. ���������� ���������� ���� ��������� 
*				�������� �� �������� ����������.
*	Parameters:	axiomSet - ������� ������
*				param - ��� ����������� �������������� ���� - ��������� �������� �� ������� ����������
*				refer - ��������� �������� �� ������ ����������
*				refStat - ���������� �� ������������� ������ ��� �������� ��������� ����������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
inline signed int RecognizerExtend::initRefs (AxiomSet& axiomSet, const int param, std::vector <RefMarkupSet> &refer, std::vector <bool> &refStat) const {
	// ���������� ���������� ���������� �������
	int numOfClasses;
	std::vector<int> numOfMultiTS;
	std::vector<std::vector<int> > numOfTS;
	bool b = dataSet.getClassSize (numOfClasses, numOfMultiTS, numOfTS);
	std::vector<int> resRef;
	std::vector <bool> curStat;
	// ���������� ������ ��� ������� ����
	refer.resize (numOfClasses);
	std::vector<double> vec; // ������ ������������ � ����� - ��� ���������
	vec.clear();
	bool bres = false;
	// ���������� ������� ������ ��������������
	for (int i = 0; i < numOfClasses; i++) {
		RefMarkupSet rms;
		rms.setPrecision(precision);
		for (int j = 0; j < numOfMultiTS[i]; j++) {
			bres = dataSet.getTSByIndexFromClass (vec, i, j, param);
			if (bres) {
				axiomSet.enter (resRef, vec, 0, (unsigned long) vec.size(), curStat);
				updateVec (refStat, curStat);
				rms.addRefMarkup(resRef, comments);
			}
		}
		refer[i] = rms;
	}
	return 0;
}


/****************************************************************************
*				RecognizerExtend::analizeTest
*
*	Description:	������ ������ ����� - ���������� ����� ����� ���������� ���������� ��������� � ������ �����
*	Parameters:		corrMarkUp - ���������� �������� �����
*					res - ����� ���������� � ������ �����, ������� ���������� ����������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline signed int RecognizerExtend::analizeTest (std::vector <int> &corrMarkUp, int &res) {
	unsigned int i = 0;
	unsigned int j = 0;
	res = 0;
	while (i < corrMarkUp.size()) {
		if (corrMarkUp[i] != 0) {
			res++;
			j = i+1;
			while (j < corrMarkUp.size()) {
				if (corrMarkUp[j] != corrMarkUp[i])
					break;
				j++;
			}
			i = j;
		} else {
			i++;
		}
	}
	return 0;
}


/****************************************************************************
*					RecognizerExtend::updateVec
*
*	Description:	��������������� ��������� ���������� �������
*	Parameters:		vec - ������ ������� �������� ����������
*					newVec - ������ �� ������� ���������� ����������
*	Returns:		-
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
inline void RecognizerExtend::updateVec (std::vector <bool> &vec, std::vector <bool> &newVec) const {
	if (vec.size() != newVec.size()) {
		vec = newVec;
	} else {
	for (unsigned int i = 0; i < newVec.size(); i++)
		vec[i] = vec[i] || newVec[i];
	}
}


/****************************************************************************
*					RecognizerExtend::countStat
*
*	Description:	��������������� ��������� ��� �������� ���������� �� ����������� �������������
*	Parameters:		stat - ����������� ������ ����������
*					resFisrt - ����� ������ ������� ����
*					resSecond - ����� ������ ������� ����
*					resShouldBe - ����� ����� ��������� ��������, ������� ���� � �������� ������
*					refStat - ���������� �� ������������� ������ ��� �������� ��������� ����������
*					rowStat - ���������� �� ������������� ������ ��� �������� �������� �����
*	Returns:		-
*	Throws:			AxiomLibException - ���� �� ������� ��������� �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
inline void RecognizerExtend::countStat (std::vector <double> &stat, int resFirst, int resSecond, int resShouldBe, std::vector <bool> &refStat, std::vector <bool> &rowStat) const {
	if (refStat.size() != rowStat.size())
		throw AxiomLibException("RecognizerExtend::countStat : internal error - statistial data is uncoordinated.");
	stat.resize (refStat.size());
	int temp;
	double multiplier;
	multiplier = 0.5 * (double) (resShouldBe - resSecond) / (double) (resShouldBe + resFirst); // resFisrt � resSecond ���������� ���������� �� ���� ���������, ������� � ����� ������� � ������ �������������
	for (unsigned int i = 0; i < refStat.size(); i++) {
		temp = 0;
		if (refStat[i]) temp++;
		if (rowStat[i]) temp++;
		stat[i] = (double) temp * multiplier;
	}
}


/****************************************************************************
*				RecognizerExtend::run
*
*	Description:	������ �������� �������������
*	Parameters:	AxiomSetPop& pop - ���������, � ������� ��������
*	Returns:		0
*	Throws:		AxiomLibExeption - ���� ��������� �������� �������, ��� �� ����������������, ��� ��������
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int RecognizerExtend::run (AxiomSetPop& pop) {
	const bool debug = false;
	if (debug) std::cout << "\n	RecognizerExtend:";
	// ��������; � ������ ���� ��� �� ������������������ ���������, AxiomSetPop ����� exception
	if (pop.size() == 0)
		throw AxiomLibException("RecognizerExtend::run - population of zero size.");
	// ���������� ���������� ����������
	bool bres;
	int first, second, tmpFirst, tmpSecond;
	double tmpGoal;
	std::vector <double> row;
	std::vector <std::vector <int> > markUps;
	std::vector <int> corrMarkUp;
	// std::vector <double> corrMarkUpDouble;
	std::vector < std::vector <RefMarkupSet> > refs; // ������ �������� - �� ������� �� ���������� ������������� ������ � ������ ��������� ��������
	int numOfTests = 0;
	std::vector<int> numOfTS;
	// ������ ����� �������� �����
	bool c = dataSet.getTestSize (numOfTests, numOfTS);
	// ��������������� ��������� ������������ ������������ �������� ��� �������� �������� �� ������������ ������� �������
	markUps.resize(params.size());
	refs.resize(params.size());
	// ����� �� ���� �������� ������ � ���������

	//#pragma omp parallel for schedule(dynamic, 1)
	for (int asNum = 0; asNum < pop.size(); asNum++) {
		if (debug) std::cout << "\n		As open " << asNum+1 << "\n";
		// ������ ��� ������ ������� ������, ��:
		// 1. ��������� ���� �� dataset-� ���� �������� ������:
		//      - �������� reference markups �� ���� ����������
		//      - �������� test markups �� ���� ����������
		// 2. ��������� ����� ������ ������� � ������� ���� �� ��������� test markups � ������� reference markup
		first = 0;
		second = 0;
		if (outputToFile) clearPath (pop.name(asNum), asNum);

		// �������� �������� ���� ���������� ����������� ���������
		if (debug) std::cout << "	InitRefs";
		for (int prm = 0; prm < (int) params.size(); prm++) {
			initRefs (pop, asNum, params[prm], refs[prm]);
			if (outputToFile) saveRefs (refs[prm], dataSet.paramName(params[prm]));
		}

		// �������� ��������� ���� � �������� ��� ���������
		for (int t = 0; t < numOfTests; t++) {
			if (debug) std::cout << "			Test	" << t+1;
			if (outputToFile) genFileName (t);
			corrMarkUp.clear();
			dataSet.getTSByIndexFromTests (corrMarkUp, t, -1);
			for (int prm = 0; prm < (int) params.size(); prm++) {
				(markUps[prm]).clear();
				bres = dataSet.getTSByIndexFromTests (row, t, params[prm]);
				if (bres) {
					if (debug) std::cout << "	Entering";
					// �������� �������� ������ asNum ������ �� ����� ����������� � ������� ���������
					pop.enter (markUps[prm], row, 0, (unsigned long) row.size(), asNum);
					// ���������� �������� � ����, ���� ��� ����������
					if (outputToFile) saveRows (row, markUps[prm], dataSet.paramName(params[prm]));
				}
				else {
					throw AxiomLibException("Error in RecognizerExtend::run: some params are missing in Tests in DataSets.");
				}
			}

			// ������ �������������� � �������� ����������� ��� ������ ������� ������ �� ������� �����
			if (debug) std::cout << "	Recognition";
			recognition (tmpFirst, tmpSecond, markUps, corrMarkUp, refs, t);
			if (debug) std::cout << "	Done.\n";
			first += tmpFirst;
			second += tmpSecond;
		}

		// ���������� �� ������ ������� ������ ���������� - �������� �� � �������������� ���� ������ ������ ������� ������
		pop.getStats (asNum, tmpFirst, tmpSecond, tmpGoal);
		pop.setStats (asNum, first, second, tmpGoal);
		if (debug) std::cout << "\n		As closed";
	}
	return 0;
}


/****************************************************************************
*				RecognizerExtend::run
*
*	Description:	������ �������� �������������
*	Parameters:	AxiomSetPop& pop - ���������, � ������� ��������
*	Returns:		0
*	Throws:		AxiomLibExeption - ���� ��������� �������� �������, ��� �� ����������������, ��� ��������
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int RecognizerExtend::run (AxiomSetPop& pop, std::vector <double> &precs) {
	const bool debug = false;
	if (debug) std::cout << "\n	RecognizerExtend:";
	// ��������; � ������ ���� ��� �� ������������������ ���������, AxiomSetPop ����� exception
	if (pop.size() == 0)
		throw AxiomLibException("RecognizerExtend::run - population of zero size.");
	// ���������� ���������� ����������
	bool bres;
	int first, second, tmpFirst, tmpSecond;
	double tmpGoal;
	std::vector <double> row;
	std::vector <std::vector <int> > markUps;
	std::vector <int> corrMarkUp;
	std::vector < std::vector <RefMarkupSet> > refs; // ������ �������� - �� ������� �� ���������� ������������� ������ � ������ ��������� ��������
	int numOfTests = 0;
	std::vector<int> numOfTS;
	// ������ ����� �������� �����
	bool c = dataSet.getTestSize (numOfTests, numOfTS);
	// ��������������� ��������� ������������ ������������ �������� ��� �������� �������� �� ������������ ������� �������
	markUps.resize(params.size());
	refs.resize(params.size());
	// ����� �� ���� �������� ������ � ���������
	//#pragma omp parallel for schedule(dynamic, 1)
	for (int asNum = 0; asNum < pop.size(); asNum++) {
		//if (debug) cout << "\n	process  " << omp_get_num_threads() << endl;
		if (debug) std::cout << "\n		As open " << asNum+1 << "\n";
		// ������ ��� ������ ������� ������, ��:
		// 1. ��������� ���� �� dataset-� ���� �������� ������:
		//      - �������� reference markups �� ���� ����������
		//      - �������� test markups �� ���� ����������
		// 2. ��������� ����� ������ ������� � ������� ���� �� ��������� test markups � ������� reference markup
		precision = precs[asNum];
		first = 0;
		second = 0;
		if (outputToFile) clearPath (pop.name(asNum), asNum);

		// �������� �������� ���� ���������� ����������� ���������
		if (debug) std::cout << "	InitRefs";
		for (int prm = 0; prm < (int) params.size(); prm++) {
			initRefs (pop, asNum, params[prm], refs[prm]);
			if (outputToFile) saveRefs (refs[prm], dataSet.paramName(params[prm]));
		}

		for (int t = 0; t < numOfTests; t++) {
			if (debug) std::cout << "			Test	" << t+1;
			corrMarkUp.clear();
			dataSet.getTSByIndexFromTests (corrMarkUp, t, -1);
			if (outputToFile) genFileName (t);
			for (int prm = 0; prm < (int) params.size(); prm++) {
				(markUps[prm]).clear();
				bres = dataSet.getTSByIndexFromTests (row, t, params[prm]);
				if (bres) {
					if (debug) std::cout << "	Entering";
					// �������� �������� ������ asNum ������ �� ����� ����������� � ������� ���������
					pop.enter (markUps[prm], row, 0, (unsigned long) row.size(), asNum);
					// ���������� �������� � ����, ���� ��� ����������
					if (outputToFile) saveRows (row, markUps[prm], dataSet.paramName(params[prm]));					
				}
				else {
					throw AxiomLibException("Error in RecognizerExtend::run: some params are missing in Tests in DataSets.");
				}
			}
			// ������ �������������� � �������� ����������� ��� ������ ������� ������ �� ������� �����
			if (debug) std::cout << "	Recognition";
			recognition (tmpFirst, tmpSecond, markUps, corrMarkUp, refs, t);
			if (debug) std::cout << "	Done.\n";
			first += tmpFirst;
			second += tmpSecond;
		}
		// ���������� �� ������ ������� ������ ���������� - �������� �� � �������������� ���� ������ ������ ������� ������
		pop.getStats (asNum, tmpFirst, tmpSecond, tmpGoal);
		pop.setStats (asNum, first, second, tmpGoal);
		if (debug) std::cout << "\n		As closed";
	}
	return 0;
}


/****************************************************************************
*					RecognizerExtend::run
*
*	Description:	������ ������������� �� ��������� ������ ������ � ������� ������
*	Parameters:		axiomSet - ������� ������, ������� �������������� ��������
*							��������� �����, ��� �������������
*					dataSet - ����� ������, �� ������� ������������ ������ ������� ������
*					params - ��������� ������ ������, �� ������� ������� ������
*					resFirst - ����� ������ ������� ����
*					resSecond - ����� ������ ������� ����
*					asIndex - ������, ��� ������� ��������� ������� ������, ���� ��� ����������
*	Returns:		0
*	Throws:			AxiomLibExeption - ���� ��������� �������� �������, ��� �� ����������������, ��� ��������
*	Author:			dk
*	History:
*
****************************************************************************/
signed int RecognizerExtend::run (AxiomSet& axiomSet, DataSet& extDataSet, std::vector<int>& extParams, int& resFirst, int& resSecond, const int asIndex) {
	std::vector<double> stat;
	return run (axiomSet, extDataSet, extParams, resFirst, resSecond, stat, asIndex);
}


/****************************************************************************
*					RecognizerExtend::run
*
*	Description:	������ ������������� �� ��������� ������ ������ � ������� ������
*	Parameters:		axiomSet - ������� ������, ������� �������������� ��������
*							��������� �����, ��� �������������
*					dataSet - ����� ������, �� ������� ������������ ������ ������� ������
*					params - ��������� ������ ������, �� ������� ������� ������
*					resFirst - ����� ������ ������� ����
*					resSecond - ����� ������ ������� ����
*					stat - ���������� �� ������ �� ������ �������, ������� ���������� ������������ ��
*					������� � �������� ��������� ���������� � �������� �������� �����
*					asIndex - ������, ��� ������� ��������� ������� ������, ���� ��� ����������
*	Returns:		0
*	Throws:			AxiomLibExeption - ���� ��������� �������� �������, ��� �� ����������������, ��� ��������
*	Author:			dk
*	History:
*
****************************************************************************/
signed int RecognizerExtend::run (AxiomSet& axiomSet, DataSet& extDataSet, std::vector<int>& extParams, int& resFirst, int& resSecond, std::vector<double> &stat, const int asIndex) {
	const bool debug = false;
	if (debug) std::cout << "\n	RecognizerExtend: run started\n";

	// ���������� ���������� ����������
	bool bres; // ��������� ����������, ������������ � �����
	int tmpFirst, tmpSecond; // ���������� ��� ������������ ����������
	std::vector <double> row; // ���� ����� ������������ ���� �� extDataSet'a
	std::vector <std::vector <int> > markUps; // ����� ����� ������ �������� ���� ������������
	std::vector <int> corrMarkUp; // ����� ����� ������ ���������� ������� �������� ����
	std::vector < std::vector <RefMarkupSet> > refs; // ������ �������� - �� ������� �� ���������� ������������� ������ � ������ ��������� ��������
	std::vector <bool> curStat; // ������ ��� ��������� ���������� �� ������������ ������� � ��������
	std::vector <bool> refStat; // ������ ��� ���������� ����������� �� ������������ �������� � ��������� ��������� ����������
	std::vector <bool> rowStat; // ������ ��� ���������� ����������� �� ������������ �������� � ��������� �������� �����
	// ������ ����� �������� ����� � extDataSet'e
	int numOfTests = 0;
	std::vector<int> numOfTS;
	bool c = extDataSet.getTestSize (numOfTests, numOfTS);
	// ��������������� ��������� ������������ ������������ �������� ��� �������� �������� �� ������������ ������� �������
	markUps.resize(extParams.size());
	refs.resize(extParams.size());

	// �������� ���������� ���������� �� ������ ������� ������ �� ������ extDataSet
	resFirst = 0;
	resSecond = 0;
	// �������� ���������� ����������: ������� ������ ���� ������� ����������
	int resShouldBe = 0;
	int tmpShouldBe = 0;

	if (outputToFile) clearPath (axiomSet.name(), asIndex);
	// �������� �������� ���� ���������� ����������� ���������
	if (debug) std::cout << "	InitRefs";
	for (int prm = 0; prm < (int) extParams.size(); prm++) {
		initRefs (axiomSet, extParams[prm], refs[prm], refStat);
		if (outputToFile) saveRefs (refs[prm], extDataSet.paramName(extParams[prm]));
	}
	
	// ������ ��� ������ ������� ������, ��:
	// 1. ��������� ���� �� extDataSet-� ���� �������� ������:
	//      - �������� test markups �� ���� ����������
	// 2. ��������� ����� ������ ������� � ������� ���� �� ��������� test markups � ������� reference markup
	
	// �������� ���������������� ���� ����������
	// omp_set_dynamic(1);
	// #pragma omp parallel for schedule(dynamic, 1)
	// for ...
	//cout << "\n	process  " << omp_get_num_threads() << endl;

	// ���� �� ���� ������ �� extDataSet'a
	for (int t = 0; t < numOfTests; t++) {
		if (debug) std::cout << "			Test	" << t+1;
		if (outputToFile) genFileName (t);
		corrMarkUp.clear();
		// ����������� ���������� �������� ������� �����
		extDataSet.getTSByIndexFromTests (corrMarkUp, t, -1);
		// ���� �� ���� ������������ �������� ���������� ����� �� extDataSet'a
		for (int prm = 0; prm < (int) extParams.size(); prm++) {
			(markUps[prm]).clear();
			// ���������� ���������� ��� ��� ����� ���������� ����
			// ��������� ��������� ������� �� ����� ���.
			bres = extDataSet.getTSByIndexFromTests (row, t, extParams[prm]);
			if (bres) {
				if (debug) std::cout << "	Entering";
				// �������� �������� ������ asNum ������ �� ����� ����������� � ������� ���������
				axiomSet.enter (markUps[prm], row, 0, (unsigned long) row.size(), curStat);
				updateVec (rowStat, curStat);
				// ���������� ���������� �������� � ����
				if (outputToFile) saveRows (row, markUps[prm], dataSet.paramName(params[prm]));
			}
			else {
				throw AxiomLibException("Error in RecognizerExtend::run: some params are missing in Tests in DataSets.");
			}
		}
		// ������ �������������� � �������� ����������� ��� ������ ������� ������ �� ������� �����
		if (debug) std::cout << "	Recognition";
		recognition (tmpFirst, tmpSecond, markUps, corrMarkUp, refs, t);
		if (debug) std::cout << "	Done.\n";
		resFirst += tmpFirst;
		resSecond += tmpSecond;
		// ��������� ���������� �� ������� �����
		analizeTest (corrMarkUp, tmpShouldBe);
		resShouldBe += tmpShouldBe;
	}

	// ������� ����������
	countStat (stat, resFirst, resSecond, resShouldBe, refStat, rowStat);

	if (debug) std::cout << "	Overall number of trajectories is " << resShouldBe << "\n";
		
	return 0;
}


/****************************************************************************
*					RecognizerExtend::saveRows
*
*	Description:	������� ���������� ���� � ��� �������� � ����
*	Parameters:		row - ����������� ������ ���������� ����
*					markUp - �������� ������� row �������� ������ asName
*					paramName - �������� ����������� ������� row � ���������
*					testNum - ����� ����� � ������������ ������ ������
*					asName - ��� ������� ������, ����������� ��� row � �������� markUp
*					asIndex - ������ ������� ������ � ��������� (���� ������������ ��������� ������ ������)
*	Returns:		0 - ���� ���������� ���������
*					< 0  - �����
*	Throws:			AxiomLibException - ���� ������� ��������� ������ �����
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerExtend::saveRows (std::vector <double> &row, std::vector<int> &markUp, std::string paramName) const {
	if (markUp.size() != row.size())
		throw AxiomLibException("Error in RecognizerExtend::clearPath: given current dir - is unreacheable.");
	boost::filesystem::path tmpPath( curFile );
	std::string fileName;
	fileName = curFile;
	fileName.append("^");
	boost::filesystem::path secPath( fileName );
	bool secondHand = false;
	if ( boost::filesystem::exists(tmpPath) ) {
		if ( boost::filesystem::exists(secPath) )
			boost::filesystem::remove (secPath);
		boost::filesystem::rename (tmpPath, secPath);
		secondHand = true;
	}
	
	// ����� ���������� � ����
	if (secondHand) {
		std::string str;
		boost::filesystem::ifstream inFile( secPath );
		ofstream file (curFile.c_str());
		if (!inFile.eof()) {
			std::getline(inFile, str);
			file << str;
		}
		file << paramName << ";";
		file << paramName.append("_markUp") << ";\n";
		for (unsigned int i = 0; i < row.size(); i++) {
			if (!inFile.eof()) {
				std::getline(inFile, str);
				file << str;
			}
			file << "\t" << row[i] << ";\t" << markUp[i] << ";\n";
		}
		file.close();
		inFile.close();
	} else {
		ofstream file (curFile.c_str());
		file << paramName << ";";
		file << paramName.append("_markUp") << ";\n";
		for (unsigned int i = 0; i < row.size(); i++) {
			file << "\t" << row[i] << ";\t" << markUp[i] << ";\n";
		}
		file.close();
	}

	if (secondHand)
		boost::filesystem::remove (secPath);
	return 0;
}


/****************************************************************************
*					RecognizerExtend::genFileName
*
*	Description:	������� ������� ��� ����� ��� ������� ������ � ��������� ��� �� ���������� ���������� ������
*	Parameters:		testNum - ����� ����� � ������������ ������ ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerExtend::genFileName (int testNum) {
	curFile = curDir;
	char tmpStr[10];
	sprintf(tmpStr,"%d",testNum + 1);
	curFile.append("/");
	curFile.append(tmpStr);
	curFile.append(".csv");
	return 0;
}


/****************************************************************************
*					RecognizerExtend::clearPath
*
*	Description:	������� ������� ������� �� �������� ����� � ������� � ������� ��� ����������
*	Parameters:		asName - ��� ������� ������, ����������� ��� row � �������� markUp
*					asIndex - ������ ������� ������ � ��������� (���� ������������ ��������� ������ ������)
*	Returns:		0 - ���� ���������� ���������
*					< 0  - �����
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerExtend::clearPath (std::string asName, int asIndex) {
	std::string pathToDir;
	// ��������� ���� � ����������
	pathToDir = whereToSave;
	if (whereToSave.find_last_of("/\\") != (whereToSave.size() - 1))
		pathToDir.append ("/");
	pathToDir.append (saveDirName);
	if (asIndex < 0)
		asIndex = searchFreeIndex (pathToDir);
	char tmpNum[10];
	sprintf(tmpNum,"%d",asIndex + 1);
	pathToDir.append (tmpNum);
	//pathToDir.append (asName);
	//pathToDir.append ("_");	
	//pathToDir.append (tmpNum);
	// �������� ���������� ���� ��� ���������� � �������� �� �����������
	boost::filesystem::path tmpPath( whereToSave );
	if ( !boost::filesystem::exists(tmpPath) )
		throw AxiomLibException("Error in RecognizerExtend::clearPath: given current dir - is unreacheable.");
	boost::filesystem::path dirPath( pathToDir );
	if ( boost::filesystem::exists(dirPath) )
		boost::filesystem::remove_all (dirPath);
	boost::filesystem::create_directory (dirPath);
	curDir = pathToDir;
	return 0;
}


/****************************************************************************
*					RecognizerExtend::searchFreeIndex
*
*	Description:	������� ������� ������, ��������� �� ������� �����, ��� ������ ����������� � ���� 
*					�������� � curPath ���� �������� ����������, ������� ��� �� ����������.
*	Parameters:		curPath - ������� ����������
*	Returns:		int - �������� ��������, ��������������� �������� ��� curPath, ������ � ������� 
*						���������� ��� ����������, ��� �� ������������ � ��������.
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerExtend::searchFreeIndex (const std::string &curPath) const {
	std::string path; 
	char tmss[20];
	int i = 1;
	for (i = 1; i < INT_MAX; i++) {
		path = curPath;
		tmss[0]='\0';
		sprintf(tmss,"%d",i);
		path.append (tmss);
		boost::filesystem::path tmpPath (path);
		if (!boost::filesystem::exists(tmpPath))
			break;
		if (!boost::filesystem::is_directory(tmpPath))
			break;
	}
	return (i - 1);
}


/****************************************************************************
*					RecognizerExtend::updateRows
*
*	Description:	������� ���������� ���� � ��� �������� � ����
*	Parameters:		corrMarkUp - ������ �������� �������� ���� ���������� ����������
*					result - ���������� �������� ���������� ����������
*	Returns:		0 - ���� ���������� ���������
*					< 0  - �����
*	Throws:			AxiomLibException - ���� ������� ��������� ������ �����
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerExtend::updateRows (std::vector <int> &corrMarkUp, std::vector<int> &result) const {
	if (corrMarkUp.size() != result.size())
		throw AxiomLibException("Error in RecognizerExtend::clearPath: given current dir - is unreacheable.");
	boost::filesystem::path tmpPath( curFile );
	std::string fileName;
	fileName = curFile;
	fileName.append("^");
	boost::filesystem::path secPath( fileName );
	bool secondHand = false;
	if ( boost::filesystem::exists(tmpPath) ) {
		if ( boost::filesystem::exists(secPath) )
			boost::filesystem::remove (secPath);
		boost::filesystem::rename (tmpPath, secPath);
		secondHand = true;
	}
	
	// ����� ���������� � ����
	if (secondHand) {
		std::string str;
		boost::filesystem::ifstream inFile( secPath );
		ofstream file (curFile.c_str());
		if (!inFile.eof()) {
			std::getline(inFile, str);
			file << str;
		}
		file << "correctAbnormBehavior" << ";" << "axiomSetResults" << ";\n";
		for (unsigned int i = 0; i < corrMarkUp.size(); i++) {
			if (!inFile.eof()) {
				std::getline(inFile, str);
				file << str;
			}
			file << "\t" << corrMarkUp[i] << ";\t" << result[i] << ";\n";
		}
		file.close();
		inFile.close();
	} else {
		ofstream file (curFile.c_str());
		file << "correctAbnormBehavior" << ";" << "axiomSetResults" << ";\n";
		for (unsigned int i = 0; i < corrMarkUp.size(); i++) {
			file << "\t" << corrMarkUp[i] << ";\t" << result[i] << ";\n";
		}
		file.close();
	}

	if (secondHand)
		boost::filesystem::remove (secPath);
	return 0;
}


/****************************************************************************
*					RecognizerExtend::saveRefs
*
*	Description:	������� ���������� �������� ���������� ����������� ��������� � �����
*	Parameters:		ref - ������ �������� ��������� ����� ����������� ���������
*	Returns:		0 - ���� ���������� ���������
*					< 0  - �����
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerExtend::saveRefs (std::vector <RefMarkupSet> &ref, std::string paramName) const {
	// ��������� -���������� �� ����������, � ������� ��������� �����
	boost::filesystem::path dirPath( curDir );
	if ( !boost::filesystem::exists(dirPath) )
		return -1;
	// ������� ����������� ����������
	std::string fileName, secName;
	bool secondHand;
	// ��������� ���������� ref � ����� � ������� fileName
	std::vector < std::vector <int> > axioms, axiomMinLens, axiomMaxLens;
	for (unsigned int i = 0; i < ref.size(); i++) { // ���� �� ��������� ����� ����������� ���������
		// �������� ���������� - ��� �������� � ����
		axioms.resize (ref[i].refMarkups.size());
		axiomMinLens.resize (ref[i].refMarkups.size());
		axiomMaxLens.resize (ref[i].refMarkups.size());
		for (unsigned int j = 0; j < ref[i].refMarkups.size(); j++) {
			ref[i].refMarkups[j].getCopy(axioms[j], axiomMinLens[j], axiomMaxLens[j]);
		}
		// ������� ���� � �����
		fileName = curDir;
		fileName.append("/mu_");
		fileName.append(dataSet.className (i));
		fileName.append(".csv");
		secName = fileName;
		secName.append("^");
		boost::filesystem::path secPath( secName );
		secondHand = false;
		// ���� ���� � ����� ������ ��� ����������, �� ������� ���
		boost::filesystem::path filePath( fileName );		
		if ( boost::filesystem::exists(filePath) ) {
			if ( boost::filesystem::exists(secPath) )
				boost::filesystem::remove (secPath);
			boost::filesystem::rename (filePath, secPath);
			secondHand = true;
		}
		// ����� ���������� � ����
		if (secondHand) {
			std::string str;
			boost::filesystem::ifstream inFile( secPath );
			ofstream file (fileName.c_str());
			if (!inFile.eof()) {
				std::getline(inFile, str);
				file << str;
			}
			// ���������
			unsigned int mLen = 0;
			for (unsigned int l = 0; l < axioms.size(); l++) {
				// ����� ���������
				file << "axioms[MinLen,MaxLen]";
				if (l > 0) {
					file << "_" << l;
				}
				file <<";";
				// ��������� ������������ ����� �������
				if (axioms[l].size() > mLen)
					mLen = axioms[l].size();
				// �������� �������� ������������� ���������� ������
				if ( (axioms[l].size() != axiomMinLens[l].size()) || (axioms[l].size() != axiomMaxLens[l].size()) )
					throw AxiomLibException("Error in RecognizerExtend::saveRefs: incorrect function response in RefMarkUp.");
			}
			file << "\n";
			// ���������� ��������
			for (unsigned int k = 0; k < mLen; k++) {
				if (!inFile.eof()) {
					std::getline(inFile, str);
					file << str;
				}
				for (unsigned int l = 0; l < axioms.size(); l++) {
					if (k < axioms[l].size()) {
						file << "\t" << axioms[l][k] << "[" << axiomMinLens[l][k] << "," << axiomMaxLens[l][k] << "];";
					} else {
						file << "\t\t;";
					}
				}
				file << "\n";
			}
			file.close();
			inFile.close();
		} else {
			ofstream file (fileName.c_str());
			// ���������
			unsigned int mLen = 0;
			for (unsigned int l = 0; l < axioms.size(); l++) {
				// ����� ���������
				file << "axioms[MinLen,MaxLen]";
				if (l > 0) {
					file << "_" << l;
				}
				file <<";";
				// ��������� ������������ ����� �������
				if (axioms[l].size() > mLen)
					mLen = axioms[l].size();
				// �������� �������� ������������� ���������� ������
				if ( (axioms[l].size() != axiomMinLens[l].size()) || (axioms[l].size() != axiomMaxLens[l].size()) )
					throw AxiomLibException("Error in RecognizerExtend::saveRefs: incorrect function response in RefMarkUp.");
			}
			file << "\n";
			// ���������� ��������
			for (unsigned int k = 0; k < mLen; k++) {
				for (unsigned int l = 0; l < axioms.size(); l++) {
					if (k < axioms[l].size()) {
						file << "\t" << axioms[l][k] << "[" << axiomMinLens[l][k] << "," << axiomMaxLens[l][k] << "];";
					} else {
						file << "\t\t;";
					}
				}
				file << "\n";
			}
			file.close();
		}
		if (secondHand)
			boost::filesystem::remove (secPath);
	}
	return 0;
}
