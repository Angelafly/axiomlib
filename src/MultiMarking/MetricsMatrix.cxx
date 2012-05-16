/****************************************************************************
*			MetricsMatrix.�xx
*
*	Description: ���������� ������ ������
*	Author:		armkor
*	History:
*
****************************************************************************/

#include "MetricsMatrix.h"
#include "dtwmetric.h"

namespace AxiomLib {

namespace MultiMarking {

    double Matrix::compute(const std::vector<bool>& v1, const std::vector<bool>& v2) { // �������� ������� �������
        if ( v1[currentAxiomNumber] == v2[currentAxiomNumber] )
            return 1;
        return 0;
    }

    void Matrix::computeDTWForMetric(const MultiMark &t, int i, int Nmin, int Nmax, const MultiMark &ref, std::vector<double> &result)
    {
        MultiMark temp;
        int y=result.size(); // ������ ���-�� ������
        std::vector<double> tempresult(y); // ������������� ������ �������� DTW ��� ������ �� �����
        for (int e=0;e<y;e++){ // ��������� ����� DTW
            result[e]==0;
            tempresult[e]==0;
        }
        for (int k=0;k<y;k++){
            temp[0] = ref[k];
            this->currentAxiomNumber=k; // ������������� ����� ��������������� ������.
            DTWMetric::computeDTW (this,t,i,Nmin,Nmax,temp,tempresult); // �������� ������� ��� ������� ������
            for (int e=0;e<y;e++){
                result[e]+=tempresult[e]/y;
            }
        }

        // � ����� ��� ������� �������� currentAxiomNumber ������� computeDTW,
        // ����� ������� ����������
    }

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib


