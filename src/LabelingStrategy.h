/****************************************************************************
*					LabelingStrategy
*
*	Description:	�������� ���������� ���������� 
*					�� ������ � �����������
*					�� �������� ���������� �� ��������� ����������.
*					���������.
*	Author:			wictor
*	History:	
*
****************************************************************************/
#ifndef LABELINGSTRATEGY_H
#define LABELINGSTRATEGY_H

#include <boost/shared_ptr.hpp>

#include "Environment.h"
#include "TrajectorySampleDistance.h"
#include "CompareStatistic.h"
#include "GoalStrategy.h"

namespace AxiomLib {

class LabelingStrategy
{
public:
	LabelingStrategy();
	
	virtual ~LabelingStrategy() {}
	
	// ������� ���������� �� ���������
	virtual void initFromEnv(const Environment& env) = 0;
	
	// ���������� ���������� ����������
	virtual void performLabeling(const TrajectorySampleDistance& trajectorySampleDistance, std::vector<int>& labeling) const = 0;
	
	// �������� ��������� ����������
	virtual double train(const std::vector<TrajectorySampleDistance>& trainTrajectorySampleDistance, 
				 const std::vector<std::vector<int> >& trainTrajectoryRightLabeling,
				 CompareStatistic* compareStatistic,
				 GoalStrategy* GoalStrategy) = 0;
	
	virtual LabelingStrategy* copy() const = 0;
};

}

#endif // LABELINGSTRATEGY_H
