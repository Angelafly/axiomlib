/****************************************************************************
*			RecognizerImplBase
*
*	Description:	������������� ����� � �������� �������� Recognizer. �
*					���� �������� ������ �������� ������ � ����������,
*					�������� �������� ����������� ������ ������ ����������
*					���������� �� ��������� ����������
*	Author:		wictor
*	History:	
*
****************************************************************************/
#ifndef RECOGNIZERIMPLBASE_H
#define RECOGNIZERIMPLBASE_H

#include <boost/shared_ptr.hpp>

#include "Recognizer.h"
#include "CompareStatistic.h"
#include "GoalStrategy.h"
#include "LabelingStrategy.h"
#include "TrajectorySampleDistance.h"

namespace AxiomLib {

class RecognizerImplBase : public Recognizer
{
private:
	bool m_comments;
	bool m_outputToFile;
	
	boost::shared_ptr<CompareStatistic> compareStatistic;
	boost::shared_ptr<GoalStrategy> goalStrategy;
	boost::shared_ptr<LabelingStrategy> labelingStrategy;
	
	static int coutAbnormalSegments(const vector<int> &labeling, int &abnormalSegmentsNum);
	static void countStat(std::vector<double>& stat, int resFirst, int resSecond, int abnormalSegmentsNum,
						  const std::vector<bool>& refStat, const std::vector<bool> &testStat);
	
	// ������� ������ ���������� �������� ���������� ��������, �. �. ��������� ��� ������ �� ��� ������� ���������� ��������� � �� ����
	void performLabeling(AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, 
								 std::vector<std::vector<int> >& result, 
								 // ����� ������� ����������� ��� �������� ��������� ����������
								 std::vector<bool> &refAxiomUsage, 
								 // ����� ������� ����������� ��� �������� �������� ����������
								 std::vector<bool> &testAxiomUsage);
	
protected:
	bool comments();
	bool outputToFile();
	
public:
	RecognizerImplBase();
	
	// ����������� �����������
	RecognizerImplBase(const RecognizerImplBase& other);
	
	// ��������� ������ �������� ���������� ������ comments
    virtual void setComments(const bool newValue);
  
    // ��������� ������ �������� ���������� ������ outputToFile
    virtual void setOutputToFile(const bool newValue);
  
    // ������� ������� �������� ���������� ������, � ��� ����� - ������� �������� � �������� ������
    virtual int setParamsFromEnv (const Environment& env);
    
    // ������� ������� �������� ������ ������������ ����� ���������� ������, � ��� ����� - ������� �������� � �������� ������
    virtual int setNominalParamsFromEnv (const Environment& env);
    
	//��������
    // ������� ��������� ������ ������ ������ � ������� ����� ��������������
    virtual int setDataSet (const DataSet &newDataSet);
    
	//��������
    // ������� ��������� ������ ��������� ��������� ������ params
    virtual int setParamsOfDataSet (std::vector<int> &newParams);
    
	//��������
    virtual signed int run (AxiomSetPop& pop);
    
	//��������
    virtual signed int run (AxiomSetPop& pop, std::vector <double> &precs);
  
	// �������� �������� ����� � ������� ������,
    // �������� ��������������, ��������� ����������
    // ������ 1-�� � 2-�� ����
    virtual signed int run (AxiomSet& axiomSet, DataSet& dataSet, 
							std::vector<int>& params, int& resFirst, int& resSecond, 
							const int asIndex = -1);
  
	// �������� �������� ����� � ������� ������,
    // �������� ��������������, ��������� ����������
    // ������ 1-�� � 2-�� ����, ��������� ����������
    virtual signed int run (AxiomSet& axiomSet, DataSet& dataSet, 
							std::vector<int>& params, int& resFirst, int& resSecond, 
							std::vector<double> &stat, const int asIndex = -1);
	// ������� �������� ��������� �������������
    virtual double learn (TemplateRecognizer &templateRecognizer);
	
	//---------------------------������ ����������� �������-------------------------------------
protected:
	
	// ���������� ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������
	virtual void computeDistances(AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, 
								  // ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������
								  std::vector<TrajectorySampleDistance>& result, 
								  // ����� ������� ����������� ��� �������� ��������� ����������
								  std::vector<bool>& refAxiomUsage, 
								  // ����� ������� ����������� ��� �������� �������� ����������
								  std::vector<bool>& testAxiomUsage) = 0;
public:
	
	// ������� ���������� ������-�������
	virtual int initFromEnv (const Environment& env) = 0;
	
	// ������� ���������� ����� ��������������
	// ������������� ������������� ������� ������������ �����������
    virtual Recognizer* copy (void) = 0;
};

}

#endif // RECOGNIZERIMPLBASE_H
