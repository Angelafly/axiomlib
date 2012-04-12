/****************************************************************************
*			MetricsEqual.h
*
*	Description: �����-�������. �������� ����� - ������� ���������� DTW
*                    ���� ������� - ������ ���������� ��������.
*	Author:		armkor
*	History:
*
****************************************************************************/

#include "dtwmetric.h"
#include "multimarks.h"

namespace AxiomLib {

namespace MultiMarking {

class Equal : public DTWMetric {

public:

    int s;

    public:

    Equal();

    ~Equal() { }

    // realisation distance for DTW
    double compute(std::vector<bool>& v1, std::vector<bool>& v2);

    void computeDTWForMetric(const MultiMark &t, int i, int Nmin, int Nmax, const MultiMark &ref, std::vector<double> &result);

};// end of class

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib
