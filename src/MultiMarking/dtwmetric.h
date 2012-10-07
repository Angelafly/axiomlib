/****************************************************************************
*			DTWMetric.h
*
*	Description:	DTWMetric - �����-�������� ���� ������. ��������
*                       ������ - ������������� �������, ���������� DTW.
*
*	Author:		armkor
*	History:
*
****************************************************************************/

#ifndef DTWMETRIC_H
#define DTWMETRIC_H

#include <string>

#include "multimarks.h"

namespace AxiomLib {

namespace MultiMarking {

class DTWMetric {

 private:


 public:
    //
    DTWMetric();

    //
    ~DTWMetric() { }

   // ������������� �������
   static DTWMetric* getMetric(std::string name);
   //
   virtual double compute(const std::vector<bool>& v1, const std::vector<bool>& v2)=0;

   virtual void computeDTWForMetric (const MultiMark& t, int i,int Nmin,int Nmax, const MultiMark& ref, std::vector<double>& result);

   // ���������� DTW
   static void computeDTW (DTWMetric* m, const MultiMark& t, int i,int Nmin,int Nmax, const MultiMark& ref, std::vector<double>& result);

};// end of class

};//  end of namespace MultiMarking

};//  end of namespace AxiomLib

#endif // DTWMETRIC_H
