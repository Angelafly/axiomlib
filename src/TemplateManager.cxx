/****************************************************************************
*
*				������� ������ TemplateManager
*
****************************************************************************/

#include "Logger.h"

#include "TemplateManager.h"

#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#include "mpi.h"

using namespace AxiomLib;

// ���������� ������
TemplateManager::TemplateManager (void) {
	numOfTemplates = 0;
	templateGenetics.clear();
	outputToFile = false;
}


// ���������� ������ - �������� ��� ��������� � ������ ������ ������������ �������
TemplateManager::~TemplateManager (void) {
	for (unsigned int i = 0; i < templateGenetics.size(); i++)
		if (templateGenetics[i] != NULL) delete templateGenetics[i];
}


/****************************************************************************
*				TemplateManager::initFromEnv
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
int TemplateManager::initFromEnv (const Environment& env) {
	// �������������� dataSet
	std::string datasetDir, datasetName;
	if (env.getStringParamValue(datasetDir, "BaseDataSetDir") < 0)
		throw AxiomLibException("TemplateManager::setParamsFromEnv : data set directory is undefined.");
	if (env.getStringParamValue(datasetName, "DataSet") < 0)
		throw AxiomLibException("TemplateManager::setParamsFromEnv : data set is undefined.");

	// ��������� ����������� ��� ������� ������ ��������� � ������ ������
	EnvDataSet envDataSet;
	envDataSet.readConfigFile (datasetDir, datasetName);
	// ��������� ����������� ����������� NullStr - ����������� ��������� � ������ ����� ���� ������ ���� ���������� ���������
	dataSet.setNullStr (envDataSet);
	// ���������� ��������� ����� ������ - ������� ��� �� ���������� �������������
	dataSet.readDataSet(datasetDir, datasetName);
	// ��������������� � ������ ������ ������ �������� ���������� � ������������ �������������� �� ������� ����� ����� �������� ��������
	dataSet.getParamNums(params, env, envDataSet);
	// �������������� ������� ���������������
	Logger::getInstance()->writeDebug("Generating templates...");
	generateTemplates (env);
	Logger::getInstance()->writeDebug("Finished generating templates.");
	
	// �������������� �������� �������� �������� ���������������
	TemplateGeneticsFactory templateGeneticsFactory;
	std::string tplGeneticsName;
	if (env.getStringParamValue(tplGeneticsName, "TplGenetics") < 0)
		throw AxiomLibException("TemplateManager::setParamsFromEnv : TplGenetics is undefined.");
	templateGenetics.resize(templateRecognizers.size());
	
	Logger::getInstance()->writeDebug("Initializing template genetic algorithms...");
	for (unsigned int i = 0; i < templateGenetics.size(); i++) {
		templateGenetics[i] = templateGeneticsFactory.create (tplGeneticsName);
		templateGenetics[i]->initFromEnv (env);
	}
	Logger::getInstance()->writeDebug("Finished initializing template genetic algorithms.");

	// ���������, �� ������� ��������� ������ �������
	if (env.getStringParamValue(axiomSetBaseDir, "AxiomSetBaseDir") < 0)
		throw AxiomLibException("TemplateManager::setParamsFromEnv : axiomSetBaseDir directory is undefined.");
	if (env.getStringParamValue(axiomName, "saveTo") < 0)
		throw AxiomLibException("TemplateManager::setParamsFromEnv : axiomName to save is undefined.");
	if (env.getStringParamValue(whereToSave, "PathToSaveResults") < 0)
		whereToSave = axiomSetBaseDir;
	std::string str;
	env.getStringParamValue (str, "outputMarkUpToFile");
	if ((str == "true") || (str == "True") || (str == "TRUE"))
		outputToFile = true;

	return 0;
}


/****************************************************************************
*					TemplateManager::run
*
*	Description:	�������� ������� � ������ - ��������� �������� ������
*					�������������� �� �������.
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateManager::run (void) {
	if ((numOfTemplates != templateRecognizers.size()) || (numOfTemplates != templateGenetics.size()))
		throw AxiomLibException("TemplateManager::run : unable to run templateManager because of wrong number of numOfTemplates.");

	// ����������� ����� �������� ��������
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// ����������� ����� ������� ������� �������� �������
	int from, upTo, forOne; 
	forOne = (int) ((double) numOfTemplates / (double) size);
	from = rank * forOne;
	upTo = (rank + 1) * forOne;
	if (rank == (size - 1))
		upTo = numOfTemplates;

	std::cout << "\n	Process  " << rank << " size " <<  size << " templates: " << from + 1 << " - " << upTo << " out of " << numOfTemplates << "\n";
	std::cout.flush();

	/*if (rank == 0) {
		std::cout << "\n	To continue, please, press enter.";
		std::cin.get();
	}*/
	MPI_Barrier (MPI_COMM_WORLD);

	std::vector <double> resGoals;
	resGoals.resize(upTo - from);
	unsigned int curI = 0;
	std::string curAxiomSetName;
	//std::string ss;
	char tmss[10];
	for (int i = from; i < upTo; i++) {
		(templateRecognizers[i]).setDataSet (dataSet);
		(templateRecognizers[i]).setParamsOfDataSet (params);
		(templateRecognizers[i]).recognizer->setOutputToFile(false);
		resGoals[curI] = templateGenetics[i]->run (templateRecognizers[i]);
		int resFirst = -1;
		int resSecond = -1;
		if (outputToFile)
			(templateRecognizers[i]).runWithOutput (resFirst, resSecond, i);
		else
			(templateRecognizers[i]).run (resFirst, resSecond);
		// ���������� ������ ������� ������ � ����
		curAxiomSetName = axiomName;
		tmss[0]='\0';
		sprintf(tmss,"%d",i+1);
		//ss.assign(tmss);
		curAxiomSetName.append (tmss);
		(templateRecognizers[i]).axiomSet->saveAxiomSetToFile(whereToSave, curAxiomSetName, resFirst, resSecond);
		// ���������� ���������
		std::cout << "\nTemplateRecognizer with index " << i+1 << " has the results: " << resGoals[curI] << " = (" << resFirst << ", " << resSecond << ")\n";
		std::cout.flush();
		curI++;
	}

	double bestGoal = -1;
	for (unsigned int i = 0; i < resGoals.size(); i++)
		if ((bestGoal < 0) || (bestGoal > resGoals[i]))
			bestGoal = resGoals[i];

	MPI_Barrier (MPI_COMM_WORLD);

	std::cout << "\n	Process  " << rank << " size " <<  size << " templates: " << from + 1 << " - " << upTo << " - best goal is " << bestGoal << " .\n";
	std::cout.flush();

	return 0;
}


/****************************************************************************
*					TemplateManager::runExecuteOnly
*
*	Description:	������� ��� �������� �������� ��������������� �� �������� 
*					������� ������
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� � ����� �� ���������� �������� ������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateManager::runExecuteOnly (void) {
	if ((numOfTemplates != templateRecognizers.size()) || (numOfTemplates != templateGenetics.size()))
		throw AxiomLibException("TemplateManager::run : unable to run templateManager because of wrong number of numOfTemplates.");

	// ����������� ����� �������� ��������
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// ����������� ����� ������� ������� �������� �������
	int from, upTo, forOne; 
	from = 0;
	upTo = numOfTemplates;
	
	std::vector <double> resGoals;
	resGoals.resize(upTo - from);
	unsigned int curI = 0;
	std::string curAxiomSetName;
	char tmss[10];
	for (int i = from; i < upTo; i++) {
		(templateRecognizers[i]).setDataSet (dataSet);
		(templateRecognizers[i]).setParamsOfDataSet (params);
		//(templateRecognizers[i]).recognizer->setOutputToFile(false);
		int resFirst = -1;
		int resSecond = -1;
		if (outputToFile)
			(templateRecognizers[i]).runWithOutput (resFirst, resSecond, i);
		else
			(templateRecognizers[i]).run (resFirst, resSecond);
		// ����� ���������� � ����������� �������������
		std::cout << "\nTemplateRecognizer with index " << i+1 << " has the results: " << resGoals[curI] << " = (" << resFirst << ", " << resSecond << ")\n";
		std::cout.flush();
		curI++;
	}

	return 0;
}


/****************************************************************************
*					TemplateManager::generateTemplates
*
*	Description:	������� ������� ��������� ������� ���������������
*	Parameters:		void
*	Returns:		0
*	Throws:			AxiomLibException - ���� �������� ������ ��� �������� �������
*	Author:			dk
*	History:
*
****************************************************************************/
int TemplateManager::generateTemplates (const Environment& env) {

	// ������ ����������� ������ ������� �������� ��������� ��������: 
	/*	������������ ������������ �������: 
	*		1. ������������ ����� ����� ���������� ������������� 
	*		2. ������������ ����� ��������� ������ ������, ������������� � ���������������� �����
	*			� ����� ������ ������, ������� ��������� ��������� �������
	*		3. ������������ ����� ����� ���������� ������ �������� 
	*		4. �������� �������������� ������������ ��������, ��� ���������� � ������ ������� 1-3
	*/	

	// 1. ����������� ����� ��������� ������� �������������
	int numOfPreprocessors;
	/* // ����������� ����� ���������������� ����� �� ������� �������
	PreprocessFactory preprocessFactory;
	numOfPreprocessors = preprocessFactory.getSize();*/
	// ����������� ����� ���������������� �� ����������������� �����
	std::set <std::string> preprocNames;
	if (env.getStringSetParamValue (preprocNames, "Preprocessor") < 0)
		throw AxiomLibException("TemplateManager::generateTemplates : no preprocessor is set.");
	numOfPreprocessors = preprocNames.size();
	// �������� ������������ ����� ������� �������������
	if (numOfPreprocessors < 1)
		throw AxiomLibException("TemplateManager::generateTemplates : no preprocessor is present.");

	// 2. ����������� ����� ������ ������, ������� ����� �������������� ��� ���������� �������� ���������������
	int numOfAxiomSets;
    if (env.getIntParamValue(numOfAxiomSets, "popSize") < 0)
		throw AxiomLibException("TemplateManager::generateTemplates : popSize is not set.");
	double presetAxiomSets;
	if (env.getDoubleParamValue(presetAxiomSets, "popPreset") < 0)
		throw AxiomLibException("TemplateManager::generateTemplates : popPreset is not set.");
	// �������� ������������ ����� ������ ������
	if ((numOfAxiomSets < 1) || (presetAxiomSets < 0) || (presetAxiomSets > 1))
		throw AxiomLibException("TemplateManager::generateTemplates : incorrect number of AxiomSets is present.");

	// 3. ����������� ����� ��������� ���������� ������ ��������
	int numOfRecognizers;
	// ����������� ����� ��������������� �� ����������������� �����
	std::set <std::string> recogNames;
	if (env.getStringSetParamValue (recogNames, "Recognizer") < 0)
		throw AxiomLibException("TemplateManager::generateTemplates : no recognizer is set.");
	numOfRecognizers = recogNames.size();
	// �������� ������������ ����� ������� �������������
	if (numOfRecognizers < 1)
		throw AxiomLibException("TemplateManager::generateTemplates : no recognizer is present.");

	// 4. ����������� ������ ����� �������� � �������� ����� ������������ ��������
	// 4.1. ����� ����� ��������
	numOfTemplates = numOfPreprocessors * numOfAxiomSets * numOfRecognizers;
	if (numOfTemplates < 1)
		throw AxiomLibException("TemplateManager::generateTemplates : Overall number of templates is below then one.");

	// 4.2 ������� ��������� ����� �������� ���������������
	templateRecognizers.resize (numOfTemplates);
	// ��������� ��� �������� �������� ��������� ��������� 
	if (templateRecognizers.size() != numOfTemplates)
		throw AxiomLibException("TemplateManager::generateTemplates : problems with creating recognizer-templates.");

	// 4.3 ������ ������ ���������� ��������� ������� �� �������� ���������������
	int curTemplate = 0;
	// 4.3.1 ���������� ��� ��������� �������������
	PreprocessFactory preprocessFactory;
	std::set <std::string> :: const_iterator prep;
	
	// 4.3.2 ���������� ��� ��������� ������� ������
	int knownAxioms;
	knownAxioms = round (((double) numOfAxiomSets) * presetAxiomSets);

	// *.1. ���� AxiomBase - ��� ������ ���� ������, ������ ��
    std::string axiomBaseDir;
    if (env.getStringParamValue(axiomBaseDir, "AxiomBaseDir") < 0)
		throw AxiomLibException("TemplateManager::generateTemplates : popPreset > 0 but axiom base dir is not set.");
    AxiomBase axiomBase;
	Logger::getInstance()->writeDebug("Reading axioms from directory '" + axiomBaseDir + "'");
    axiomBase.readAxioms(axiomBaseDir.c_str());
	Logger::getInstance()->writeDebug("Finished reading axioms");

    // *.2. ���� ������� ���������� � �������� ������ ������
    std::string axiomSetDir;
    if (knownAxioms > 0 && env.getStringParamValue(axiomSetDir, "AxiomSetBaseDir") < 0)
      throw AxiomLibException("TemplateManager::generateTemplates : popPreset > 0 but axiom set base dir is not set.");

    // *.3. �������� ��������� ������� ������
    std::set<std::string> axiomSetNames;
    if (knownAxioms > 0 && env.getStringSetParamValue(axiomSetNames, "AxiomSet") < 0)
      throw AxiomLibException("TemplateManager::generateTemplates : popPreset > 0 but no axiom set is given.");

    // *.4. ���������� �������� ��� �������� �� ������ �������� ������ ������
    std::set<std::string>::const_iterator curAx = axiomSetNames.begin();

	// *.5. ���������� ��������� ��� ������ ������, ����������� ��������� �������
	int numOfAxioms;
	int maxECinAxiom;
	if (presetAxiomSets < 1) {
		if (env.getIntParamValue(numOfAxioms, "numOfAxioms") < 0)
			throw AxiomLibException("TemplateManager::generateTemplates : numOfAxioms is not set.");
		if (env.getIntParamValue(maxECinAxiom, "maxECinAxiom") < 0)
			throw AxiomLibException("TemplateManager::generateTemplates : maxECinAxiom is not set.");
	}

	// 4.3.3 ���������� � ��������� ��������� �������������
	RecognizerFactory recognizerFactory;
	std::set <std::string> :: const_iterator rec;

	Logger::getInstance()->writeDebug("Filling initial population...");
	// 4.3.4 ���������� �������� ���������������
	for (prep = preprocNames.begin(); prep != preprocNames.end(); ++prep) {
		for (int axi = 0; axi < numOfAxiomSets; axi++) {
			for (rec = recogNames.begin(); rec != recogNames.end(); ++rec) {
				// a. ������������� �������� �������������
				templateRecognizers[curTemplate].preprocess = preprocessFactory.create (*prep);
				templateRecognizers[curTemplate].preprocess->initFromEnv (env);

				// b. ������������� ������� ������
				// ���������� ����� ������� ������ ������������� [�������� / ���������]
				if (axi < knownAxioms) {
					// ������ ���������� ���� �� ������ ������ ������������� � ���������������� �����
					//std::cout << "TemplateManager::generateTemplates. Filling slot axiomSet with axiomSet '" << *curAx << "'\n";
					templateRecognizers[curTemplate].axiomSet = new AxiomSet;
					templateRecognizers[curTemplate].axiomSet->initAxiomSetFromFile(axiomSetDir, *curAx, axiomBase);
					// �������� �������� �� ������������ �������� ������
					++curAx;
					if (curAx == axiomSetNames.end())
						curAx = axiomSetNames.begin();
				} else {
					// ������ ���������� ������� ������, ��������������� ��������
					templateRecognizers[curTemplate].axiomSet = new AxiomSet;
					templateRecognizers[curTemplate].axiomSet->initAxiomSetByRand (numOfAxioms, maxECinAxiom);
				}

				// c. ������������� �������� ������ ��������
				templateRecognizers[curTemplate].recognizer = recognizerFactory.create (*rec);
				templateRecognizers[curTemplate].recognizer->setParamsFromEnv(env);

				// d. �������� ������� �������� ��� �������� � ���������� ���������� �������
				curTemplate++;
			}
		}
	}

	return 0;
}
