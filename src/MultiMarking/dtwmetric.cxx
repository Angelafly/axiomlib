/****************************************************************************
*			dtwmetric.�xx
*
*	Description: ���������� ������� ������
*	Author:		armkor
*	History:
*
****************************************************************************/

#include "dtwmetric.h"


namespace AxiomLib {

namespace MultiMarking {

    double calculate(double a,double b,double c) {
        return ((a+b)/(c+1));
    }

    void DTWMetric::computeDTW (DTWMetric* m, const MultiMark& t,int i,int Nmin,
                                int Nmax, const MultiMark& ref, std::vector<double>& result) {
        // ������������� ������
        // vector<vector<double> >
        MultiMark D,R,S;
        int len_t=Nmax; // ������������ ������ ���� = ������ �������
        int len_ref=ref.size(); /* ����� ��������� ����������*/

        // ������ ��� �������. ������� ���, �� R S ����������� � �����
        // ��������� ��������� ������
        for (int w=0;w<(len_ref+1);w++) {
            for (int q=0;q<(len_t+1);q++) {
                S[w].push_back(0);
                R[w].push_back(0);
                D[w].push_back(0);
            }
        }

        // �������� ������� ����������
        for (int a=0 ; a < len_ref ; a++) {
            for (int b=0 ; b < len_t ; b++ ) {
                D[a][b]=m->compute ( ref[a],t[i-len_t+1+b] );
            }
        }

        // ������ � ��������� R � S
        S[len_ref-1][len_t-1]=D[len_ref-1][len_t-1];
        R[len_ref-1][len_t-1]=1;

        // ���������� �������������� ������ � �������
        for(int z=0;z<len_ref+1;z++) {
            S[z][len_t]=D[len_ref-1][len_t-1];
            R[z][len_t]=1;
        }

        for (int z=0;z<len_t+1;z++) {
            S[len_ref][z]=D[len_ref-1][len_t-1];
            R[len_ref][z]=1;
        }

        // ���������� ������
        double diag,right,down;
        for (int b=(len_t-1);b>-1;b--) {
            for (int a=(len_ref-1);a>-1;a--) {

                // ������� diag,right,down ��� ������� ������
                diag=calculate(D[a][b],S[a+1][b+1],R[a+1][b+1]);
                right=calculate(D[a][b],S[a][b+1],R[a][b+1]);
                down=calculate(D[a][b],S[a+1][b],R[a+1][b]);
                if ((down<right)&&(down<diag)) {
                    S[a][b]=D[a][b]+S[a+1][b];
                    R[a][b]=R[a+1][b]+1;
                }
                if ((diag<=right)&&(diag<=down)) {
                    S[a][b]=D[a][b]+S[a+1][b+1];
                    R[a][b]=R[a+1][b+1]+1;
                }
                if ((right<diag)&&(right<down)) {
                    S[a][b]=D[a][b]+S[a][b+1];
                    R[a][b]=R[a][b+1]+1;
                }
            }
        }

        for (int j=0;j<(Nmax-Nmin+1);j++) {
            result[j]=(S[0][j]/R[0][j]);
        }
    }

    void DTWMetric::computeDTWForMetric(const MultiMark &t, int i, int Nmin, int Nmax, const MultiMark &ref, std::vector<double> &result)
    {
        computeDTW(this, t, i, Nmin, Nmax, ref, result);
    }

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib
