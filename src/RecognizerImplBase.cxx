#include "TemplateRecognizer.h"
#include "CompareClassFactory.h"
#include "LabelingStrategyFactory.h"
#include "GoalStrategyFactory.h"
#include "Logger.h"


#include "RecognizerImplBase.h"

using namespace AxiomLib;

RecognizerImplBase::RecognizerImplBase()
{
}

/****************************************************************************
*                       ReducedRecognizerDistance::run
*
*       Description:    ����������� ����������� -- ��������� labelingStrategy
*       Parameters:     other
*       Returns:        -
*       Throws:         -
*       Author:         wictor
*       History:
*
****************************************************************************/
RecognizerImplBase::RecognizerImplBase(const RecognizerImplBase &other)
	: Recognizer(other),
	  m_comments(other.m_comments),
	  m_outputToFile(other.m_outputToFile),
	  compareStatistic(other.compareStatistic),
	  goalStrategy(other.goalStrategy),
	  labelingStrategy(boost::shared_ptr<LabelingStrategy>(other.labelingStrategy->copy()))
{ }

void AxiomLib::RecognizerImplBase::setComments(const bool newValue)
{
	m_comments = newValue;
}

void AxiomLib::RecognizerImplBase::setOutputToFile(const bool newValue)
{
	m_outputToFile = newValue;
}

int AxiomLib::RecognizerImplBase::setParamsFromEnv(const AxiomLib::Environment &env)
{
	std::string compareClassName;
	if(env.getStringParamValue(compareClassName, "compareClass")) {
		throw AxiomLibException("RecognizerImplBase::setParamsFromEnv : compare class not specified");
	}
	
	CompareClassFactory factory;
	compareStatistic = boost::shared_ptr<CompareStatistic>(factory.create(compareClassName));
	
	if(compareStatistic.get() == 0) {
		throw AxiomLibException("RecognizerImplBase::initFromEnv : incorrect compareClass name in config");
	}
	
	compareStatistic->setParamsFromEnv(env);
	
	std::string labelingStrategyName;
	env.getMandatoryParamValue(labelingStrategyName, "LabelingStrategy");
	
	LabelingStrategyFactory labelingStrategyFactory;
	labelingStrategy = boost::shared_ptr<LabelingStrategy>
			(labelingStrategyFactory.create(labelingStrategyName));
	if(labelingStrategy.get() == 0) {
		throw AxiomLibException("RecognizerImplBase::initFromEnv : incorrect LabelingStrategy name in config");
	}
	
	labelingStrategy->initFromEnv(env);
	
	std::string goalStrategyName;
	GoalStrategyFactory gsf;
	if (env.getStringParamValue(goalStrategyName, "goalClass") < 0) {
		throw AxiomLibException("RecognizerImplBase::initFromEnv : goal strategy is undefined.");
	}
	goalStrategy = boost::shared_ptr<GoalStrategy>(gsf.create(goalStrategyName));
	
	if(goalStrategy.get() == 0) {
		throw AxiomLibException("RecognizerImplBase::initFromEnv : incorrect GoalStrategy name in config");
	}
	
	goalStrategy->setParamsFromEnv(env);
	
	return initFromEnv(env);
}

int AxiomLib::RecognizerImplBase::setNominalParamsFromEnv(const AxiomLib::Environment &env)
{
	return setParamsFromEnv(env);
}

int AxiomLib::RecognizerImplBase::setDataSet(const AxiomLib::DataSet &/*newDataSet*/)
{
	//��������
	return -1;
}

int AxiomLib::RecognizerImplBase::setParamsOfDataSet(std::vector<int> &/*newParams*/)
{
	//��������
	return -1;
}

signed int AxiomLib::RecognizerImplBase::run(AxiomLib::AxiomSetPop &/*pop*/)
{
	//��������
	return -1;
}

signed int AxiomLib::RecognizerImplBase::run(AxiomLib::AxiomSetPop &/*pop*/, std::vector<double> &/*precs*/)
{
	//��������
	return -1;
}

signed int AxiomLib::RecognizerImplBase::run(AxiomLib::AxiomSet &axiomSet, AxiomLib::DataSet &dataSet, 
											 std::vector<int> &params, int &resFirst, int &resSecond, 
											 const int asIndex)
{
	std::vector<double> stat;
	return run(axiomSet, dataSet, params, resFirst, resSecond, stat, asIndex);
}

signed int AxiomLib::RecognizerImplBase::run(AxiomSet& axiomSet, DataSet& extDataSet,
											 std::vector<int>& extParams, int& resFirst, int& resSecond, 
											 std::vector<double> &stat, const int /*asIndex*/)
{
	/*
	  1. �������� ���������� �������� ���������� � ���������� ������������� ������
	  2. ��������� ����� ������ � ����������
	*/
	//Logger::getInstance()->writeDebug("Entering RecognizerImplBase::run()");
	
	std::vector<std::vector<int> > labelings;
	std::vector<bool> refStat;
	std::vector<bool> testStat;
	
	// 1. �������� ���������� �������� ���������� � ���������� ������������� ������
	//Logger::getInstance()->writeDebug("RecognizerImplBase::run(): labeling...");
	performLabeling(axiomSet, extDataSet, extParams, labelings, refStat, testStat);
	//Logger::getInstance()->writeDebug("RecognizerImplBase::run(): labeling finished");
	
	//int nTests = labelings.size();
	
	int nTests;
	std::vector<int> nTS;
	//Logger::getInstance()->writeDebug("RecognizerImplBase::run(): checking test size");
	extDataSet.getTestSize(nTests, nTS);
	if(nTests != labelings.size()) {
		throw AxiomLibException("RecognizerImplBase::run() : number of labelings does not match number of tests");
	}
	
	
	resFirst = 0;
	resSecond = 0;
	
	int resShouldBe = 0;
	
	//int nDims = extParams.size();
	std::vector <int> corrLabeling; // ����� ����� ������ ���������� ���������� �������� ����
	
	// 2. ������� ����� ������
	//Logger::getInstance()->writeDebug("RecognizerImplBase::run(): counting errors");
	for (int i = 0; i < nTests; ++i) {
		double tmpFirst = 0, tmpSecond = 0; // ���������� ��� ������������ ����������
		int tmpShouldBe = 0;
		extDataSet.getTSByIndexFromTests (corrLabeling, i, -1);
		//Logger::getInstance()->writeDebug("Getting statistics...");
		compareStatistic->getStatistic(labelings[i], corrLabeling, tmpFirst, tmpSecond, comments());
		//Logger::getInstance()->writeDebug("Finished getting statistics");
		resFirst += (int)tmpFirst;
		resSecond += (int)tmpSecond;
		coutAbnormalSegments(corrLabeling, tmpShouldBe);
		resShouldBe += tmpShouldBe;
	}
	
	//Logger::getInstance()->writeDebug("RecognizerImplBase::run() : counting stat");
	
	// ������� ����������
	countStat(stat, resFirst, resSecond, resShouldBe, refStat, testStat);
			
	//Logger::getInstance()->writeDebug("Leaving RecognizerImplBase::run()");
	return 0;	
}

void RecognizerImplBase::performLabeling(AxiomSet &axiomSet, DataSet &dataSet, 
										 std::vector<int> &params, 
										 std::vector<std::vector<int> > &result, 
										 std::vector<bool> &refAxiomUsage, 
										 std::vector<bool> &testAxiomUsage) {
	//Logger::getInstance()->writeDebug("Entering RecognizerImplBase::performLabeling()");
	{
		std::vector<TrajectorySampleDistance> sampleDistances;
		
		computeDistances(axiomSet, dataSet, params, sampleDistances, refAxiomUsage, testAxiomUsage);
		
		if(dataSet.getNumberOfTests() != sampleDistances.size()) {
			throw AxiomLibException("RecognizerImplBase::performLabeling() : sampleDistances size does not match number of test trajectories");
		}
		
		result.resize(sampleDistances.size());
		
		//Logger::getInstance()->writeDebug("RecognizerImplBase::performLabeling() : labeling");
		
		for (std::vector<TrajectorySampleDistance>::size_type i = 0; i < sampleDistances.size(); ++i) {
			//Logger::getInstance()->writeDebug("RecognizerImplBase::performLabeling() : labeling test trajectory " + boost::lexical_cast<std::string>(i));
			labelingStrategy->performLabeling(sampleDistances[i], result[i]);
			//Logger::getInstance()->writeDebug("RecognizerImplBase::performLabeling() : finished labeling test trajectory " + boost::lexical_cast<std::string>(i));
		}
	
	}
	//Logger::getInstance()->writeDebug("Leaving RecognizerImplBase::performLabeling()");
}

double RecognizerImplBase::learn(TemplateRecognizer &templateRecognizer)
{
	std::vector<TrajectorySampleDistance> sampleDistances;
	std::vector<bool> refAxiomUsage, testAxiomUsage;
	
	DataSet& dataSet = templateRecognizer.prepDataSet;
	AxiomSet& axiomSet = *(templateRecognizer.axiomSet);
	computeDistances(axiomSet, dataSet, templateRecognizer.params,
					 sampleDistances, refAxiomUsage, testAxiomUsage);
	
	int numClasses = templateRecognizer.prepDataSet.getNumberOfClasses();
	
	std::vector<std::vector<int> > rightLabelings(numClasses);
	
	for(int i = 0; i < numClasses; ++i) {
		dataSet.getTSByIndexFromTests(rightLabelings[i], i, -1);
	}
	
	if(labelingStrategy.get() == 0) {
		Logger::getInstance()->writeDebug("RecognizerImplBase::learn() : warning : labelingStrategy is null");
	}
	
	return labelingStrategy->train(sampleDistances, rightLabelings, 
								   compareStatistic.get(), goalStrategy.get());
	
}

/****************************************************************************
*				RecognizerImplBase::coutAbnormalSegments
*
*	Description:	������ ������ ����� - ���������� ����� ����� ���������� ���������� ��������� � ������ �����
*	Parameters:		labeling - ���������� �������� �����
*					abnormalSegmentsNum - ����� �������� ���������� ���������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int RecognizerImplBase::coutAbnormalSegments(const std::vector<int>& labeling, int &abnormalSegmentsNum)
{
	unsigned int i = 0;
	unsigned int j = 0;
	abnormalSegmentsNum = 0;
	while (i < labeling.size()) {
		if (labeling[i] != 0) {
			abnormalSegmentsNum++;
			j = i+1;
			while (j < labeling.size()) {
				if (labeling[j] != labeling[i])
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
*					RecognizerImplBase::countStat
*
*	Description:	��������������� ��������� ��� �������� ���������� �� ����������� �������������
*	Parameters:		stat - ����������� ������ ����������
*					resFisrt - ����� ������ ������� ����
*					resSecond - ����� ������ ������� ����
*					abnormalSegmentsNum - ����� ����� ��������� ��������, ������� ���� � �������� ������
*					refStat - ���������� �� ������������� ������ ��� �������� ��������� ����������
*					testStat - ���������� �� ������������� ������ ��� �������� �������� �����
*	Returns:		-
*	Throws:			AxiomLibException - ���� �� ������� ��������� �� �����������
*	Author:			dk
*	History:
*
****************************************************************************/
void RecognizerImplBase::countStat(std::vector<double> &stat, int resFirst, int resSecond, 
								   int abnormalSegmentsNum, 
								   const std::vector<bool> &refStat, 
								   const std::vector<bool> &testStat)
{
	if (refStat.size() != testStat.size())
		throw AxiomLibException("RecognizerExtend::countStat : internal error - statistial data is uncoordinated.");
	stat.resize (refStat.size());
	int temp;
	double multiplier;
	multiplier = 0.5 * (double) (abnormalSegmentsNum - resSecond) / (double) (abnormalSegmentsNum + resFirst); // resFisrt � resSecond ���������� ���������� �� ���� ���������, ������� � ����� ������� � ������ �������������
	for (unsigned int i = 0; i < refStat.size(); i++) {
		temp = 0;
		if (refStat[i]) temp++;
		if (testStat[i]) temp++;
		stat[i] = (double) temp * multiplier;
	}
}

bool AxiomLib::RecognizerImplBase::comments()
{
	return m_comments;
}

bool AxiomLib::RecognizerImplBase::outputToFile()
{
	return m_outputToFile;
}
