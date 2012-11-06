/****************************************************************************
*			RecognizerMultiMarkup
*
*	Description:  RecognizerMultiMarkup
*
*	Author:		armkor
*	History:
*
****************************************************************************/

#ifndef RECOGNIZERMULTIMARKUP_H
#define RECOGNIZERMULTIMARKUP_H

#include "RecognizerImplBase.h"
#include "MultiMarking/dtwmetric.h"
#include "MultiMarking/AxiomUsageWrapper.h"
#include "Environment.h"
#include "AxiomSet.h"
#include "DataSet.h"

namespace AxiomLib {

typedef double (*ResultStrategy)(const std::vector<double>& v);

class RecognizerMultiMarkup : public RecognizerImplBase {

private:
	double mStretch; // ������������ ��� �������� �������� ����
	MultiMarking::DTWMetric* mMetric;
	std::string mMetricName; // name of metric
	ResultStrategy mResultStrategy;
	bool mCancelAxioms;

    ~RecognizerMultiMarkup();

protected:

    // ���������� ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������
    void computeDistances(AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, // ������ �������� ������ ���� ������������
                                                              // ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������
                                                              std::vector<TrajectorySampleDistance>& result,
                                                              // ����� ������� ����������� ��� �������� ��������� ����������
                                                              std::vector<bool>& refAxiomUsage,
                                                              // ����� ������� ����������� ��� �������� �������� ����������
                                                              std::vector<bool>& testAxiomUsage) ;
public:

	RecognizerMultiMarkup() : mMetric(0) {}
    // ������� ���������� ������-������� �������� ��� ������ �������
    int initFromEnv (const Environment& env) ;

    // ������� ���������� ����� ��������������
    Recognizer* copy (void) ;

    RecognizerMultiMarkup(const RecognizerMultiMarkup& other);

};// end of class

}; //  end of namespace

#endif // RECOGNIZERMULTIMARKUP_H
