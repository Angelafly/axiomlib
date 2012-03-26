#include "RecognizerMultimarkup.h"

private:

    int stretch; // ������������ ��� �������� �������� ����


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

    // ������� ���������� ������-������� �������� ��� ������ �������
    int initFromEnv (const Environment& env) ;

    // ������� ���������� ����� ��������������
    // ������������� ������������� ������� ������������ �����������
    Recognizer* copy (void) ;
