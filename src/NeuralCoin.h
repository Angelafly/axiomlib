/****************************************************************************
*				NeuralCoin.h
*
*	Description:	����, ���������� ����������� ������� coin() � ������ Coiner
*	Author:		wictor
*	History:	
*
****************************************************************************/

#ifndef COIN_H_
#define COIN_H_

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>

namespace AxiomLib {


//������� ���������� true � ������������ p, ����� false. ���������� ����� �� �������������� ������� srand(time(0))
inline bool coin(double p=0.5);

/****************************************************************************
*				Coiner
*
*	Description:	����� - ������� ��� vector<bool>, ������� ���� ��� ���� 
*					� ������ (true) � ������� (false) ������
*	Author:		wictor
*	History:	
*
****************************************************************************/
class Coiner
{
	// ������
	std::vector<bool> v;
	//size_t curr;
public:
	// �����������
	Coiner(size_t num, double p);
	// ������� ���������� i-� ������� ���������� �������, ������ ���� 
	// ��������� true � ��� ������ ����������, �� �� ����� ��������
	bool coin(int i) const;
};

};

#endif /* COIN_H_ */
