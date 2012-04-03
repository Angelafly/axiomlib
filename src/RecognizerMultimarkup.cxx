#include "RecognizerMultimarkup.h"

/*
    int stretch; // ������������ ��� �������� �������� ����
    DTWMetric* metric;

    // ���������� ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������

    void computeDistances(AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, // ������ �������� ������ ���� ������������
                                                              // ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������
                                                              std::vector<TrajectorySampleDistance>& result,
                                                              // ����� ������� ����������� ��� �������� ��������� ����������
                                                              std::vector<bool>& refAxiomUsage,
                                                              // ����� ������� ����������� ��� �������� �������� ����������
                                                              std::vector<bool>& testAxiomUsage) {

        // �������� ��������� ����������.
        vector <MultiMarking::MultiMark> etalon(dataSet.getNumberOfClasses());
        for (int i=0;i<dataSet.getNumberOfClasses();i++){
             axiomSet.enter(etalon[i], dataSet.referenceClassesTS[i] , 0 , dataSet.referenceClassesTS[i].size(), &testAxiomUsage)
        }







        //������������ ����������, � ������� ����� ��������� ��������� ���
        std::vector<double> row;
        // ��������� ���������� ����
        dataSet.getTSByIndexFromClass(&row,params[0],params[1],params[2]);
        // ������������� �������� ��������������� ����������
        MultiMarking::MultiMark marked_in;
        MultiMarking::MultiMark marked_etalon;
        axiomSet.enter(&marked_in, &row , 0 , row.size(), &testAxiomUsage)

    }

/*
    int enter (MultiMarking::MultiMark& marked, const std::vector<double>& row, const unsigned long begin, const unsigned long end, std::vector<bool> &stat);
bool getTSByIndexFromClass (std::vector<double> &vec, int indexClass, int indexMultiTS, int indexTS) const;
*/
    // ������� ���������� ������-������� �������� ��� ������ �������
/*
    int RecognizerMultiMarkup::initFromEnv (const Environment& env) {
        // ������ ��������� ������� ����
        env.getIntParamValue(stretch, "stretch");
        // ������ ��������� - �������
        std::string name_metric;
        env.getStringParamValue(name_metric, "metric");
        metric = metric.getMetric(name_metric);
    }

    // ������� ���������� ����� ��������������
    // ������������� ������������� ������� ������������ �����������

    Recognizer* RecognizerMultiMarkup::copy (void){
        Recognizer* m;
        m.stretch= this->stretch;
        m.metric = this->metric;
    }
*/
