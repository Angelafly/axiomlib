/****************************************************************************
*			Metric1
*
*	Description:	Metric1
*
*	Author:		armkor
*	History:
*
****************************************************************************/

#ifndef METRIC1_H
#define METRIC1_H

#include "dtwmetric.h"
#include "multimarks.h"

namespace AxiomLib {

namespace MultiMarking {

class Metric1 : public DTWMetric {

private:

    // creation
    Metric1(MultiMark& f);

    public:

    // realisation distance for DTW
    double compute(std::vector<bool>& v1, std::vector<bool>& v2);

};// end of class

};//  end of namespace MultiMraking

};//  end of namespace AxiomLib

#endif // METRIC1_H