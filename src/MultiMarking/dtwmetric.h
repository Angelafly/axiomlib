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
#include <boost/numeric/ublas/matrix.hpp>

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


	virtual DTWMetric* clone() const = 0;

	virtual bool isWeighted() const { return false; }

	virtual void setWeights(const std::vector<double>& w) { return; }

   // ������������� �������
   static DTWMetric* getMetric(std::string name);
   //
   virtual double compute(const std::vector<bool>& v1, const std::vector<bool>& v2)=0;

   virtual void computeDTWForMetric (const MultiMark& t, int i,int Nmin,int Nmax, const MultiMark& ref, std::vector<double>& result);

   // ���������� DTW
   static void computeDTW (DTWMetric* m, const MultiMark& t, int i,int Nmin,int Nmax, const MultiMark& ref, std::vector<double>& result);

protected:
   template<class Derived>
   static DTWMetric* clone_impl(const Derived* d) {
	   return new Derived(*d);
   }


};// end of class

};//  end of namespace MultiMarking

};//  end of namespace AxiomLib

#endif // DTWMETRIC_H
