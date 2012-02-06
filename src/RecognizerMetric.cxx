/****************************************************************************
*				RecognizerMetric.cxx
*
*	Description:	������� ������ RecognizerMetric - ����������
*	Author:		gevor
*	History:		
*
****************************************************************************/
#include <limits.h>

#include "RecognizerMetric.h"

using namespace AxiomLib;

// ����������� ����������� - ������� �������� ���������� ���������� �� ���������
RecognizerMetric::RecognizerMetric (void) { 
	comments = false; 
	outputToFile = false;
	whereToSave = "./";
	saveDirName = "axiomSet_";
	curDir.clear();
	curFile.clear();
	precision = 0.0; 
	compareStatistic = NULL;
}

// ����������� ���������� - �������� ���� ������������ �������� ������
RecognizerMetric::~RecognizerMetric () { 
	if (compareStatistic != NULL) delete compareStatistic; 
}

/****************************************************************************
*                       RecognizerMetric::setParamsFromEnv
*
*       Description:    ���������������� ��������� ������� �� env
*       Parameters:     const Environment& env - �����
*       Returns:        0
*       Throws:         AxiomLibException - ���� ��� ������-�����������
*       Author:         gevor
*       History:
*
****************************************************************************/
int RecognizerMetric::setParamsFromEnv (const Environment& env) {
	// �������������� dataset
	std::string datasetDir, datasetName;
	if (env.getStringParamValue(datasetDir, "BaseDataSetDir") < 0)
		throw AxiomLibException("RecognizerMetric::setParamsFromEnv : data set directory is undefined.");
	if (env.getStringParamValue(datasetName, "DataSet") < 0)
		throw AxiomLibException("RecognizerMetric::setParamsFromEnv : data set is undefined.");

	// ��������� ����������� ��� ������� ������ ��������� � ������ ������
	EnvDataSet envDataSet;
	envDataSet.readConfigFile (datasetDir, datasetName);

	// ��������� ����������� ����������� NullStr - ����������� ��������� � ������ ����� ���� ������ ���� ���������� ���������
	dataSet.setNullStr (envDataSet);
	// ���������� ��������� ����� ������ - ������� ��� �� ���������� �������������
	dataSet.readDataSet(datasetDir, datasetName);

	// ��������������� � ������ ������ ������ �������� ���������� � ������������ �������������� �� ������� ����� ����� �������� ��������
	dataSet.getParamNums(params, env, envDataSet);

	// ��������� ���������� ��� ������ ������ �������� ��������� ����������
	rms.setParamsFromEnv(env);

	// ��������� ���������� ����������� � MarkUp
	if (env.getDoubleParamValue(precision, "MaxDistance") < 0)
		throw AxiomLibException("RecognizerMetric::setParamsFromEnv : precision is undefined.");
 	//std::cout << "\n Precision set to " << precision << " !\n";
	// �������� ������ �������� ����������

	std::string compClassName;
	CompareClassFactory ccf;
	if (env.getStringParamValue(compClassName, "compareClass") < 0)
		throw AxiomLibException("RecognizerMetric::setParamsFromEnv : compare-class is undefined.");
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
*                       RecognizerMetric::setNominalParamsFromEnv
*
*       Description:	���������������� ������ �������� ��������� ������� �� env
*       Parameters:	const Environment& env - �����
*       Returns:		0
*       Throws:		AxiomLibException - ���� ��� ������-�� ���������
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerMetric::setNominalParamsFromEnv (const Environment& env) {
	// ��������� ���������� ����������� � ��������
	if (env.getDoubleParamValue(precision, "precision") < 0)
		throw AxiomLibException("RecognizerMetric::setParamsFromEnv : precision is undefined.");
	
	// ��������� ���������� ��� ������ ������ �������� ��������� ����������
	rms.setParamsFromEnv(env);
	
	// �������� ������ �������� ����������
	std::string compClassName;
	CompareClassFactory ccf;
	if (env.getStringParamValue(compClassName, "compareClass") < 0)
		throw AxiomLibException("RecognizerMetric::setParamsFromEnv : compare-class is undefined.");
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
*                       RecognizerMetric::setDataSet
*
*       Description:	������� ��������� ������ ������ ������ � ������� ����� ��������������
*       Parameters:	newDataSet - ����� ����� ������
*       Returns:		0
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerMetric::setDataSet (const DataSet &newDataSet) {
	dataSet = newDataSet;
	return 0;
}


/****************************************************************************
*                       RecognizerMetric::setParamsOfDataSet
*
*       Description:	������� ��������� ������ ��������� ��������� ������ params
*       Parameters:	newParams - ����� �������� �������
*       Returns:		0
*       Throws:		-
*       Author:		dk
*       History:
*
****************************************************************************/
int RecognizerMetric::setParamsOfDataSet (std::vector<int> &newParams) {
	params = newParams;
	return 0;
}


/****************************************************************************
*                       RecognizerMetric::setPrecision
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
int RecognizerMetric::setPrecision (const double newPrecision) {
	if ( (newPrecision > 0.0) && (newPrecision < 1.0) ) {
		precision = newPrecision;
		return 0;
	}
	return -1;
}


/****************************************************************************
*                       RecognizerMetric::setCompareStatistic
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
int RecognizerMetric::setCompareStatistic (CompareStatistic *newCompareStatistic) {
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
*                       RecognizerMetric::setRMS
*
*       Description:	������� ��������� ������ ������ ��������
*       Parameters:		newRMS - ����� ��������, ����� �������� 
*						��������������� ��� ����� ����� ��������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
int RecognizerMetric::setRMS (RefMarkupSetSimple &newRMS) {
	rms = newRMS;
	return 0;
}


/****************************************************************************
*                       RecognizerMetric::copy
*
*       Description:	������� ���������� ����� ������� ��������������
*       Parameters:		-
*       Returns:		Recognizer* - ������ �� ����� ������� ��������������
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
Recognizer* RecognizerMetric::copy (void) {
	RecognizerMetric* toReturn;
	toReturn = new RecognizerMetric;
	toReturn->setDataSet (dataSet);
	toReturn->setParamsOfDataSet (params);
	toReturn->setPrecision (precision);
	toReturn->setCompareStatistic (compareStatistic);
	toReturn->setRMS(rms);
	toReturn->setOutputToFile(outputToFile, whereToSave, saveDirName);
	return (Recognizer*) toReturn;
}


/****************************************************************************
*						RecognizerMetric::learn
*
*       Description:	������� �������� ������� ��������� �������������
*       Parameters:		templateRecognizer - ������ ��������������, �� �������
*						��������� �������� �������� ������� ��������� �������������
*       Returns:		0
*       Throws:			-
*       Author:			dk
*       History:
*
****************************************************************************/
double RecognizerMetric::learn (TemplateRecognizer &templateRecognizer) {

	return -1.0;
}


/****************************************************************************
*				RecognizerMetric::joinResults
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
inline signed int RecognizerMetric::joinResults (std::vector <int> &result, std::vector < std::vector <int> > &prelimResult, const int numOfClasses) const {
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
		throw AxiomLibException("RecognizerMetric::joinResults: incorrect results in recognition process.");
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
		for (unsigned int t = 0; t < bulletin.size(); t++) {
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
*				RecognizerMetric::recognition
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
inline signed int RecognizerMetric::recognition (int &first, int &second,  std::vector <std::vector <int> > &markUps, 
													std::vector <int> &corrMarkUp, 
													std::vector < std::vector <RefMarkupSetSimple> > &refs,
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
	int flag = 0;
	double tmpFirst, tmpSecond; // ��������� ��� �������� ����� ������ �� ������ �� ������
	int wholeNum; // ����� ������� �� ������ �� ������
	std::vector < std::vector <int> > prelimResult;
	prelimResult.resize(markUps.size());
	std::vector <int> result;
	first = 0;
	second = 0;
	
	// 1. ���� �� ���� ��������������� ������������ ��������� �����
	for (int j = 0; j < (int) markUps.size(); j++) {
		(prelimResult[j]).resize((markUps[j]).size());
		prelimResult[j][0] = 0;
		// 1. ����� ��������� �������� ��������������� �� markUps[j]
		for (long int i = 1; i < (long int) markUps[j].size(); i++) {
			prelimResult[j][i] = 0;
			flag = 0;
			// ���������� ���� �� ���������� � ���������� ������ - �.�. ���� ���� �� 1 ����� �������������
			for (int k = 0; k < (int) refs[j].size(); k++) {
				flag = (refs[j][k]).check (precision, markUps[j], i);
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
		throw AxiomLibException("Error in RecognizerMetric::recognition: incorrect results in process of getting statistic.");
	
	// 4. ���������� ����� �� ���������� � ������� �� �������
	first = (int) tmpFirst;
	second = (int) tmpSecond;
	return 0;
}


/****************************************************************************
*				RecognizerMetric::initRefs
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
inline signed int RecognizerMetric::initRefs (AxiomSetPop& pop, const int asNum, const int param, std::vector <RefMarkupSetSimple> &refer) {
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
*				RecognizerMetric::run
*
*	Description:	������ �������� �������������
*	Parameters:	AxiomSetPop& pop - ���������, � ������� ��������
*	Returns:		0
*	Throws:		AxiomLibExeption - ���� ��������� �������� �������, ��� �� ����������������, ��� ��������
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int RecognizerMetric::run (AxiomSetPop& pop) {
	const bool debug = false;
	if (debug) std::cout << "\n	Recognizer:";
	// ��������; � ������ ���� ��� �� ������������������ ���������, AxiomSetPop ����� exception
	if (pop.size() == 0)
		throw AxiomLibException("RecognizerMetric::run - population of zero size.");
	// ���������� ���������� ����������
	bool bres;
	int first, second, tmpFirst, tmpSecond;
	double tmpGoal;
	std::vector <double> row;
	std::vector <std::vector <int> > markUps;
	std::vector <int> corrMarkUp;
	std::vector <double> corrMarkUpDouble;
	std::vector < std::vector <RefMarkupSetSimple> > refs; // ������ �������� - �� ������� �� ���������� ������������� ������ � ������ ��������� ��������
	int numOfTests = 0;
	std::vector<int> numOfTS;
	// ������ ����� �������� �����
	bool c = dataSet.getTestSize (numOfTests, numOfTS);
	// ��������������� ��������� ������������ ������������ �������� ��� �������� �������� �� ������������ ������� �������
	markUps.resize(params.size());
	refs.resize(params.size());
	// ����� �� ���� �������� ������ � ���������
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
			dataSet.getTSByIndexFromTests (corrMarkUpDouble, t, -1);
			// ������� ������� ���������� �������� � ����� �����
			if (debug) std::cout << "        Resizing";
			corrMarkUp.resize(corrMarkUpDouble.size());
			for (unsigned int r = 0; r < corrMarkUpDouble.size(); r++) corrMarkUp[r] = (int) corrMarkUpDouble[r];
			for (unsigned int prm = 0; prm < params.size(); prm++) {
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
					throw AxiomLibException("Error in RecognizerMetric::run: some params are missing in Tests in DataSets.");
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
		if (debug) std::cout << "		As closed";
	}
	return 0;
}


// �������� �������� ����� � ������� ������,
  // �������� ��������������, ��������� ����������
  // ������ ������� � ������� ����
signed int RecognizerMetric::run (AxiomSetPop& pop, std::vector <double> &precs) {
	run (pop);
	return 0;
}


/****************************************************************************
*					RecognizerMetric::saveRows
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
int RecognizerMetric::saveRows (std::vector <double> &row, std::vector<int> &markUp, std::string paramName) const {
	if (markUp.size() != row.size())
		throw AxiomLibException("Error in RecognizerMetric::clearPath: given current dir - is unreacheable.");
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
*					RecognizerMetric::genFileName
*
*	Description:	������� ������� ��� ����� ��� ������� ������ � ��������� ��� �� ���������� ���������� ������
*	Parameters:		testNum - ����� ����� � ������������ ������ ������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerMetric::genFileName (int testNum) {
	curFile = curDir;
	char tmpStr[10];
	sprintf(tmpStr,"%d",testNum + 1);
	curFile.append("/");
	curFile.append(tmpStr);
	curFile.append(".csv");
	return 0;
}


/****************************************************************************
*					RecognizerMetric::clearPath
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
int RecognizerMetric::clearPath (std::string asName, int asIndex) {
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
		throw AxiomLibException("Error in RecognizerMetric::clearPath: given current dir - is unreacheable.");
	boost::filesystem::path dirPath( pathToDir );
	if ( boost::filesystem::exists(dirPath) )
		boost::filesystem::remove_all (dirPath);
	boost::filesystem::create_directory (dirPath);
	curDir = pathToDir;
	return 0;
}


/****************************************************************************
*					RecognizerMetric::searchFreeIndex
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
int RecognizerMetric::searchFreeIndex (const std::string &curPath) const {
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
*					RecognizerMetric::updateRows
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
int RecognizerMetric::updateRows (std::vector <int> &corrMarkUp, std::vector<int> &result) const {
	if (corrMarkUp.size() != result.size())
		throw AxiomLibException("Error in RecognizerMetric::clearPath: given current dir - is unreacheable.");
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
*					RecognizerMetric::saveRefs
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
int RecognizerMetric::saveRefs (std::vector <RefMarkupSetSimple> &ref, std::string paramName) const {
	// ��������� -���������� �� ����������, � ������� ��������� �����
	boost::filesystem::path dirPath( curDir );
	if ( !boost::filesystem::exists(dirPath) )
		return -1;
	// ������� ����������� ����������
	std::string fileName, secName;
	bool secondHand;
	// ��������� ���������� ref � ����� � ������� fileName
	std::vector < std::vector <int> > axioms;
	for (unsigned int i = 0; i < ref.size(); i++) { // ���� �� ��������� ����� ����������� ���������
		// �������� ���������� - ��� �������� � ����
		axioms.resize (ref[i].refMarkups.size());
		for (unsigned int j = 0; j < ref[i].refMarkups.size(); j++) {
			ref[i].refMarkups[j].getCopy(axioms[j]);
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
				file << "axioms";
				if (l > 0) {
					file << "_" << l;
				}
				file <<";";
				// ��������� ������������ ����� �������
				if (axioms[l].size() > mLen)
					mLen = axioms[l].size();
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
						file << "\t" << axioms[l][k] << ";";
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
				file << "axioms";
				if (l > 0) {
					file << "_" << l;
				}
				file <<";";
				// ��������� ������������ ����� �������
				if (axioms[l].size() > mLen)
					mLen = axioms[l].size();
			}
			file << "\n";
			// ���������� ��������
			for (unsigned int k = 0; k < mLen; k++) {
				for (unsigned int l = 0; l < axioms.size(); l++) {
					if (k < axioms[l].size()) {
						file << "\t" << axioms[l][k] << ";";
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
