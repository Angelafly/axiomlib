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
#include "Environment.h"
#include "AxiomSet.h"
#include "DataSet.h"

namespace AxiomLib {

class RecognizerMultiMarkup : public RecognizerImplBase {

private:



    int stretch; // ������������ ��� �������� �������� ����
    MultiMarking::DTWMetric* metric;//������������ �������
    std::string name_metric; // name of metric

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

    RecognizerMultiMarkup() : metric(0) {}
    // ������� ���������� ������-������� �������� ��� ������ �������
    int initFromEnv (const Environment& env) ;

    // ������� ���������� ����� ��������������
    // ������������� ������������� ������� ������������ �����������
    Recognizer* copy (void) ;

    RecognizerMultiMarkup(const RecognizerMultiMarkup& other);





};// end of class

}; //  end of namespace

#endif // RECOGNIZERMULTIMARKUP_H
