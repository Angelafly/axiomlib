#include "NeuralCoin.h"

namespace AxiomLib {

/****************************************************************************
*					coin
*
*	Description: ������� ���������� true � ������������ p, ����� false. ���������� ����� �� �������������� ������� srand(time(0))
*	Parameters:	p - ����������� ���������� true, �� ��������� 0.5
*	Returns:	true � ������������ p, ����� false
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
inline bool coin(double p) {
	return p>=1? true : (p<=0 ? false :((long double)(std::rand()%RAND_MAX)/(long double)(RAND_MAX)) < p);
}


/****************************************************************************
*					Coiner::Coiner
*
*	Description: �����������
*	Parameters:	num - ����� ����� ��������� (�����)
*				p - ���� true (����� �����)
*	Returns:	-
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
Coiner::Coiner(size_t num, double p):v(num, false)
{
	std::srand((unsigned) std::time(0));
	
	// clip the probability to [0, 1]
	if(p>1) p=1;
	if(p<0) p=0;

	
	double p1 = num*p;

	size_t n = static_cast<size_t>(std::floor(p1));

	// ������� ����� ������������ ���� ����� ����� �� �� ����� �����
	double p2 = p1 - n;

	for(size_t i=0; i<n; i++)
	{
		v[i]=true;
	}

	if(n<num)
	{
		// � ���� ������������ ��������� ��� ���� ����� ���
		if(AxiomLib::coin(p2)) v[n] = true;
	}
	
	// ������������ ���� ��������� �������
	std::random_shuffle(v.begin(), v.end());
}

/****************************************************************************
*					Coiner::coin
*
*	Description: ������� ���������� i-� ������� ���������� �������, ������ ���� 
*				��������� true � ��� ������ ����������, �� �� ����� ��������
*	Parameters:	i - ������� � �������. ������ ���� ������ num, ��������� � ������������
*	Returns:	true ���� false
*	Throws:		-
*	Author:		wictor
*	History:
*
****************************************************************************/
bool Coiner::coin(int i) const
{
	if(i >= (int) v.size() || i < 0) return false;
	return v[i];
}

};