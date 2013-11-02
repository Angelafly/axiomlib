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

    public:

    Equal();

	virtual DTWMetric* clone() const { return DTWMetric::clone_impl(this); }

    ~Equal() { }
    // realisation distance for DTW
    double compute(const std::vector<bool>& v1, const std::vector<bool>& v2);

    void computeDTWForMetric (const MultiMark& t, int i,int Nmin,int Nmax, const MultiMark& ref, std::vector<double>& result);

};// end of class

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib
