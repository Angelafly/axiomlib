/****************************************************************************
*
*			������� ������ GaOps
*
****************************************************************************/

//#include "AxiomSetWithStats.h"
#include "GaOps.h"
//#include "AxiomSet.h"

using namespace AxiomLib;
using namespace std;
namespace AxiomLib {

/****************************************************************************
*			CrossoverOp_Params
*
*	Description:	����������� �� ������ ���������� ������������ �������
*	Parameters:	AxiomSetWithStats& result - ��������� �����������
*				const AxiomSet& first - ������ �������
*				const AxiomSet& second - ������ �������
*	Returns:		0
*	Throws:		AxiomLibException - ���� ������������ �����������
*	Author:		gevor
*	History:
*
****************************************************************************/
signed int CrossoverOp_Params(AxiomSetWithStats& result, const AxiomSet& first, const AxiomSet& second)
{
  // ��������� ����������� ������������

  // ������� ���������� ��� ����������
  result.voidStats();
  return 0;
}

}; // rnd of namespace
