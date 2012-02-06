/****************************************************************************
*			SimpleSelectionStrategy
*
*	Description:	������� ������ SimpleSelectionStrategy - ����������
*	Author:		gevor
*	History:		
*
****************************************************************************/
#include "SimpleSelectionStrategy.h"
#include "AxiomLibException.h"

using namespace AxiomLib;
using namespace std;

/****************************************************************************
*			SimpleSelectionStrategy::setParamsFromEnv
*
*	Description:	��������� ���������� �������� �� �����
*	Parameters:	const Environment& env - ������ �������
*	Returns:	0
*	Throws:		AxiomLibException - ���� ��������� ���
*	Author:		gevor
*	History:
*
****************************************************************************/
int SimpleSelectionStrategy::setParamsFromEnv (const Environment& env)
{
  if (env.getDoubleParamValue(pBest, "selectNumBest") < 0) {
    throw AxiomLibException ("Error in SimpleSelectionStrategy::setParamsFromEnv - can not find all needed parameners.");
  }
  return 0;
}


/****************************************************************************
*			SimpleSelectionStrategy::sort
*
*	Description:	������� ���������� ������� �������� mask �� �������� ��������  goals 
*	Parameters:	mask - ����������� ������
*				goals - ������� �������� - �� �������� ������� ����������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� ������ �����
*	Author:		dk
*	History:
*
****************************************************************************/
inline int SimpleSelectionStrategy::sort (std::vector<int> &mask, std::vector<double> &goals) const {
	// �������� ������� ���������
	if ( (mask.size() != goals.size()) || (mask.size() < 1) )
		throw AxiomLibException ("Error in SimpleSelectionStrategy::sort: input parameters are of wrong length.");
	// ���������� ���������� �� �������� - ���������� � ������� ���������� �� goals �������� � mask
	int j = 0;
	for (int i = 0; i < (int) goals.size(); i++){
		int j = 0;
		// ����� ����������
		while ( j < i ) {
			if (goals[i] < goals[mask[j]]) {
				break;
			}
			j++;
		}
		// �������� ����, � ���� goal ������
		for (int k = i; k > j; k--) {
			mask[k] = mask[k-1];
		}
		// ���������� ���������� �������
		mask[j] = i;
	}
	return 0;
}


/****************************************************************************
*			SimpleSelectionStrategy::quickSort
*
*	Description:	������� ���������� ������� �������� mask �� �������� ��������  goals �� ������ "������� ���������"
*	Parameters:	mask - ����������� ������
*				goals - ������� �������� - �� �������� ������� ����������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������� ������ �����
*	Author:		dk
*	History:
*
****************************************************************************/
inline int SimpleSelectionStrategy::quickSort (std::vector<int> &mask, std::vector<double> &goals) const {
	// �������� ������� ���������
	if ( (mask.size() != goals.size()) || (mask.size() < 1) )
		throw AxiomLibException ("Error in SimpleSelectionStrategy::sort: input parameters are of wrong length.");
	// ���������� ���������� �� �������� - ���������� � ������� ������������� �� goals �������� � mask
	int l;
	int d = 1;
	int l1, r1, l2, r2;
	div_t divRes;
	std::vector <int> tmpMask;
	tmpMask.resize(mask.size());
	while (d < (int) goals.size()){
		divRes = div ((int) goals.size() - 1,d);
		l = divRes.quot;
		for (int j = 0; j < l; j+=2){
			l1 = d * j;
			r1 = d * (j+1) - 1;
			l2 = d * (j+1);
			r2 = d * (j+2) - 1;
			if (r2 >= (int) goals.size()) r2 = (int) goals.size() - 1;
			for (int h = l1; h <= r2; h++) {
				if (l1 > r1) {
					tmpMask[h] = mask[l2];
					l2++;
					continue;
				}
				if (l2 > r2) {
					tmpMask[h] = mask[l1];
					l1++;
					continue;
				}
				if (goals[mask[l1]] > goals[mask[l2]]) {
					tmpMask[h] = mask[l1];
					l1++;
				}
				else {
					tmpMask[h] = mask[l2];
					l2++;
				}
				// throw AxiomLibException ("Error in SimpleSelectionStrategy::quickSort: this cannot happen!");
				//std::cerr << "\n This cannot happen! \n";
			}
		}
		mask = tmpMask;
		d *= 2;
	}
	return 0;
}


/*****************************************************************************
*			SimpleSelectionStrategy::select
*
*	Description:	������� �������� ��� ������ ���������
*	Parameters:	AxiomSetPop - ���������� ���������
*	Returns:		0 - ���� ����� ���������
*				1 - ���� ������ �������� ��������� ��������� � �������� ��������� �� ������ - �������� �� ����
*	Throws:		AxiomLibException - ���� ����� �� ��� ������������������ ��� ����������� ��������� ������, ���� ���� �� ��� ����� �� ������ �� ���������� ������� �������
*	Author:		dk
*	History:
*
****************************************************************************/
int SimpleSelectionStrategy::select (AxiomSetPop& pop) const {
	// ����, �� ��������� pop �� ������ ������� ����� pop.basePopSize() ������
	// 1. �������� ����������� ��������
	int basePopSize = pop.getBasePopSize();
	int curPopSize = pop.size();
	// �������� ������������ ��������� ������
	if ( (pBest < 0) || (basePopSize > curPopSize) ) {
		throw AxiomLibException ("Error in SimpleSelectionStrategy::select - selection class has not been initialized or wrong input parameters.");
	}
	
	// 2. ���������, ���� �� ���������
	if (basePopSize == curPopSize) {
		// ���������� �� ���� - ������ � ������� ������ �� ����
		// ������ ������� �������� ������� �������
		int tmpFirst, tmpSecond;
		double goal;
		std::cout << " 	Goals on this stage are \n	";
		for (int asNum = 0; asNum < curPopSize; asNum++) {
			pop.getStats (asNum, tmpFirst, tmpSecond, goal);
			std::cout << "  " << goal;
		}
		return 1;
	}
	
	// ���� ��������� ����. �� - ���������: 
	// 3. ���������� ����� ������ - ������� ������� ����� � ����� ���������
	int nBest = (int) ((double) pBest * (double) basePopSize); // ������� ������ ��������� (��������� - ��������
	
	// ���������� ���������� ��������������� ���������
	int numOthers, curOther, wholeOthers;
	int tmpFirst, tmpSecond;
	std::vector <double> goals;
	std::vector <int> mask;
	std::vector <int> newPop;
	std::vector <bool> stayAlive;
	
	// 4. ������� �������� ������� ������� �� ������ �� ������ ������ 
	goals.resize(curPopSize);
//	if (comments) std::cout << "\n 		Goals and first and second: \n";
	for (int asNum = 0; asNum < curPopSize; asNum++) {
		pop.getStats (asNum, tmpFirst, tmpSecond, goals[asNum]);
// 		if (comments) { // ����� ����������� ������ ������ � ������� �������� ������� �������
// 			std::cout << "\n		" << goals[asNum] << "	" << tmpFirst << "	" << tmpSecond;
// 		}
		if (goals[asNum] < 0) {
			throw AxiomLibException ("Error in SimpleSelectionStrategy::select: Goal function is not computed in of the AxiomSetsWithStat.");
		}
	}
//	if (comments) std::cout << "\n";
	
	// 5. �������� ������ ����� - �� �������� ������� ������ - ��� ����������� ����� ����� �������� ����� � ����� ���������, � �� ����� �������� �� �������
	mask.resize(curPopSize);
	// ������������� �����
	for (int asNum = 0; asNum < curPopSize; asNum++) {
		mask[asNum] = asNum;
	}
	// ���������� mask �� �������� ������� �������
	sort (mask, goals); // �� ��� quickSort (mask, goals);
	
	if (comments) { // ����� ����������� ������ ������ � ������� �������� ������� �������
		std::cout << " 	Goals on this stage are \n	";
		for (unsigned int g = 0; g < mask.size(); g++) {
			std::cout << "  " << goals[mask[g]];
		}
		std::cout << "\n";
	}
	
	// 6. ������ ���� ������ �������� ������������������ ������� ������ ������ � ������� ���������� ������� ������� - ��������� � ������ �������� ������
	stayAlive.resize(curPopSize);
	// ������������� ������� - ���������� ����� ����� ������� ����� ��������
	for (int a = 0; a < curPopSize; a++) {
		stayAlive[a] = false;
	}
	// ��������� ��������� ����� ������ ������
	for (int alive = 0; alive < nBest; alive++) {
		stayAlive[mask[alive]] = true;
	}
	// ��������� �������� ���������� �����
	numOthers = basePopSize - nBest; // ����� ���������� �� �������
	wholeOthers = curPopSize - nBest; // ����� ����������
	for (int i = 0; i < numOthers; i++) {
		curOther = (int) ( ((double) rand() / (double) RAND_MAX) * ((double) wholeOthers - (double) i - 1.0) );
		while (stayAlive[mask[nBest + curOther]] == true) {
			curOther++;
		}
		stayAlive[mask[nBest + curOther]] = true;
	}
	
	pop.cutDown (stayAlive);
	return 0;
}
