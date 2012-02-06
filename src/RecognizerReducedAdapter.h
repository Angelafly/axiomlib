/****************************************************************************
*					RecognizerReducedAdapter
*
*	Description:	�����-�������, ����������� ��������� ReducedRecognizer
*					���, ��� ����� Recognizer
*	Author:			wictor
*	History:	
*
****************************************************************************/
#ifndef RECOGNIZERREDUCEDADAPTER_H
#define RECOGNIZERREDUCEDADAPTER_H

#include <boost/shared_ptr.hpp>

#include "RecognizerImplBase.h"
#include "ReducedRecognizer.h"
#include "TrajectorySampleDistance.h"

namespace AxiomLib {

class RecognizerReducedAdapter : public RecognizerImplBase
{
private:
	boost::shared_ptr<ReducedRecognizer> reducedRecognizer;
	
public:
	RecognizerReducedAdapter();
	
	// ������� ���������� ������-�������
	virtual int initFromEnv (const Environment& env);
	
	// ������� ������ ���������� �������� ���������� ��������, �. �. ��������� ��� ������ �� ��� ������� ���������� ��������� � �� ����
	virtual void computeDistances(AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, 
								 std::vector<TrajectorySampleDistance >& result, 
								 // ����� ������� ����������� ��� �������� ��������� ����������
								 std::vector<bool> &refAxiomUsage, 
								 // ����� ������� ����������� ��� �������� �������� ����������
								 std::vector<bool> &testAxiomUsage);
	
	// ������� ���������� ����� ��������������
    virtual Recognizer* copy ();
	
};

}

#endif // RECOGNIZERREDUCEDADAPTER_H
