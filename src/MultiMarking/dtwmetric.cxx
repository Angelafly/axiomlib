/****************************************************************************
*			dtwmetric.�xx
*
*	Description: ���������� ������� ������
*	Author:		armkor
*	History:
*
****************************************************************************/

#include "dtwmetric.h"
#include <boost/algorithm/string/case_conv.hpp>

#include "../AxiomLibException.h"
#include "MetricsBetweenSets1.h"
#include "MetricsBetweenSets2.h"
#include "MetricsEqual.h"
#include "MetricsEuclidean.h"
#include "MetricsHamming.h"
#include "MetricsMatrix.h"

namespace AxiomLib {

namespace MultiMarking {

    DTWMetric::DTWMetric(){}


    double calculate(double a,double b,double c) {
        return ((a+b)/(c+1));
    }
                                        // t-��������������� ref - ���������
    void DTWMetric::computeDTW (DTWMetric* m, const MultiMark& t,int i,int Nmin,
                                int Nmax, const MultiMark& ref, std::vector<double>& result) {
        int len_t=Nmax; // ������������ ������ ���� = ������ �������
        int len_ref=ref.size(); /* ����� ��������� ����������*/
        // ������������� ������
        std::vector<std::vector<double>> D(len_t+1),R(len_t+1),S(len_t+1);
        for (int u=0;u<len_ref+1;u++){
            D[u].resize(len_ref+1);
            R[u].resize(len_ref+1);
            S[u].resize(len_ref+1);
        }

        // ���������� ������.
        for (int b=0 ; b < len_ref ; b++ ){
               for (int a=0 ; a < len_t ; a++) {
                       D.at(a).at(b)=m->compute ( ref[b],t[i-len_t+a] );
                   }
               }

        // ������ � ��������� R � S
        S[len_t-1][len_ref-1]=D[len_t-1][len_ref-1];
        R[len_t-1][len_ref-1]=1;

        // ���������� �������������� ������ � �������
        for(int z=0;z<len_ref+1;z++) {
            S[len_t][z]=D[len_t-1][len_ref-1];
            R[len_t][z]=1;
        }

        for (int z=0;z<len_t+1;z++) {
            S[z][len_ref]=D[len_t-1][len_ref-1];
            R[z][len_ref]=1;
        }

        // ���������� ������
        double diag,right,down;
        for (int a=(len_t-1);a>-1;a--) {
            for (int b=(len_ref-1);b>-1;b--) {
                if ((a!=(len_t-1)) or (b!=len_ref-1)){
                // � ��������� �� ���� ������������ ����� ������ ������. ��-����� ��� ��� ��������������.
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
        }

        result.resize(Nmax-Nmin+1);
        for (int j=0;j<(Nmax-Nmin+1);j++) {
            result[j]=(S[j][0]/R[j][0]);
        }
    }


    DTWMetric* DTWMetric::getMetric(std::string name){
        // �������������� � �������� �����
          std::string nameCopy = boost::algorithm::to_lower_copy(name);
          DTWMetric* result;
          if(nameCopy == "betweensets1") {
              BetweenSets1* result;
              return result;
          }
          if(nameCopy == "betweensets2") {
              BetweenSets2* result;
              return result;
          }
          if(nameCopy == "equal") {
              Equal* result;
              return result;
          }
          if(nameCopy == "euclidean") {
              Euclidean* result;
              return result;
          }
          if(nameCopy == "hamming") {
              Hamming* result;
              return result;
          }
          if(nameCopy == "matrix") {
              result=new Matrix;
              return result;
          }
          throw AxiomLibException("Can not find a name of metric");
    }


};//  end of namespace MultiMraking

};//  end of namespace AxiomLib
