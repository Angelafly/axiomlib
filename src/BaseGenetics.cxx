/****************************************************************************
*
*			������� ������ BaseGenetics
*
****************************************************************************/

#include "BaseGenetics.h"

using namespace AxiomLib;

// ���������� ������ � �������� ��������� ���������� �� ���������
BaseGenetics::BaseGenetics (void) {
	numOfIter = -1; 
	comments = true;
	exitCond = 0;
	axiomSetBaseDir = NULL;
	whereToSave = NULL;
	selectionStrategy = NULL;
	transmuteStrategy = NULL;
	crossoverStrategy = NULL;
	goalStrategy = NULL;
	recognizer = NULL;
} 


// ���������� ������ - �������� ��� ��������� � ������ ������ ������������ �������
BaseGenetics::~BaseGenetics (void) {
	if (recognizer != NULL) delete recognizer;
	if (selectionStrategy != NULL) delete selectionStrategy;
	if (transmuteStrategy != NULL) delete transmuteStrategy;
	if (crossoverStrategy != NULL) delete crossoverStrategy;
	if (goalStrategy != NULL) delete goalStrategy;
	if (axiomSetBaseDir != NULL) delete axiomSetBaseDir;
	if (whereToSave != NULL) delete whereToSave;
}


/****************************************************************************
*			BaseGenetics::initFromEnv
*
*	Description:	������������� ���������� � ������� �� env
*	Parameters:	const Environment& env - ��������� � ����������� ������������
*	Returns:	0
*	Throws:		AxiomLibException - ���� ������������ �����������
*	Author:		gevor
*	History:
*
****************************************************************************/
int BaseGenetics::initFromEnv(const Environment& env)
{
  // ��������� ����� ��������
  if (env.getIntParamValue(numOfIter, "numOfIter") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : number of iterations has not been set.");

  // �������������� ��������� ���������
  axiomSetPop.initFromEnv(env);

  // �������������� ��������������
  string recognizerName;
  RecognizerFactory rf;
  if (env.getStringParamValue(recognizerName, "Recognizer") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : Recognizer is undefined.");

  recognizer = rf.create(recognizerName);
  recognizer->setParamsFromEnv(env);

  // �������������� ��������� ��������
  string selectionStrategyName;
  SelectionStrategyFactory ssf;
  if (env.getStringParamValue(selectionStrategyName, "selectClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : selection strategy is undefined.");
  selectionStrategy = ssf.create(selectionStrategyName);
  selectionStrategy->setParamsFromEnv(env);
  selectionStrategy->setComments(comments);

  // �������������� ��������� �������
  string transmuteStrategyName;
  TransmuteStrategyFactory tsf;
  if (env.getStringParamValue(transmuteStrategyName, "transClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : mutation strategy is undefined.");
  transmuteStrategy = tsf.create(transmuteStrategyName);
  transmuteStrategy->setParamsFromEnv(env);

  // �������������� ��������� �����������
  string crossoverStrategyName;
  CrossoverStrategyFactory csf;
  if (env.getStringParamValue(crossoverStrategyName, "crossClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : crossover strategy is undefined.");
  crossoverStrategy = csf.create(crossoverStrategyName);
  crossoverStrategy->setParamsFromEnv(env);

  // �������������� ��������� ���������� ������� �������
  string goalStrategyName;
  GoalStrategyFactory gsf;
  if (env.getStringParamValue(goalStrategyName, "goalClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : goal strategy is undefined.");
  goalStrategy = gsf.create(goalStrategyName);
  goalStrategy->setParamsFromEnv(env);

  // �������������� ������� ���������� ������� �� ����� ������������� ���������
  if (env.getDoubleParamValue (exitCond, "exitCond") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : goal strategy is undefined.");

  // ��������� ���� �� �������� ��������� ������ ������� ������ �� ����������� ������ �����
  std::string temp;
  if (env.getStringParamValue(temp, "AxiomSetBaseDir") >= 0) {
    axiomSetBaseDir = new string;
    *axiomSetBaseDir = temp;
  }
  if (env.getStringParamValue(temp, "saveTo") >= 0) {
    whereToSave = new string;
    *whereToSave = temp;
  }

  return 0;
}


/****************************************************************************
*			BaseGenetics::shortInitFromEnv
*
*	Description:	������������� ������ ������������ ����� ���������� � ������� �� env
*	Parameters:	const Environment& env - ��������� � ����������� ������������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������������ �����������
*	Author:		gevor, dk
*	History:
*
****************************************************************************/
int BaseGenetics::shortInitFromEnv(const Environment& env)
{
  // �������������� ��������� ���������
  axiomSetPop.initFromEnv (env);

  // �������������� ��������������
  string recognizerName;
  RecognizerFactory rf;
  if (env.getStringParamValue(recognizerName, "Recognizer") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : Recognizer is undefined.");
  recognizer = rf.create(recognizerName);
  recognizer->setNominalParamsFromEnv(env);
  
  // �������������� ��������� ��������
  string selectionStrategyName;
  SelectionStrategyFactory ssf;
  if (env.getStringParamValue(selectionStrategyName, "selectClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : selection strategy is undefined.");
  selectionStrategy = ssf.create(selectionStrategyName);
  selectionStrategy->setParamsFromEnv (env);
  selectionStrategy->setComments(comments);

  // �������������� ��������� �������
  string transmuteStrategyName;
  TransmuteStrategyFactory tsf;
  if (env.getStringParamValue(transmuteStrategyName, "transClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : mutation strategy is undefined.");
  transmuteStrategy = tsf.create(transmuteStrategyName);
  transmuteStrategy->setParamsFromEnv (env);

  // �������������� ��������� �����������
  string crossoverStrategyName;
  CrossoverStrategyFactory csf;
  if (env.getStringParamValue(crossoverStrategyName, "crossClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : crossover strategy is undefined.");
  crossoverStrategy = csf.create(crossoverStrategyName);
  crossoverStrategy->setParamsFromEnv (env);
  
  // �������������� ��������� ���������� ������� �������
  string goalStrategyName;
  GoalStrategyFactory gsf;
  if (env.getStringParamValue(goalStrategyName, "goalClass") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : goal strategy is undefined.");
  goalStrategy = gsf.create(goalStrategyName);
  goalStrategy->setParamsFromEnv (env);
  
  // ������������� ������� ���������� ������� �� ����� ������������� ���������
  if (env.getDoubleParamValue (exitCond, "exitCond") < 0)
    throw AxiomLibException("BaseGenetics::initFromEnv : goal strategy is undefined.");
  
  return 0;
}


/****************************************************************************
*				BaseGenetics::setDataSet
*
*	Description:	��������� ������ ������ ������ ��� ������� ������ ������� ������� ������
*	Parameters:	newDataSet - ����� ����� ������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int BaseGenetics::setDataSet (const DataSet &newDataSet) {
	return recognizer->setDataSet (newDataSet);
}
  

/****************************************************************************
*				BaseGenetics::setParamsOfDataSet
*
*	Description:	��������� ������� �������� ������������� ������� 
*				���������� ������ ������ �� ������� ������� ������
*	Parameters:	params - ��������������� ������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int BaseGenetics::setParamsOfDataSet (std::vector<int> &params) {
	return recognizer->setParamsOfDataSet(params);
}


/****************************************************************************
*				BaseGenetics::setNumOfIter
*
*	Description:	��������� ������ �������� ����� �������� ��� ������� ����������� ������� ������
*	Parameters:	newNumOfIter - ����� �������� ����� ��������
*	Returns:		0
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int BaseGenetics::setNumOfIter (const int newNumOfIter) {
	numOfIter = newNumOfIter;
	return 0;
}


/****************************************************************************
*			BaseGenetics::run
*
*	Description:	������ ��
*	Parameters:	-
*	Returns:		0
*	Throws:		AxiomLibException - ���� ��������� ������ ��� �� ��
*				��� ���������������
*	Author:		gevor, dk
*	History:
*
****************************************************************************/
int BaseGenetics::run(void) {
	// ������������ ������������ ��������:
	// 0. ���������, ����������� �� ���������
	// � �����:
	// 1. ��������� �� ������������ �������� ������ ��������������
	// 2. ��������� �������� ������� ������� �� ������ �� ������ ������
	// 3. ��������������, ��������� �������� ��������
	// 4. ��������
	// 5. ����������

	// 0. �������� ����������
	if (numOfIter < 0)
		throw AxiomLibException("BaseGenetics::run : number of iterations < 0; probably GA has not been initialized.");
		
	AxiomSetWithStats bestAxiomSet; // ������� ���������� ��� �������� ������ ������� ������ �� �������
	// �������� ����
	for (int gaIter=0; gaIter < numOfIter; gaIter++) {
		if (comments) { 
			cout << "	BaseGenetics: iteration " << gaIter+1 << " out of " << numOfIter << ", pop size = " << axiomSetPop.size() << "\n";
			std::cout.flush();
		}

		// 1. ��������� �� ������������ ��������� �������� �������� � �������������, �������� ����� ������ ������� � ������� ����, ��������� ����������.
		recognizer->run(axiomSetPop);
		
		// 2. ��������� ���������� H(first, second)
		goalStrategy->compute(axiomSetPop);
		
		// 3. �������� �������� �������� (�� ������ �����, ��������, �������������)
		if (gaIter != 0) {
			selectionStrategy->select(axiomSetPop);
		}
		
		// ��������� ������� ������
		if (axiomSetPop.exit(exitCond, bestAxiomSet)) break;
		
		// 4. ��������
		transmuteStrategy->transmute(axiomSetPop);
		
		// 5. ����������    
		crossoverStrategy->crossover(axiomSetPop);
		
		if (comments) cout << "\n";
	}
	// ��������� ������ ������� ������ � ����
	if (comments) {
		int  first, second;
		double goal;
		bestAxiomSet.unsafeGetStats (first, second, goal);
		std::cout << "\n	Best Results are:\n	    first and second and goal\n		" << first << "	"  << second << "	 " << goal << "\n";
		std::cout.flush();
	}
	if ( (axiomSetBaseDir != NULL) && (whereToSave != NULL) )
		bestAxiomSet.saveAxiomSetToFile (*axiomSetBaseDir, *whereToSave);

	return 0;
}


/****************************************************************************
*			BaseGenetics::run
*
*	Description:	������ ������������� ��������� � ������� ������������ 
*				��������� � ��������� ���������� �������� ������� �������
*	Parameters:	asp - ���������� ��� ������ �������� ������������ ���������
*	Returns:		double - ��������� ���������� �������� ������� �������
*	Throws:		AxiomLibException - ���� ��������� ������ ��� �� ��
*				��� ���������������
*	Author:		dk
*	History:
*
****************************************************************************/
double BaseGenetics::run(AxiomSetPop &asp) {
	// ������������ ������������ ��������:
	// 0. ���������, ����������� �� ���������
	// � �����:
	// 1. ��������� �� ������������ �������� ������ ��������������
	// 2. ��������� �������� ������� ������� �� ������ �� ������ ������
	// 3. ��������������, ��������� �������� ��������
	// 4. ��������
	// 5. ����������

	// 0. �������� ����������
	if (numOfIter < 0)
		throw AxiomLibException("BaseGenetics::run : number of iterations < 0; probably GA has not been initialized.");
		
	AxiomSetWithStats bestAxiomSet; // ������� ���������� ��� �������� ������ ������� ������ �� �������
	// �������� ����
	for (int gaIter=0; gaIter < numOfIter; gaIter++) {
		//if (comments) cout << "	BaseGenetics: iteration " << gaIter+1 << " out of " << numOfIter << ", pop size = " << axiomSetPop.size() << "\n";
		// 1. ��������� �� ������������ ��������� �������� �������� � �������������, �������� ����� ������ ������� � ������� ����, ��������� ����������.
		recognizer->run(axiomSetPop);
		
		// 2. ��������� ���������� H(first, second)
		goalStrategy->compute(axiomSetPop);
		
		// 3. �������� �������� �������� (�� ������ �����, ��������, �������������)
		if (gaIter != 0) {
			selectionStrategy->select(axiomSetPop);
		}
		
		// ��������� ������� ������
		if (axiomSetPop.exit(exitCond, bestAxiomSet)) break;
		
		// 4. ��������
		transmuteStrategy->transmute(axiomSetPop);
		
		// 5. ����������    
		crossoverStrategy->crossover(axiomSetPop);
		
		if (comments) cout << "\n";
	}
	// ������ ���������� ��������� ��� ��������
	//asp = axiomSetPop;
	// ���������� ������ �������� ������� �������
	int  first, second;
	double goal;
	bestAxiomSet.unsafeGetStats (first, second, goal);
	//std::cout << "\n	BG: going out. Press Enter please.\n";
	//std::cin.get();
	return goal;
}


/****************************************************************************
*			BaseGenetics::writeResultsToDisplay
*
*	Description:	����� ����������� �� �� �������
*	Parameters:	0
*	Returns:	0
*	Throws:		AxiomLibException - ���� ��������� ������ ��� �� ��
*				��� �������
*	Author:		gevor
*	History:
*
****************************************************************************/
int BaseGenetics::writeResultsToDisplay(void)
{
	return 0;
}
