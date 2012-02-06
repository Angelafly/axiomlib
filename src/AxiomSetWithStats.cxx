/****************************************************************************
*
*			������� ������ AxiomSetWithStats
*
****************************************************************************/

#include "AxiomSetWithStats.h"
#include "AxiomLibException.h"
#include <vector>

using namespace AxiomLib;
using namespace std;

/****************************************************************************
*			AxiomSetWithStats::operator<
*
*	Description:	��������� ���� ������� �� ������� �������
*	Parameters:	const AxiomSetWithStats& second
*	Returns:	false ���� first >= second
*			true ���� first < second
*	Throws:		AxiomLibException
*	Author:		gevor
*	History:
*
****************************************************************************/
bool AxiomSetWithStats::operator< (const AxiomSetWithStats& second) const
{
  if (nFirst < 0 || second.nFirst < 0)
    throw AxiomLibException("AxiomSetWithStatsPop::operator< - comparing sets without valid stats.\n");
  
  return (goal < second.goal);
}

/****************************************************************************
*			AxiomSetWithStats::initAxiomSetFromFile
*
*	Description:	�������������� ������� ������ ����������, ����������� � �����
*	Parameters:	string axiomSetBaseDir - ��� ������� ����������
*			string axiomSetName - �������� ����� � �������
*			AxiomBase& axiomBase - ����� ���� ������ ������������
*	Returns:	0
*	Throws:		AxiomLibException
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int AxiomSetWithStats::initAxiomSetFromFile (std::string axiomSetBaseDir, std::string axiomSetName, AxiomBase &axiomBase)
{
  AxiomSet::initAxiomSetFromFile(axiomSetBaseDir, axiomSetName, axiomBase);

  goal = -1;
  nFirst = -1;
  nSecond = -1;
  
  //outputToCout();

  return 0;
}

/****************************************************************************
*			AxiomSetWithStats::outputToCout
*
*	Description:	������� ����� ������ �� cout
*	Parameters:	-
*	Returns:	0
*	Throws:		AxiomLibException
*	Author:		gevor
*	History:
*
****************************************************************************/
int AxiomSetWithStats::outputToCout (void) const
{
  // �������� - �������
  cout << "    AxiomSet " << name() << " from file.\n";
  for (vector<Axiom*>::const_iterator ax = axioms.begin(); ax != axioms.end(); ax++) {
    cout << "        Axiom " << (*ax)->name() << "\n";
    vector<string> ecNames;
    (*ax)->getECNames(ecNames);
    for (vector<string>::const_iterator ec = ecNames.begin(); ec != ecNames.end(); ec++) {
      cout << "            EC " << *ec << "\n";
    }
  }

  return 0;
}


/****************************************************************************
*			AxiomSetWithStats::operator=
*
*	Description:	����������� second � this
*	Parameters:	AxiomSetWithStats - ��� ����������
*	Returns:	0
*	Throws:		AxiomLibException
*	Author:		gevor
*	History:
*
****************************************************************************/
AxiomSetWithStats & AxiomSetWithStats::operator= (const AxiomSetWithStats &second)
{
//  (AxiomSet)(*this) = (AxiomSet)second;;
  goal = second.goal;
  nFirst = second.nFirst;
  nSecond = second.nSecond;
 
  // �������� �������� ������������ ����
  crossoverControlBit = second.getCrossoverControlBit ();
  // �������� ��� ������� ������
  nameOfAxiomSet = second.name ();
  // �������� �������� � ������� �������
  for (unsigned int i = 0; i < axioms.size(); i++) {
    (axioms[i])->clear();
    delete (axioms[i]);
  }
  int num;
  num = second.getNumOfAxioms ();
  axioms.resize(num);
  for (int i = 0; i < num; i++) {
    axioms[i] = new Axiom;
    *axioms[i] = second.getAxiom (i);
  }
  
  return *this;
}


/****************************************************************************
*				AxiomSetWithStats::exit
*
*	Description:	������� ��������� - ������� �������� ���� ��������� �������� ��� ���
*	Parameters:	exitCond - ��������, � ������� ���� ��������� ������� �������
*	Returns:		true - ���� ������� ������� ��������� ���� ��������� ���������
*				false - � ��������� ������
*	Throws:		AxiomLibException - ������� ������� �� ���������� (��� ����� ���� �����)
*	Author:		dk
*	History:
*
****************************************************************************/
// bool AxiomSetWithStats::exit (double exitCond) { 
// 	if (goal < 0)
// 		throw AxiomLibException("AxiomSetWithStatsPop::exit - goal stat was not initialized.\n");
// 	if (goal <= exitCond) 
// 		return true; 
// 	return false;
// }
