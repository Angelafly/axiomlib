/****************************************************************************
*			MetricsMatrix.�xx
*
*	Description: ���������� ������ ������
*	Author:		armkor
*	History:
*
****************************************************************************/

#include "MetricsMatrix.h"


namespace AxiomLib {

namespace MultiMarking {

    double Matrix::compute(const std::vector<bool>& v1, const std::vector<bool>& v2) {
        int indicator=0;
        std::vector<bool> s=v1;
        // ���������� �� ��������� ������ v1[currentAxiomNumber] � v2[currentAxiomNumber]
        for (int i=0;i<s.size();i++) {
            if (v1[i]!=v2[i]) {
                indicator=1;
            }
        }
        return indicator;
    }

    double Matrix::computeDTWForMetric(const MultiMark &t, int i, int Nmin, int Nmax, const MultiMark &ref, std::vector<double> &result)
    {
        // � ����� ��� ������� �������� currentAxiomNumber ������� computeDTW,
        // ����� ������� ����������
    }

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib


