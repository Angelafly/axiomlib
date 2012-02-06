/****************************************************************************
*
*			������� ������ ElemConditionsFactory
*
****************************************************************************/

#include <iostream>

#include "ElemConditionsFactory.h"
// ��� ����������� ��� includes ������������ ������ �� �������� axioms
#include "all_axioms.h"
#include "AxiomLibException.h"

using namespace AxiomLib;

// ��������������� ������������ ����� ������������ ������� � ����� ���������� � ������� ������� ������
#define	recomendedMax		7

#define max(a,b)	(((a) >= (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#define min3(a,b,c)	(min(min(a,b),c))

/****************************************************************************
*			ElemConditionsFactory::ElemConditionsFactory
*
*	Description:	�����������, ��������� ������ ������ ����� ���������
*	Parameters:	-
*	Returns:	-
*	Throws:		-
*	Author:		Eugene A. Vasin
*	History:
*
****************************************************************************/
ElemConditionsFactory::ElemConditionsFactory (void)
{
  all_axioms.resize(N_EL_AXIOMS);
// BEGIN CREATE_ALL_AXIOMS
  all_axioms[3] = new DecreaseRightElemCondition();
  all_axioms[4] = new SecDerivativeDecreaseLeft();
  all_axioms[5] = new DerivativeDecreaseLeft();
  all_axioms[6] = new BetweenTwoLevels();
  all_axioms[7] = new Volatility();
  all_axioms[8] = new SecDerivativeIncreaseLeft();
  all_axioms[9] = new SecDerivativeDecreaseRight();
  all_axioms[10] = new DerivativeDecreaseRight();
  all_axioms[11] = new DerivativeIncreaseRight();
  all_axioms[12] = new DerivativeIncreaseLeft();
  all_axioms[13] = new SecDerivativeIncreaseRight();
  all_axioms[0] = new Integral();
  all_axioms[14] = new IncreaseRightElemCondition();
  all_axioms[1] = new DecreaseLeftElemCondition();
  all_axioms[2] = new IncreaseLeftElemCondition();
// END CREATE_ALL_AXIOMS

	// ��������� �������� ������� ��������� ������������ �������
	compatibilityMatrix.resize(N_EL_AXIOMS);
	for (int i = 0; i < N_EL_AXIOMS; i++) {
		compatibilityMatrix[i].resize(N_EL_AXIOMS);
	}
	for (int i = 0; i < N_EL_AXIOMS; i++) {
		for (int j = i+1; j < N_EL_AXIOMS; j++) {
			if ( ( all_axioms[i]->checkAndCompatibility( *(all_axioms[j])) ) > 0 )
				compatibilityMatrix[i][j] = true;
			else 
				compatibilityMatrix[i][j] = false;
			compatibilityMatrix[j][i] = compatibilityMatrix[i][j];
		}
		compatibilityMatrix[i][i] = false;
	}
}

/****************************************************************************
*			ElemConditionsFactory::~ElemConditionsFactory
*
*	Description:	����������, ����������� ��� ������� �� ������
*	Parameters:	-
*	Returns:	-
*	Throws:		-
*	Author:		Eugene A. Vasin
*	History:
*
****************************************************************************/
ElemConditionsFactory::~ElemConditionsFactory (void)
{
  for (int i = 0; i < N_EL_AXIOMS; i++) {
    delete all_axioms[i];
  }
}

/****************************************************************************
*			ElemConditionsFactory::createAxiomByNum
*
*	Description:	�������� ������� � �������� ������� (� ������ ���������� ��� ������� ����� ������, ����� ��� ������ ������� all_axioms + 1)
*	Parameters:	signed int num - ����� �������
*	Returns:		ElemCondition* - ��������� �� ������� (������� ����� <��� ��������������� �������>*)
*				��� NULL, ���� ������� � ������� num �� ����������
*	Throws:		AxiomLibException - ���� ����� out of range
*	Author:		Eugene A. Vasin
*	History:
*
****************************************************************************/
ElemCondition* ElemConditionsFactory::createAxiomByNum(signed int num)
{
  // �������� ������ ������ ������� �� �������
  if (num < 1 || num > (int) all_axioms.size()) {
    throw AxiomLibException("ElemConditionsFactory::createAxiomByNum : argument out of range.");
    return 0;
  }

//  el_axiom* to_return = (all_axioms[num])->create();
  ElemCondition* toReturn;
  switch (num) {
// BEGIN CREATE_ALL_CREATE_AXIOM_BY_NUM
    case 4:
      toReturn = new DecreaseRightElemCondition();
      break;
    case 5:
      toReturn = new SecDerivativeDecreaseLeft();
      break;
    case 6:
      toReturn = new DerivativeDecreaseLeft();
      break;
    case 7:
      toReturn = new BetweenTwoLevels();
      break;
    case 8:
      toReturn = new Volatility();
      break;
    case 9:
      toReturn = new SecDerivativeIncreaseLeft();
      break;
    case 10:
      toReturn = new SecDerivativeDecreaseRight();
      break;
    case 11:
      toReturn = new DerivativeDecreaseRight();
      break;
    case 12:
      toReturn = new DerivativeIncreaseRight();
      break;
    case 13:
      toReturn = new DerivativeIncreaseLeft();
      break;
    case 14:
      toReturn = new SecDerivativeIncreaseRight();
      break;
    case 1:
      toReturn = new Integral();
      break;
    case 15:
      toReturn = new IncreaseRightElemCondition();
      break;
    case 2:
      toReturn = new DecreaseLeftElemCondition();
      break;
    case 3:
      toReturn = new IncreaseLeftElemCondition();
      break;
// END CREATE_ALL_CREATE_AXIOM_BY_NUM
    default:
      toReturn = NULL;
  }
  
  return toReturn;
}


/****************************************************************************
*				ElemConditionsFactory::findMaxNum
*
*	Description:	��������������� �������. ���� ������������ ����� ����� ������� ������������� ����� � ���������� ���.
*	Parameters:	������ ����� �����
*	Returns:		-1 - ���� ���-�� �� ���
*	Throws:		-
*	Author:		dk
*	History:
*
****************************************************************************/
int ElemConditionsFactory::findMaxNum (std::vector <int> &vec) {
	if (vec.size() == 0) {
		return -1;
	}
	if (vec.size() == 1) {
		return vec[0];
	}
	else {
		int toReturn = vec[0];
		for (unsigned int i = 0; i < vec.size(); i++) {
			if (vec[i] > toReturn) {
				toReturn = vec[i];
			}
		}
		return toReturn;
	}
	return -1;
}


/****************************************************************************
*				ElemConditionsFactory::compatTable
*
*	Description:	��������������� �������.
*					������� ������� ������ ���������� �� ����� �������� ������� ������������ �������, ����������� ����� �����.
*					��� ���� ����� ���� �������� �������� ��� �������� �������. ������� ������������ �� ����� � ���� ����� ��������.
*	Parameters:		prev - ����������� ������ ������� ������������ ������������ �������
*					numOfEC - ��������� ����� ����������� ������������ ������� � ������ �� ��������
*	Returns:		0
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ElemConditionsFactory::compatTable (std::vector < std::vector<int> >& prev, const int numOfEC) {
	// ����������� ������� �������� �������
	int f =  (int) (N_EL_AXIOMS * ((double) rand() / (double) ((unsigned int) RAND_MAX + 1)) );
	std::vector <int> cur;
	cur.resize(1);
	cur[0] = f;
	if (numOfEC < 2) {
		prev.resize(1);
		prev[0] = cur;
	}
	// ���� � ������� ����� ������ �������� - �� ��������� ��������������� ������� ������������ ������� ������� ����������� ������������ �������
	prev.resize(0);
	int size = 0;
	std::vector <bool> curComp;
	curComp = compatibilityMatrix[f];
	subproc (prev, size, cur, curComp, numOfEC);
	prev.resize(size);

	return 0;
}


/****************************************************************************
*			ElemConditionsFactory::andVector
*
*	Description:	������� ������������ �������� ���������� ��� ���� �������� ���������� �����
*	Parameters:		whereVec   - ������, � ������� �������� ����������
*					whatFirst  - ������ ������ ��� �������� "�"
*					whatSecond - ������ ������ ��� �������� "�"
*	Returns:		0
*	Throws:			AxiomLibException - ���� ������� ������ �����
*	Author:			dk
*	History:
*
****************************************************************************/
int ElemConditionsFactory::andVector (std::vector <bool>& whereVec, const std::vector <bool>& whatFirst, const std::vector <bool>& whatSecond) {
	int sizeFirst = (int) whatFirst.size();
	if (sizeFirst != whatSecond.size()) 
		throw AxiomLibException("Error in ElemConditionsFactory::andVector: given vectors are of different lengths.");
	whereVec.resize (sizeFirst);
	for (int i = 0; i < sizeFirst; i++) 
		whereVec[i] = whatFirst[i] & whatSecond[i];
	return 0;
}


/****************************************************************************
*			ElemConditionsFactory::subproc
*
*	Description:	������� ������������ ������ ������������ ������ ������������ �������
*	Parameters:		prev - ����������� ������ �������� ������� ����������� ����� ����� �������
*					prevLast - ����� ���������� �������� � ������� prev (������������ ��� ���� ����� �������� ����� ������� �������� � 10 ��� ����)
*					cur  - ����� ������� ����������� ������������ �������
*					curComp  - ������ ������������� ��� ������ cur
*					numOfEC  - ��������� ����� ��������� � ����������� ������� ����������� ������������ �������
*	Returns:		0 ���� ��� ok
*	Throws:			-
*	Author:			dk
*	History:
*
****************************************************************************/
int ElemConditionsFactory::subproc (std::vector < std::vector<int> >& prev, int &prevLast, std::vector <int>& cur, std::vector <bool> &curComp, const int numOfEC) {
	// ��������� ���������� ��������
	if (cur.size() == numOfEC) {
		if (prevLast == prev.size())
			prev.resize(prevLast + 10);
		prev[prevLast] = cur;
		prevLast++;
		return 1;
	}
	// ��������� ����������� ��������
	std::vector <int> temp;
	temp = cur;
	temp.resize(temp.size() + 1);
	std::vector <bool> tempComp;
	for (unsigned int i = 0; i < N_EL_AXIOMS; i++) {
		if (curComp[i]) {
			temp[temp.size() - 1] = i;
			andVector (tempComp, compatibilityMatrix[i], curComp);
			subproc (prev, prevLast, temp, tempComp, numOfEC);
		}
	}
	return 0;
}


/****************************************************************************
*			ElemConditionsFactory::createAxiomByRand
*
*	Description:	�������� ������� ��������� ������� � �������� ������ ������������ �������
*	Parameters:	toReturn - ������������ ������ ������������ �������, ����������� ����� �����
*			numOfEC - ����� ������������ ������� � ����������� �������
*	Returns:	0 ���� ��� ok
*	Throws:		AxiomLibException - ���� numOfEC ����������� ����� ����� ��������� ������������ �������
*	Author:		dk,gevor
*	History:
*
****************************************************************************/
int  ElemConditionsFactory::createAxiomByRand(std::vector<ElemCondition* >& toReturn, const signed int numOfEC) {
    // �������� ������ ��������� �� �������
    if ((numOfEC > (int) all_axioms.size()) || (numOfEC < 1)) {
		throw AxiomLibException("Error in ElemConditionsFactory::createAxiomByRand : argument out of range.");
    }
    std::vector < std::vector<int> > vec; // ��� � �������, ������, ���������� ������ ������� ����������� �� ������� "�" ������������ ������� - gevor
    compatTable (vec, numOfEC);
    if (vec.size() > 0) { 
	  int i;
	  i = (int)(((double) rand() / (double) RAND_MAX) * ((double) vec.size() - 1.0));
//	  cout << "Randomly selecting axiom " << i << " of " << vec.size() << ".\n";
	  toReturn.resize(vec[i].size());
	  for (int j = 0; j < (int) vec[i].size(); j++) {
		  toReturn[j] = createAxiomByNum(vec[i][j] + 1);
	  }
    }
    else {
//	  throw AxiomLibException("Error in ElemConditionsFactory::createAxiomByRand : vec.size() = 0.");
	  toReturn.resize(0);
    }

    return 0;
}


/****************************************************************************
*					ElemConditionsFactory::createAxiomByRand
*
*	Description:	�������� ������� ��������� ������� � �������� ������ ������������ �������
*	Parameters:		toReturn - ������������ ������ �������� ������������ �������, ������� ��������� ��������� ��� ������������� ���������
*					numOfEC - ����� ������������ ������� � ����������� �������
*	Returns:		0 - ���� ��� ok
*	Throws:			AxiomLibException - ���� numOfEC ����������� ����� ����� ��������� ������������ �������
*	Author:			dk
*	History:
*
****************************************************************************/
int  ElemConditionsFactory::createAxiomByRand(std::vector< std::vector <int> > &toReturn, const signed int numOfEC) {
	// �������� ������ ��������� �� �������
    if (numOfEC < 1) {
		throw AxiomLibException("Error in ElemConditionsFactory::createAxiomByRand(II) : input argument out of range.");
    }
	// ����������� ����������� ����������
    std::vector < std::vector<int> > vec; // ������ ��� �������� ������� ������� ����������� �� ������� "�" ������������ �������
	toReturn.clear();
	int curNumToInclude, curMaxToInclude, i;
	int toInclude = numOfEC;
	// ���� - ���� �� ������� � ������ ������� �������� ���������� ������������ �������
	while (toInclude > 0) {
		//andECs = new std::vector <ElemCondPlus>;
		// � ������ ����� ���������� �������� �� ������ all_axioms.size() �������, ��� ��� �� ��������� � ��� ��������� ����������
		curMaxToInclude = min3 (recomendedMax, toInclude, (int) all_axioms.size());
		// �������� ���������� - ������� ������� ���������� � ������ ����������
		curNumToInclude = (int)(((double) rand() / (double) RAND_MAX) * ((double) curMaxToInclude - 1.0) + 1.0);
		// ���������� ������ ����������� ������������ �������
		vec.clear();
		compatTable (vec, curNumToInclude);
		// ���� �� ������� ���� �� ���� ������ ����������� ������� - ��������� ����������
		while (vec.size() < 1) {
			curMaxToInclude--;
			if (curMaxToInclude < 1)
				throw AxiomLibException("Error in ElemConditionsFactory::createAxiomByRand(II) : internal error - not enough elementary conditions.");
			curNumToInclude = (int)(((double) rand() / (double) RAND_MAX) * ((double) curMaxToInclude - 1.0) + 1.0);
			compatTable (vec, curNumToInclude);
		}
		// ���������� ����� �� �������� ����������� ������� ������������
		i = (int)(((double) rand() / (double) RAND_MAX) * ((double) vec.size() - 1.0));
		// ��������� ������, ����������� ���� ����������
		toReturn.push_back (vec[i]);
		toInclude -= vec[i].size();
	}
    return 0;
}


/****************************************************************************
*			ElemConditionsFactory::create_axiom_by_name
*
*	Description:	�������� ������� � �������� ������
*	Parameters:	const char* name - ��� �������
*	Returns:	el_axiom* - ��������� �� ������� (������� ����� <��� ��������������� �������>*)
*			��� NULL, ���� ������� � ������ name �� ����������
*	Throws:		AxiomLibException - ���� ��� �� ����������
*	Author:		Eugene A. Vasin
*	History:
*
****************************************************************************/
ElemCondition* ElemConditionsFactory::createAxiomByName(const char* name) {
	ElemCondition* toReturn = NULL;
	// ����
	//  for(std::vector<el_axiom*>::iterator i = all_axioms.begin(); i != all_axioms.end(); i++)
	for(int i = 0; i < N_EL_AXIOMS; i++)
	{
		if (all_axioms[i]->name() == name) {
	//      to_return = (*i)->create();
		toReturn = createAxiomByNum(i+1);
		}
	}
	
	if (toReturn == NULL) {
		//��� �������� ����� �� �����
		throw AxiomLibException("ElemConditionsFactory::createAxiomByName : invalid name.");
	}
	
	return toReturn;
}


