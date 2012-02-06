/****************************************************************************
*					GridSearch
*
*	Description:	����� �������� ������� ����������� �������� �� ����������� �����.
*	Author:			wictor
*	History:	
*
****************************************************************************/
#ifndef GRIDSEARCH_H
#define GRIDSEARCH_H

#include "../Environment.h"
#include "Minimizer.h"

namespace AxiomLib {
namespace Optimization {

class GridSearch : public Minimizer
{
	int numPartitions;
public:
	GridSearch();
	
    // ������������� ���������� ������������ �� ���������
	void initFromEnv(const Environment &env);
	
    // ����� �������� �������� ������� � �������� ��������
	void findMinimum(const FunctionOneVariable *function, 
					 double lowerBound, double upperBound, 
				     double &bestX, double &bestF);
};

}

}

#endif // GRIDSEARCH_H
