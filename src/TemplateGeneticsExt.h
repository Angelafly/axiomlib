/****************************************************************************
*					TemplateGeneticsExt
*
*	Description:	�����, ����������� �������� �������� �������� ���������������.
*					�������� �������: 
*					- ��������� ��������� ������ ������ ������������ ���������������� ������
*					- ������ ��������� ���������� ������� ������� ����������� ����������
*					- ��������� �������� �������� ���� ����� ����� ��������, ���� ����� �������� ��� ���������
*	Author:			dk
*	History:	
*
****************************************************************************/

#ifndef _AXIOMS_LIB_TEMPLATE_GENETICS_EXT_HXX
#define _AXIOMS_LIB_TEMPLATE_GENETICS_EXT_HXX

#include "Environment.h"
#include "AxiomLibException.h"
#include "AxiomSetPlus.h"
#include "GoalStrategyFactory.h"
#include "GoalStrategy.h"
#include "CrossoverTpl.h"
#include "TransmuteTpl.h"
#include "DataSet.h"
#include "TemplateGenetics.h"
#include "Round.h"

#include <set>
#include <string>
#include <vector>

namespace AxiomLib {

class TemplateGeneticsExt : public TemplateGenetics {

  private:
	
	// ���������
	std::vector <AxiomSetPlus> *population;
	// Initial population size (number from environment)
	int popSize;
	
	// ������� �������� ��������� ��������� �������� ��������
	int numOfIter; // ������������ ���������� �������� ���������
	int numOfBadIter; // ������������ ����� �������� ��� ��������� ������� �������
	double exitCond; // ���������� ��� �������� �������� ������� �������, �� �������� �������� ��������� ���������� ��������� �������� ��������

	// ������� �������� ��������� �������� ������� ������ ������
	int numOfGenIter; // ������������ ���������� �������� ���������
	int numOfBadGenIter; // ������������ ����� �������� ��� ��������� ������� �������
	double exitCondGen; // ���������� ��� �������� �������� ������� �������, �� �������� �������� ��������� ���������� ������������� ���������
	// ��������� ��������
	double percentBest;
	// ��������� ��������� �����������
	double asCrossProbMin;
	double asCrossProbMax;
	double axCrossProbMin;
	double axCrossProbMax;
	double ecCrossProbMin;
	double ecCrossProbMax;
	// ��������� ��������� �������
	double prTransProbMin;
	double prTransProbMax;
	double asTransProbMin;
	double asTransProbMax;
	double axTransProbMin;
	double axTransProbMax;
	double ecTransProbMin;
	double ecTransProbMax;

	// ��������� �������� ������� ������� �� ������� I � II ����, ������������������ �� ����������������� �����
	GoalStrategy* goalStrategy;

	// ����� ��� ���������� �������� �����������
	CrossoverTpl crossoverTpl;

	// ����� ��� ���������� �������� ������� ���������
	TransmuteTpl transmuteTpl;

  protected:

	// ���������� ������� ����� ����� � ������� ������� ��������
	  inline void sortVector (std::vector<unsigned int> &vec) const;

  public:

	// ����������� � �������� ���������� �� ���������
	TemplateGeneticsExt (void);

	// ���������� - ��������� ��� ��������� � ������ ������� ������ ������������ �������
	~TemplateGeneticsExt(void);

	// ��������� ���������� ��������� ��������
	int initFromEnv (const Environment& env);

	// �������� �������� ���������
	int select (const std::vector<unsigned int> &mask);

	// �������� ������� ������ ���������
	int transmute ();

	// �������� ����������� ��������� ���������
	int crossover ();

	// �������� ������ ������ �� �������� �������
	inline int recognize (TemplateRecognizer& templateRecognizer);

	// �������� ���������� ������� ������� �� ����� ������ ������� � ������� ���� ��� ��������� ���������
	inline int compute ();

	// ������� ���������� ���������� �� ��������� ��������� �� ������ ������ ���������� ��� �������������
	inline int fillStatistic (std::vector<unsigned int> &mask);

	// ���������� ������� ����� ��� ��������� ��������� �� ��������� ������� �������
	inline int sort (std::vector<unsigned int> &mask) const;

	// ������ ��������� �������� �������
	double run(TemplateRecognizer& templateRecognizer);
	
}; // end of class

}; // end of namespace

#endif /* _AXIOMS_LIB_TEMPLATE_GENETICS_EXT_HXX */
