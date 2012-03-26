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

private:

    int because_of_error;

    public:

    // realisation distance for DTW
    double compute(std::vector<bool>& v1, std::vector<bool>& v2);

};// end of class

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib
