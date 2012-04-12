/****************************************************************************
*			MetricsHamming.h
*
*	Description: �����-�������. ������� - ���������� ��������.
*	Author:		armkor
*	History:
*
****************************************************************************/

#include "dtwmetric.h"
#include "multimarks.h"

namespace AxiomLib {

namespace MultiMarking {

class Hamming : public DTWMetric {

    private:

    int because_of_error;

    public:

    //
    double compute(std::vector<bool>& v1, std::vector<bool>& v2);

    void computeDTWForMetric(const MultiMark &t, int i, int Nmin, int Nmax, const MultiMark &ref, std::vector<double> &result);

};// end of class

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib

