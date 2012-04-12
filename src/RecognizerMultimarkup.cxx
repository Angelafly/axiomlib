#include "RecognizerMultimarkup.h"
#include "MultiMarking/dtwmetric.h"
#include "Environment.h"
#include "AxiomSet.h"
#include "DataSet.h"
namespace AxiomLib {


    void compare (std::vector<bool>& m, std::vector<bool> p){
        for (int i=0;i<m.size();i++){
         if (p[i])
             m[i]=true;
        }
    }
    double minimum (std::vector<double> r){
        double min=r[0];
        for(int i=0;i<r.size();i++){
            if (r[i]<min)
                min=r[i];
        }
        return min;
    }

    // ���������� ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������

    void RecognizerMultiMarkup::computeDistances(AxiomSet& axiomSet, DataSet& dataSet, std::vector<int>& params, // ������ �������� ������ ���� ������������
                                                              // ���������� �� �������� �������� �������� ���������� �� �������� ��������� ����������
                                                              std::vector<TrajectorySampleDistance>& result,
                                                              // ����� ������� ����������� ��� �������� ��������� ����������
                                                              std::vector<bool>& refAxiomUsage,
                                                              // ����� ������� ����������� ��� �������� �������� ����������
                                                              std::vector<bool>& testAxiomUsage) {
        // ������, � ������� �������� ������������� �������� ������������ ������
        std::vector<bool> temp_testAxiomUsage;
        std::vector<bool> temp_refAxiomUsage;
        temp_testAxiomUsage.resize(axiomSet.size());
        temp_refAxiomUsage.resize(axiomSet.size());
        testAxiomUsage.resize(axiomSet.size());
        refAxiomUsage.resize(axiomSet.size());

        // �������� ����������

        // �������� ��������� ����������
        // ������, � ������� ����� ��������� �������� ��������� ����������
        vector <MultiMarking::MultiMark> etalon(dataSet.getNumberOfClasses()); // ��� ���������� getClassSize �� getNumberOfClasses()
        // ������(���������), � ������� ��������� ��������� ���
        std::vector<double> vec;  // ���� �� � ������ �������
        // ���������� ����� �������
        for (int i=0;i<dataSet.getNumberOfClasses();i++){
            if (!(dataSet.getTSByIndexFromClass(vec, i , 0, params[0])))
                throw AxiomLibException("Can not create row");
            axiomSet.enter(etalon[i], vec , 0 , vec.size(), temp_refAxiomUsage);
            compare (&refAxiomUsage,temp_refAxiomUsage);
        }

        // �������� �������� ����������
        // ������, � ������� �������� �������� �������� ����������
        // ������, � ������� ����� ��������� �������� ��������� ����������
        vector <MultiMarking::MultiMark> tests (dataSet.getTestSize());
        for (int i=0;i<dataSet.getTestSize();i++){
            if (!(dataSet.getTSByIndexFromTests(vec, i, params[0])))
                throw AxiomLibException("Can not create row");
            axiomSet.enter(tests[i], vec , 0 , vec.size(), temp_refAxiomUsage);
            compare (&refAxiomUsage,temp_refAxiomUsage);
        }

        // DTW
        std::vector<double> temp_result;
        for (int j=0;j<tests.size();j++){
            for (int i=0;i<etalon.size();i++){ // ������ ����� � ������ ������ ��������?
                // ��� ��� ���� �� ������ ������� � �������� ���������� + ������� ������, � ������� ��� ������� ������� - ����������� ����������
                MultiMarking::DTWMetric::computeDTW (metric, tests[j], 0 , stretch*etalon[i],etalon[i]*(1.0/stretch), etalon[i], &temp_result); // ����������� ������������ ����� ����
                // ��� ���� ������� ����� result � ����� ��� � ������, ����������� ����
                // ����� �������� � TrajectorySampleDistance
            }
        }


    }




    // ������� ���������� ������-������� �������� ��� ������ �������
    int RecognizerMultiMarkup::initFromEnv (const Environment& env) {
        // ������ ��������� ������� ����   ��������� ���� - ���� �� ����, �������� �� ������ ������ ������+���� �� ����
        if (env.getIntParamValue(stretch, "stretch")!=0)
            throw AxiomLibException("Can not find stretch");
        // ������ ��������� - �������
        std::string name_metric;
        if (env.getStringParamValue(name_metric, "metric")!=0)
            throw AxiomLibException("Can not find a name of metric");
        metric = MultiMarking::DTWMetric::getMetric(name_metric);  // +namepace
    }

    // ������� ���������� ����� ��������������
    // ������������� ������������� ������� ������������ �����������

    Recognizer* RecognizerMultiMarkup::copy (void){
        return new RecognizerMultiMarkup(*this);
    }

    RecognizerMultiMarkup::RecognizerMultiMarkup(const RecognizerMultiMarkup& other) {
        stretch = other.stretch;
        metric_name = other.metric_name;
        metric = DTWMetric::getMetric(other.metric_name);
    }

    ~RecognizerMultiMarkup() {
        if(metric) delete metric;
    }
}

